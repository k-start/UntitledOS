#include <Y/String.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

String::String() {
    buffer = 0;
    m_capacity = 0;
    m_size = 0;
}

// potentially bad implementation?
String::String(const char *str) {
    size_t len = strlen(str);
    buffer = new char[len + 1];
    memcpy(buffer, str, len);
    buffer[len] = '\0';
    // buffer = (char*)str;
    m_capacity = len;
    m_size = len;
}

String::~String() {
    delete[] buffer;
}

bool String::empty() const {
    return m_size == 0;
}

void String::append(const char &value) {
    if(m_size >= m_capacity) {
        if(m_capacity == 0) {
            reserve(1);
        } else {
            reserve(m_capacity * 2);
        }
    }
    buffer[m_size++] = value;
}

void String::append(const String &value) {
    if(m_size + value.length() >= m_capacity) {
        reserve(m_size + value.length());
    }

    for(int i = 0; i < value.length(); i++) {
        buffer[m_size + i] = String(value)[i];
    }

    m_size = m_size + value.length();
    m_capacity = m_size + value.length();
}

void String::reserve(unsigned int capacity) {
    char *newBuffer = new char[capacity];

    for (unsigned int i = 0; i < m_size; i++) {
        newBuffer[i] = buffer[i];
    }

    m_capacity = capacity;

    delete[] buffer;
    buffer = newBuffer;
}

char &String::operator[](unsigned int index) {
    return buffer[index];
}

String &String::operator+=(const char &value) {
    append(&value);
    return *this;
}

String &String::operator+=(const String &str) {
    append(str);
    return *this;
}

String &String::operator+=(const int &i) {
    char str[20];
    itoa(i, str);
    int len = strlen(str);
    for(int i = 0; i < len; i++) {
        append(str[i]);
    }
    return *this;
}

String &String::operator+(const String &str) {
    String *string = new String(buffer);
    *string += str;
    return *string;
}

// String &String::operator+(const int &i) {
//     String *string = new String(buffer);
//     char str[20];
//     itoa(i, str);
//     int len = strlen(str);
//     for(int i = 0; i < len; i++) {
//         string->append(str[i]);
//     }
//     return *string;
// }

String &String::operator<<(const char &value) {
    append(value);
    return *this;
}

String::operator const char*() const {
    return buffer;
}