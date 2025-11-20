#include "../../include/main.hpp"

Server::initError::initError(const std::string& error)
{
	std::strncpy(_error, "Error on : ", 99);
	std::strncat(_error, error.c_str(), 99 - std::strlen(_error));
	_error[99] = '\0';
}

const char*	Server::initError::what() const throw()
{
	return (_error);
}
