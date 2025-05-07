#include "../../includes/webserv.class.hpp"

webserv::webserv(std::vector<std::vector<ServerInfo> >&   confset)
{
    _confset = confset;

    _epfd = epoll_create(10);
    if (_epfd < 0)
    {
        throw AnyErrorException("Cannot create epoll.");
    }

    if (!createSockByConf())
    {
        throw AnyErrorException("Cannot create socket for each conf.");
    }
}

webserv::~webserv()
{
    this->closeFds();
}

void    webserv::runWebserv()
{
    log::loginfo(T_INFO, "Webserv is running.");
    for (size_t i = 0; i < _confset.size(); i++)
    {
        std::stringstream ss;
        ss << _confset[i][0].getIp() << ":" << _confset[i][0].getPort();
        log::loginfo(T_INFO, ss.str());
    }
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, handleSigint);
    this->listenSockets();
}

void    webserv::listenSockets()
{
    struct epoll_event events[MAX_EVENTS];
    
    while (sigRecieved != SIGINT)
    {
        try
        {
            this->handleReqTimeout();
            this->handleCgiFinish();
            int numEvent = epoll_wait(_epfd, events, MAX_EVENTS, 100);
            if (numEvent == -1) 
            {
                if (sigRecieved == SIGINT)
                    return;
                log::loginfo(T_ERR, "Epoll wait failed.");
                continue;
            }
            for (int i = 0; i < numEvent; i++)
            {
                if (_fdset.find(events[i].data.fd) == _fdset.end())
                    continue;

                int     fdType = _fdset[events[i].data.fd][0];
                if (_fdset[events[i].data.fd][0] == CLIENT_FD && events[i].events & (EPOLLHUP | EPOLLRDHUP))
                {
                    this->clientDisconnect(events[i]);
                }
                else if (events[i].events & (EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLRDHUP))
                {
                    switch (fdType)
                    {
                        case SOCK_FD:
                            this->acceptClient(events[i]);
                            break;
                        
                        case CLIENT_FD:
                            this->eventHandle(events[i], _confset[_fdset[events[i].data.fd][1]]);
                            break;

                        case PIPE_FD_WRITE:
                            this->writeCGI(events[i]);
                            break;
                        
                        case PIPE_FD_READ:
                            this->readCGI(events[i], PIPE_FD_READ);
                            break;

                        case CGI_ERR_FD:
                            this->readCGI(events[i], CGI_ERR_FD);
                            break;

                        default :
                            break;
                    }
                }
            }
        }
        catch (const std::exception &e)
        {
            log::loginfo(T_ERR, e.what());
        }
        catch (...)
        {
            log::loginfo(T_ERR, "Other exception.");
        }
    }
}

void    webserv::eventHandle(struct epoll_event& event, std::vector<ServerInfo>& wconf)
{
    int clientFd = event.data.fd;

    //if the fd not in Request map, add it to request map, this is to record the request status, useful when cannot finish in one read and write
    if (_ConnectionMap.find(clientFd) == _ConnectionMap.end())
        _ConnectionMap[clientFd] = connection(clientFd, wconf, _ipMap[clientFd], this);

    // check if we read a complet http request, if not we can read
    int readStatus = _ConnectionMap[clientFd].readFinishStatus();
    if (readStatus == NOT_END && event.events & EPOLLIN)
    {
        this->handleRequestRead(_ConnectionMap, event);
        return;
    }
    
    readStatus = _ConnectionMap[clientFd].readFinishStatus();
    if (readStatus == READ_FINISH || readStatus == READ_ERR || readStatus == PARSE_ERR || readStatus == OTHER_ERR) //finish reading
    {
        handleRequestProcess(_ConnectionMap, event);
    }
    if (_ConnectionMap[clientFd].reqProcessed() == PROCESSED) 
    {
        this->handleRequestWrite(_ConnectionMap, event);
    }
}

void    webserv::writeCGI(struct epoll_event& event)
{
    if (_fdset[(event.data.fd)][0] == PIPE_FD_WRITE)
    {
        int pipeFd = event.data.fd;
        int clientFd = _fdset[pipeFd][2];

        std::string res;
        _ConnectionMap[clientFd].getWriteBuf(res);
        if (res != "" && event.events & EPOLLOUT)
        {
            int byteWrite = write(pipeFd, res.data(), res.size());
            if (byteWrite < 0)
            {
                log::loginfo(T_ERR, "Cannot write to pipe.");
                _ConnectionMap[clientFd].join_response(500);
                removePipeFd(event);
                return ;
            }
            else if (byteWrite == 0)
            {
                removePipeFd(event);
                _ConnectionMap[clientFd].join_response(500);
                return ;
            }
            else
            {
                _ConnectionMap[clientFd].updateByteWrite(byteWrite);
            }
        }
        if (_ConnectionMap[clientFd].finishWriteBuf())
        {
            removePipeFd(event);
            return ;
        }
    }
}

