%{
#include "Flex.h"
#include <algorithm>
#include <cctype>
#include <string>

#undef  YY_DECL
#define YY_DECL int Flex::yylex()
%}

%option batch
%option c++
%option noyywrap
%option yyclass="Flex"

%x S_AFTER_TYPE S_NAME S_BRACKETS
%x S_LIT_START S_LIT_DIGITS S_AFTER_RBRACE

LETTER     [a-zA-Z]
DIGIT      [0-9]
NZ_DIGIT   [1-9]
ALNUM      [a-zA-Z0-9]
ID         {LETTER}{ALNUM}*
SIZE       {NZ_DIGIT}{DIGIT}*
LITERAL    "-"?{DIGIT}+
ANY        (.|\n)

%%

<INITIAL>"int"          { current_type = "int";   BEGIN(S_AFTER_TYPE); }
<INITIAL>"short"        { current_type = "short"; BEGIN(S_AFTER_TYPE); }
<INITIAL>"long"         { current_type = "long";  BEGIN(S_AFTER_TYPE); }
<INITIAL>{ANY}          { BEGIN(INITIAL); return 0; }

<S_AFTER_TYPE>" "       { BEGIN(S_NAME); }
<S_AFTER_TYPE>{ANY}     { BEGIN(INITIAL); return 0; }

<S_NAME>{ID} {
    current_name.assign(yytext, yyleng);
    std::transform(current_name.begin(), current_name.end(),
                   current_name.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    BEGIN(S_BRACKETS);
}
<S_NAME>{ANY}           { BEGIN(INITIAL); return 0; }

<S_BRACKETS>"["{SIZE}"]={" {
    int num_len = yyleng - 4;
    std::string num(yytext + 1, num_len);
    declared_size  = std::stoi(num);
    literal_count  = 0;
    BEGIN(S_LIT_START);
}
<S_BRACKETS>{ANY}       { BEGIN(INITIAL); return 0; }

<S_LIT_START>{LITERAL}  { literal_count++; BEGIN(S_LIT_DIGITS); }
<S_LIT_START>{ANY}      { BEGIN(INITIAL); return 0; }

<S_LIT_DIGITS>","       { BEGIN(S_LIT_START); }
<S_LIT_DIGITS>"}"       {
    if (literal_count == declared_size) {
        if (types.find(current_name) == types.end()) {
            types[current_name] = current_type;
        } else {
            conflicts.insert(current_name + " - " + types[current_name]);
        }
        params[current_name]++;
        isAcceptable = true;
    } else {
        isAcceptable = false;
    }
    BEGIN(S_AFTER_RBRACE);
}
<S_LIT_DIGITS>{ANY}     { BEGIN(INITIAL); return 0; }

<S_AFTER_RBRACE>\n      { BEGIN(INITIAL); return 0; }
<S_AFTER_RBRACE>.       { isAcceptable = false; BEGIN(INITIAL); return 0; }

<*><<EOF>>              { BEGIN(INITIAL); return 0; }

%%