#ifndef LAB1T_FLEX_H
#define LAB1T_FLEX_H

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <string>
#include "../smc/Analyz.h"

class Flex : public yyFlexLexer, public Analyz {
public:
    Flex();
    bool checkString(std::string& str) override;
    int yylex() override;

private:
    std::string current_type;
    std::string current_name;
    int declared_size = -1;
    int literal_count = 0;
};

#endif //LAB1T_FLEX_H