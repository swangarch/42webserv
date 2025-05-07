#include "../../includes/connection.class.hpp"

connection::connection()  //only needed when map key
{
}

connection::connection(int clientfd, const std::vector<ServerInfo>& wconf, const t_ip &ip, webserv* server)
{
    _clientfd = clientfd;
    _wconf = wconf;
    _status = 200;
    _writeBufIndex = 0;
    _reqBuf = "";
    _respStr = "";
    _reqBody = "";
    _isCGI = 0;
    _ReqProcessed = NOT_PROCESSED;
    _readStatus = NOT_END;
    _readHeadDone = READ_HEAD_NOT_END;
    _connection = "keep-alive";
    _ip = ip;
    _cgi.reset();
    _server = server;
    _server->addReqTimeStamp(_clientfd);
}

connection::~connection()
{

}

void    connection::reset()
{
    _respStr = "";
    _status = 200;
    _writeBufIndex = 0;
    _readStatus = NOT_END;
    _readHeadDone = READ_HEAD_NOT_END;
    _reqBuf = "";
    _reqBody = "";
    _isCGI = 0;
    _ReqProcessed = NOT_PROCESSED;
    _cgiBin = "";
    _cgiResp = "";
    _cgiErr = "";
    _cgiBody = "";
    _cgi.reset();
    _req.reset();
    if (_connection != "close")
        _server->addReqTimeStamp(_clientfd);
}

int     connection::reqProcessed()
{
    return (_ReqProcessed);
}

void    connection::handleReadSuccess(char *buf, int byteRead)//, webserv& server)
{
    int addByteBuffer = 0;
    if (_readHeadDone == READ_HEAD_NOT_END) //not have full head yet
    {
        _reqBuf += std::string(buf, byteRead);
        addByteBuffer = 1;
        int posBody = request::readHeadFinish(_reqBuf);
        if (posBody > 0)  //finish reading head
        {
            log::printRequest(_reqBuf.substr(0, posBody));  //should cut body if exist

            int parseStatus = reqParser::parseHead(_reqBuf, _req, _wconf[0]); //if parse failed handle error  

            ServerInfo::matchLocation(_wconf[ServerInfo::matchServerName(_wconf, _req.getHost())], _matchconf, parseTools::getUriPath(const_cast<std::string&>(_req.getRessource())));

            log::loginfo(T_REQ,  _ip.ip + ":" + _ip.port + "  " + _req.getMethod() + " " + _req.getRessource());

            _status = parseStatus;
            if (_status >= 400)
            {
                _readStatus = PARSE_ERR;
                return ;
            }
            else if (_req.getContentLength() != "" && _matchconf.getLocation()[0].getBodySize() < static_cast<size_t>(std::atoi(_req.getContentLength().c_str())))
            {
                _status = 413;
                _readStatus = OTHER_ERR;
                return ;
            }

            _readHeadDone = READ_HEAD_FINISH;
            _server->addReqTimeStamp(_clientfd);

            if (static_cast<size_t>(posBody + 4) < (_reqBuf.size()))
                _reqBody += _reqBuf.substr(posBody + 4);

            if (_req.getMethod() == "GET" || _req.getMethod() == "DELETE" || _req.getMethod() == "HEAD")  //if it's POST, we need to continue to process
            {
                _reqBuf = _reqBody;
                _readStatus = READ_FINISH;
                return;
            }
        }
        else if (posBody == 0)
        {
            _status = 400;
            _readStatus = OTHER_ERR;
            return;
        }
        else   //not finish reading for the head
        {
            if (_reqBuf.size() > 8192) //head length is maximum 8KB
            {
                _readStatus = OTHER_ERR;
                _status = 431;
                return;
            }
            else
            {
                _readStatus = NOT_END;
                return;
            }
        }
    }
    
    if (_readHeadDone == READ_HEAD_FINISH)  //already have full head
    {
        if (addByteBuffer == 0)
            _reqBody += std::string(buf, byteRead);
        if (_req.getMethod() == "POST")
        { 
            if (_req.getTransferEncoding() == "chunked")
            {
                if (_reqBody.size() > _matchconf.getLocation()[0].getBodySize())
                {
                    _readStatus = OTHER_ERR;
                    _status = 413;
                    return ;
                }
                int pos = request::readChunkFinish(_reqBody);
                if (pos >= 0) //and check it appare at the end
                {
                    int status;
                    _reqBody = _reqBody.substr(0, pos); //put the body in the body until the end of chunk mark
                    _reqBuf = _reqBody.substr(pos); //if there are still bytes, put to request buffer for next read
                    if ((status = request::unChunkBody(_reqBody, _matchconf.getLocation()[0].getBodySize())) >= 400) {
                        _readStatus = OTHER_ERR;
                        _status = status;
                        return ;
                    }
                    _readStatus = READ_FINISH;
                    return;
                } else if (pos == -1) {
                    _readStatus = NOT_END;
                    _server->addReqTimeStamp(_clientfd);
                    return;
                } else if (pos == -2) {
                    _status = 400;
                    _readStatus = OTHER_ERR;
                    return ;
                }
            }
            else // content length is provided
            {
                int contentLength = std::atoi(_req.getContentLength().data());

                if (static_cast<unsigned int>(contentLength) > _reqBody.size()) //body not read finished
                {
                    _server->addReqTimeStamp(_clientfd);
                    _readStatus = NOT_END;
                    return ;
                }
                else if (static_cast<unsigned int>(contentLength) <= (_reqBody.size())) //read more than body, need to store the rest
                {
                    _reqBuf = _reqBody.substr(contentLength);
                    _reqBody = _reqBody.substr(0, contentLength);
                    _readStatus = READ_FINISH;
                    return;
                }
            }
        }
    }
    _status = 400;
    _readStatus = OTHER_ERR;
}

