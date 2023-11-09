#ifndef CUSTOM_ERROR_HPP
#define CUSTOM_ERROR_HPP

#include <exception>
#include <string>

class CustomError : public std::exception {
private:
	std::string _message;
public:
	CustomError(const std::string& message) : _message(message) {}

	virtual const char* what() const throw() {
		return _message.c_str();
	}
};

#endif
