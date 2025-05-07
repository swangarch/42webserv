#pragma once

#include "standardLib.hpp"
#include "define.hpp"
# include "AnyErrorException.hpp"
# include "ServerInfo.hpp"
# include "CheckFileInfo.hpp"

class   Location {
public:
    Location();
    Location(const Location& src);
    ~Location();

    Location&   operator=(const Location& src);

    void    setPath(const std::string& src);
    void    setRoot(const std::string& src);
    void    setRoot(const std::string& src, bool state);
    void    setAlias(const std::string& src);
    void    setIndex(const std::string& src);
    void    setIndexState(bool state);
    void    setReturn(std::vector<std::string>& token, size_t *i);
    void    setBodySize(const std::string& src);
    void    setBodySize(size_t size);
    void    setAutoindex(const std::string& src);
    void    setAutoindex(bool autoindex);
    void    setUpload(const std::string& src);
    void    setErrorPage(std::vector<std::string>& token, size_t *i);
    void    setErrorPage(const std::map<short, std::string>& src);
    void    setMethods(std::vector<std::string>& token, size_t *i);
    void    setMethods(const std::set<std::string>& methods);
    void    setCgiExt(std::vector<std::string>& token, size_t *i);
    void    setCgi(const std::set<std::string>& ext, std::map<std::string, std::string>& path);

    const std::string&  getPath() const;
    const std::string&  getRoot() const;
    const std::string&  getAlias() const;
    bool    getRootState() const;
    const std::string&  getIndex() const;
    bool    getIndexState() const;
    bool    getReturnState() const;
    const std::string&  getReturnContent() const;
    short   getReturnCode() const;
    bool    getSizeState() const;
    size_t  getBodySize() const;
    bool    getAutoindexState() const;
    bool    getAutoindex() const;
    const std::string&  getUpload() const;
    const std::map<short, std::string>& getErrorPages() const;
    const std::string getErrorPages(const short key) const;
    const std::set<std::string>&    getMethods() const;
    const std::set<std::string>& getCgiExt() const;
    const std::map<std::string, std::string>&   getCgiPath() const;
    const std::string   getCgiPath(const std::string& src) const;

    void    printLocation() const;

private:
    std::string                         _path;
    std::string                         _root;
    std::string                         _alias;
    bool                                _is_root_set;
    std::string                         _index;
    bool                                _is_index_set;
    std::string                         _return_content;
    short                               _return_code;
    bool                                _is_return_set;
    size_t                              _client_max_body_size;
    bool                                _is_size_set;
    bool                                _autoindex;
    bool                                _is_autoindex_set;
    std::string                         _upload;
    std::map<short, std::string>        _error_pages;
    std::set<std::string>               _methods;
	std::set<std::string>               _cgi_ext;
    std::map<std::string, std::string>  _cgi_path;

    bool    isValidRoot(const std::string& src) const;
    bool    isValidOrdre() const;
    bool    isValidReturnContent(const std::string& src) const;
    bool    isValidPath(const std::string& src) const;
    void    setCgiPath(std::string& ext, std::string& path);
};