void    connection::readClient()
{
    char    buf[READ_BUFFER_SIZE];
    int     byteRead;

    byteRead = read(_clientfd, buf, READ_BUFFER_SIZE);
    if (byteRead < 0)
    {
        _readStatus = READ_ERR;
        return;
    }
    else if (byteRead == 0)
    {
        _readStatus = CLIENT_CLOSE;
        return;
    }
    else
    {
        if (_connection == "close")
            return ;
        this->handleReadSuccess(buf, byteRead);
    }
}

int    connection::readFinishStatus()
{
    return (_readStatus);
}

void         connection::handleRequest()
{
    if (_status >= 400)
    {
        this->join_response();
        return ;
    }
    std::set<std::string>   methods = _matchconf.getLocation()[0].getMethods();
    if (methods.find(_req.getMethod()) == methods.end())
    {
        this->join_response(405);
        return ;
    }

    if (_matchconf.getLocation()[0].getReturnState())
    {
        _status = _matchconf.getLocation()[0].getReturnCode();
        this->join_response();
    }
    else if (this->hasCGI())
    {
        this->handleCGI();
    }
    else if (_req.getMethod() == "GET")
    {
        this->getMethod();
    }
    else if (_req.getMethod() == "POST")
    {
        this->postMethod();
    }
    else if (_req.getMethod() == "DELETE")
    {
        this->deleteMethod();
    }
    else if (_req.getMethod() == "HEAD")
    {
        this->headMethod();
    }
    else
    {
        this->join_response(501);
    }
}

void        connection::setCgiToServer(std::vector<int>& pipeFds, pid_t childPid)
{
    _server->addPipeToEpoll(pipeFds[0], _matchconf, _clientfd, PIPE_FD_WRITE);
    _server->addPipeToEpoll(pipeFds[1], _matchconf, _clientfd, PIPE_FD_READ);
    _server->addPipeToEpoll(pipeFds[2], _matchconf, _clientfd, CGI_ERR_FD);
    _server->addCgiStartTime(childPid, _clientfd);
}

bool    connection::checkCGIPathInfo(const std::string& res, std::string& pathInfo, std::string& script) {
    std::vector<std::string>    tokens;
    std::string                 set("/");
    ConfigManager::setToken(res, set, tokens);
    size_t  i;
    for (i = 0; i < tokens.size(); ++i) {
        size_t  pos = tokens[i].rfind('.');
        if (pos == std::string::npos) {
            continue ;
        } else {
            std::string suffix = tokens[i].substr(pos);
            _cgiBin = _matchconf.getLocation()[0].getCgiPath(suffix);
            if (_cgiBin.empty()) {
                _isCGI = 0;
                break ;
            } else {
                _isCGI = 1;
                size_t  pos_suffix = res.find(suffix);
                script = res.substr(0, pos_suffix + suffix.size());
                break ;
            }
        }
    }
    for (++i; i < tokens.size(); ++i) {
        pathInfo += "/" + tokens[i];
    }
    pathInfo += (res[res.size() - 1] == '/') ? "/" : "";
    return _isCGI;
}

