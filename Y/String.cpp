#include <Y/String.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <kernel/kstdio.h>

String::String() {
    buffer = 0;
    m_capacity = 0;
    m_size = 0;
}

String::String(const char *str) {
    size_t len = strlen(str);
    buffer = new char[len + 1];
    memcpy(buffer, str, len);
    buffer[len] = '\0';
    
    m_capacity = len;
    m_size = len;
}

String::String(const String &str) {
    buffer = new char[str.length() + 1];
    
    memcpy(buffer, str.c_str(), str.length());

    buffer[str.length()] = '\0';
    m_capacity = str.length();
    m_size = str.length();
}


String::~String() {
    if(buffer != NULL) {
        delete[] buffer;
        buffer = NULL;
    }
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

    if(m_capacity != 0) {
        delete[] buffer;
        buffer = 0;
    }

    m_capacity = capacity;
    
    buffer = newBuffer;
}

void String::pop_back() {
    if(m_size > 0) {
        buffer[m_size-1] = '\0';
        m_size--;
    }
}

void String::clear() {
    memset(buffer, '\0', m_capacity);
    m_size = 0;
}

String String::toUpper() {
    String newString;
    for(int i = 0; i < length(); i++) {
        if(buffer[i] >= 'a' && buffer[i] <= 'z') {
            newString.append(buffer[i] - ('a' - 'A'));
        } else {
            newString.append(buffer[i]);
        }
    }
    return newString;
}

Vector<String> String::split(char delim) {
    Vector<String> v;
    String s;

    for(int i = 0; i < length(); i++) {
        if(buffer[i] == delim) {
            if(s.length() > 0) {
                v.push_back(s);
                sout("%s\n", s);
                s.clear();
            }
        } else {
            s.append(buffer[i]);
        }
    }
    if(s.length() > 0) {
        v.push_back(s);
    }
    return v;
}

char *String::c_str() const {
    buffer[m_size] = '\0';
    return buffer;
}

char &String::operator[](unsigned int index) {
    return buffer[index];
}

char &String::operator[](unsigned int index) const {
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

String &String::operator=(const String &str) {
    delete[] buffer;

    buffer = new char[str.length() + 1];
    
    memcpy(buffer, str.c_str(), str.length());

    buffer[str.length()] = '\0';
    m_capacity = str.length();
    m_size = str.length();

    return *this;
}

bool String::operator==(const String &str) {
    if(str.length() != length()) {
        return false;
    }
    for(int i = 0; i < length(); i++) {
        if(str[i] != buffer[i]) {
            return false;
        }
    }
    return true;
}

// String::operator const char*() const {
//     buffer[m_size] = '\0';
//     return buffer;
// }