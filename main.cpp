#include <iostream>
#include <limits>
#include "fstream"
#include "chrono"
#include "filesystem"
#include "regex/Regex.h"
#include "smc/Analyz.h"
#include "smc/AppClass.h"

using namespace statemap;

int main() {
    std::cout << "Working dir: " << std::filesystem::current_path() << std::endl;
    try {
        int option = 0;
        Analyz* analyzer = nullptr;
        std::string str;
        std::string f;
        std::ofstream out;
        std::ifstream in;
        std::string name;

        std::cout << "Choose an engine:" << std::endl;
        std::cout << "1. Regular" << std::endl;
        std::cout << "2. SMC" << std::endl;
        std::cin >> option;

        if (option == 1) {
            analyzer = new Regex();
            name += "regex";
        } else if (option == 2) {
            analyzer = new AppClass();
            name += "smc";
        } else {
            std::cout << "Invalid choice" << std::endl;
            return 1;
        }

        option = 0;
        while (option != 5 && std::cin.good()) {
            std::cout << "Choose an option:" << std::endl;
            std::cout << "1. Read from file" << std::endl;
            std::cout << "2. Write to file" << std::endl;
            std::cout << "3. Input" << std::endl;
            std::cout << "4. Output" << std::endl;
            std::cout << "5. Exit" << std::endl;
            std::cin >> option;

            switch (option) {
                case 1: {
                    std::cout << "Enter the file name:" << std::endl;
                    std::cin >> f;
                    in.open(f);
                    if (!in.is_open()) {
                        std::cout << "File does not exist!" << std::endl;
                        break;
                    }
                    auto start = std::chrono::high_resolution_clock::now();
                    analyzer->checkFile(in);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> diff = end - start;
                    std::cout << "[" << name << "] time: " << diff.count() << " s" << std::endl;
                    in.close();
                    std::cout << "Successfully read from file!" << std::endl;
                    break;
                }
                case 2:
                    out.open("result.txt");
                    if (!out.is_open()) {
                        std::cout << "File could not be opened!" << std::endl;
                        break;
                    }

                    // Записываем таблицу
                    out << "--- Variables Table ---" << std::endl;
                    out << analyzer->getTable() << std::endl;

                    // Записываем конфликты
                    out << "--- Conflicts ---" << std::endl;
                    if (analyzer->conflicts.empty()) {
                        out << "None" << std::endl;
                    } else {
                        for (const std::string& conflictLine : analyzer->conflicts) {
                            out << conflictLine << std::endl;
                        }
                    }

                    out.close();
                    std::cout << "Successfully written to result.txt" << std::endl;
                    break;
                case 3:
                    std::cout << "Enter the string: " << std::endl;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, str);
                    std::cout << "Result: " << (analyzer->checkString(str) ? "ACCEPTABLE" : "NOT ACCEPTABLE") << std::endl;
                    break;
                case 4:
                    std::cout << "--- Variables Table ---" << std::endl;
                    std::cout << analyzer->getTable() << std::endl;
                    std::cout << "--- Conflicts ---" << std::endl;
                    if (analyzer->conflicts.empty()) {
                        std::cout << "None" << std::endl;
                    } else {
                        for (const std::string& conflictLine : analyzer->conflicts) {
                            std::cout << conflictLine << std::endl;
                        }
                    }
                    break;
                case 5:
                    std::cout << "Exit:" << std::endl;
                    break;
                default:
                    std::cout << "Invalid option" << std::endl;
                    break;
            }
            if (option != 5) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        delete analyzer;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}