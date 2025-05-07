#include "../../includes/request.class.hpp"

request::request()
{

}

request::~request()
{
    
}

void    request::reset()
{
    _method = "";
    _ressource = "";
    _protocol = "";
    _queryStr = "";
    _host = "";
    _contentType = "";
    _contentLength = "";
    _origin = "";

    _transferEncoding = "";
    _authorization = "";
    _userAgent = "";
    _accept = "";
    _acceptLanguage = "";
    _acceptEncoding = "";
    _acceptCharset = "";
    _connection = "";
    _refer = "";
    _secFetchDest = "";
    _secFetchSite = "";
    _secFetchMode = "";
    _secFetchUser = "";
    _ifModifiedSince = "";
    _priority = "";
    _cookie = "";
    _pathInfo = "";
    _script = "";
}

void    request::checkAccess(const std::string& ressourcePath, int& status)
{
    if (access(ressourcePath.data(), F_OK | R_OK) == 0)
        return ;
    else if (access(ressourcePath.data(), F_OK) == 0)
        status = 403;
    else
        status = 404;
}

void   request::setConnection(int status, std::string& connection, const std::string& clientConnection)
{
    switch (status)
    {
        case 400:
            connection = "close";
            break;
        
        case 408:
            connection = "close";
            break;
        
        case 413:
            connection = "close";
            break;
        
        case 414:
            connection = "close";
            break;

        case 431:
            connection = "close";
            break;
        
        case 500:
            connection = "close";
            break;

        case 501:
            connection = "close";
            break;
        
        case 502:
            connection = "close";
            break;

        case 503:
            connection = "close";
            break;
        
        case 504:
            connection = "close";
            break;
        
        default:
            break;
    }
    if (clientConnection == "close")
    {
        connection = "close";
    }
}

void         request::setContentType(std::string& type, int status, request& req, cgi& cgi, std::map<std::string, std::string>& resType)
{
    if (status < 400 && (req.getMethod() == "GET" || req.getMethod() == "HEAD"))
    {
        type = infopage::getRessourceType(parseTools::getSuffix(req.getRessource()), resType);
    }

    if (cgi.getOk())
    {
        type = cgi.getContentType();
    }

    if (type == "")
    {
        type = "text/html";
    }
}

void         request::setLocation(const ServerInfo& matchconf, const cgi& cgi, std::stringstream& ss)
{
    if (matchconf.getLocation().size() != 0 && matchconf.getLocation()[0].getReturnState())
    {
        ss << "Location: " << matchconf.getLocation()[0].getReturnContent() << "\r\n";
    }
    else if (cgi.getOk() && cgi.getLocation() != "")
    {
        ss << "Location: " << cgi.getLocation() << "\r\n";
    }
}

void         request::setCookie(cgi& cgi, std::stringstream& ss)
{
    if (cgi.getOk() && cgi.getCookie().size() != 0)
    {
        for (size_t i = 0; i < cgi.getCookie().size(); i++)
            ss << "Set-Cookie: " << cgi.getCookie()[i] << "\r\n";
    }
}

void         request::handleAlias(std::string& output, const std::string& alias, const std::string& path, std::string ressource)
{
    size_t pos = ressource.find(path);
    std::string tmp;


    if (pos != std::string::npos)
    {
        if (path[path.size() - 1] == '/')
            tmp = ressource.substr(pos + path.size());
        else if (ressource.size() > path.size())
            tmp = ressource.substr(pos + path.size() + 1);
        output = alias + tmp;
    }
}

bool   request::isMethod(const std::string& method)
{
    if (method == "GET" || method == "POST" || method == "DELETE" || method == "HEAD")
        return (true);
    return (false); 
}

int    request::readHeadFinish(const std::string str)
{
    size_t  pos = str.find("\r\n\r\n");
    if (pos != std::string::npos)
        return (static_cast<int>(pos));
    return (-1);
}

int    request::readChunkFinish(const std::string str)
{
    size_t  i = 0;
    while (i < str.size()) {
        size_t  pos_crlf = str.find("\r\n", i);
        if (pos_crlf == std::string::npos) {
            return (-1);
        }
        std::string token = str.substr(i, pos_crlf - i);
        if (!parseTools::isValidNumber(token)) {
            return (-2);
        }
        size_t  count;
        std::stringstream   ss(token);
        ss >> std::hex >> count;
        if (ss.fail() || !ss.eof()) {
            return (-2);
        }
        if (count == 0) {
            if (str.substr(pos_crlf, 4) == "\r\n\r\n") {
                return (pos_crlf + 4);
            } else {
                return (-1);
            }
        }
        i = pos_crlf + 2;
        if (i + count + 2 > str.size()) {
            return (-1);
        }
        i += count;
        if (str.substr(i, 2) != "\r\n") {
            return (-2);
        }
        i += 2;
    }
    return (-1);
}


