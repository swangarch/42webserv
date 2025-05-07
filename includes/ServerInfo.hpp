#pragma once

# include "standardLib.hpp"
# include "define.hpp"
# include "Location.hpp"
# include "AnyErrorException.hpp"

class   Location;

class   ServerInfo {
public:
    ServerInfo();
    ServerInfo(const ServerInfo& src);
    ~ServerInfo();

    ServerInfo& operator=(const ServerInfo& src);

    void    setPort(std::string src);
    void    setHost(const std::string& src);
    void    setServerName(std::string src);
    void    setRoot(std::string src);
    void    setBodySize(std::string src);
    void    setAutoindex(std::string src);
    void    setIndex(std::string src);
    void    setCurIndex(size_t index);
    void    setErrorPage(std::vector<std::string>& token, size_t *i);
    void    setMethods(std::vector<std::string>& token, size_t *i);
    void    setMethods();
    void    setCgiExt(std::vector<std::string>& token, size_t *i);
    void    setLocation(std::vector<std::string>& token, size_t *i);
    
    uint16_t    getPort() const;
    struct in_addr  getHost() const;
    const std::string&  getIp() const;
    const std::string&  getIpPort() const;
    const std::string&  getServerName() const;
    const std::string&  getRoot() const;
    size_t  getBodySize() const;
    bool    getAutoindex() const;
    const std::string&  getIndex() const;
    struct sockaddr_in  getAddress() const;
    size_t  getCurIndex() const;
    const std::map<short, std::string>& getErrorPages() const;
    const std::string getErrorPages(const short key) const;
    const std::set<std::string>&    getMethods() const;
    std::vector<Location>::iterator    getLocation(const std::string& key);
    const std::vector<Location>&    getLocation() const;
    const std::set<std::string>& getCgiExt() const;
    const std::map<std::string, std::string>&   getCgiPath() const;
    const std::string   getCgiPath(const std::string& src) const;

    void    printServerinfo() const;
    
    static void     checkToken(std::string& src);
    static bool     isValidMethod(const std::string& src);
    static bool     isValidCgiExt(const std::string& src);
    static bool     isValidCgiPath(const std::string& src);
    static void     matchLocation(const ServerInfo& config, ServerInfo& match, const std::string& path);
    static size_t   matchServerName(const std::vector<ServerInfo>& wconf, const std::string& name);
    static bool     isValidIp(const std::string& src);
    static bool     isValidDomain(const std::string& src);
    static bool     isValidLocPath(const std::string& src);

private:
    uint16_t                            _port;
    struct in_addr                      _host;
    std::string                         _ip;
    std::string                         _ipPort;
    std::string                         _server_name;
    std::string                         _root;
    size_t                              _client_max_body_size;
    bool                                _autoindex;
    std::string                         _index;
    std::map<short, std::string>        _error_pages;
    std::vector<Location>               _location;
    std::set<std::string>               _location_path;
    struct sockaddr_in                  _address;
    size_t                              _cur_index;
    std::set<std::string>               _methods;
    std::set<std::string>               _cgi_ext;
    std::map<std::string, std::string>  _cgi_path;

    bool    isValidRootPath(std::string& src) const;
    void    setCgiPath(std::string& ext, std::string& path);
    void    setLocation(const std::string& path, std::vector<std::string>& src);
};
