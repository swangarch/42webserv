#ifndef WEBSERV_CLASS_HPP
#define WEBSERV_CLASS_HPP

class   connection;
class   request;

#include "standardLib.hpp"
# include "ServerInfo.hpp"
# include "define.hpp"
# include "connection.class.hpp"
# include "log.class.hpp"
# include "reqParser.class.hpp"
# include "file.class.hpp"
# include "infopage.class.hpp"

class   webserv
{
    private:
        webserv();
        webserv(const webserv& other);
        webserv& operator=(const webserv& other);

        std::vector<std::vector<ServerInfo> >      _confset;
        int                                 _epfd;
        std::map<int, std::vector<int> >    _fdset;
        std::map<int, connection>           _ConnectionMap;
        std::map<pid_t, long long>          _cgiTime;
        std::map<pid_t, int>                _childClient;
        std::map<int, t_ip>                 _ipMap;
        std::map<int, long long>            _reqTime;

        int     createSock(const ServerInfo& wconf);
        void    addSockToEpoll(int sockfd);
        int     createSockByConf();
        void    acceptClient(struct epoll_event& event);
        void    listenSockets();
        void    eventHandle(struct epoll_event& event, std::vector<ServerInfo> &wconf);
        void    writeCGI(struct epoll_event& event);
        void    readCGI(struct epoll_event& event, int type);
        void    closeFds();

    public:
        webserv(std::vector<std::vector<ServerInfo> >&   confset);
        ~webserv();

        void    runWebserv();
        void    addPipeToEpoll(int pipefd, ServerInfo& conf, int clientfd, int fdtype);
        void    removePipeFd(epoll_event& event);
        void    addCgiStartTime(pid_t childPid, int clientfd);
        void    addReqTimeStamp(int clientfd);
        void    handleReqTimeout();
        void    handleCgiFinish();
        void    clientDisconnect(epoll_event& event);
        void    clientDisconnect(int clientFd);

        //static----------------------------------------------------------------------------------
        void    handleRequestRead(std::map<int, connection>& connectionMap, struct epoll_event& event);
        void    handleRequestProcess(std::map<int, connection>& connectionMap, struct epoll_event& event);
        void    handleRequestWrite(std::map<int, connection>& connectionMap, struct epoll_event& event);
        static void    handleSigint(int sig);
        static std::string     getIp(struct sockaddr_in client);
        static void    removeByKey(std::map<pid_t, int>& setMap, pid_t key);
};  

#endif