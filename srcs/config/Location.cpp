#include "../../includes/Location.hpp"

Location::Location()
    :   _path(""),
        _root(""),
        _alias(""),
        _is_root_set(false),
        _index(""),
        _is_index_set(false),
        _return_content(""),
        _return_code(200),
        _is_return_set(false),
        _client_max_body_size(MAX_BODY_LENGTH),
        _is_size_set(false),
        _autoindex(false),
        _is_autoindex_set(false),
        _upload(""),
        _error_pages(),
        _methods(),
        _cgi_ext(),
        _cgi_path() {
}

Location::~Location() {
}

Location::Location(const Location& src) {
    *this = src;
}

Location&   Location::operator=(const Location& src) {
    if (this != &src) {
        _path = src._path;
        _root = src._root;
        _alias = src._alias;
        _is_root_set = src._is_root_set;
        _index = src._index;
        _is_index_set = src._is_index_set;
        _return_content = src._return_content;
        _return_code = src._return_code;
        _is_return_set = src._is_return_set;
        _client_max_body_size = src._client_max_body_size;
        _is_size_set = src._is_size_set;
        _autoindex = src._autoindex;
        _is_autoindex_set = src._is_autoindex_set;
        _upload = src._upload;
        _error_pages = src._error_pages;
        _methods = src._methods;
        _cgi_path = src._cgi_path;
        _cgi_ext = src._cgi_ext;
    }
    return *this;
}

const std::string&  Location::getPath() const {
    return _path;
}

const std::string&  Location::getRoot() const {
    return _root;
}

const std::string&  Location::getAlias() const {
    return _alias;
}

bool    Location::getRootState() const {
    return _is_root_set;
}

const std::string&  Location::getIndex() const {
    return _index;
}

bool    Location::getIndexState() const {
    return _is_index_set;
}

bool    Location::getReturnState() const {
    return _is_return_set;
}

const std::string&  Location::getReturnContent() const {
    return _return_content;
}

short   Location::getReturnCode() const {
    return _return_code;
}

bool    Location::getSizeState() const {
    return _is_size_set;
}

size_t  Location::getBodySize() const {
    return _client_max_body_size;
}

bool    Location::getAutoindexState() const {
    return _is_autoindex_set;
}

bool    Location::getAutoindex() const {
    return _autoindex;
}

const std::string&  Location::getUpload() const {
    return _upload;
}

const std::map<short, std::string>& Location::getErrorPages() const {
    return _error_pages;
}

const std::string Location::getErrorPages(const short key) const {
    std::map<short, std::string>::const_iterator it = _error_pages.find(key);
    if (it == _error_pages.end()) {
        return "";
    }
    return it->second;
}

const std::set<std::string>&    Location::getMethods() const {
    return _methods;
}

const std::set<std::string>& Location::getCgiExt() const {
    return _cgi_ext;
}

const std::map<std::string, std::string>&   Location::getCgiPath() const {
    return _cgi_path;
}

const std::string   Location::getCgiPath(const std::string& src) const {
    if (src.empty())
        return ("");
    std::map<std::string, std::string>::const_iterator  it = _cgi_path.find(src);
    if (it != _cgi_path.end()) {
        return it->second;
    } else {
        return "";
    }
}

void    Location::setPath(const std::string& src) {
    _path = src;
}

void    Location::setRoot(const std::string& src) {
    if (!isValidOrdre()) {
        throw AnyErrorException("Location error: invalid ordre of root!");
    }
    if (isValidRoot(src)) {
        _root = src;
        if (_root[_root.size() - 1] != '/') {
            _root += "/";
        }
        _is_root_set = true;
    } else {
        throw AnyErrorException("Location error: invalid root!");
    }
}

void    Location::setRoot(const std::string& src, bool state) {
    if (state) {
        _root = src;
    }
}

void    Location::setAlias(const std::string& src) {
    if ((_path[_path.size() - 1] == '/' && src[src.size() - 1] != '/') \
        || (_path[_path.size() - 1] != '/' && src[src.size() - 1] == '/')) {
        throw AnyErrorException("Location error: alias is not compatible with location path!");
    }
    if (src.empty() || src[0] != '/' || src.find("..") != std::string::npos) {
        throw AnyErrorException("Location error: invalid alias!");
    }
    for (size_t i = 0; i < src.size(); ++i) {
        if (!std::isalnum(src[i]) && src[i] != '/' && src[i] != '_' \
            && src[i] != '-' && src[i] != '.') {
            throw AnyErrorException("Location error: invalid alias!");
        }
    }
    _alias = src;
    if (_alias[_alias.size() - 1] != '/') {
        _alias += "/";
    }
}

void    Location::setIndex(const std::string& src) {
    _index = src;
}

