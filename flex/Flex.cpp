#include "Flex.h"
#include <algorithm>
#include <cstring>

Flex::Flex() = default;

int Flex::LexerInput(char* buf, int max_size) {
    if (input_left == 0 || max_size <= 0) {
        return 0;
    }
    size_t chunk_cap = static_cast<size_t>(max_size);
    if (first_chunk) {
        first_chunk = false;
        if (chunk_cap > 256) {
            chunk_cap = 256;
        }
    }
    const size_t n = std::min(input_left, chunk_cap);
    std::memcpy(buf, input_ptr, n);
    input_ptr += n;
    input_left -= n;
    return static_cast<int>(n);
}

bool Flex::checkString(std::string& str) {
    isAcceptable  = false;
    current_type.clear();
    current_name.clear();
    declared_size = -1;
    literal_count = 0;

    input_ptr   = str.data();
    input_left  = str.size();
    first_chunk = true;

    yyrestart(nullptr);
    yylex();

    return isAcceptable;
}