#pragma once

# include "standardLib.hpp"

class   AnyErrorException : public std::exception {
public:
    AnyErrorException(const std::string message) throw();
    virtual ~AnyErrorException() throw();
    virtual const char  *what() const throw();
private:
    std::string     _error;
};