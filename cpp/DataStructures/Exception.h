#include <stdexcept>

#ifndef ALGORITHMS_EXCEPTION_H
#define ALGORITHMS_EXCEPTION_H

namespace dsa {

class InvalidArgument : public std::invalid_argument {
  public:
    InvalidArgument(const std::string &message) : std::invalid_argument(message) {}

    InvalidArgument(const char *message) : std::invalid_argument(message) {}
};

class EmptyContainer : public std::out_of_range {
  public:
    EmptyContainer(const std::string &message) : std::out_of_range(message) {}

    EmptyContainer(const char *message) : std::out_of_range(message) {}
};

} // namespace dsa

#endif // ALGORITHMS_EXCEPTION_H
