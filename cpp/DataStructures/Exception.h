#include <stdexcept>

#ifndef DATASTRUCTURE_EXCEPTION_H
#define DATASTRUCTURE_EXCEPTION_H

namespace ds {

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

class OutOfRange : public std::out_of_range {
  public:
    OutOfRange(const std::string &message) : std::out_of_range(message) {}

    OutOfRange(const char *message) : std::out_of_range(message) {}
};

} // namespace ds

#endif // DATASTRUCTURE_EXCEPTION_H