int     connection::hasCGI()
{
    std::string ressource = _req.getRessource();
    std::string pathInfo;
    std::string script;
    checkCGIPathInfo(ressource, pathInfo, script);
    if (_isCGI) {
        _req.setPathInfo(pathInfo);
        _req.setScript(script);
    }
    return (_isCGI);
}

void                connection::setCgiEnv(std::vector<std::string> &envStr) //complete all the cgi env vars
{
    envStr.push_back("GATEWAY_INTERFACE=CGI/1.1");
    envStr.push_back("SERVER_SOFTWARE=webserv/1.0");
    envStr.push_back("SERVER_PROTOCOL=" + _req.getProtocol());
    envStr.push_back("REQUEST_METHOD=" + _req.getMethod());
    envStr.push_back("CONTENT_TYPE=" + _req.getContentType());
    envStr.push_back("CONTENT_LENGTH=" + parseTools::ft_itoa(_reqBody.size()));
    envStr.push_back("HTTP_HOST=" + _req.getHost());
    envStr.push_back("HTTP_ACCEPT=" + _req.getAccept());
    envStr.push_back("HTTP_ACCEPT_ENCODING=" + _req.getAcceptEncoding());
    envStr.push_back("HTTP_ACCEPT_CHARSET=" + _req.getAcceptCharset());
    envStr.push_back("HTTP_ACCEPT_LANGUAGE=" + _req.getAcceptLanguage());
    envStr.push_back("HTTP_CONNECTION=" + _req.getConnection());
    envStr.push_back("HTTP_USER_AGENT=" + _req.getUserAgent());
    envStr.push_back("QUERY_STRING=" + _req.getQueryStr());
    envStr.push_back("HTTP_COOKIE=" + _req.getCookie());
    envStr.push_back("SERVER_NAME=" + _matchconf.getServerName());

    std::string port = parseTools::ft_itoa(static_cast<int>(_matchconf.getPort()));
    envStr.push_back("SERVER_PORT=" + port);
    std::string     pathUpload;
    std::string     script = _req.getScript();
    std::string     pathInfo = _req.getPathInfo();
    std::string     relPathUpload = _matchconf.getLocation()[0].getUpload();

    if (_matchconf.getLocation()[0].getAlias().empty()) {
        pathUpload = _matchconf.getLocation()[0].getRoot() + parseTools::cleanResName(relPathUpload);
        script = _matchconf.getLocation()[0].getRoot() + parseTools::cleanResName(script);
        if (!pathInfo.empty())
            pathInfo = _matchconf.getLocation()[0].getRoot() + parseTools::cleanResName(pathInfo);
    } else {
        pathUpload = _matchconf.getLocation()[0].getAlias() + parseTools::cleanResName(relPathUpload);
        request::handleAlias(script, _matchconf.getLocation()[0].getAlias(), _matchconf.getLocation()[0].getPath(), parseTools::getUriPath(script));
        if (!pathInfo.empty())
            request::handleAlias(pathInfo, _matchconf.getLocation()[0].getAlias(), _matchconf.getLocation()[0].getPath(), parseTools::getUriPath(pathInfo));
    }
    envStr.push_back("SCRIPT_NAME=" + script);
    envStr.push_back("REMOTE_ADDR=" + _ip.ip);
    envStr.push_back("REMOTE_PORT=" + _ip.port);
    envStr.push_back("REDIRECT_STATUS=200");
    envStr.push_back("UPLOAD_PATH=" + pathUpload);
    envStr.push_back("PATH_INFO=" + pathInfo);
}

