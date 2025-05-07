#include "../../includes/cgi.class.hpp"

cgi::cgi()
{
    _ok = false;
    _contentType = "";
    _status = 0;
    _location = "";
    _cookie.clear();
}

cgi::~cgi()
{

}

void    cgi::reset()
{
    _ok = false;
    _contentType = "";
    _status = 0;
    _location = "";
    _cookie.clear();
}

//getter Setter----------------------------------------------------------------------------

bool cgi::getOk() const { return _ok; }

int cgi::getStatus() const { return _status; }
        
const std::string& cgi::getContentType() const { return _contentType; }
        
const std::string& cgi::getLocation() const { return _location; }
        
std::vector<std::string>& cgi::getCookie() { return _cookie; }

// Setter
        
void cgi::setOk(bool ok) { _ok = ok; }
        
void cgi::setStatus(int status) { _status = status; }
        
void cgi::setContentType(const std::string& type) { _contentType = type; }
        
void cgi::setLocation(const std::string& location) { _location = location; }
        
void cgi::setCookie(const std::vector<std::string>& cookie) { _cookie = cookie; }

//static ----------------------------------------------------------------------------------

void        cgi::toCstrTab(std::vector<std::string>& envStr, std::vector<char *>& envC)
{
    for (size_t i = 0; i < envStr.size(); i++)
        envC.push_back(const_cast<char *>(envStr[i].data()));
    envC.push_back(NULL);
}

int          cgi::setCgiNonBlockPipe(std::vector<int>& pipeFds, int childPid)
{
    for (size_t i = 0; i < pipeFds.size(); i++)
    {
        if (fcntl(pipeFds[0], F_SETFL, O_NONBLOCK | FD_CLOEXEC) == -1) {
            log::loginfo(T_ERR, "Fcntl can't set flag");
            close(pipeFds[0]);
            close(pipeFds[1]);
            close(pipeFds[2]);
            kill(childPid, SIGTERM);
            usleep(1000);
            kill(childPid, SIGKILL);
            return (-1);
        }
    }
    return (1);
}

void        cgi::setCgiArgs(std::vector<char *>& cgiArgs, const std::string& cgiBin, const std::string& script)
{
    cgiArgs.push_back(const_cast<char *>(cgiBin.data()));
    cgiArgs.push_back(const_cast<char *>(script.data()));
    cgiArgs.push_back(NULL);
}

int    cgi::execCGI(char** cgiArgs, char** env,  std::vector<int>& pipeFds, int& childPid)
{
    int pipfdIn[2];
    int pipfdOut[2];
    int pipfdErr[2];

    if (access(cgiArgs[0], F_OK | X_OK) != 0)
    {
        log::loginfo(T_ERR, "Cannot open CGI program.");
        return (-1);
    }

    if (pipe(pipfdIn) == -1)
    {
        log::loginfo(T_ERR, "Failed to create pipe.");
        return (-1);
    }

    if (pipe(pipfdOut) == -1)
    {
        log::loginfo(T_ERR, "Failed to create pipe.");
        close(pipfdIn[0]);
        close(pipfdIn[1]);
        return (-1);
    }

    if (pipe(pipfdErr) == -1)
    {
        close(pipfdIn[0]);
        close(pipfdIn[1]);
        close(pipfdOut[0]);
        close(pipfdOut[1]);
        log::loginfo(T_ERR, "Failed to create pipe.");
        return (-1);
    }

    int pid = fork();
    if (pid < 0)
    {
        close(pipfdIn[0]);
        close(pipfdIn[1]);
        close(pipfdOut[0]);
        close(pipfdOut[1]);
        close(pipfdErr[0]);
        close(pipfdErr[1]);
        log::loginfo(T_ERR, "Faild to fork for CGI.");
        return (-1);
    }
    if (pid == 0)
    {
        close(pipfdIn[1]);
        close(pipfdOut[0]);
        close(pipfdErr[0]);

        dup2(pipfdIn[0], STDIN_FILENO); 
        dup2(pipfdOut[1], STDOUT_FILENO);
        dup2(pipfdErr[1], STDERR_FILENO);

        close(pipfdIn[0]);
        close(pipfdOut[1]);
        close(pipfdErr[1]);

        signal(SIGPIPE, SIG_IGN);
        execve(cgiArgs[0], cgiArgs, env);
        log::loginfo(T_ERR, "Faild to execute CGI.");
        return (-1);
    }   
    else
    {
        childPid = pid;
        close(pipfdIn[0]);
        close(pipfdOut[1]);
        close(pipfdErr[1]);

        pipeFds.push_back(pipfdIn[1]);
        pipeFds.push_back(pipfdOut[0]);
        pipeFds.push_back(pipfdErr[0]);

        return (1);
    }
}

