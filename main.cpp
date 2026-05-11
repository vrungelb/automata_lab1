#include <iostream>
#include <limits>
#include "fstream"
#include "chrono"
#include "filesystem"
#include "regex/Regex.h"
#include "smc/Analyz.h"
#include "smc/AppClass.h"
#include "flex/Flex.h"
#include "strgen.h"

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
        std::cout << "3. Flex" << std::endl;
        std::cin >> option;

        if (option == 1) {
            analyzer = new Regex();
            name += "regex";
        } else if (option == 2) {
            analyzer = new AppClass();
            name += "smc";
        } else if (option == 3) {
            analyzer = new Flex();
            name += "flex";
        } else {
            std::cout << "Invalid choice" << std::endl;
            return 1;
        }

        option = 0;
        while (option != 6 && std::cin.good()) {
            std::cout << "Choose an option:" << std::endl;
            std::cout << "1. Read from file" << std::endl;
            std::cout << "2. Write to file" << std::endl;
            std::cout << "3. Input" << std::endl;
            std::cout << "4. Output" << std::endl;
            std::cout << "5. Generate test file" << std::endl;
            std::cout << "6. Exit" << std::endl;
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
                case 2: {
                    std::string outName = "result_" + name + ".txt";
                    out.open(outName);
                    if (!out.is_open()) {
                        std::cout << "File could not be opened!" << std::endl;
                        break;
                    }

                    out << "--- Variables Table ---" << std::endl;
                    out << analyzer->getTable() << std::endl;

                    out << "--- Conflicts ---" << std::endl;
                    if (analyzer->conflicts.empty()) {
                        out << "None" << std::endl;
                    } else {
                        for (const std::string& conflictLine : analyzer->conflicts) {
                            out << conflictLine << std::endl;
                        }
                    }

                    out.close();
                    std::cout << "Successfully written to " << outName << std::endl;
                    break;
                }
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
                case 5: {
                    int genMode = 0;
                    int count = 0;
                    std::cout << "Generation mode (1 - only valid, 2 - mixed): ";
                    std::cin >> genMode;
                    if (genMode != 1 && genMode != 2) {
                        std::cout << "Invalid mode" << std::endl;
                        break;
                    }
                    std::cout << "Number of strings: ";
                    std::cin >> count;
                    if (count <= 0) {
                        std::cout << "Invalid count" << std::endl;
                        break;
                    }
                    const std::string genPath = "tests.txt";
                    auto stats = strgen::generateToFile(genPath, count, genMode);
                    if (stats.good == 0 && stats.bad == 0) {
                        std::cout << "Failed to write to " << genPath << std::endl;
                        break;
                    }
                    std::cout << "Generated " << stats.good << " valid and "
                              << stats.bad << " invalid strings to " << genPath << std::endl;
                    break;
                }
                case 6:
                    std::cout << "Exit:" << std::endl;
                    break;
                default:
                    std::cout << "Invalid option" << std::endl;
                    break;
            }
            if (option != 6) {
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