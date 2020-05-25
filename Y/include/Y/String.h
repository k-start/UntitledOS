#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <kernel/kmalloc.h>

class String {
    public:
        String();
        String(const char *str);
        String(const String &str) ;
        ~String();

        bool empty() const;

        void append(const char &value);
        void append(const String &value);

        void reserve(unsigned int capacity);
        void pop_back();
        void clear();

        unsigned int capacity() const { return m_capacity; }
        unsigned int size() const { return m_size; }
        int length() const { return (int)m_size; }

        char *c_str() const;

        char &operator[](unsigned int index);
        char &operator[](unsigned int index) const;
        String &operator+=(const char &value);
        String &operator+=(const String &str);
        String &operator+=(const int &i);
        String &operator+(const String &str);
        // String &operator+(const int &i);

        String &operator=(const String &str);

        bool operator==(const String &str);

        String &operator<<(const char &value);
        
        // operator const char*() const;

    private:
        unsigned int m_capacity = 0;
        unsigned int m_size = 0;
        char *buffer;
        
};

#endif