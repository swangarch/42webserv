#include "../../includes/AnyErrorException.hpp"

AnyErrorException::AnyErrorException(const std::string message) throw() {
    _error = "Error: " + message;
}

AnyErrorException::~AnyErrorException() throw() {
}

const char  *AnyErrorException::what() const throw() {
    return _error.c_str();
}