int    request::unChunkBody(std::string& body, size_t max_body_size)
{
    std::string     unChunkedStr;
    std::string     token;
    size_t          count;
    size_t          start = 0;
    size_t          pos = body.find("\r\n");

    while (pos != std::string::npos)
    {
        token  = body.substr(start, pos - start);
        if (!parseTools::isValidNumber(token)) {
            return (400);
        }
        std::stringstream   ss(token);
        ss >> std::hex >> count;
        if (ss.fail() || !ss.eof()) {
            return (400);
        }
        if (count == 0 && body.substr(pos, 4) == "\r\n\r\n") {
            break;
        } else if (count == 0) {
            return (400);
        }
        start = pos + 2;
        if (body.substr(start, count).size() == count) {
            unChunkedStr += body.substr(start, count);
        } else {
            return (400);
        }
        if (unChunkedStr.size() > max_body_size) {
            return (413);
        }
        start += count;
        if (body.substr(start, 2) != "\r\n") {
            return (400);
        }
        start += 2;
        pos = body.find("\r\n", start);
    }
    body = unChunkedStr;
    return (200);
}


//--------------------------------------------------------------GETTER SETTER----------------------------------------
const std::string& request::getMethod() const {
    return _method;
}
void request::setMethod(const std::string &method) {
    _method = method;
}

const std::string& request::getRessource() const {
    return _ressource;
}
void request::setRessource(const std::string &ressource) {
    _ressource = ressource;
}

const std::string& request::getAuthority() const {
    return _authority;
}
void request::setAuthority(const std::string &authority) {
    _authority = authority;
}

const std::string& request::getProtocol() const {
    return _protocol;
}
void request::setProtocol(const std::string &protocol) {
    _protocol = protocol;
}

const std::string& request::getQueryStr() const {
    return _queryStr;
}
void request::setQueryStr(const std::string &queryStr) {
    _queryStr = queryStr;
}

const std::string& request::getHost() const {
    return _host;
}
void request::setHost(const std::string &host) {
    _host = host;
}

const std::string& request::getContentType() const {
    return _contentType;
}
void request::setContentType(const std::string &contentType) {
    _contentType = contentType;
}

const std::string& request::getContentLength() const {
    return _contentLength;
}
void request::setContentLength(const std::string &contentLength) {
    _contentLength = contentLength;
}

const std::string& request::getOrigin() const {
    return _origin;
}
void request::setOrigin(const std::string &origin) {
    _origin = origin;
}

const std::string& request::getTransferEncoding() const {
    return _transferEncoding;
}
void request::setTransferEncoding(const std::string &transferEncoding) {
    _transferEncoding = transferEncoding;
}

const std::string& request::getAuthorization() const {
    return _authorization;
}
void request::setAuthorization(const std::string &authorization) {
    _authorization = authorization;
}

const std::string& request::getUserAgent() const {
    return _userAgent;
}
void request::setUserAgent(const std::string &userAgent) {
    _userAgent = userAgent;
}

const std::string& request::getAccept() const {
    return _accept;
}
void request::setAccept(const std::string &accept) {
    _accept = accept;
}

const std::string& request::getAcceptLanguage() const {
    return _acceptLanguage;
}
void request::setAcceptLanguage(const std::string &acceptLanguage) {
    _acceptLanguage = acceptLanguage;
}

const std::string& request::getAcceptEncoding() const {
    return _acceptEncoding;
}
void request::setAcceptEncoding(const std::string &acceptEncoding) {
    _acceptEncoding = acceptEncoding;
}

const std::string& request::getAcceptCharset() const {
    return _acceptCharset;
}
void request::setAcceptCharset(const std::string &acceptCharset) {
    _acceptCharset = acceptCharset;
}

const std::string& request::getConnection() const {
    return _connection;
}
void request::setConnection(const std::string &connection) {
    _connection = connection;
}

const std::string& request::getRefer() const {
    return _refer;
}
void request::setRefer(const std::string &refer) {
    _refer = refer;
}

const std::string& request::getSecFetchDest() const {
    return _secFetchDest;
}
void request::setSecFetchDest(const std::string &secFetchDest) {
    _secFetchDest = secFetchDest;
}

const std::string& request::getSecFetchMode() const {
    return _secFetchMode;
}
void request::setSecFetchMode(const std::string &secFetchMode) {
    _secFetchMode = secFetchMode;
}

const std::string& request::getSecFetchSite() const {
    return _secFetchSite;
}
void request::setSecFetchSite(const std::string &secFetchSite) {
    _secFetchSite = secFetchSite;
}

const std::string& request::getSecFetchUser() const {
    return _secFetchUser;
}
void request::setSecFetchUser(const std::string &secFetchUser) {
    _secFetchUser = secFetchUser;
}

const std::string& request::getIfModifiedSince() const {
    return _ifModifiedSince;
}
void request::setIfModifiedSince(const std::string &ifModifiedSince) {
    _ifModifiedSince = ifModifiedSince;
}

const std::string& request::getPriority() const {
    return _priority;
}
void request::setPriority(const std::string &priority) {
    _priority = priority;
}

const std::string& request::getCookie() const {
    return _cookie;
}
void request::setCookie(const std::string &cookie) {
    _cookie = cookie;
}

const std::string& request::getPathInfo() const {
    return _pathInfo;
}

void request::setPathInfo(const std::string &pathInfo) {
    _pathInfo = pathInfo;
}

const std::string& request::getScript() const {
    return _script;
}

void request::setScript(const std::string &script) {
    _script = script;
}

//--------------------------------------------------------------GETTER SETTER----------------------------------------