#include "Flex.h"
#include <sstream>
#include <fstream>
Flex::Flex() = default;

bool Flex::checkString(const std::string& str) {
    std::string prepared = str + "\n";
    std::stringstream ss(prepared);
    this->is_valid = false;
    this->declared_size = -1;
    this->literal_count = 0;
    switch_streams(&ss, nullptr);
    yylex();
    return is_valid;
}

void Flex::checkFile(std::ifstream &file) {
    switch_streams(&file, nullptr);
    yylex();
}