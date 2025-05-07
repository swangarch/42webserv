#ifndef CONNECTION_CLASS_HPP
#define CONNECTION_CLASS_HPP

class   request;
class   webserv;

#include "standardLib.hpp"
# include "cgi.class.hpp"
# include "request.class.hpp"
# include "webserv.class.hpp"
# include "ServerInfo.hpp"
# include "define.hpp"
# include "ConfigManager.hpp"

class   connection
{
    private:

        request             _req;
        int                 _clientfd;
        std::vector<ServerInfo>          _wconf;
        ServerInfo          _matchconf;
        std::string         _reqBuf;
        std::string         _respStr;
        std::string         _reqBody;
        int                 _readStatus; 
        int                 _readHeadDone;
        int                 _isCGI;    
        std::string         _cgiBin;
        int                 _ReqProcessed;
        int                 _status;
        std::string         _connection;
        int                 _writeBufIndex;
        std::string         _cgiResp;
        std::string         _cgiErr;
        cgi                 _cgi;
        std::string         _cgiBody;
        t_ip                _ip;
        webserv*            _server;

        static std::map<std::string, std::string>   _resType;
        static std::map<int, std::string>           _statusInfo;
        
        //read-------------------------------------------
        void                handleReadSuccess(char *buf, int byteRead);
        //read-------------------------------------------
        
        //http request----------------------------------------
        void                getMethod();
        void                headMethod();
        void                postMethod();
        void                deleteMethod();
        void                handleCGI();
        //http request----------------------------------------
        
        //cgi--------------------------------------------------
        int                 hasCGI();
        void                setCgiEnv(std::vector<std::string> &envStr);
        bool                checkCGIPathInfo(const std::string& res, std::string& pathInfo, std::string& script);
        //cgi--------------------------------------------------
        
        //http response----------------------------------------
        int                 handleDirHasIndex(std::string& ressourcePath);
        void                getBody(std::string& path, std::string& body);
        std::string         getErrorPage();
        void                join_response(std::string& body);
        void                join_response();
        //http response----------------------------------------
        
        public:
        connection();
        connection(int clientfd, const std::vector<ServerInfo>& wconf, const t_ip &ip, webserv* server);
        ~connection();

        void                reset();

        //read-------------------------------------------
        void                readClient();
        int                 readFinishStatus();
        //read-------------------------------------------

        //http request----------------------------------------
        void                handleRequest();
        //http request----------------------------------------

        //cgi--------------------------------------------------
        int                 readCgiResp(int fd, int fdType);
        void                processCgiResp();
        void                setCgiToServer(std::vector<int>& pipeFds, pid_t childPid);
        //cgi--------------------------------------------------

        //http response----------------------------------------
        void                join_response(int status);
        int                 reqProcessed();
        //http response----------------------------------------

        //write-------------------------------------------
        void                updateByteWrite(int byteWrite);
        void                getWriteBuf(std::string& res);
        bool                finishWriteBuf();
        int                 getConnection();
        //write-------------------------------------------
};

#endif