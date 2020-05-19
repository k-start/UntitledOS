#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
// #include <kernel/kstdlib.h>

template<typename T> class Vector {
    public:
        Vector(); 
        Vector(unsigned int size);
        ~Vector();

        bool empty() const;

        void push_back(const T &value);
        void pop_back();

        void reserve(unsigned int capacity);

        unsigned int capacity() const { return m_capacity; }
        unsigned int size() const { return m_size; }
        unsigned int length() const { return m_size; }

        T &operator[](unsigned int index);
        Vector<T> &operator+=(const T &value);
        Vector<T> &operator<<(const T &value);

    private:
        unsigned int m_capacity = 0;
        unsigned int m_size = 0;
        T *buffer;
        
};

template<typename T>
Vector<T>::Vector() {
    buffer = 0;
    m_capacity = 0;
    m_size = 0;
}

template<typename T>
Vector<T>::Vector(unsigned int size) {
    buffer = new T[size];
    m_capacity = size;
    m_size = 0;
}

template<typename T>
Vector<T>::~Vector() {
    delete[] buffer;
}

template <typename T>
bool Vector<T>::empty() const {
    return m_size == 0;
}

template <typename T>
void Vector<T>::push_back(const T &value) {
    if(m_size >= m_capacity) {
        if(m_capacity == 0) {
            reserve(1);
        } else {
            reserve(m_capacity * 2);
        }
    }
    buffer[m_size++] = value;
}

template <typename T>
void Vector<T>::pop_back() {
    if(m_size > 0) {
        (reinterpret_cast<T*>(buffer)[m_size-1]).~T();
        m_size--;
    }
}

template<class T>
void Vector<T>::reserve(unsigned int capacity) {
    T *newBuffer = new T[capacity];

    for (unsigned int i = 0; i < m_size; i++) {
        newBuffer[i] = buffer[i];
    }

    m_capacity = capacity;
    delete[] buffer;
    buffer = newBuffer;
}

template<class T>
T &Vector<T>::operator[](unsigned int index) {
    return buffer[index];
}

template<class T>
Vector<T> &Vector<T>::operator+=(const T &value) {
    push_back(value);
    return *this;
}

template<class T>
Vector<T> &Vector<T>::operator<<(const T &value) {
    push_back(value);
    return *this;
}

#endif