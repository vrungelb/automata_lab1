#ifndef LAB1T_FLEX_H
#define LAB1T_FLEX_H
#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif
#include <string>
#include <unordered_map>
#include "../smc/Analyz.h"

class Flex : public Analyz, public yyFlexLexer {
public:
    bool is_valid = false;
    std::string current_type;
    std::string current_name;
    int declared_size = -1;
    int literal_count = 0;
    Flex();
    bool checkString(const std::string &str) override;
    void checkFile(std::ifstream &file) override;
    int yylex() override;
};

#endif


