#include "AppClass.h"

AppClass::AppClass()
    : _fsm(*this)
    , _litCount(0)
    , _declaredCount(0)
{}
bool AppClass::checkString(std::string& str)
{
    _arrName.clear();
    _countStr.clear();
    _litCount = 0;
    _declaredCount = 0;
    _fsm.setState(Map1::Start);

    isAcceptable = true;    // будем считать строку правильной, пока не будет доказано обратное

    for (const char c : str) {
        _fsm.symbol(c);
        if (!isAcceptable) break;
    }

    if (isAcceptable) _fsm.EOS();

    isAcceptable = isAcceptable && (_litCount == _declaredCount);

    if (isAcceptable) {
        if (!types.contains(_arrName)) {
            types[_arrName] = _currentType;
        } else {
            conflicts.insert(_arrName + " - " + types[_arrName]);
        }
        params[_arrName]++;
    }
    return isAcceptable;
}
