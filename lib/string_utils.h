#pragma once

#include <string>
#include <sstream>

size_t string_actual_printed_length(const std::string& str);

class String_builder {
    private:
        std::stringstream ss;
    public:
        template<typename T>
            String_builder& operator+(T&& t) {
                ss << std::forward<T>(t);
                return *this;
            }
        operator std::string() {
            return ss.str();
        }
};
String_builder operator""_str(const char* s, long unsigned int);

