#include "../../includes/log.class.hpp"

void    log::loginfo(int type, std::string info)
{
    if (info.size() > MAX_SHOW_LENGTH)
    {
        info = info.substr(0, MAX_SHOW_LENGTH) + "...";
    }

    switch (type)
    {
        case T_INFO:
            std::cout << YELLOW << "[" << time::getSimpleFormattedTime() << "] INFO [ " << info << " ]"<< COLOR_END << std::endl;
            break;
        
        case T_REQ:
            std::cout << GREEN << "[" << time::getSimpleFormattedTime() << "] REQ  [ " << info << " ]"<< COLOR_END << std::endl;
            break;
        
        case T_RESP:
            std::cout << BLUE << "[" << time::getSimpleFormattedTime() << "] RESP [ " << info << " ]"<< COLOR_END << std::endl;
            break;

        case T_ERR:
            std::cerr << RED << "[" << time::getSimpleFormattedTime() << "] ERR  [ " << info << " ]"<< COLOR_END << std::endl;
            break;
        
        default:
            break;
    }
}

void    log::logResp(const ServerInfo& matchconf, int status, std::string StatusMsg)
{
    std::stringstream info;
    if (matchconf.getIp() == "0.0.0.0" || matchconf.getPort() == 0)
        return;
    info << matchconf.getIp() << ":" << matchconf.getPort() << "  " << status << " " <<  StatusMsg;
    loginfo(T_RESP, info.str());
}

void    log::logConfig(const ServerInfo& wconf)
{
    if (DEBUG)
    {
        std::cout << YELLOW;
        wconf.printServerinfo();
        std::cout << "______________________________________________________________" << COLOR_END << std::endl;
    }
}

void    log::printRequest(const std::string& request)
{
    if (request.size() == 0)
        return ;

    if (DEBUG)
    {
        std::cout << GREEN << "-------------------------HTTP REQUEST--------------------------" << std::endl
                  << request << std::endl
                  << "-------------------------HTTP REQUEST--------------------------" << COLOR_END << std::endl;
    }
}

void    log::printResponse(const std::string& response)
{
    if (response.size() == 0)
        return ;

    if (DEBUG)
    {
        std::cout << BLUE << "-------------------------HTTP RESPONSE--------------------------" << std::endl
                    << response
                    << "-------------------------HTTP RESPONSE END---------------------" << COLOR_END << std::endl;
    }
}

void    log::printCgiError(const std::string& cgiErr)
{
    if (cgiErr.size() == 0)
        return ;

    if (DEBUG)
    {
        std::cerr << RED 
                << "--------------------------CGI STDERR---------------------------" << std::endl
                << cgiErr 
                << "--------------------------CGI STDERR END-----------------------"
                << COLOR_END 
                << std::endl;
    }
}