void    webserv::readCGI(struct epoll_event& event, int type)
{
    int pipeFd = event.data.fd;
    int clientFd = _fdset[pipeFd][2];

    if (event.events & EPOLLIN)
    {
        int readStatus = _ConnectionMap[clientFd].readCgiResp(pipeFd, type);
        if (readStatus == READ_ERR)
        {
            _ConnectionMap[clientFd].join_response(502);
            removePipeFd(event);
        }
        else if (readStatus == CGI_READ_FINISH)
        {
            removePipeFd(event);
        }
    }
    else if (event.events & (EPOLLHUP | EPOLLRDHUP))
    {
        if (type == PIPE_FD_READ)
        {
            try
            {
                _ConnectionMap[clientFd].processCgiResp();
            }
            catch (const std::exception &e)
            {
                log::loginfo(T_ERR, e.what());
                _ConnectionMap[clientFd].join_response(503);
            }
            catch (...)
            {
                log::loginfo(T_ERR, "Other exception.");
                _ConnectionMap[clientFd].join_response(503);
            }
        }
        removePipeFd(event);
    }
}

void    webserv::acceptClient(struct epoll_event& event)
{
    if (!(event.events & EPOLLIN))
        return;

    struct sockaddr_in  client;
    socklen_t           client_addrlen = sizeof(client);

    int clientFd = accept(event.data.fd, reinterpret_cast<struct sockaddr *>(&client), &client_addrlen);
    if (clientFd < 0)
    {
        log::loginfo(T_ERR, "Cannot connect.");
        return ;
    }

    log::loginfo(T_INFO, "Client " + parseTools::ft_itoa(clientFd) + " " + getIp(client) + ":" + parseTools::ft_itoa(ntohs(client.sin_port)) + " is connected.");

    t_ip ipStruct;
    ipStruct.ip = getIp(client);
    ipStruct.port = parseTools::ft_itoa(ntohs(client.sin_port));

    _ipMap[clientFd] = ipStruct;

    if (fcntl(clientFd, F_SETFL, O_NONBLOCK | FD_CLOEXEC) == -1) {
        log::loginfo(T_ERR, "Client " + parseTools::ft_itoa(clientFd) + " set nonblock failed.");
        close(clientFd);
        return ;
    }
    _fdset[clientFd].push_back(CLIENT_FD);
    _fdset[clientFd].push_back(_fdset[event.data.fd][1]);  //Index of configuration
    _fdset[clientFd].push_back(event.data.fd);             //add socket fd to client
    this->addSockToEpoll(clientFd);
}

void    webserv::clientDisconnect(epoll_event& event)
{
    int             clientFd = event.data.fd;
    std::string     ip = _ipMap[clientFd].ip;
    std::string     port = _ipMap[clientFd].port;

    if (epoll_ctl(_epfd, EPOLL_CTL_DEL, clientFd, &event) == -1)
    { 
        log::loginfo(T_ERR, "Cannot remove event of client.");
    }

    log::loginfo(T_INFO, "Client " + parseTools::ft_itoa(clientFd) + " " + ip + ":" + port + " is disconnected.");

    for (std::map<int, std::vector<int> >::iterator it = _fdset.begin(); it != _fdset.end(); ++it)
    {
        if (clientFd == it->first)
        {
            _fdset.erase(clientFd);
            if (_ConnectionMap.find(clientFd) != _ConnectionMap.end())
                _ConnectionMap.erase(clientFd);
            if (_ipMap.find(clientFd) != _ipMap.end())
                _ipMap.erase(clientFd);
            if (_reqTime.find(clientFd) != _reqTime.end())
                _reqTime.erase(clientFd);
            break ;
        }
    }
    close (clientFd);
}

void    webserv::clientDisconnect(int clientFd)
{
    struct epoll_event event;

    event.events = EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLRDHUP;
    event.data.fd = clientFd;

    this->clientDisconnect(event);
}

