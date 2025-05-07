#pragma once

# include "standardLib.hpp"
# include "AnyErrorException.hpp"

class   CheckFileInfo {
public:
    CheckFileInfo();
    CheckFileInfo(const std::string& path);
    CheckFileInfo(const CheckFileInfo& src);
    ~CheckFileInfo();

    CheckFileInfo&  operator=(const CheckFileInfo& src);

    int     checkFileType() const;
    int     checkFileAccess(int mode) const;
    void    getFileContent(std::string& content) const;
private:
    std::string     _path;
};