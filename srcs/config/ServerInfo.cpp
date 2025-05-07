#include "../../includes/ServerInfo.hpp"

ServerInfo::ServerInfo()
    :   _port(0),
        _host(),
        _ip("0.0.0.0"),
        _ipPort(""),
        _server_name(""),
        _root(""),
        _client_max_body_size(MAX_BODY_LENGTH),
        _autoindex(false),
        _index(""),
        _error_pages(),
        _location(),
        _location_path(),
        _cur_index(0),
        _methods(),
        _cgi_ext(),
        _cgi_path() {
            _host.s_addr = INADDR_ANY;
            std::memset(&_address, 0, sizeof(_address));
            _address.sin_family = AF_INET;
            _address.sin_addr.s_addr = INADDR_ANY;
            _address.sin_port = 0;
}

ServerInfo::~ServerInfo() {
}

ServerInfo::ServerInfo(const ServerInfo& src) {
    *this = src;
}

ServerInfo& ServerInfo::operator=(const ServerInfo& src) {
    if (this != &src) {
        _port = src._port;
        _host = src._host;
        _ip = src._ip;
        _ipPort = src._ipPort;
        _server_name = src._server_name;
        _root = src._root;
        _client_max_body_size = src._client_max_body_size;
        _autoindex = src._autoindex;
        _index = src._index;
        _error_pages = src._error_pages;
        _location = src._location;
        _location_path = src._location_path;
        _address = src._address;
        _cur_index = src._cur_index;
        _methods = src._methods;
        _cgi_ext = src._cgi_ext;
        _cgi_path = src._cgi_path;
    }
    return *this;
}

uint16_t    ServerInfo::getPort() const {
    return _port;
}

struct in_addr  ServerInfo::getHost() const {
    return _host;
}

const std::string&  ServerInfo::getIp() const {
    return _ip;
}

const std::string&  ServerInfo::getIpPort() const {
    return _ipPort;
}

const std::string&  ServerInfo::getServerName() const {
    return _server_name;
}

const std::string&  ServerInfo::getRoot() const {
    return _root;
}

size_t  ServerInfo::getBodySize() const {
    return _client_max_body_size;
}

bool    ServerInfo::getAutoindex() const {
    return _autoindex;
}

const std::string&  ServerInfo::getIndex() const {
    return _index;
}

struct sockaddr_in  ServerInfo::getAddress() const {
    return _address;
}

size_t  ServerInfo::getCurIndex() const {
    return _cur_index;
}

const std::map<short, std::string>& ServerInfo::getErrorPages() const {
    return _error_pages;
}

const std::string ServerInfo::getErrorPages(const short key) const {
    std::map<short, std::string>::const_iterator it = _error_pages.find(key);
    if (it == _error_pages.end()) {
        return "";
    }
    return it->second;
}

const std::set<std::string>&    ServerInfo::getMethods() const {
    return _methods;
}

std::vector<Location>::iterator    ServerInfo::getLocation(const std::string& key) {
    std::vector<Location>::iterator   it = _location.begin();
    for (; it != _location.end(); ++it) {
        if (it->getPath() == key) {
            return it;
        }
    }
    // throw AnyErrorException("Path of location not found!");
    return it;
}

const std::vector<Location>&    ServerInfo::getLocation() const {
    return _location;
}

const std::set<std::string>& ServerInfo::getCgiExt() const {
    return _cgi_ext;
}

const std::map<std::string, std::string>&   ServerInfo::getCgiPath() const {
    return _cgi_path;
}

const std::string   ServerInfo::getCgiPath(const std::string& src) const {
    if (src.empty())
        return ("");
    std::map<std::string, std::string>::const_iterator  it = _cgi_path.find(src);
    if (it != _cgi_path.end()) {
        return it->second;
    } else {
        return "";
    }
}

void    ServerInfo::setPort(std::string src) {
    checkToken(src);
    std::string     ip_part;
    std::string     port_part;
    size_t          pos = src.find(':');
    if (pos != std::string::npos) {
        ip_part = src.substr(0, pos);
        port_part = src.substr(pos + 1);
        _ipPort = src;
    } else {
        port_part = src;
        _ipPort = "0.0.0.0:" + src;
    }
    if (port_part.empty() || port_part.size() > 5) {
        throw AnyErrorException("Invalid port!");
    }
    if (port_part[0] == '+' || (port_part[0] == '0' && port_part.size() > 1)) {
        throw AnyErrorException("Invalid port!");
    }
    std::stringstream   ss(port_part);
    int                 port = 0;
    ss >> port;
    if (ss.fail() || !ss.eof()) {
        throw AnyErrorException("Invalid port!");
    }
    if (port < 1 || port > 65535) {
        throw AnyErrorException("Invalid port");
    }
    if (!ip_part.empty()) {
        setHost(ip_part);
    }
    _port = static_cast<uint16_t>(port);
    _address.sin_port = htons(_port);
}