void    webserv::removePipeFd(epoll_event& event)
{
    int pipeFd = event.data.fd;

    if (epoll_ctl(_epfd, EPOLL_CTL_DEL, pipeFd, &event) == -1)
    { 
        log::loginfo(T_ERR, "Cannot remove event of pipe.");
    }
    close (pipeFd);

    for (std::map<int, std::vector<int> >::iterator it = _fdset.begin(); it != _fdset.end(); ++it)
    {
        if (pipeFd == it->first)
        {
            _fdset.erase(pipeFd);
            break ;
        }
    }
}

int     webserv::createSock(const ServerInfo& wconf)
{
    int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0)
    {
        log::loginfo(T_ERR, "Cannot create socket.");
        return (-1);
    }
    struct sockaddr_in addr;

    int opt = 1;

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        log::loginfo(T_ERR, "Set socket option SO_REUSERADDR failed.");
        close(sockfd);
        return (-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = wconf.getHost().s_addr;
    addr.sin_port = htons(wconf.getPort());

    if (bind(sockfd, (sockaddr *)&addr, sizeof(addr)) != 0)
    {
        log::loginfo(T_ERR, "Failed to bind.");
        close(sockfd);
        return (-1);
    }

    log::logConfig(wconf);

    if (listen(sockfd, 32767) < 0)
    {
        log::loginfo(T_ERR, "Failed to listen to socket.");
        close(sockfd);
        return (-1);
    }

    return (sockfd);
}

void    webserv::addSockToEpoll(int sockfd)
{
    struct epoll_event event;

    event.events = EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLRDHUP;
    event.data.fd = sockfd;

    if (epoll_ctl(_epfd, EPOLL_CTL_ADD, sockfd, &event) == -1)
    {
        throw AnyErrorException("Error: cannot add event.");
    }
}

void    webserv::addPipeToEpoll(int pipefd, ServerInfo& conf, int clientfd, int fdtype)
{
    struct epoll_event event;

    event.events = EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLRDHUP;
    event.data.fd = pipefd;

    _fdset[pipefd].push_back(fdtype);
    _fdset[pipefd].push_back(conf.getCurIndex());
    _fdset[pipefd].push_back(clientfd);

    if (epoll_ctl(_epfd, EPOLL_CTL_ADD, pipefd, &event) == -1)
    {   
        log::loginfo(T_ERR, "Cannot add event.");
    }
}

int     webserv::createSockByConf()
{
    int sockfd;

    for (size_t i = 0; i < _confset.size(); i++)
    {
        sockfd = this->createSock(_confset[i][0]);
        if (sockfd < 0)
        {
            _fdset.clear();
            return (0);
        }
        _fdset[sockfd].push_back(SOCK_FD);
        _fdset[sockfd].push_back(i);
        this->addSockToEpoll(sockfd);
    }
    return (1);
}

void    webserv::closeFds()
{
    struct epoll_event event;

    for (std::map<int, std::vector<int> >::iterator it = _fdset.begin(); it != _fdset.end(); ++it)
    {
        event.events = EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLRDHUP;
        event.data.fd = it->first;
        if (epoll_ctl(_epfd, EPOLL_CTL_DEL, it->first, &event) == -1)
        {   
            log::loginfo(T_ERR, "Cannot remove event in epoll.");
        }
        close(it->first);
    }
    close(_epfd);
}

void    webserv::addReqTimeStamp(int clientfd)
{
    _reqTime[clientfd] = time::getTimeStamp();
}

void    webserv::handleReqTimeout()
{
    long long currentTime = time::getTimeStamp();

    std::map<int, long long>::iterator it = _reqTime.begin();
    for (; it != _reqTime.end();)
    {
        if (_ConnectionMap.find(it->first) == _ConnectionMap.end())
        {
            std::map<int, long long>::iterator tmp = it++;
            _reqTime.erase(tmp);
            continue ;
        }

        if (_ConnectionMap[it->first].readFinishStatus() == NOT_END && _ConnectionMap[it->first].reqProcessed() == NOT_PROCESSED && currentTime - _reqTime[it->first] > REQ_TIMEOUT * 1000000)
        {
            std::map<int, long long>::iterator tmp = it++;
            _ConnectionMap[tmp->first].join_response(408);
            _reqTime.erase(tmp);
        }
        else
        {
            ++it;
        }
    }
}

void    webserv::addCgiStartTime(pid_t childPid, int clientfd)
{
    _cgiTime[childPid] = time::getTimeStamp();
    _childClient[childPid] = clientfd;
}

