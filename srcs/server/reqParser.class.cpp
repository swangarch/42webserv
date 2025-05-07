#include "../../includes/reqParser.class.hpp"

bool    reqParser::uriCharset(char c)
{
    static const std::string allowed = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "-_.~"
        ":/?#[]@"
        "!$&'()*+,;=%";

    return (allowed.find(c) != std::string::npos);
}

bool    reqParser::isValidUriChar(std::string& line)
{
    for (size_t i = 0; i < line.size(); i++)
    {
        if (!uriCharset(line[i]))
        {
            return (false);
        }
    }
    return (true);
}

bool    reqParser::checkUri(request& req, std::string& token, ServerInfo& wconf) {
    size_t  pos = 0;
    size_t  end = 0;
    if ((pos = token.find("http://", pos)) != std::string::npos) {
        if ((end = token.find("/", pos + 7)) != std::string::npos) {
            req.setAuthority(token.substr(pos + 7, end - pos - 7));
            token = token.substr(end);
            if ((pos = req.getAuthority().find(':')) != std::string::npos) {
                std::string host = req.getAuthority().substr(0, pos);
                std::string port = req.getAuthority().substr(pos + 1);

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

                if (wconf.getPort() != port_num) {
                    return false;
                }

            } else {
                if (!(ServerInfo::isValidIp(req.getAuthority()) || ServerInfo::isValidDomain(req.getAuthority()))) {
                    return false;
                }
            }
        } else {
            return false;
        }
    }
    std::string no_query = parseTools::getUriPath(token);
    if (no_query.empty() || no_query[0] != '/') {
        return false;
    }
    return true;
}

int     reqParser::setFirstLine(request& req, std::string& line, ServerInfo& wconf)
{
    int     startPos = 0;
    size_t     pos;
    std::string     token;
    int     wordCount = 0;
    
    while (true)
    {
        pos = line.find(" ", startPos);
        if (pos != std::string::npos)
        {
            token = line.substr(startPos, pos - startPos);

            if (wordCount == 0)
            {
                if (token == "")
                {
                    return (400);
                }
                if (!request::isMethod(token))
                {
                    return (501);
                }
                req.setMethod(token);
            }
            else if (wordCount == 1)
            {
                if (token.size() > 4096) //uri too long
                {
                    return (414);
                }
                if (!isValidUriChar(token))
                {
                    log::loginfo(T_ERR, "Uri contains invalid character");
                    return (400);
                }
                if (!checkUri(req, token, wconf)) {
                    return (400);
                }
                if (token.find("..") != std::string::npos)
                {
                    return (403);
                }
                req.setRessource(parseTools::getUriPath(token));
                req.setQueryStr(parseTools::getQueryStr(token));
            }
            else  //more than 3 tokens
            {
                return (400);
            }
            wordCount++;
            startPos = pos + 1;
        }
        else
        {
            token = line.substr(startPos);
            if (wordCount > 2) 
            {
                return (400);
            }
            if (wordCount == 2 && token != "HTTP/1.1")
            {
                return (505);
            }
            req.setProtocol(token);
            break;
        }
    }
    return (200);
}