void         connection::handleCGI()
{
    if (_req.getMethod() == "POST" && _matchconf.getLocation()[0].getUpload().empty())
    {
        this->join_response(403);
        return ;
    }

    std::vector<std::string>    envStr;
    std::vector<char *>         envC;
    std::string                 script;
    std::vector<char *>         cgiArgs;
    std::vector<int>            pipeFds;                            //0 for write, 1 for read, 2 for error
    std::string                 ressource = _req.getRessource();
    int                         childPid;

    this->setCgiEnv(envStr);
    cgi::toCstrTab(envStr, envC);
    script = _req.getScript();
    if (_matchconf.getLocation()[0].getAlias().empty())
        script = _matchconf.getLocation()[0].getRoot() + parseTools::cleanResName(script);
    else
        request::handleAlias(script, _matchconf.getLocation()[0].getAlias(), _matchconf.getLocation()[0].getPath(), parseTools::getUriPath(script));
    
    request::checkAccess(script, _status);
    if (_status >= 400)
    {
        this->join_response(_status);
        return ;
    }

    cgi::setCgiArgs(cgiArgs, _cgiBin, script);

    if (cgi::execCGI(cgiArgs.data(), envC.data(), pipeFds, childPid) < 0)
    {
        this->join_response(500);
    }
    else if (cgi::setCgiNonBlockPipe(pipeFds, childPid) == -1)
    {
        this->join_response(500);
    }
    else
    {
        setCgiToServer(pipeFds, childPid);
        _ReqProcessed = WAIT_CGI;
    }
}

void       connection::processCgiResp()
{
    if ( _connection == "close")
        return;

    if (_cgiErr != "")
    {
        std::string errStr;
        
        log::printCgiError(_cgiErr);
        log::loginfo(T_ERR, "CGI error: " + _cgiErr);
        this->join_response(502);
    }
    else if (_cgiResp == "")
    {
        if ( _ReqProcessed == PROCESSED)
            return;
        log::loginfo(T_ERR, "Noting returned form CGI");
        this->join_response(502);
    }
    else
    {
        if (cgi::parseCgiResp(_cgiResp, _cgi, _cgiBody, _statusInfo, _resType) == 502)
        {
            this->join_response(502);
        }
        _cgi.setOk(true);
        this->join_response(_cgiBody);
    }
}

int        connection::readCgiResp(int fd, int fdType)
{
    char    buf[READ_BUFFER_SIZE];
    int     byteRead;

    byteRead = read(fd, buf, READ_BUFFER_SIZE);
    if (byteRead < 0)
    {
        this->join_response(502);
        return (READ_ERR);
    }
    else if (byteRead == 0)   //cgi EOF means, the child process is terminated
    {
        if (fdType == PIPE_FD_READ)
        {
            try
            {
                this->processCgiResp();
            }
            catch (const std::exception &e)
            {
                log::loginfo(T_ERR, e.what());
                this->join_response(503);
            }
            catch (...)
            {
                log::loginfo(T_ERR, "Other exception.");
                this->join_response(503);
            }
        }
        return (CGI_READ_FINISH);
    }
    else
    {
        if (fdType == PIPE_FD_READ)
        {
            this->_cgiResp += std::string(buf, byteRead);
        }
        else if (fdType == CGI_ERR_FD)
        {
            this->_cgiErr += std::string(buf, byteRead);
        }
        return (NOT_END);
    }
}

int         connection::handleDirHasIndex(std::string& ressourcePath)
{
    std::string ressourceIndexPath;

    if (_matchconf.getLocation()[0].getIndex() != "") //if index is provided in config, 
    {
        ressourceIndexPath = ressourcePath + _matchconf.getLocation()[0].getIndex();
        if (access(ressourceIndexPath.data(), F_OK) == 0) // index provided in config file exist
        {
            if (access(ressourceIndexPath.data(), R_OK) != 0)
            {
                _status = 403;
            }
            std::string     body;
            this->getBody(ressourceIndexPath, body);
            this->join_response(body);
            return (1);
        }
        else
        {
            _status = 404;
            return(1);
        }
    }
    return (0);
}