void    ServerInfo::setHost(const std::string& src) {
    if (!isValidIp(src)) {
        throw AnyErrorException("Invalid IP address!");
    }
    _ip = src;
    struct addrinfo hints, *result;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICHOST;
    if (getaddrinfo(_ip.c_str(), NULL, &hints, &result) != 0) {
        throw AnyErrorException("Function getaddrinfo failed!");
    }
    struct sockaddr_in  *addr = reinterpret_cast<struct sockaddr_in *>(result->ai_addr);
    _host = addr->sin_addr;
    _address.sin_addr = _host;
    freeaddrinfo(result);
}

void    ServerInfo::setServerName(std::string src) {
    checkToken(src);
    std::string::iterator   it;
    for (it = src.begin(); it != src.end(); ++it) {
        *it = std::tolower(*it);
    }
    if (!isValidDomain(src)) {
        throw AnyErrorException("Invalid domain!");
    }
    _server_name = src;
}

void    ServerInfo::setRoot(std::string src) {
    checkToken(src);
    if (!isValidRootPath(src)) {
        throw AnyErrorException("Invalid root path!");
    }
    _root = src;
}

void    ServerInfo::setBodySize(std::string src) {
    checkToken(src);
    int                 size = 0;
    std::stringstream   ss(src);
    ss >> size;
    if (ss.fail() || !ss.eof()) {
        throw AnyErrorException("Invalid client body size!");
    }
    if (size < 0 || size > MAX_BODY_LENGTH) {
        throw AnyErrorException("Invalid Client body size!");
    }
    _client_max_body_size = static_cast<size_t>(size);
}

void    ServerInfo::setAutoindex(std::string src) {
    checkToken(src);
    if (src == "on") {
        _autoindex = true;
    } else if (src == "off") {
        _autoindex = false;
    } else {
        throw AnyErrorException("Invalid autoindex!");
    }
}

void    ServerInfo::setIndex(std::string src) {
    checkToken(src);
    _index = src;
}

void    ServerInfo::setCurIndex(size_t index) {
    _cur_index = index;
}

void    ServerInfo::setErrorPage(std::vector<std::string>& token, size_t *i) {
    if (*i + 1 >= token.size()) {
        throw AnyErrorException("Invalid error_page: missing arguments!");
    }
    std::vector<short>  error_code;
    std::string         error_page_path;
    while (*i + 1 < token.size() && (token[*i + 1].find(';') == std::string::npos)) {
        if (token[*i + 1][0] == '+' || (token[*i + 1][0] == '0' && token[*i + 1].size() > 1)) {
            throw AnyErrorException("Invalid error_page: invalid code!");
        }
        std::stringstream   ss(token[*i + 1]);
        int                 code = 0;
        ss >> code;
        if (ss.fail() || !ss.eof()) {
            throw AnyErrorException("Invalid error_page: code is not digit!");
        }
        if (code < 400 || code > 599) {
            throw AnyErrorException("Invalid error_page: wrong range of code!");
        }
        error_code.push_back(static_cast<short>(code));
        ++(*i);
    }
    if (*i + 1 >= token.size()) {
        throw AnyErrorException("Invalid error_page: missing path!");
    }
    error_page_path = token[*i + 1];
    ++(*i);
    checkToken(error_page_path);
    if (error_page_path.empty()) {
        throw AnyErrorException("Invalid error page: missing argument!");
    }
    for (size_t k = 0; k < error_code.size(); ++k) {
        _error_pages[error_code[k]] = error_page_path;
    }
}

void    ServerInfo::setMethods(std::vector<std::string>& token, size_t *i) {
    if (*i + 1 >= token.size()) {
        throw AnyErrorException("Invalid methods: missing arguments!");
    }
    while (*i + 1 < token.size() && (token[*i + 1].find(';') == std::string::npos)) {
        if (!isValidMethod(token[*i + 1])) {
            throw AnyErrorException("Invalid Methods!");
        }
        _methods.insert(token[*i + 1]);
        ++(*i);
    }
    if (*i + 1 >= token.size()) {
        throw AnyErrorException("Invalid methods: missing ';'!");
    }
    std::string     last_method = token[*i + 1];
    ++(*i);
    checkToken(last_method);
    if (last_method.empty()) {
        return ;
    }
    if (!isValidMethod(last_method)) {
        throw AnyErrorException("Invalid Methods!");
    } else {
        _methods.insert(last_method);
    }
}

