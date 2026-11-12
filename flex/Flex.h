#ifndef LAB1T_FLEX_H
#define LAB1T_FLEX_H

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <cstddef>
#include <string>
#include "../Analyz.h"

class Flex : public yyFlexLexer, public Analyz {
public:
    Flex();
    bool checkString(std::string& str) override;
    int yylex() override;

protected:
    int LexerInput(char* buf, int max_size) override;

private:
    std::string current_type;
    std::string current_name;
    long long declared_size = -1;
    long long literal_count = 0;
    const char* input_ptr = nullptr;
    size_t input_left = 0;
    bool first_chunk = true;
};

#endif //LAB1T_FLEX_H