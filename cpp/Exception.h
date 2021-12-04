#include <exception>
#include <stdexcept>

#ifndef ALGORITHMS_EXCEPTION_H
#define ALGORITHMS_EXCEPTION_H

class InvalidArgument : public std::invalid_argument {
public:
    InvalidArgument(const std::string &message) : std::invalid_argument(message) {
    }

    InvalidArgument(const char *message) : std::invalid_argument(message) {
    }
};

#endif //ALGORITHMS_EXCEPTION_H