// below cgi parsing---------------------------------

bool    cgi::checkUri(std::string token) {
    if (!reqParser::isValidUriChar(token)) {
        return false;
    }
    size_t  pos = 0;
    size_t  end = 0;
    if ((pos = token.find("http://", pos)) != std::string::npos) {
        if ((end = token.find("/", pos + 7)) != std::string::npos) {
            std::string authority = token.substr(pos + 7, end - pos - 7);
            token = token.substr(end);
            if ((pos = authority.find(':')) != std::string::npos) {
                std::string host = authority.substr(0, pos);
                std::string port = authority.substr(pos + 1);

                if (!(ServerInfo::isValidIp(host) || ServerInfo::isValidDomain(host))) {
                    return false;
                }

                if (port.empty() || port.size() > 5) {
                    return false;
                }
                std::stringstream   ss(port);
                int                 port_num = 0;
                ss >> port_num;
                if (ss.fail() || !ss.eof()) {
                    return false;
                }
                if (port_num < 1 || port_num > 65535) {
                    return false;
                }
            } else {
                if (!(ServerInfo::isValidIp(authority) || ServerInfo::isValidDomain(authority))) {
                    return false;
                }
            }
        } else {
            return false;
        }
    }
    std::string no_query = parseTools::getUriPath(token);
    if (no_query.empty() || no_query[0] != '/' || no_query.find("..") != std::string::npos) {
        return false;
    }
    return true;
}

void        cgi::trimStr(std::string& value)
{
    size_t  begin;
    for (begin = 0; begin < value.size(); ++begin) {
        if (value[begin] != ' ') {
            break ;
        }
    }
    value = value.substr(begin);
    for (begin = value.size(); begin > 0; --begin) {
        if (value[begin - 1] != ' ') {
            break ;
        }
    }
    value = value.substr(0, begin);
}

void        cgi::parseColonSepLine(const std::string& line, std::string& key, std::string& value)
{
    size_t pos_colon = line.find(":");
    if (pos_colon != std::string::npos && pos_colon != 0)
    {
        key = line.substr(0, pos_colon);
        value = line.substr(pos_colon + 1);
        trimStr(value);
    }
}

bool        cgi::isValidMimeType(std::string type, std::map<std::string, std::string>& resType)
{
    size_t      pos = type.find(";");
    if (pos != std::string::npos)
    {
        type = type.substr(0, pos);
    }

    std::map<std::string, std::string>::iterator it = resType.begin();

    for (; it != resType.end(); ++it)
    {
        if (it->second == type)
            return (true);
    }
    return (false);
}

bool       cgi::checkSetCookie(std::string line)
{
    std::stringstream ss(line);
    std::string token;
    int         count = 0;

    while (std::getline(ss, token, ';'))
    {
        trimStr(token);
        size_t pos = token.find('=');
        if (pos != std::string::npos)
        {
            if (pos == 0 )
                return false;
            if (token.substr(0, 5) == "Path=")
            {
                std::string     path = token.substr(5);
                if (path.empty() || path[0] != '/' || path.find("..") != std::string::npos || !reqParser::isValidUriChar(path))
                {
                    return false;
                }
            }
            else if (token.substr(0, 8) == "Max-Age=")
            {
                std::string     age = token.substr(8);
                if (age[0] == '0' && age.size() > 1)
                {
                    return false;
                }
                for (size_t i = 0; i < age.size(); i++)
                {
                    if (!std::isdigit(age[i]))
                    {
                        return false;
                    }
                }
                std::stringstream   age_ss(age);
                int                 age_num;
                age_ss >> age_num;
                if (age_ss.fail() || !age_ss.eof())
                {
                    return false;
                }
            }
            else
            {
                ++count;
            }
        }
    }
    if (count > 0)
    {
        return (true);
    }
    return (false);
}

