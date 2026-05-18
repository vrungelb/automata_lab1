#include "Bench.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../smc/Analyz.h"
#include "../smc/AppClass.h"
#include "../regex/Regex.h"
#include "../flex/Flex.h"
#include "../strgen/strgen.h"

namespace bench {
namespace {

// фиксированный средний размер массива в первом проходе
constexpr int LINES_PASS_ARRAY_SIZE = 100;

// фиксированное число строк во втором проходе
constexpr int SIZE_PASS_LINES = 1000;

// число прогонов на одну точку
constexpr int RUNS_PER_POINT = 5;

const std::array<int, 7> N_LINES_POINTS = {100, 500, 1000, 5000, 10000, 50000, 100000}; // для фикс. кол-ва элементов
const std::array<int, 7> ARRAY_SIZE_POINTS = {10, 50, 200, 1000, 5000, 20000, 50000};   // для фикс. кол-ва строк

std::string suffixForMode(int dataMode) {
    if (dataMode == 1) return "valid";
    if (dataMode == 3) return "invalid";
    return "mixed";
}

// загружает строки из файла в вектор, чтобы исключить I/O из замеров
std::vector<std::string> loadLines(const std::string& path) {
    std::vector<std::string> lines;
    std::ifstream in(path);
    if (!in) return lines;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        lines.push_back(std::move(line));
    }
    return lines;
}

// медиана из вектора double
double median(std::vector<double>& v) {
    std::ranges::sort(v);
    return v[v.size() / 2];
}

// один прогон по всем строкам, возврат секунд
double measureOnce(Analyz& analyzer, std::vector<std::string>& lines) {
    analyzer.reset();
    auto start = std::chrono::high_resolution_clock::now();
    for (auto& s : lines) {
        analyzer.checkString(s);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return diff.count();
}

// несколько прогонов с тем же входом, возврат медианы
double measureMedian(Analyz& analyzer, std::vector<std::string>& lines) {
    std::vector<double> times;
    times.reserve(RUNS_PER_POINT);
    for (int i = 0; i < RUNS_PER_POINT; i++) {
        times.push_back(measureOnce(analyzer, lines));
    }
    return median(times);
}

struct EnginePoint {
    double regexTime;
    double smcTime;
    double flexTime;
};

EnginePoint measureAllEngines(std::vector<std::string>& lines) {
    EnginePoint p{};

    {
        Regex eng;
        p.regexTime = measureMedian(eng, lines);
    }
    {
        AppClass eng;
        p.smcTime = measureMedian(eng, lines);
    }
    {
        Flex eng;
        p.flexTime = measureMedian(eng, lines);
    }
    return p;
}

bool passByLines(int dataMode, const std::string& csvPath) {
    std::ofstream csv(csvPath);
    if (!csv) {
        std::cout << "Failed to open " << csvPath << std::endl;
        return false;
    }
    csv << "n_lines,regex_s,smc_s,flex_s\n";

    const std::string tmpPath = "bench_tmp_lines.txt";

    std::cout << "--- Pass 1: time and number of lines (array size = "
              << LINES_PASS_ARRAY_SIZE << ") ---" << std::endl;

    for (int n : N_LINES_POINTS) {
        std::cout << "  generating n=" << n << " ... " << std::flush;
        auto stats = strgen::generateToFile(tmpPath, n, dataMode, LINES_PASS_ARRAY_SIZE);
        if (stats.good == 0 && stats.bad == 0) {
            std::cout << "FAIL" << std::endl;
            return false;
        }
        std::cout << "loading ... " << std::flush;
        auto lines = loadLines(tmpPath);
        std::cout << "measuring ... " << std::flush;
        EnginePoint p = measureAllEngines(lines);
        csv << n << "," << p.regexTime << "," << p.smcTime << "," << p.flexTime << "\n";
        csv.flush();
        std::cout << "regex=" << p.regexTime
                  << "s smc=" << p.smcTime
                  << "s flex=" << p.flexTime << "s" << std::endl;
    }

    std::cout << "  wrote " << csvPath << std::endl;
    return true;
}

bool passBySize(int dataMode, const std::string& csvPath) {
    std::ofstream csv(csvPath);
    if (!csv) {
        std::cout << "Failed to open " << csvPath << std::endl;
        return false;
    }
    csv << "array_size,regex_s,smc_s,flex_s\n";

    const std::string tmpPath = "bench_tmp_size.txt";

    std::cout << "--- Pass 2: time and array size (lines = "
              << SIZE_PASS_LINES << ") ---" << std::endl;

    for (int sz : ARRAY_SIZE_POINTS) {
        std::cout << "  generating array_size=" << sz << " ... " << std::flush;
        auto stats = strgen::generateToFile(tmpPath, SIZE_PASS_LINES, dataMode, sz);
        if (stats.good == 0 && stats.bad == 0) {
            std::cout << "FAIL" << std::endl;
            return false;
        }
        std::cout << "loading ... " << std::flush;
        auto lines = loadLines(tmpPath);
        std::cout << "measuring ... " << std::flush;
        EnginePoint p = measureAllEngines(lines);
        csv << sz << "," << p.regexTime << "," << p.smcTime << "," << p.flexTime << "\n";
        csv.flush();
        std::cout << "regex=" << p.regexTime
                  << "s smc=" << p.smcTime
                  << "s flex=" << p.flexTime << "s" << std::endl;
    }

    std::cout << "  wrote " << csvPath << std::endl;
    return true;
}

}  // anonymous namespace

bool runBenchmark(int dataMode) {
    if (dataMode != 1 && dataMode != 2 && dataMode != 3) {
        std::cout << "Invalid data mode" << std::endl;
        return false;
    }

    const std::string sfx = suffixForMode(dataMode);
    const std::string linesCsv = "bench_by_lines_" + sfx + ".csv";
    const std::string sizeCsv  = "bench_by_size_"  + sfx + ".csv";

    std::cout << "Data quality: " << sfx << std::endl;

    if (!passByLines(dataMode, linesCsv)) return false;
    if (!passBySize(dataMode, sizeCsv))   return false;

    std::cout << "\033[32mBenchmark complete.\033[0m" << std::endl;
    return true;
}

}  // namespace bench
