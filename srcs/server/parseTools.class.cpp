#include "../../includes/parseTools.class.hpp"

std::string    parseTools::ft_itoa(int num)
{
    std::stringstream ss;

    ss << num;
    return (ss.str());
}

std::string     parseTools::getSuffix(const std::string& filename)
{
    size_t  pos = filename.find_last_of(".");
    if (pos != std::string::npos)
    {
        return (filename.substr(pos));
    }
    return ("");
}

std::string     parseTools::cleanResName(std::string& ressource)  //this func is for join with the abs path
{
    if (ressource[0] == '/')
        ressource = ressource.substr(1);

    std::size_t pos = ressource.find('?');
    if (pos != std::string::npos) {
        ressource =  ressource.substr(0, pos);
    }
    return (ressource);
}

std::string     parseTools::getUriPath(std::string& ressource)  // this func to get clean uri
{
    std::size_t pos = ressource.find('?');
    if (pos != std::string::npos) {
        return (ressource.substr(0, pos));
    }
    return (ressource);
}

std::string     parseTools::getQueryStr(std::string& ressource)
{
    std::size_t pos = ressource.find('?');
    if (pos != std::string::npos && pos + 1 < ressource.size()) {
        return (ressource.substr(pos + 1));
    }
    return ("");
}



int     parseTools::isValidNumber(std::string& num)
{
    if (num.empty()) {
        return (0);
    }
    for (size_t i = 0; i < num.size(); ++i) {
        if (!std::isxdigit(num[i])) {
            return (0);
        }
    }
    return (1);
}

void         parseTools::addSlashToDirEnd(std::string& ressourcePath, std::string& ressource)
{
    if (ressourcePath[ressourcePath.size() - 1] != '/')
    {
        ressourcePath = ressourcePath + '/';
        ressource = ressource + "/";
    }
}

void         parseTools::addSlashToDirEnd(std::string& path)
{
    if (path[path.size() - 1] != '/')
    {
        path = path + '/';
    }
}

void    parseTools::clearString(std::string& str)
{
    std::string empty = "";
    str = empty;
}