void    ServerInfo::setMethods() {
    _methods.insert("GET");
    _methods.insert("POST");
    _methods.insert("DELETE");
    _methods.insert("HEAD");
}

void    ServerInfo::setCgiExt(std::vector<std::string>& token, size_t *i) {
    if (*i + 3 >= token.size()) {
        throw AnyErrorException("Invalid cgi setting: missing contents!");
    }
    if (token[*i + 2] != "cgi_path") {
        throw AnyErrorException("Invalid cgi setting: missing path!");
    }
    std::string     cgi_ext(token[*i + 1]);
    std::string     cgi_path(token[*i + 3]);
    checkToken(cgi_ext);
    checkToken(cgi_path);
    if (!isValidCgiExt(cgi_ext)) {
        throw AnyErrorException("Invalid cgi extention!");
    }
    if (!isValidCgiPath(cgi_path)) {
        throw AnyErrorException("Invalid cgi path!");
    }
    if (_cgi_ext.find(cgi_ext) != _cgi_ext.end()) {
        throw AnyErrorException("Cgi extention is duplicated!");
    } else {
        _cgi_ext.insert(cgi_ext);
    }
    setCgiPath(cgi_ext, cgi_path);
    *i += 3;
}

void    ServerInfo::setCgiPath(std::string& ext, std::string& path) {
    if (_cgi_path.find(ext) != _cgi_path.end()) {
        throw AnyErrorException("Cgi path is duplicated!");
    } else {
        _cgi_path[ext] = path;
    }
}

bool    ServerInfo::isValidCgiExt(const std::string& src) {
    if (src.size() < 2 || src.size() > 20) {
        return false;
    }
    if (src[0] != '.' || std::isdigit(src[1])) {
        return false;
    }
    for (size_t i = 1; i < src.size(); ++i) {
        if (!std::isalnum(src[i]) && src[i] != '_') {
            return false;
        }
    }
    return true;
}

bool    ServerInfo::isValidCgiPath(const std::string& src) {
    if (src.empty() || src[0] != '/') {
        return false;
    }
    if (src.find("..") != std::string::npos) {
        return false;
    }
    CheckFileInfo   fileinfo(src);
    if (fileinfo.checkFileType() != 0) {
        return false;
    }
    if (fileinfo.checkFileAccess(X_OK) == 1) {
        return false;
    }
    return true;
}

void    ServerInfo::setLocation(std::vector<std::string>& token, size_t *i) {
    if (*i + 3 >= token.size()) {
        throw AnyErrorException("Invalid location: missing contents!");
    }
    if (token[*i + 2] != "{") {
        throw AnyErrorException("Invalid location: wrong character!");
    }
    std::string     path(token[*i + 1]);
    *i += 3;
    if (!path.empty() && path != "/" && path[path.size() - 1] == '/') {
        path.erase(path.size() - 1);
    }
    if (_location_path.count(path)) {
        throw AnyErrorException("Location is duplicated!");
    }
    _location_path.insert(path);
    std::vector<std::string>    content;
    while (*i < token.size() && token[*i] != "}") {
        content.push_back(token[*i]);
        ++(*i);
    }
    if (*i >= token.size()) {
        throw AnyErrorException("Invalid location: missing '}'!");
    }
    setLocation(path, content);
}

