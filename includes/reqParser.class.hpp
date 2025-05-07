#ifndef REQ_PARSER_CLASS_HPP
#define REQ_PARSER_CLASS_HPP

#include "standardLib.hpp"
# include "request.class.hpp"
# include "ServerInfo.hpp"
# include "parseTools.class.hpp"

class request;

class reqParser
{
    private:
        reqParser();
        ~reqParser();

        static int     setReqAttribut(request& req, std::string& key, std::string& value, int lineCount);
        static int     afterCheck(request& req);
        static int     setFirstLine(request& req, std::string& line, ServerInfo& wconf);
        static bool    checkUri(request& req, std::string& token, ServerInfo& wconf);
        static bool    uriCharset(char c);
        
    public:
        static int     parseHead(std::string& reqBuf, request& req, ServerInfo& wconf);
        static bool    isValidUriChar(std::string& line);

};

#endif