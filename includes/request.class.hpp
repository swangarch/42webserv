#ifndef REQUEST_CLASS_HPP
#define REQUEST_CLASS_HPP

#include "standardLib.hpp"
#include "infopage.class.hpp"
#include "parseTools.class.hpp"
#include "ServerInfo.hpp"
#include "cgi.class.hpp"

class request
{
    private:

        std::string     _method;
        std::string     _ressource;
        std::string     _authority;
        std::string     _protocol;
        std::string     _queryStr;
        std::string     _host;
        std::string     _contentType;
        std::string     _contentLength;
        std::string     _origin;

        std::string     _transferEncoding;
        std::string     _authorization;
        std::string     _userAgent;
        std::string     _accept;
        std::string     _acceptLanguage;
        std::string     _acceptEncoding;
        std::string     _acceptCharset;
        std::string     _connection;
        std::string     _refer;
        std::string     _secFetchDest;
        std::string     _secFetchMode;
        std::string     _secFetchSite;
        std::string     _secFetchUser;
        std::string     _ifModifiedSince;
        std::string     _priority;
        std::string     _cookie;

        std::string     _pathInfo;
        std::string     _script;

    public:
        request();
        ~request();

        void reset();

        static int     readHeadFinish(const std::string str);
        static int     readChunkFinish(const std::string str);
        static bool    isMethod(const std::string& method);  //use this 
        static int     unChunkBody(std::string& body, size_t max_body_size);
        static void    setConnection(int status, std::string& connection, const std::string& clientConnection);
        static void    setContentType(std::string& type, int status, request& req, cgi& cgi, std::map<std::string, std::string>& resType);
        static void    setCookie(cgi& cgi, std::stringstream& ss);
        static void    setLocation(const ServerInfo& matchconf, const cgi& cgi, std::stringstream& ss);
        // static void    resetCgiStruct(cgi& cgiHead);
        static void    checkAccess(const std::string& ressourcePath, int& status);
        static void    handleAlias(std::string& output, const std::string& alias, const std::string& path, std::string ressource);

        // below getter and setter ---------------------------------------------------------------------------------------------
        const std::string& getMethod() const;
        void setMethod(const std::string &method);

        const std::string& getRessource() const;
        void setRessource(const std::string &ressource);

        const std::string& getAuthority() const;
        void setAuthority(const std::string &authority);

        const std::string& getProtocol() const;
        void setProtocol(const std::string &protocol);

        const std::string& getQueryStr() const;
        void setQueryStr(const std::string &queryStr);

        const std::string& getHost() const;
        void setHost(const std::string &host);

        const std::string& getContentType() const;
        void setContentType(const std::string &contentType);

        const std::string& getContentLength() const;
        void setContentLength(const std::string &contentLength);

        const std::string& getOrigin() const;
        void setOrigin(const std::string &origin);

        const std::string& getTransferEncoding() const;
        void setTransferEncoding(const std::string &transferEncoding);

        const std::string& getAuthorization() const;
        void setAuthorization(const std::string &authorization);

        const std::string& getUserAgent() const;
        void setUserAgent(const std::string &userAgent);

        const std::string& getAccept() const;
        void setAccept(const std::string &accept);

        const std::string& getAcceptLanguage() const;
        void setAcceptLanguage(const std::string &acceptLanguage);

        const std::string& getAcceptEncoding() const;
        void setAcceptEncoding(const std::string &acceptEncoding);

        const std::string& getAcceptCharset() const;
        void setAcceptCharset(const std::string &acceptCharset);

        const std::string& getConnection() const;
        void setConnection(const std::string &connection);

        const std::string& getRefer() const;
        void setRefer(const std::string &refer);

        const std::string& getSecFetchDest() const;
        void setSecFetchDest(const std::string &secFetchDest);

        const std::string& getSecFetchMode() const;
        void setSecFetchMode(const std::string &secFetchMode);

        const std::string& getSecFetchSite() const;
        void setSecFetchSite(const std::string &secFetchSite);

        const std::string& getSecFetchUser() const;
        void setSecFetchUser(const std::string &secFetchUser);

        const std::string& getIfModifiedSince() const;
        void setIfModifiedSince(const std::string &ifModifiedSince);

        const std::string& getPriority() const;
        void setPriority(const std::string &priority);

        const std::string& getCookie() const;
        void setCookie(const std::string &cookie);

        const std::string& getPathInfo() const;
        void setPathInfo(const std::string &pathInfo);

        const std::string& getScript() const;
        void setScript(const std::string &script);
};

#endif