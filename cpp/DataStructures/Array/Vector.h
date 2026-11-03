#ifndef DATASTRUCTURE_VECTOR_H
#define DATASTRUCTURE_VECTOR_H

#include "../Exception.h"
#include <algorithm>
#include <cstddef>
#include <limits>
#include <utility>

namespace ds {

template <typename T> class VectorIterator {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = T &;
    using pointer = T *;

    explicit VectorIterator(T *ptr = nullptr) : m_pointer(ptr) {}

    reference operator*() const { return *m_pointer; }
    pointer operator->() { return m_pointer; }
    reference operator[](difference_type off) const { return m_pointer[off]; }

    VectorIterator &operator++() {
        m_pointer++;
        return *this;
    }

    VectorIterator operator++(int) {
        VectorIterator tmp = *this;
        m_pointer++;
        return tmp;
    }

    VectorIterator &operator--() {
        m_pointer--;
        return *this;
    }

    VectorIterator operator--(int) {
        VectorIterator tmp = *this;
        m_pointer--;
        return tmp;
    }

    // 3. Arithmetic Operators (Random Access)
    VectorIterator &operator+=(difference_type off) {
        m_pointer += off;
        return *this;
    }

    VectorIterator operator+(difference_type off) const { return VectorIterator(m_pointer + off); }

    friend VectorIterator operator+(difference_type off, const VectorIterator &it) {
        return VectorIterator(it.m_pointer + off);
    }

    VectorIterator &operator-=(difference_type off) {
        m_pointer -= off;
        return *this;
    }

    VectorIterator operator-(difference_type off) const { return VectorIterator(m_pointer - off); }
    difference_type operator-(const VectorIterator &other) const { return m_pointer - other.m_pointer; }

    // 4. Comparison Operators
    bool operator==(const VectorIterator &other) const { return m_pointer == other.m_pointer; }
    bool operator!=(const VectorIterator &other) const { return m_pointer != other.m_pointer; }
    bool operator<(const VectorIterator &other) const { return m_pointer < other.m_pointer; }
    bool operator<=(const VectorIterator &other) const { return m_pointer <= other.m_pointer; }
    bool operator>(const VectorIterator &other) const { return m_pointer > other.m_pointer; }
    bool operator>=(const VectorIterator &other) const { return m_pointer >= other.m_pointer; }

  private:
    T *m_pointer;
};

template <typename T> class Vector {
  public:
    using size_type = std::size_t;
    using iterator = VectorIterator<T>;
    using const_iterator = VectorIterator<const T>;

    Vector() : m_data(nullptr), m_size(0), m_capacity(0) {}

    ~Vector() { delete[] m_data; }

    // TODO: rule of three: copy constructor and copy assignment (deep copy).
    // TODO: rule of five: noexcept move constructor and move assignment.

    // Capacity
    size_type size() const { return m_size; }
    size_type max_size() const { return std::numeric_limits<std::size_t>::max() / sizeof(T); }
    size_type capacity() const { return m_capacity; }
    bool empty() const { return m_size == 0; }
    void shrink_to_fit() {
        if (m_size == m_capacity)
            return;
        T *new_data = new T[m_size];

        for (size_type i = 0; i < m_size; ++i) {
            new_data[i] = std::move_if_noexcept(m_data[i]);
        }

        delete[] m_data;
        m_data = new_data;
        m_capacity = m_size;
    }

    void reserve(size_type new_capacity) {
        if (new_capacity <= m_capacity)
            return;

        T *new_data = new T[new_capacity];

        for (size_type i = 0; i < m_size; ++i) {
            new_data[i] = std::move_if_noexcept(m_data[i]);
        }

        delete[] m_data;
        m_data = new_data;
        m_capacity = new_capacity;
    }

    // Modifiers
    void clear() {
        m_size = 0;
        m_capacity = 0;
        delete[] m_data;
        m_data = nullptr;
    }

    void insert(const size_type index, T value) {
        if (index > m_size)
            throw OutOfRange("Vector index out of bounds");

        if (m_size == m_capacity) {
            size_type new_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
            reserve(new_capacity);
        }

        for (size_type i = m_size; i > index; i--) {
            m_data[i] = std::move(m_data[i - 1]);
        }

        m_data[index] = std::move(value);
        m_size++;
    }

    void push_back(const T &value) { insert(m_size, value); }

    T pop_back() {
        if (empty())
            throw EmptyContainer("Vector is empty");
        T value = std::move(m_data[size() - 1]);

        m_size--;
        return value;
    }

    // TODO: swap m_data, m_size and m_capacity with other.
    void swap(Vector<T> &other) {}
    void erase(const size_type index) {
        checkIndex(index);
        for (size_type i = index + 1; i < m_size; i++) {
            m_data[i - 1] = std::move(m_data[i]);
        }

        m_size--;
    }

    void erase(const size_type start, size_type end) {
        if (start == end)
            return;
        if (end > m_size)
            throw OutOfRange("Vector index out of bounds");
        if (start > end)
            throw OutOfRange("Invalid Range: Start is after end");

        size_type left = start;
        size_type right = end;
        while (right < m_size) {
            m_data[left] = m_data[right];
            left++;
            right++;
        }

        m_size = m_size - (end - start);
        shrink_to_fit();
    }

    // TODO: iterator-based overloads for erase, as std::vector offers them.
    // iterator erase(const_iterator first, const_iteratvor last);
    iterator erase(const_iterator position) {
        size_type index = position - cbegin();
        checkIndex(index);
        T *target = m_data + index;

        std::move(target + 1, m_data + m_size, target);

        --m_size;

        return iterator(target);
    }

    // TODO: iterator-based overloads, as std::vector offers them. Each takes a
    // const_iterator (a position only; nothing is written through it) and returns
    // an iterator to the element that now sits at that position, because
    // inserting or erasing invalidates the iterators at and after it.
    //
    // iterator insert(const_iterator position, const T &value);
    // iterator insert(const_iterator position, size_type count, const T &value);
    // template <typename InputIt> iterator insert(const_iterator position, InputIt first, InputIt last);

    // Element Access
    T &at(const size_type index) {
        checkIndex(index);
        return m_data[index];
    }

    const T &at(const size_type index) const {
        checkIndex(index);
        return m_data[index];
    }

    T &front() {
        checkNotEmpty();
        return m_data[0];
    }

    const T &front() const {
        checkNotEmpty();
        return m_data[0];
    }

    T &back() {
        checkNotEmpty();
        return m_data[size() - 1];
    }

    const T &back() const {
        checkNotEmpty();
        return m_data[size() - 1];
    }

    T &operator[](const size_type index) { return m_data[index]; }

    const T &operator[](const size_type index) const { return m_data[index]; }

    // TODO: const overloads returning an iterator over const T, so a const Vector stays read-only.
    iterator begin() const { return iterator(m_data); }
    iterator end() const { return iterator(m_data + m_size); }
    const_iterator cbegin() const { return const_iterator(m_data); }
    const_iterator cend() const { return const_iterator(m_data + m_size); }

  private:
    T *m_data;
    size_type m_size;
    size_type m_capacity;

    void checkIndex(const size_type index) const {
        if (index >= m_size)
            throw OutOfRange("Vector index out of bounds");
    }

    void checkNotEmpty() const {
        if (empty())
            throw EmptyContainer("Vector is empty");
    }
};
} // namespace ds
#endif // DATASTRUCTURE_VECTOR_H
