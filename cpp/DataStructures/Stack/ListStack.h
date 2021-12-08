
#ifndef ALGORITHMS_LISTSTACK_H
#define ALGORITHMS_LISTSTACK_H

#include <iostream>
#include <exception>
#include "Stack.h"

namespace dsa {
    template<typename T>
    class ListStack : public Stack<T> {
        int capacity_;
    };
}

#endif //ALGORITHMS_LISTSTACK_H
