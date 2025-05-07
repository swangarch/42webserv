#ifndef FILE_CLASS_HPP
#define FILE_CLASS_HPP

#include "standardLib.hpp"
#include "log.class.hpp"
#include "infopage.class.hpp"
#include "reqParser.class.hpp"

class   file
{
    private:
        file();
        ~file();

    public:
        static int  delDir(const std::string& pathDir);
        static int  handleUpload(const std::string& data, const std::string& path, const std::string& boundary);
        static int  getBoudary(const std::string& contentType, std::string& boudary);
        static int  saveFile(const std::string& path, const std::string& content);
        static int  isDir(const std::string& path);
        static int  readFile(std::string& filename, std::string& buf);
        static int  getAutoIndex(const std::string& path, const std::string& relpath, std::string& body);
};

#endif