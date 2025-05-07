#ifndef LOG_CLASS_HPP
#define LOG_CLASS_HPP

#include "standardLib.hpp"
# include "ServerInfo.hpp"
# include "time.class.hpp"
# include "parseTools.class.hpp"

class log
{
    private:
        log();
        ~log();

    public:
        static void     loginfo(int type, std::string info);
        static void     logResp(const ServerInfo& matchconf, int status, std::string StatusMsg);
        static void     logConfig(const ServerInfo& wconf);
        static void     printRequest(const std::string& request);
        static void     printResponse(const std::string& response);
        static void     printCgiError(const std::string& cgiErr);
};

#endif