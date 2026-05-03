#ifndef LAB1T_APPCLASS_H
#define LAB1T_APPCLASS_H
#define APPCLASS_H

#include <iostream>
#include <string>
#include "Analyz.h"
#include "Sm_sm.h"


class AppClass : public Analyz {
    SmContext _fsm;
    std::string _arrName;
    std::string _countStr;
    int         _litCount;       // подсчит. кол-во элементов массива
    long long   _declaredCount;  // заявл. кол-во элементов массива
    std::string _currentType;
public:
    AppClass();
    void setTypeInt()   { _currentType = "int"; }
    void setTypeShort() { _currentType = "short"; }
    void setTypeLong()  { _currentType = "long"; }
    bool checkString(std::string& str) override;
    static bool isLetter (char c) { return isalpha(c); }
    static bool isAlnum  (char c) { return isalnum(c); }
    static bool isDigit  (char c) { return isdigit(c); }
    static bool isNonZero(char c) { return c >= '1' && c <= '9';}
    size_t nameLen() const { return _arrName.size();  }  // геттер длины имени
    size_t countLen() const { return _countStr.size(); } // геттер длины строки кв. скобок
    int  litCount() const { return _litCount;}
    long long declaredCount() const { return _declaredCount; }
    void addNameChar (char c) { _arrName  += static_cast<char>(std::tolower(c)); }
    void addCountChar(char c) {
        _countStr += c;
        _declaredCount = std::stoll(_countStr);
    }
    void incLit() {
        ++_litCount;
    }
    void Suc()  { isAcceptable = true;  }
    void error(){ isAcceptable = false; }
};


#endif //LAB1T_APPCLASS_H
