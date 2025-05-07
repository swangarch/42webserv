#include "../../includes/CheckFileInfo.hpp"

CheckFileInfo::CheckFileInfo() {
}

CheckFileInfo::CheckFileInfo(const std::string& path) : _path(path) {
}

CheckFileInfo::CheckFileInfo(const CheckFileInfo& src) {
    *this = src;
}

CheckFileInfo&  CheckFileInfo::operator=(const CheckFileInfo& src) {
    if (this != &src) {
        _path = src._path;
    }
    return *this;
}

CheckFileInfo::~CheckFileInfo() {
}

int     CheckFileInfo::checkFileType() const {
    struct stat     fileInfo;

    if (stat(_path.c_str(), &fileInfo) == 0) {
        if (S_ISREG(fileInfo.st_mode)) {
            return 0;
        } else if (S_ISDIR(fileInfo.st_mode)) {
            return 1;
        } else {
            return 2;
        }
    } else {
        return -1;
    }
}

int     CheckFileInfo::checkFileAccess(int mode) const {
    if (access(_path.c_str(), mode) == 0) {
        return 0;
    } else {
        return 1;
    }
}

void    CheckFileInfo::getFileContent(std::string& content) const {
    if (_path.empty()) {
        throw AnyErrorException("path of CheckFileINfo is empty!");
    }
    std::ifstream   file(_path.c_str());
    if (!file.is_open()) {
        throw AnyErrorException("Open config file failed!");
    }
    std::stringstream   ss;
    ss << file.rdbuf();
    content = ss.str();
}