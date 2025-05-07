#include "../../includes/ConfigManager.hpp"

ConfigManager::ConfigManager() : _num_server(0) {
}

ConfigManager::ConfigManager(const ConfigManager& src) {
    *this = src;
}

ConfigManager&  ConfigManager::operator=(const ConfigManager& src) {
    if (this != &src) {
        _server_content = src._server_content;
        _file_content = src._file_content;
        _server_set = src._server_set;
        _num_server = src._num_server;
    }
    return *this;
}

ConfigManager::~ConfigManager() {
}

std::vector<ServerInfo>&  ConfigManager::getServerSet() {
    return _server_set;
}

ServerInfo&   ConfigManager::getServerSet(size_t index) {
    if (index < _server_set.size()) {
        return _server_set[index];
    } else {
        throw AnyErrorException("Index is out of range!");
    }
}

size_t  ConfigManager::getNumServer() const {
    return _num_server;
}

void    ConfigManager::creatConfig(const std::string& path) {
    CheckFileInfo   fileInfo(path);
    if (fileInfo.checkFileType() != 0) {
        throw AnyErrorException("Not a regular file or error occurred!");
    }
    if (fileInfo.checkFileAccess(R_OK) == 1) {
        throw AnyErrorException("Read permission denied!");
    }
    fileInfo.getFileContent(_file_content);
    eraseComments();
    eraseSpace();
    splitContent();
    for (size_t i = 0; i < _num_server; ++i) {
        initServer(i);
    }
    if (_num_server > 1) {
        checkRepeatServer();
    }
}

void    ConfigManager::eraseComments() {
    size_t  pos = 0;
    while ((pos = _file_content.find('#', pos)) != std::string::npos) {
        size_t  end = _file_content.find('\n', pos);
        if (end == std::string::npos) {
            _file_content.erase(pos);
        } else {
            _file_content.erase(pos, end - pos);
        }
    }
}

void    ConfigManager::eraseSpace() {
    size_t  begin = 0;
    while (begin < _file_content.length() && isspace(_file_content[begin])) {
        ++begin;
    }
    size_t  end = _file_content.length() - 1;
    while (end > begin && isspace(_file_content[end])) {
        --end;
    }
    _file_content = _file_content.substr(begin, end - begin + 1);
}

void    ConfigManager::splitContent() {
    if (_file_content.find("server", 0) == std::string::npos) {
        throw AnyErrorException("No 'server' block!");
    }
    size_t  begin = 0;
    size_t  end = 1;
    while (begin != end && begin < _file_content.length()) {
        begin = beginServerBlock(begin);
        end = endServerBlock(begin);
        if (begin == end) {
            throw AnyErrorException("Invalid server block");
        }
        _server_content.push_back(_file_content.substr(begin, end - begin));
        ++_num_server;
        begin = end + 1;
    }
}

size_t  ConfigManager::beginServerBlock(size_t begin) const {
    size_t  pos = _file_content.find("server", begin);
    if (pos == std::string::npos) {
        throw AnyErrorException("Server block isn't in the right place!");
    }
    for (size_t i = begin; i < pos; ++i) {
        if (!isspace(_file_content[i])) {
            throw AnyErrorException("Illegal element between server block!");
        }
    }
    size_t  after = pos + 6;
    pos = _file_content.find('{', after);
    if (pos == std::string::npos) {
        throw AnyErrorException("'{' is expected after 'server'!");
    }
    for (; after < pos; ++after) {
        if (!isspace(_file_content[after])) {
            throw AnyErrorException("Illegal element between 'server' and '{'!");
        }
    }
    return pos + 1;
}

size_t  ConfigManager::endServerBlock(size_t begin) const {
    size_t  i = begin;
    size_t  num = 0;
    while (i < _file_content.length()) {
        if (_file_content[i] == '{') {
            ++num;
        } else if (_file_content[i] == '}') {
            if (num == 0) {
                return i;
            }
            --num;
        }
        ++i;
    }
    throw AnyErrorException("Missing closing '}'!");
}

