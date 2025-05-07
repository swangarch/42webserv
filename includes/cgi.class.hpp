#ifndef CGI_CLASS_HPP
#define CGI_CLASS_HPP

#include "standardLib.hpp"
#include "log.class.hpp"

class cgi
{
    private:
    bool                        _ok;
    int                         _status;
    std::string                 _contentType;
    std::string                 _location;
    std::vector<std::string>    _cookie;
    
    static bool                checkUri(std::string token);
    static void        parseColonSepLine(const std::string& line, std::string& key, std::string& value);
    static bool        isValidMimeType(std::string type, std::map<std::string, std::string>& resType);
    static int         setCgiHead(cgi& cgi, std::string& key, std::string& value, std::map<int, std::string>& statusInfo, std::map<std::string, std::string>& resType);
    static void        trimStr(std::string& value);
    static bool        checkSetCookie(std::string line);

    public:
    cgi();
    ~cgi();
    
    void reset();
    
    //getter setter ---------------------------------------------------------
    
    bool                        getOk() const;
    int                         getStatus() const;
    const std::string&          getContentType() const;
    const std::string&          getLocation() const;
    std::vector<std::string>&   getCookie();
    
    // Setter
    void setOk(bool ok);
    void setStatus(int status);
    void setContentType(const std::string& type);
    void setLocation(const std::string& location);
    void setCookie(const std::vector<std::string>& cookie);
    
    //static ----------------------------------------------------------------
    
    static void        toCstrTab(std::vector<std::string>& envStr, std::vector<char *>& envC);
    static int         setCgiNonBlockPipe(std::vector<int>& pipeFds, int childPid);
    static void        setCgiArgs(std::vector<char *>& cgiArgs, const std::string& cgiBin, const std::string& script);
    static int         execCGI(char** cgiArgs, char** env,  std::vector<int>& pipeFds, int& childPid);
    static int         parseCgiResp(std::string& cgiResp, cgi& cgi, std::string& cgiBody, std::map<int, std::string>& statusInfo, std::map<std::string, std::string>& resType);
};
#include "reqParser.class.hpp"

#endif