void    ServerInfo::setLocation(const std::string& path, std::vector<std::string>& src) {
    Location    location_tmp;
    if (isValidLocPath(path)) {
        location_tmp.setPath(path);
    } else {
        throw AnyErrorException("Location error: path is invalid!");
    }
    size_t      len = src.size();
    for (size_t i = 0; i < len; ++i) {
        if (src[i] == "root" && i + 1 < len) {
            if (!location_tmp.getRoot().empty()) {
                throw AnyErrorException("Location error: root is duplicated!");
            }
            checkToken(src[++i]);
            location_tmp.setRoot(src[i]);
        } else if (src[i] == "index" && i + 1 < len) {
            if (location_tmp.getIndexState()) {
                throw AnyErrorException("Location error: index is duplicated!");
            }
            checkToken(src[++i]);
            location_tmp.setIndex(src[i]);
            location_tmp.setIndexState(true);
        } else if (src[i] == "return") {
            if (location_tmp.getReturnState()) {
                throw AnyErrorException("Location error: return is duplicated!");
            }
            location_tmp.setReturn(src, &i);
        } else if (src[i] == "client_max_body_size" && i + 1 < len) {
            if (location_tmp.getSizeState()) {
                throw AnyErrorException("Location error: client body size is duplicated!");
            }
            checkToken(src[++i]);
            location_tmp.setBodySize(src[i]);
        } else if (src[i] == "autoindex" && i + 1 < len) {
            if (location_tmp.getAutoindexState()) {
                throw AnyErrorException("Location error: autoindex is duplicated!");
            }
            checkToken(src[++i]);
            location_tmp.setAutoindex(src[i]);
        } else if (src[i] == "upload_destination" && i + 1 < len) {
            if (!location_tmp.getUpload().empty()) {
                throw AnyErrorException("Location error: upload is duplicated!");
            }
            checkToken(src[++i]);
            location_tmp.setUpload(src[i]);
        } else if (src[i] == "error_page") {
            location_tmp.setErrorPage(src, &i);
        } else if (src[i] == "method") {
            if (!location_tmp.getMethods().empty()) {
                throw AnyErrorException("Location error: method is duplicated!");
            }
            location_tmp.setMethods(src, &i);
        } else if (src[i] == "cgi_extention") {
            location_tmp.setCgiExt(src, &i);
        } else if (src[i] == "alias") {
            if (!location_tmp.getAlias().empty()) {
                throw AnyErrorException("Location error: alias is duplicated!");
            }
            if (!location_tmp.getRoot().empty()) {
                throw AnyErrorException("Location error: root is already set before alias!");
            }
            checkToken(src[++i]);
            location_tmp.setAlias(src[i]);
        } else {
            throw AnyErrorException("Location error: invalid elements!");
        }
    }
    _location.push_back(location_tmp);
}

bool    ServerInfo::isValidLocPath(const std::string& src) {
    if (src.empty() || src[0] != '/' || src.find("..") != std::string::npos) {
        return false;
    }
    for (size_t i = 0; i < src.size(); ++i) {
        if (!std::isalnum(src[i]) && src[i] != '/' && src[i] != '_' && src[i] != '-' && src[i] != '.') {
            return false;
        }
    }
    return true;
}

bool    ServerInfo::isValidMethod(const std::string& src) {
    std::set<std::string>   legal_methods;
    legal_methods.insert("GET");
    legal_methods.insert("POST");
    legal_methods.insert("DELETE");
    legal_methods.insert("HEAD");
    if (legal_methods.find(src) != legal_methods.end()) {
        return true;
    } else {
        return false;
    }
}

bool    ServerInfo::isValidRootPath(std::string& src) const {
    if (src.empty() || src[0] != '/') {
        return false;
    }
    if (src.find("..") != std::string::npos) {
        return false;
    }
    CheckFileInfo   fileinfo(src);
    if (fileinfo.checkFileType() != 1) {
        return false;
    }
    // if (fileinfo.checkFileAccess(R_OK | W_OK | X_OK) == 1) {
    //     return false;
    // }
    if (src[src.size() - 1] != '/') {
        src += "/";
    }
    return true;
}

bool    ServerInfo::isValidDomain(const std::string& src) {
    if (src.empty() || src.size() > 253) {
        return false;
    }
    if (src == "localhost") {
        return true;
    }
    size_t  i;
    for (i = 0; i < src.size(); ++i) {
        if (!std::isdigit(src[i]) && src[i] != '.') {
            break ;
        }
    }
    if (i == src.size()) {
        return false;
    }
    std::stringstream   ss(src);
    std::string         tmp;
    int     count = 0;
    while (std::getline(ss, tmp, '.')) {
        ++count;
        if (tmp.empty() || tmp.size() > 63) {
            return false;
        }
        if (tmp[0] == '-' || tmp[tmp.size() - 1] == '-') {
            return false;
        }
        std::string::const_iterator it;
        for (it = tmp.begin(); it != tmp.end(); ++it) {
            if (!(std::isalnum(*it) || *it == '-')) {
                return false;
            }
        }
    }
    if (!ss.eof() || count < 2) {
        return false;
    }
    return true;
}