void         connection::getMethod()
{
    std::string ressource;
    std::string ressourcePath;
    std::string body;

    ressource = _req.getRessource();

    if (_matchconf.getLocation()[0].getAlias().empty())
        ressourcePath = _matchconf.getLocation()[0].getRoot() + parseTools::cleanResName(ressource);
    else
        request::handleAlias(ressourcePath, _matchconf.getLocation()[0].getAlias(), _matchconf.getLocation()[0].getPath(), parseTools::getUriPath(ressource));

    int     ressourceIsDir = file::isDir(ressourcePath);
    if (ressourceIsDir == 1)  //if the ressource is a dir
    {
        parseTools::addSlashToDirEnd(ressourcePath, ressource);

        if (this->handleDirHasIndex(ressourcePath)) // handle dir has index
            return ;
        if (_matchconf.getLocation()[0].getAutoindex())
        {
            if (file::getAutoIndex(ressourcePath, ressource, body) == 0)  //set autoindex
            {
                this->join_response(body);
            }
            else
            {
                this->join_response(500);
            }
        }
        else
        {
            this->join_response(403);
        }
    }
    else if (access(ressourcePath.data(), F_OK | R_OK | X_OK) == 0 && ressourceIsDir == -1) //if syscall failed
    {
        this->join_response(500);
    }
    else
    {
        request::checkAccess(ressourcePath, _status);
        this->getBody(ressourcePath, body);
        this->join_response(body);
    }
}

void         connection::headMethod()
{
    this->getMethod();
}



void         connection::postMethod()
{
    std::string     relpath = _matchconf.getLocation()[0].getUpload();
    std::string     uploadPath;
    std::string     body;
    std::string     boundary;

    if(relpath.empty() || _req.getRessource() != "/upload")
    {
        this->join_response(403);
        return ;
    }

    if (_req.getContentType().substr(0, 19) == "multipart/form-data")
    {
        if (file::getBoudary(_req.getContentType(), boundary) == -1)
        {
            this->join_response(400);
            return ;
        }

        if (_matchconf.getLocation()[0].getAlias().empty())
            uploadPath  = _matchconf.getLocation()[0].getRoot() + parseTools::cleanResName(relpath);
        else
        {
            this->join_response(403);
            return ;
        }
        int     uploadStatus = file::handleUpload(_reqBody, uploadPath, boundary);
        if (uploadStatus == -1)
        { 
            this->join_response(400);
            return ;
        }
        else if (uploadStatus == -2)
        {
            this->join_response(500);
            return ;
        }
        else
        {            
            infopage::getInfoPage(body, "File uploaded and saved successfully");
            _status = 201;
            this->join_response(body);
            return ;
        }
    }
    else
    {
        this->join_response(415);
    }
}
        
void         connection::deleteMethod()
{
    std::string ressource = _req.getRessource();
    std::string ressourcePath;
    std::string body;

    if (_matchconf.getLocation()[0].getAlias().empty())
        ressourcePath = _matchconf.getLocation()[0].getRoot() + parseTools::cleanResName(ressource);
    else
        request::handleAlias(ressourcePath, _matchconf.getLocation()[0].getAlias(), _matchconf.getLocation()[0].getPath(), parseTools::getUriPath(ressource));

    if (access(ressourcePath.c_str(), F_OK) != 0)
    {
        _status = 404;
    }
    if (_status >= 400)
    {
        this->join_response();
        return ;
    }

    int     fileStatus = file::isDir(ressourcePath);
    if (fileStatus == 1)
    {
        if (file::delDir(ressourcePath) == 0)
        {
            _status = 204;
            infopage::getInfoPage(body, "The directory is successfully deleted.");
            this->join_response(body);
        }
        else
        {
            if (errno == EACCES || errno == EPERM) {
                join_response(403);
            } else if (errno == ENOTEMPTY) {
                join_response(409);
            } else {
                join_response(500);
            }
        }
    }
    else if (fileStatus == 0)
    {
        if (std::remove(ressourcePath.data()) == 0)
        {
            _status = 204;
            infopage::getInfoPage(body, "The ressource is successfully deleted.");
            this->join_response(body);
        }
        else
        {
            if (errno == ENOENT)
            {
                this->join_response(404);
            }
            else if (errno == EACCES)
            {
                this->join_response(403);
            }
            else
            {
                this->join_response(500);
            }
        }
    }
    else
    {
        this->join_response(500);
    }
}

