#ifndef LAB1T_BENCH_H
#define LAB1T_BENCH_H

#include <string>

namespace bench {

    // dataMode: 1 - только валидные, 2 - смешанные, 3 - только невалидные
    // возвращает true при успехе
    bool runBenchmark(int dataMode);

}  // namespace bench

#endif //LAB1T_BENCH_H