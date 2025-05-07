#ifndef INFO_PAGE_CLASS_HPP
#define INFO_PAGE_CLASS_HPP

#include "standardLib.hpp"

class infopage
{
    private:
        infopage();
        ~infopage();    

    public:
        static std::map<std::string, std::string>      initResType();
        static std::map<int, std::string>              initStatusInfo();
        static std::string                             getRessourceType(const std::string& type, std::map<std::string, std::string>& resType);
        static std::string                             getStatusMsg(int status, std::map<int, std::string>  &statusInfo);
        static std::string                             getDefaultErrorPage(int status, std::map<int, std::string> &statusInfo);
        static int                                     getInfoPage(std::string& body, const std::string& info);
};

#endif