std::string         connection::getErrorPage()
{
    std::string pathErrorPage;
    std::string errorStr;

    if (_matchconf.getLocation().size() == 0)
        return (infopage::getDefaultErrorPage(_status, _statusInfo));

    if (_matchconf.getLocation()[0].getErrorPages(static_cast<short>(_status)) == "")
        return (infopage::getDefaultErrorPage(_status, _statusInfo));

    std::string resErroPage = _matchconf.getLocation()[0].getErrorPages(static_cast<short>(_status));

    if (_matchconf.getLocation()[0].getAlias().empty())
        pathErrorPage = _matchconf.getLocation()[0].getRoot() + parseTools::cleanResName(resErroPage);
    else
        request::handleAlias(pathErrorPage, _matchconf.getLocation()[0].getAlias(), _matchconf.getLocation()[0].getPath(), parseTools::getUriPath(resErroPage));

    if (access(pathErrorPage.data(), F_OK | R_OK) != 0)
        return (infopage::getDefaultErrorPage(_status, _statusInfo));
    
    file::readFile(pathErrorPage, errorStr);
    return (errorStr);
}

void         connection::getBody(std::string& path, std::string& body)
{
    if (_status >= 400)
    {
        body = this->getErrorPage();
        return ;
    }
    if (file::readFile(path, body) == 1)
        return;
    else
    {
        _status = 500;
        body = this->getErrorPage();
        return ;
    }
}

void         connection::join_response(std::string& body)
{
    if (_ReqProcessed == PROCESSED || _connection == "close")
        return ;

    if (_cgi.getOk() && _cgi.getStatus() != 0)
    {
        _status = _cgi.getStatus();
    }

    std::stringstream ss;
    std::string type;

    request::setContentType(type, _status, _req, _cgi, _resType);
    request::setConnection(_status, _connection, _req.getConnection());

    ss << "HTTP/1.1" << " " << _status << " " << infopage::getStatusMsg(_status, _statusInfo) << "\r\n"
       << "Date: " << time::getFormattedTime() << "\r\n"
       << "Server: " << "Webserv/1.0 (Linux)" << "\r\n"
       << "Content-Length: " << body.size() << "\r\n"
       << "Content-Type: " << type << "\r\n"
       << "Connection: " << _connection << "\r\n";

    request::setLocation(_matchconf, _cgi, ss);
    request::setCookie(_cgi, ss);

    ss << "\r\n";

    _respStr = ss.str();

    log::printResponse(_respStr);

    if (_req.getMethod() != "HEAD")
        _respStr += body;

    log::logResp(_matchconf, _status, infopage::getStatusMsg(_status, _statusInfo));

    _ReqProcessed = PROCESSED; 
    _writeBufIndex = 0;
}

void         connection::join_response()
{
    std::string body = "";

    if (_status >= 400)
    {
        body = this->getErrorPage();
    }
    this->join_response(body);
}

void         connection::join_response(int status)
{
    std::string body = "";

    _status = status;
    if (_status >= 400)
    {
        body = this->getErrorPage();
    }
    this->join_response(body);
}

void    connection::getWriteBuf(std::string& res)
{
    if (_ReqProcessed == PROCESSED)
    {
        if (static_cast<size_t>(_writeBufIndex) >= _respStr.size())
        return;

        res = _respStr.substr(_writeBufIndex, WRITE_BUFFER_SIZE);
    }
    else if (_ReqProcessed == WAIT_CGI)
    {
        if (static_cast<size_t>(_writeBufIndex) >= _reqBody.size())
        return;

        res = _reqBody.substr(_writeBufIndex, WRITE_BUFFER_SIZE);
    }
}

void     connection::updateByteWrite(int byteWrite)
{
    _writeBufIndex += byteWrite;
}

bool    connection::finishWriteBuf()
{  
    if (_ReqProcessed == PROCESSED)
    {
        if (static_cast<size_t>(_writeBufIndex) >= _respStr.size())
        {
            return (true);
        }
        return (false);
    }
    else if (_ReqProcessed == WAIT_CGI)
    {
        if (static_cast<size_t>(_writeBufIndex) >= _reqBody.size())
        {
            return (true);
        }
        return (false);
    }
    return (false);
}

int     connection::getConnection()
{
    if (_connection == "close")
        return (0);
    return (1);
}

////////////////////////////////////////////////STATIC////////////////////////////////////////////////////////////////////////////////

std::map<std::string, std::string>      connection::_resType = infopage::initResType();
std::map<int, std::string>              connection::_statusInfo = infopage::initStatusInfo();