int        cgi::setCgiHead(cgi& cgi, std::string& key, std::string& value, std::map<int, std::string>& statusInfo, std::map<std::string, std::string>& resType)
{
    if (key == "" || value == "")
    {
        log::loginfo(T_ERR, "Bad CGI response head.");
        return (502);
    }

    if (key == "Status")
    {
        if (cgi.getStatus() != 0)
        {
            log::loginfo(T_ERR, "Repeat CGI response status.");
            return (502);
        }

        std::string code = value.substr(0,3);
        std::string info;
        int         intCode;

        if (code.size() < 3)
        {
            log::loginfo(T_ERR, "Bad CGI response status.");
            return (502);
        }
        for (size_t i = 0; i < code.size(); i++)
        {
            if (!std::isdigit(code[i]))
            {
                log::loginfo(T_ERR, "Bad CGI response status.");
                return (502);
            }
        }
        if (code[0] == '0')
        {
            return (502);
        }
        intCode = std::atoi(code.c_str());

        if (intCode < 100 || intCode > 599)
        {
            log::loginfo(T_ERR, "Bad CGI response status code.");
            return (502);
        }
        if (value.size() == 4)
        {
            log::loginfo(T_ERR, "Bad CGI response status.");
            return (502);
        }
        if (value.size() > 4)
        {
            if (value[3] != ' ')
            {
                log::loginfo(T_ERR, "Bad CGI response status.");
                return (502);
            }
            info = value.substr(4);
        }
        if (info != "" && statusInfo[intCode] != info)
        {
            log::loginfo(T_ERR, "Bad CGI response status.");
            return (502);
        }
        else if  (info == "" && statusInfo.find(intCode) == statusInfo.end())
        {
            log::loginfo(T_ERR, "Bad CGI response status code.");
            return (502);
        }
        cgi.setStatus(intCode);
    }
    else if (key == "Content-Type")
    {
        if (cgi.getContentType() != "")
        {
            log::loginfo(T_ERR, "Repeat CGI content type.");
            return (502);
        }

        if (isValidMimeType(value, resType))
        {
            cgi.setContentType(value);
        }
        else
        {
            log::loginfo(T_ERR, "Bad CGI content type.");
            return (502);
        }
    }
    else if (key == "Set-Cookie")
    {
        if (!checkSetCookie(value))
        {
            log::loginfo(T_ERR, "Bad CGI set cookie.");
            return (502);
        }
        cgi.getCookie().push_back(value); 
    }
    else if (key == "Location")
    {
        if (cgi.getLocation() != "")
        {
            log::loginfo(T_ERR, "Repeat CGI location.");
            return (502);
        }
        if (!checkUri(value))
        {
            log::loginfo(T_ERR, "Bad CGI location uri.");
            return (502);
        }
        cgi.setLocation(value);
    }
    return (200);
}

int        cgi::parseCgiResp(std::string& cgiResp, cgi& cgi, std::string& cgiBody, std::map<int, std::string>& statusInfo, std::map<std::string, std::string>& resType)
{
    size_t              pos;
    size_t              startPos = 0;
    std::string         line;
    std::string         key;
    std::string         value;
    bool                findEmptyLine = false;

    pos = cgiResp.find("\r\n", startPos);
    while (pos != std::string::npos)
    {
        line = cgiResp.substr(startPos, pos - startPos);

        if (line.size() == 0)
        {
            findEmptyLine = true;
            break;
        }

        parseColonSepLine(line, key, value);
        if (setCgiHead(cgi, key, value, statusInfo, resType) == 502)
        {
            return (502);
        }        
        startPos = pos + 2;
        pos = cgiResp.find("\r\n", startPos);
    }

    cgiBody = cgiResp.substr(pos + 2);

    if (cgi.getContentType() == "" || findEmptyLine == false)
    {
        return (502);
    }
    return (200);
}