void    ConfigManager::initServer(int index) {
    ServerInfo  server_tmp;
    bool        is_loc = false;
    bool        is_max_body_set = false;
    bool        is_autoindex_set = false;
    bool        is_methods_set = false;
    server_tmp.setCurIndex(index);
    std::vector<std::string>    token;
    std::string     process_str;
    preprocess(process_str, _server_content[index]);
    setToken(process_str, " \t\n", token);
    size_t      len = token.size();
    if (len < 4) {
        throw AnyErrorException("Not enough element in config file!");
    }
    for (size_t i = 0; i < len; ++i) {
        if (token[i] == "listen" && i == 0 && i + 1 < len && !is_loc) {
            if (server_tmp.getPort()) {
                throw AnyErrorException("Port is duplicated!");
            }
            server_tmp.setPort(token[++i]);
        } else if (token[i] == "server_name" && i + 1 < len && !is_loc) {
            if (!server_tmp.getServerName().empty()) {
                throw AnyErrorException("Server name is duplicated!");
            }
            server_tmp.setServerName(token[++i]);
        } else if (token[i] == "root" && i + 1 < len && !is_loc) {
            if (!server_tmp.getRoot().empty()) {
                throw AnyErrorException("Root is duplicated!");
            }
            server_tmp.setRoot(token[++i]);
        } else if (token[i] == "client_max_body_size" && i + 1 < len && !is_loc) {
            if (is_max_body_set) {
                throw AnyErrorException("Client_max_body_size is duplicated!");
            }
            server_tmp.setBodySize(token[++i]);
            is_max_body_set = true;
        } else if (token[i] == "autoindex" && i + 1 < len && !is_loc) {
            if (is_autoindex_set) {
                throw AnyErrorException("Autoindex is duplicated!");
            }
            server_tmp.setAutoindex(token[++i]);
            is_autoindex_set = true;
        } else if (token[i] == "index" && i + 1 < len && !is_loc) {
            if (!server_tmp.getIndex().empty()) {
                throw AnyErrorException("Index is duplicated!");
            }
            server_tmp.setIndex(token[++i]);
        } else if (token[i] == "error_page" && !is_loc) {
            if (server_tmp.getRoot().empty()) {
                throw AnyErrorException("Root is not set before error_page!");
            }
            server_tmp.setErrorPage(token, &i);
        } else if (token[i] == "method" && !is_loc) {
            if (is_methods_set) {
                throw AnyErrorException("Allow methods is duplicated!");
            }
            server_tmp.setMethods(token, &i);
            is_methods_set = true;
        } else if (token[i] == "cgi_extention" && !is_loc) {
            server_tmp.setCgiExt(token, &i);
        } else if (token[i] == "location") {
            server_tmp.setLocation(token, &i);
            is_loc = true;
        } else {
            throw AnyErrorException("Invalid element in the server block!");
        }
    }
    if (server_tmp.getPort() == 0) {
        throw AnyErrorException("Port not found!");
    }
    if (server_tmp.getRoot().empty()) {
        throw AnyErrorException("Root not found!");
    }
    if (server_tmp.getIndex().empty()) {
        throw AnyErrorException("Index not found!");
    }
    if (!is_methods_set) {
        server_tmp.setMethods();
    }
    _server_set.push_back(server_tmp);
}

void    ConfigManager::preprocess(std::string& result, const std::string& src) const {
    for (size_t i = 0; i < src.size(); ++i) {
        if (src[i] == '{' || src[i] == '}') {
            result = result + ' ' + src[i] + ' ';
        } else {
            result += src[i];
        }
    }
}

void    ConfigManager::setToken(const std::string& content, const std::string& set, std::vector<std::string>& token) {
    size_t  begin = content.find_first_not_of(set, 0);
    size_t  end = 0;
    while (begin != std::string::npos) {
        end = content.find_first_of(set, begin);
        if (end == std::string::npos) {
            token.push_back(content.substr(begin));
            break ;
        }
        token.push_back(content.substr(begin, end - begin));
        begin = content.find_first_not_of(set, end);
    }
}

void    ConfigManager::checkRepeatServer() const {
    std::vector<ServerInfo>::const_iterator it_first, it_second;
    for (it_first = _server_set.begin(); it_first != _server_set.end(); ++it_first) {
        for (it_second = it_first + 1; it_second != _server_set.end(); ++it_second) {
            if ((it_first->getIp() == it_second->getIp()) \
                && (it_first->getServerName() == it_second->getServerName()) \
                && (it_first->getPort() == it_second->getPort())) {
                    throw AnyErrorException("Server block duplicated!");
                }
        }
    }
}

void    ConfigManager::printConfig() const {
    // std::cout << std::endl << "Content of _server_content: " << std::endl;
    // std::vector<std::string>::const_iterator it1 = _server_content.begin();
    // for (int i = 0; it1 != _server_content.end(); ++it1, ++i) {
    //     std::cout << "Vector " << i << ": " << std::endl;
    //     std::cout << *it1 << std::endl;
    // }
    std::cout << "--------------------" << std::endl;
    std::cout << std::endl << "Content of _server_set: " << std::endl;
    std::vector<ServerInfo>::const_iterator it2 = _server_set.begin();
    for (; it2 != _server_set.end(); ++it2) {
        it2->printServerinfo();
    }
    std::cout << "--------------------" << std::endl;
    std::cout << "server number: " << _num_server << std::endl;
}

void    ConfigManager::handleMultiServer(std::vector<ServerInfo>& server_set, std::vector<std::vector<ServerInfo> >& server_grp) {
    std::vector<ServerInfo>::iterator it;
    std::set<std::string>    ipPort;
    for (it = server_set.begin(); it != server_set.end(); ++it) {
        ipPort.insert(it->getIpPort());
    }
    std::set<std::string>::iterator it1;
    for (it1 = ipPort.begin(); it1 != ipPort.end(); ++it1) {
        std::vector<ServerInfo>     multiServer;
        for (it = server_set.begin(); it != server_set.end(); ++it) {
            if (it->getIpPort() == *it1) {
                multiServer.push_back(*it);
            }
        }
        server_grp.push_back(multiServer);
    }
    std::vector<std::vector<ServerInfo> >::iterator    it2;
    size_t  index = 0;
    for (it2 = server_grp.begin(); it2 != server_grp.end(); ++it2, ++index) {
        for (it = it2->begin(); it != it2->end(); ++it) {
            it->setCurIndex(index);
        }
    }
}