#pragma once

# include "standardLib.hpp"
# include "ServerInfo.hpp"
#include "define.hpp"
# include "AnyErrorException.hpp"
# include "CheckFileInfo.hpp"

class   ServerInfo;

class   ConfigManager {
public:
    ConfigManager();
    ConfigManager(const ConfigManager& src);
    ~ConfigManager();

    ConfigManager&  operator=(const ConfigManager& src);

    std::vector<ServerInfo>&  getServerSet();
    ServerInfo&   getServerSet(size_t index);
    size_t  getNumServer() const;

    void    creatConfig(const std::string& path);
    void    printConfig() const;
    static void    setToken(const std::string& content, const std::string& set, std::vector<std::string>& token);
    static void    handleMultiServer(std::vector<ServerInfo>& server_set, std::vector<std::vector<ServerInfo> >& server_grp); 
    
private:
    std::vector<std::string>    _server_content;
    std::string                 _file_content;
    std::vector<ServerInfo>     _server_set;
    size_t                      _num_server;
    size_t  beginServerBlock(size_t begin) const;
    size_t  endServerBlock(size_t begin) const;
    void    eraseComments();
    void    eraseSpace();
    void    splitContent();
    void    initServer(int index);
    void    checkRepeatServer() const;
    void    preprocess(std::string& result, const std::string& src) const;
};