#include "string_utils.h"

size_t string_actual_printed_length(const std::string& str) {
    int length = 0;
    for (size_t i = 0; i < str.size(); ++i) {
        auto c = str[i];
        if (c == '\x1B') // skip non-printable characters
            i += 3;
        else
            length += (c & 0xc0) != 0x80;
    }
    return length;
}

String_builder operator""_str(const char* s, long unsigned int) {
    return std::move(String_builder() + s);
}