void    webserv::handleCgiFinish()
{
    std::map<int, long long>::iterator  it = _cgiTime.begin();
    long long                           currentTime = time::getTimeStamp();

    for (; it != _cgiTime.end();)
    {
        int status;

        int waitPid = waitpid(it->first, &status, WNOHANG);
        if (waitPid == -1)
        {
            log::loginfo(T_ERR, "Cannot wait pid.");
        }
        else if (waitPid == it->first)
        {
            std::map<int, long long>::iterator  tmp = it++;
            _cgiTime.erase(tmp);
            int code = WIFEXITED(status);
            int clientFd = _childClient[waitPid];
            if (code != 0)
            {
                log::loginfo(T_INFO, "CGI for client " + parseTools::ft_itoa(clientFd) + " is terminated with success.");
            }
            else
            {
                log::loginfo(T_ERR, "CGI for client " + parseTools::ft_itoa(clientFd) + " is terminated with error.");
                _ConnectionMap[clientFd].join_response(502);
            }
            removeByKey(_childClient, waitPid);
        }
        else if (currentTime - it->second > CGI_TIMEOUT * 1000000)
        {
            pid_t   childPid = it->first;

            kill(it->first, SIGTERM);
            usleep(1000);
            kill(it->first, SIGKILL);
            
            int clientFd = _childClient[childPid];
            _ConnectionMap[clientFd].join_response(504);
            removeByKey(_childClient, childPid);
            log::loginfo(T_ERR, "CGI for client " + parseTools::ft_itoa(clientFd) + " timeout, killed.");
            std::map<int, long long>::iterator  tmp = it++;
            _cgiTime.erase(tmp);
        }
        else
        {
            ++it;
        }
    }
}


//static-------------------------------------------------------------------------------

void    webserv::handleRequestRead(std::map<int, connection>& connectionMap, struct epoll_event& event)
{
    int readStatus;
    int clientFd = event.data.fd;

    connectionMap[clientFd].readClient();
    readStatus = connectionMap[clientFd].readFinishStatus();
    if (readStatus == CLIENT_CLOSE)  // client close server should close also
    {
        connectionMap.erase(clientFd);
        this->clientDisconnect(event);
    }
    else if (readStatus == READ_ERR)  // if there is an error, send a error code to client and close
    {
        connectionMap.erase(clientFd);
        this->clientDisconnect(event);
    }
}

void    webserv::handleRequestProcess(std::map<int, connection>& connectionMap, struct epoll_event& event)
{
    int clientFd = event.data.fd;

    if (connectionMap[clientFd].reqProcessed() == NOT_PROCESSED)
    {
        try
        {
            connectionMap[clientFd].handleRequest();
        }
        catch (const std::exception &e)
        {
            log::loginfo(T_ERR, e.what());
            connectionMap[clientFd].join_response(503);
        }
        catch (...)
        {
            log::loginfo(T_ERR, "Other exception.");
            connectionMap[clientFd].join_response(503);
        }
    }
}

void    webserv::handleRequestWrite(std::map<int, connection>& connectionMap, struct epoll_event& event)
{
    int clientFd = event.data.fd;

    std::string res;
    connectionMap[clientFd].getWriteBuf(res);
    if (res != "" && event.events & EPOLLOUT)
    { 
        int byteWrite = send(clientFd, res.data(), res.size(), MSG_NOSIGNAL);
        if (byteWrite < 0)
        {
            connectionMap.erase(clientFd);
            this->clientDisconnect(event);
            return ;
        }
        else if (byteWrite == 0)
        {
            connectionMap.erase(clientFd);
            this->clientDisconnect(event);
            return ;
        }
        else
        {
            connectionMap[clientFd].updateByteWrite(byteWrite);
        }
    }
    if (connectionMap[clientFd].finishWriteBuf())
    {
        connectionMap[clientFd].reset();
        return ;
    }
}

void webserv::handleSigint(int sig)
{
    sigRecieved = sig;
}

std::string     webserv::getIp(struct sockaddr_in client)
{
    uint32_t ip = ntohl(client.sin_addr.s_addr);
    std::stringstream ip_stream;
    ip_stream << ((ip >> 24) & 0xFF) << '.'
            << ((ip >> 16) & 0xFF) << '.'
            << ((ip >> 8) & 0xFF) << '.'
            << (ip & 0xFF);

    return (ip_stream.str());
}

void    webserv::removeByKey(std::map<pid_t, int>& setMap, pid_t key)
{
    std::map<pid_t, int>::iterator it = setMap.find(key);
    if (it != setMap.end())
    {
        setMap.erase(it);
    }
}