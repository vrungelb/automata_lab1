#include "Flex.h"
#include <sstream>

Flex::Flex() = default;

bool Flex::checkString(std::string& str) {
    isAcceptable    = false;
    current_type.clear();
    current_name.clear();
    declared_size   = -1;
    literal_count   = 0;

    std::string prepared = str + "\n";
    std::istringstream ss(prepared);
    switch_streams(&ss, nullptr);
    yylex();

    return isAcceptable;
}