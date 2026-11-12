#ifndef LAB1T_ANALYZ_H
#define LAB1T_ANALYZ_H

#include <fstream>
#include <unordered_set>
#include <string>
#include <unordered_map>

class Analyz {
protected:
    bool isAcceptable = false;
    std::unordered_map<std::string, int> params;
    std::unordered_map<std::string, std::string> types;
    std::unordered_set<std::string> conflicts;

    void recordValid(const std::string& arrName, const std::string& type) {
        if (!types.contains(arrName)) {
            types[arrName] = type;
        } else {
            conflicts.insert(arrName + " - " + types[arrName]);
        }
        params[arrName]++;
    }

public:
    virtual bool checkString(std::string& str) = 0;

    virtual void checkFile(std::ifstream& file) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            checkString(line);
        }
    }

    // сброс накопленного состояния между прогонами в бенчмарке
    virtual void reset() {
        params.clear();
        types.clear();
        conflicts.clear();
        isAcceptable = false;
    }

    std::string getTable() const {
        std::string t;
        for (const auto& [key, value] : params) {
            t += key + "=" + std::to_string(value) + "\n";
        }
        return t;
    }

    const std::unordered_set<std::string>& getConflicts() const {
        return conflicts;
    }

    virtual ~Analyz() = default;
};

#endif //LAB1T_ANALYZ_H