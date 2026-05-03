%{
#include "Flex.h"
#include <string>
#undef YY_DECL
#define YY_DECL int Flex::yylex()
%}

%option c++
%option noyywrap
%option yyclass="Flex"

%x STATE_NAME STATE_SIZE STATE_LIST STATE_ERROR

TYPE       "int"|"short"|"long"
ID         [a-zA-Z][a-zA-Z0-9]*
DIGIT      [0-9]
NZ_DIGIT   [1-9]
SIZE       {NZ_DIGIT}{DIGIT}*
LITERAL    "-"?{DIGIT}+
WS         [ \t]+

%%

<INITIAL>{
    {TYPE}      { this->current_type = yytext; BEGIN(STATE_NAME); }
    .|\n        ;
}

<STATE_NAME>{
    {WS}        ;
    {ID}        { this->current_name = yytext; BEGIN(STATE_SIZE); }
    .           { BEGIN(STATE_ERROR); }
}

<STATE_SIZE>{
    "["         ;
    {SIZE}      { this->declared_size = std::stoi(yytext); }
    "]"         { }
    "="         ;
    "{"         { this->literal_count = 0; BEGIN(STATE_LIST); }
    .           { BEGIN(STATE_ERROR); }
}

<STATE_LIST>{
    {LITERAL}   { this->literal_count++; }
    ","         ;
"}" {
    if (this->declared_size != -1 && this->literal_count != this->declared_size) {
        this->is_valid = false;
    }
    else {
        if (this->types.find(this->current_name) == this->types.end()) {
            this->types[this->current_name] = this->current_type;
        } else if (this->types[this->current_name] != this->current_type) {
            this->conflicts.insert(this->current_name + " - " + this->types[this->current_name]);
        }
        this->params[this->current_name]++;
        this->is_valid = true;
    }
    BEGIN(INITIAL);
}
    .           { BEGIN(STATE_ERROR); }
}

<STATE_ERROR>{
    \n          { this->is_valid = false; BEGIN(INITIAL); }
    .           ;
}

%%