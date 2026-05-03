#include "AppClass.h"

AppClass::AppClass()
    : _fsm(*this)
    , _litCount(0)
    , _declaredCount(0)
{}
bool AppClass::checkString(std::string& str)
{
    isAcceptable = true;
    _arrName.clear();
    _countStr.clear();
    _litCount = 0;
    _declaredCount = 0;
    _fsm.setState(Map1::Start);

    for (const char c : str) {
        _fsm.symbol(c);
        if (!isAcceptable) break;
    }
    _fsm.EOS();
    bool valid = (_litCount == _declaredCount);
    if (!valid) isAcceptable = false;
    if (valid) {
            if (!types.contains(_arrName)) {
                types[_arrName] = _currentType;
            } else {
                conflicts.insert(_arrName + " - " + types[_arrName]);
            }
            params[_arrName]++;
        }
    return isAcceptable;
}

void AppClass::checkFile(std::ifstream& file)
{
    std::string line;
    while (std::getline(file, line, '\n')) {
        checkString(line);
    }
}