bool    ServerInfo::isValidIp(const std::string& src) {
    std::stringstream   ss(src);
    std::string         tmp;
    int     count = 0;
    while (std::getline(ss, tmp, '.')) {
        ++count;
        if (count > 4) {
            return false;
        }
        if (tmp.empty() || tmp.size() > 3 || (tmp[0] == '0' && tmp.size() > 1)) {
            return false;
        }
        std::stringstream   ss_tmp(tmp);
        int     part = 0;
        ss_tmp >> part;
        if (ss_tmp.fail() || !ss_tmp.eof()) {
            return false;
        }
        if (part < 0 || part > 255) {
            return false;
        }
    }
    if (!ss.eof() || count != 4) {
        return false;
    }
    return true;
}

void    ServerInfo::checkToken(std::string& src) {
    size_t  pos = src.find(';');
    if (pos == std::string::npos) {
        throw AnyErrorException("';' not found!");
    }
    if (pos != src.size() - 1) {
        throw AnyErrorException("';' is not in the end of token!");
    }
    src.erase(pos);
}

void     ServerInfo::matchLocation(const ServerInfo& config, ServerInfo& match, const std::string& path) {
    size_t  max_len = 0;
    const Location  *best_match = NULL;
    match = config;
    match._location.clear();
    for (size_t i = 0; i < config._location.size(); ++i) {
        const Location& loc = config._location[i];
        const std::string&  loc_path = loc.getPath();
        if ((path.compare(0, loc_path.size(), loc_path) == 0) && (path.size() == loc_path.size() || path[loc_path.size()] == '/' || loc_path == "/")) {
            if (loc_path.size() > max_len) {
                max_len = loc_path.size();
                best_match = &loc;
            }
        }
    }
    if (best_match) {
        match._location.push_back(*best_match);
    } else {
        match._location.push_back(Location());
    }
    if (!match._location[0].getRootState()) {
        match._location[0].setRoot(match._root, true);
    }
    if (!match._location[0].getIndexState()) {
        match._location[0].setIndex(match._index);
    }
    if (!match._location[0].getSizeState()) {
        match._location[0].setBodySize(match._client_max_body_size);
    }
    if (!match._location[0].getAutoindexState()) {
        match._location[0].setAutoindex(match._autoindex);
    }
    if (match._location[0].getMethods().empty()) {
        match._location[0].setMethods(match._methods);
    }
    match._location[0].setErrorPage(match._error_pages);
    match._location[0].setCgi(match._cgi_ext, match._cgi_path);
}

size_t   ServerInfo::matchServerName(const std::vector<ServerInfo>& wconf, const std::string& name) {
    for (size_t index = 0; index < wconf.size(); ++index) {
        if (wconf[index].getServerName() == name) {
            return index;
        }
    }
    return 0;
}

void    ServerInfo::printServerinfo() const {
    std::cout << std::endl << "Vector " << _cur_index << ": " << std::endl;
    std::cout << "listen: " << _port << std::endl;
    std::cout << "IP: " << _ip << std::endl;
    if (!_server_name.empty())
        std::cout << "server name: " << _server_name << std::endl;
    std::cout << "root: " << _root << std::endl;
    std::cout << "client max body size: " << _client_max_body_size << std::endl;
    std::cout << "autoindex: " << (_autoindex ? "on" : "off") << std::endl;
    std::cout << "index: " << _index << std::endl;
    if (!_error_pages.empty()) {
        std::cout << "error_page: " << std::endl;
        for (std::map<short, std::string>::const_iterator it = _error_pages.begin(); it != _error_pages.end(); ++it) {
            std::cout << "    " << it->first << " ==> " << it->second << std::endl;
        }
    }
    if (!_methods.empty()) {
        std::cout << "allow_methods: ";
        for (std::set<std::string>::const_iterator it = _methods.begin(); it != _methods.end(); ++it) {
            std::cout << " " << *it;
        }
        std::cout << std::endl;
    }
    if (!_cgi_ext.empty()) {
        std::cout << "cgi info: " << std::endl;
        for (std::set<std::string>::const_iterator it = _cgi_ext.begin(); it != _cgi_ext.end(); ++it) {
            std::cout << "  >>>  " << *it << " ==> " << getCgiPath(*it) << std::endl;
        }
    }
    if (!_location.empty()) {
        std::cout << "location info: " << std::endl;
        for (std::vector<Location>::const_iterator it = _location.begin(); it != _location.end(); ++it) {
            std::cout << "*****************************" << std::endl;
            it->printLocation();
        }
    }
    std::cout << "*****************************" << std::endl;
}