#include "../../includes/file.class.hpp"

int         file::isDir(const std::string& path)
{
    struct  stat buf;

    if (stat(path.data(), &buf) == -1)
    {
        return (-1);
    }
    
    if (S_ISDIR(buf.st_mode))
        return (1);
    return (0);
}

int          file::saveFile(const std::string& path, const std::string& content)
{
    std::ofstream       outfile(path.data());

    if (!outfile)
    {
        log::loginfo(T_ERR, "Failed to open file.");
        return (-1);
    }

    outfile << content; //error check;

    if (outfile.fail() || outfile.bad())
    {
        log::loginfo(T_ERR, "Failed to write file.");
        return (-1);
    }

    outfile.close();

    if (outfile.fail())
    {
        log::loginfo(T_ERR, "Failed to close file.");
        return (-1);
    }

    return (1);
}

int         file::readFile(std::string& filename, std::string& buf)
{
    std::ifstream file(filename.data(), std::ios::binary);
    if (!file)
    {
        log::loginfo(T_ERR, "Failed to open file.");
        return (0);
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    buf.resize(size);
    file.read(&buf[0], size);
    file.close();
    return (1);
}

int     file::getBoudary(const std::string& contentType, std::string& boudary)  //if return -1 means bad request
{
    size_t pos = contentType.find("multipart/form-data; boundary=");
    if (pos == std::string::npos || pos != 0)
    {
        log::loginfo(T_ERR, "Wrong multipart/form-data format.");
        return (-1);
    }

    boudary = contentType.substr(30);

    if (boudary.size() > 70 || boudary.size() == 0)
    {
        log::loginfo(T_ERR, "Wrong multipart/form-data boundary size.");
        return (-1);
    }
    return (1);
}

int          file::handleUpload(const std::string& data, const std::string& path, const std::string& boundary)
{
    std::string         sepMark = "--" + boundary;
    std::string         endMark = "--" + boundary + "--";
    std::string         fileContent;
    size_t              startPos = 0;
    size_t              sepLen = sepMark.size();

    while (true)
    {
        size_t          boundaryPos = data.find(sepMark, startPos);
        if (boundaryPos == std::string::npos)
        {
            break;
        }

        size_t          nextBoudaryPos = data.find(sepMark, boundaryPos + sepLen);
        if (nextBoudaryPos == std::string::npos)
        {
            if (data.find(endMark, boundaryPos) == std::string::npos)
                return (-1);
            else
                return (1);
        }
        std::string     part = data.substr(boundaryPos + sepLen, nextBoudaryPos - boundaryPos - sepLen);
        size_t          headEndPos = part.find("\r\n\r\n");
        {
            if (headEndPos != std::string::npos)
            {
                std::string     filename;
                std::string     header = part.substr(0, headEndPos);
                std::string     body = part.substr(headEndPos + 4);
                size_t          posFilename = header.find("filename=\"");

                if (posFilename != std::string::npos)
                {
                    size_t  posEndl = header.find("\"\r\n", posFilename + 10);
                    if (posEndl == std::string::npos)
                        return (-1);
                    
                    filename = header.substr(posFilename + 10, posEndl - 10 - posFilename);
                    
                    if (!reqParser::isValidUriChar(filename))
                    {
                        return (-1);
                    }

                    if (isDir((path + "/files").c_str()) != 1 || access((path + "/files").c_str(), F_OK | X_OK) != 0)
                    {
                        log::loginfo(T_ERR, "Path to upload doesn't exist.");
                        return (-2);
                    }

                    std::string         fullpath = path + "/files/" + filename;
                    if (body.size() >= 2 && body.substr(body.size() - 2) == "\r\n")
                        body = body.substr(0, body.size() - 2);
                    if (saveFile(fullpath, body) == -1)
                        return (-2);
                }
            }
            else
            {
                return (-1);
            }
        }
        startPos = nextBoudaryPos;
    }
    return (1);
}

int         file::delDir(const std::string& pathDir)  
{
    DIR* dir;
    struct dirent *structDir;
    std::string element;

    std::string path = pathDir;
    parseTools::addSlashToDirEnd(path);
    dir = opendir(path.c_str());
    if (!dir)
    {
        log::loginfo(T_ERR, "Opendir failed.");
        return (-1);
    }
    errno = 0;

    structDir = readdir(dir);
    if (structDir == NULL && errno != 0)
    {
        log::loginfo(T_ERR, "Readdir failed.");
        closedir(dir);
        return (-1);
    }
    while (structDir)
    {
        element = std::string(structDir->d_name);

        if (element == "." || element == "..")
        {
            structDir = readdir(dir);
            continue;
        }

        std::string     fullpath = path + element;
        int             dirStatus = isDir(fullpath.c_str());
        if (dirStatus == 1)
        {
            if (delDir(path + element) == -1)
            {
                if (closedir(dir) == -1)
                {
                    log::loginfo(T_ERR, "Closedir failed.");
                    return (-1);
                }
                return (-1);
            }
        }
        else if (dirStatus == -1)
        {
            return (-1);
        }
        else 
        {
            if (std::remove(fullpath.c_str()) == -1)
            {
                log::loginfo(T_ERR, "Remove failed.");
                if (closedir(dir) == -1)
                {
                    log::loginfo(T_ERR, "Closedir failed.");
                    return (-1);
                }
                return (-1);
            }
        }

        errno = 0;
        structDir = readdir(dir);
        if (structDir == NULL && errno != 0)
        {
            log::loginfo(T_ERR, "Readdir failed.");
            closedir(dir);
            return (-1);
        }
    }

    if (closedir(dir) == -1)
    {
        log::loginfo(T_ERR, "Closedir failed.");
        return (-1);
    }

    if (std::remove(pathDir.c_str()) == -1)
    {
        log::loginfo(T_ERR, "Remove failed.");
        return (-1);
    }
    return (0);
}

int         file::getAutoIndex(const std::string& path, const std::string& relpath, std::string& body)
{
    DIR* dir;
    struct dirent *structDir;
    std::string element;

    dir = opendir(path.data());
    if (!dir)
    {
        log::loginfo(T_ERR, "Opendir failed.");
        return (-1);
    }

    body += "AUTOINDEX";

    errno = 0;

    structDir = readdir(dir);
    while (structDir)
    {
        element = std::string(structDir->d_name);

        if (element == "." || element == "..")
        {
            structDir = readdir(dir);
            continue;
        }
        body += "<p><a href=\"";
        if (relpath != "/")
            body += "/";
        body += relpath;
        body += element;
        body += "\">";
        body += element;
        body += "</a></p>";

        structDir = readdir(dir);
    }

    if (errno != 0)
    {
        log::loginfo(T_ERR, "Readdir failed.");
        parseTools::clearString(body);
        if (closedir(dir) == -1)
        {
            return (-1);
        }
        return (-1);
    }

    infopage::getInfoPage(body, body);

    if (closedir(dir) == -1)
    {
        log::loginfo(T_ERR, "Closedir failed.");
        parseTools::clearString(body);
        return (-1);
    }
    return (0);
}