int     reqParser::setReqAttribut(request& req, std::string& key, std::string& value, int lineCount)
{
    if (key == "Host" && lineCount == 1) {
        if (!req.getAuthority().empty() && value != req.getAuthority()) {
            return (400);
        }
        req.setHost(value);
    }
    else if (key == "Connection")
    {
        if (value != "close" && value != "keep-alive")
        {
            return (400);
        }
        req.setConnection(value);
    }
    else if (key == "User-Agent")
    {
        if (req.getUserAgent() != "")
        {
            return (400);
        }
        req.setUserAgent(value);
    }
    else if (key == "Accept")
    {
        req.setAccept(value);
    }
    else if (key == "Accept-Encoding")
    {
        req.setAcceptEncoding(value);
    }
    else if (key == "Accept-Language")
    {
        req.setAcceptLanguage(value);
    }
    else if (key == "Referer")
    {
        if (req.getRefer() != "")
        {
            return (400);
        }
        req.setRefer(value);
    }
    else if (key == "If-Modified-Since")
    {
        req.setIfModifiedSince(value);
    }
    else if (key == "Sec-Fetch-Site")
    {
        req.setSecFetchSite(value);
    }
    else if (key == "Sec-Fetch-Mode")
    {
        req.setSecFetchMode(value);
    }
    else if (key == "Sec-Fetch-Dest")
    {
        req.setSecFetchDest(value);
    }
    else if (key == "Sec-Fetch-User")
    {
        req.setSecFetchUser(value);
    }
    else if (key == "Priority")
    {
        if (req.getPriority() != "")
        {
            return (400);
        }
        req.setPriority(value);
    }
    else if (key == "Content-Type")
    {
        if (req.getContentType() != "")
        {
            return (400);
        }
        req.setContentType(value);
    }
    else if (key == "Content-Length")
    {
        if (req.getContentLength() != "")
        {
            return (400);
        }
        if (value == "")
        {
            return (400);
        }
        if (value[0] == '0' && value.size() > 1)
        {
            return (400);
        }
        for (size_t i = 0; i < value.size(); i++)
        {
            if (!std::isdigit(value[i]))
            {
                return (400);
            }
        }
        std::stringstream ss(value);
        int size;

        ss >> size;

        if (ss.fail() || !ss.eof())
        {
            return (400);
        }
        
        req.setContentLength(value);
    }
    else if (key == "Origin")
    {
        if (req.getOrigin() != "")
        {
            return (400);
        }
        req.setOrigin(value);
    }
    else if (key == "Cookie")
    {
        if (req.getCookie() != "")
        {
            return (400);
        }
        req.setCookie(value);
    }
    else if (key == "Transfer-Encoding")
    {
        if (value != "" && value != "chunked")
            return (501);
        req.setTransferEncoding(value);
    }
    return (200);
}

int    reqParser::afterCheck(request& req)
{
    if (req.getMethod() == "" || req.getRessource() == "" || req.getProtocol() == "")
    {
        return (400);
    }

    if (req.getRessource().find("..") != std::string::npos)
    {
        return (403);
    }

    if (req.getHost() == "")
    {
        return (400);
    }

    if (req.getMethod() == "POST")
    {
        if (req.getTransferEncoding() == "chunked" && req.getContentLength() != "")
            return (400);
        else if (req.getTransferEncoding() == "" && req.getContentLength() == "")
            return (411);
        else if (std::atoi(req.getContentLength().c_str()) > MAX_BODY_LENGTH)
            return (413);
    }
    return (200);
}

int    reqParser::parseHead(std::string& reqBuf, request& req, ServerInfo& wconf)
{
    std::string     line;
    int             lineCount = 0;
    size_t          posCl;
    size_t          startpos = 0;
    int             parseStatus;

    if (reqBuf.size() == 0)
    {
        return (400);
    }

    while (true)
    {
        posCl = reqBuf.find("\r\n", startpos);
        if (posCl != std::string::npos)
        {
            line = reqBuf.substr(startpos, posCl - startpos);
            if (line.size() == 0)
            {
                break;
            }
            if (lineCount == 0) //the first line;
            {
                parseStatus = setFirstLine(req, line, wconf);
                if (parseStatus >= 400)
                {
                    return (parseStatus);
                }
            }
            else //the other line
            {
                size_t pos = line.find(":");
                if (pos != std::string::npos && pos != 0)
                {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
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
                    
                    parseStatus = setReqAttribut(req, key, value, lineCount);
                    if (parseStatus >= 400)
                    {
                        return (parseStatus);
                    }
                }
                else
                {
                    return (400);  //if a line without :, there is a bad request
                }
            }
            startpos = posCl + 2;
            lineCount++;
        }
        else
        {
            break;
        }
    }
    parseStatus  = afterCheck(req);
    return (parseStatus);
}