void    Location::setIndexState(bool state) {
    _is_index_set = state;
}

void    Location::setReturn(std::vector<std::string>& token, size_t *i) {
    if (*i + 2 > token.size()) {
        throw AnyErrorException("Location error: missing return contents!");
    }
    _is_return_set = true;
    if (token[*i + 1][0] == '+' || (token[*i + 1][0] == '0' && token[*i + 1].size() > 1)) {
        throw AnyErrorException("Location error: invalid code!");
    }
    std::stringstream   ss(token[++(*i)]);
    short   code = 0;
    ss >> code;
    if (ss.fail() || !ss.eof()) {
        throw AnyErrorException("Location error: code is not digit!");
    }
    if (code < 200 || code > 599) {
        throw AnyErrorException("Location error: wrong range of code!");
    }
    _return_code = code;
    ServerInfo::checkToken(token[++(*i)]);
    if (isValidReturnContent(token[*i])) {
        _return_content = token[*i];
    } else {
        throw AnyErrorException("Location error: invalid path of return!");
    }
}

void    Location::setBodySize(const std::string& src) {
    _is_size_set = true;
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

void    Location::setBodySize(size_t size) {
    _is_size_set = true;
    _client_max_body_size = size;
}

void    Location::setAutoindex(const std::string& src) {
    _is_autoindex_set = true;
    if (src == "on") {
        _autoindex = true;
    } else if (src == "off") {
        _autoindex = false;
    } else {
        throw AnyErrorException("Location error: invalid autoindex!");
    }
}

void    Location::setAutoindex(bool autoindex) {
    _is_autoindex_set = true;
    _autoindex = autoindex;
}

void    Location::setUpload(const std::string& src) {
    if (isValidPath(src)) {
        _upload = src;
    } else {
        throw AnyErrorException("Location error: invalid upload path!");
    }
}

void    Location::setErrorPage(std::vector<std::string>& token, size_t *i) {
    if (*i + 1 >= token.size()) {
        throw AnyErrorException("Location error: missing error page arguments!");
    }
    std::vector<short>  error_code;
    std::string         error_page_path;
    while (*i + 1 < token.size() && (token[*i + 1].find(';') == std::string::npos)) {
        if (token[*i + 1][0] == '+' || (token[*i + 1][0] == '0' && token[*i + 1].size() > 1)) {
            throw AnyErrorException("Location error:  error page invalid code!");
        }
        std::stringstream   ss(token[*i + 1]);
        int                 code = 0;
        ss >> code;
        if (ss.fail() || !ss.eof()) {
            throw AnyErrorException("Location error:  error page code is not digit!");
        }
        if (code < 400 || code > 599) {
            throw AnyErrorException("Location error: wrong range of error page code!");
        }
        error_code.push_back(static_cast<short>(code));
        ++(*i);
    }
    if (*i + 1 >= token.size()) {
        throw AnyErrorException("Invalid error_page: missing path!");
    }
    error_page_path = token[*i + 1];
    ++(*i);
    ServerInfo::checkToken(error_page_path);
    if (error_page_path.empty()) {
        throw AnyErrorException("Location error: missing error page arguments!");
    }
    for (size_t k = 0; k < error_code.size(); ++k) {
        _error_pages[error_code[k]] = error_page_path;
    }
}

void    Location::setErrorPage(const std::map<short, std::string>& src) {
    for (std::map<short, std::string>::const_iterator it = src.begin(); it != src.end(); ++it) {
        if (_error_pages.find(it->first) == _error_pages.end()) {
            _error_pages[it->first] = it->second;
        }
    }
}

void    Location::setCgi(const std::set<std::string>& ext, std::map<std::string, std::string>& path) {
    for (std::set<std::string>::const_iterator it = ext.begin(); it != ext.end(); ++it) {
        if (_cgi_ext.find(*it) == _cgi_ext.end()) {
            _cgi_ext.insert(*it);
            _cgi_path[*it] = path[*it];
        }
    }
}

void    Location::setMethods(std::vector<std::string>& token, size_t *i) {
    if (*i + 1 >= token.size()) {
        throw AnyErrorException("Location error: missing method arguments!");
    }
    while (*i + 1 < token.size() && (token[*i + 1].find(';') == std::string::npos)) {
        if (!ServerInfo::isValidMethod(token[*i + 1])) {
            throw AnyErrorException("Location error: invalid Methods!");
        }
        _methods.insert(token[*i + 1]);
        ++(*i);
    }
    if (*i + 1 >= token.size()) {
        throw AnyErrorException("Location error: method missing ';'!");
    }
    std::string     last_method = token[*i + 1];
    ++(*i);
    ServerInfo::checkToken(last_method);
    if (last_method.empty()) {
        _methods.insert("");
        return ;
    }
    if (!ServerInfo::isValidMethod(last_method)) {
        throw AnyErrorException("Location error: invalid Methods!");
    } else {
        _methods.insert(last_method);
    }
}

void    Location::setMethods(const std::set<std::string>& methods) {
    _methods = methods;
}

void    Location::setCgiExt(std::vector<std::string>& token, size_t *i) {
    if (*i + 3 >= token.size()) {
        throw AnyErrorException("Location error: missing cgi contents!");
    }
    if (token[*i + 2] != "cgi_path") {
        throw AnyErrorException("Location error: missing cgi path!");
    }
    std::string     cgi_ext(token[*i + 1]);
    std::string     cgi_path(token[*i + 3]);
    ServerInfo::checkToken(cgi_ext);
    ServerInfo::checkToken(cgi_path);
    if (!ServerInfo::isValidCgiExt(cgi_ext)) {
        throw AnyErrorException("Location error: invalid cgi extention!");
    }
    if (!ServerInfo::isValidCgiPath(cgi_path)) {
        throw AnyErrorException("Location error: invalid cgi path!");
    }
    if (_cgi_ext.find(cgi_ext) != _cgi_ext.end()) {
        throw AnyErrorException("Location error: cgi extention is duplicated!");
    } else {
        _cgi_ext.insert(cgi_ext);
    }
    setCgiPath(cgi_ext, cgi_path);
    *i += 3;
}

void    Location::setCgiPath(std::string& ext, std::string& path) {
    if (_cgi_path.find(ext) != _cgi_path.end()) {
        throw AnyErrorException("Location error: cgi path is duplicated!");
    } else {
        _cgi_path[ext] = path;
    }
}

bool    Location::isValidPath(const std::string& src) const {
    if (src.empty() || src[0] != '/' || src.find("..") != std::string::npos) {
        return false;
    }
    for (size_t i = 0; i < src.size(); ++i) {
        if (!std::isalnum(src[i]) && src[i] != '/' && src[i] != '_' \
            && src[i] != '-' && src[i] != '.') {
            return false;
        }
    }
    return true;
}

bool    Location::isValidReturnContent(const std::string& src) const {
    if (src.empty()) {
        return false;
    }
    if (src.find("http://") == 0 || src.find("https://") == 0) {
        return true;
    }
    return src[0] == '/';
}

bool    Location::isValidRoot(const std::string& src) const {
    if (src.empty() || src[0] != '/' || src.find("..") != std::string::npos) {
        return false;
    }
    CheckFileInfo   fileinfo(src);
    if (fileinfo.checkFileType() != 1) {
        return false;
    }
    return true;
}

bool    Location::isValidOrdre() const {
    if (_index.empty() && !_is_return_set && !_is_autoindex_set && _upload.empty() \
            && _error_pages.empty() && _methods.empty() && _alias.empty()) {
        return true;
    } else {
        return false;
    }
}

void    Location::printLocation() const {
    if (!_path.empty()) {
        std::cout << "   >> path: " << _path << std::endl;
    }
    if (!_root.empty()) {
        std::cout << "   >> root: " << _root << std::endl;
    }
    if (!_index.empty()) {
        std::cout << "   >> index: " << _index << std::endl;
    }
    if (_is_return_set) {
        std::cout << "   >> return: " << _return_code << "  " << _return_content << std::endl;
    }
    if (_is_size_set) {
        std::cout << "   >> body size: " << _client_max_body_size << std::endl;
    }
    if (_is_autoindex_set) {
        std::cout << "   >> autoindex: " << (_autoindex ? "on" : "off") << std::endl;
    }
    if (!_upload.empty()) {
        std::cout << "   >> upload destination: " << _upload << std::endl;
    }
    if (!_error_pages.empty()) {
        std::cout << "   >> error page: " << std::endl;
        for (std::map<short, std::string>::const_iterator it = _error_pages.begin(); it != _error_pages.end(); ++it) {
            std::cout << "     >>>> " << it->first << " ==> " << it->second << std::endl;
        }
    }
    if (!_methods.empty()) {
        std::cout << "   >> allow methods: ";
        for (std::set<std::string>::const_iterator it = _methods.begin(); it != _methods.end(); ++it) {
            std::cout << " " << *it;
        }
        std::cout << std::endl;
    }
    if (!_cgi_ext.empty()) {
        std::cout << "   >> cgi info: " << std::endl;
        for (std::set<std::string>::const_iterator it = _cgi_ext.begin(); it != _cgi_ext.end(); ++it) {
            std::cout << "     >>>> " << *it << " ==> " << getCgiPath(*it) << std::endl;
        }
    }
}