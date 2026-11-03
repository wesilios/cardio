#ifndef DATASTRUCTURE_ARRAY_H
#define DATASTRUCTURE_ARRAY_H

#include "../Exception.h"
#include <cstddef>

namespace ds {
template <typename T, std::size_t N> struct Array {
    using size_type = std::size_t;

    T m_data[N];

    constexpr size_type size() const { return N; }
    constexpr bool empty() const { return N == 0; }

    T &at(size_type index) {
        if (index >= N)
            throw OutOfRange("Array index out of bounds");

        return m_data[index];
    }

    const T &at(size_type index) const {
        if (index >= N)
            throw OutOfRange("Array index out of bounds");

        return m_data[index];
    }

    T &operator[](size_type index) { return m_data[index]; }
    const T &operator[](size_type index) const { return m_data[index]; }
};
} // namespace ds
#endif // DATASTRUCTURE_ARRAY_H
