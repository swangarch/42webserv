#ifndef PARSE_TOOLS_CLASS_HPP
#define PARSE_TOOLS_CLASS_HPP

#include "standardLib.hpp"

class parseTools
{
    private:
        parseTools();
        ~parseTools();

    public:
        static std::string     ft_itoa(int num);
        static std::string     getSuffix(const std::string& filename);
        static std::string     cleanResName(std::string& ressource);
        static std::string     getUriPath(std::string& ressource);
        static std::string     getQueryStr(std::string& ressource);
        static int             isValidNumber(std::string& num);
        static void            addSlashToDirEnd(std::string& ressourcePath, std::string& ressource);
        static void            addSlashToDirEnd(std::string& path);
        static void            clearString(std::string& str);
};

#endif