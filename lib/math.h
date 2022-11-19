#pragma once

#include "array.h"

template<typename T>
T greatest_common_divisor(T o1, T o2) {
    if (o2 == 0)
        return o1;
    return greatest_common_divisor(o2, o1 % o2);
}

template<typename T>
T least_common_multiple(T o1, T o2) {
    return o1 * o2 / greatest_common_divisor(o1, o2);
}

/**
 * Divides integers rounding up.
 */
template<typename T>
T divide_round_up_int(T dividend, T divisor) {
    T mod = 0;
    for (; dividend > divisor; dividend -= divisor, ++mod);
    if (divisor - dividend != divisor) 
        ++mod;
    return mod;
}

class Factorial_registry {
    private:
        Array<int> factorials_;
        static Array<int> calculate(int size) {
            Array<int> factorials(size);
            factorials[0] = 1;
            int previous = 1;
            for (int i = 1; i < size; ++i) {
                auto factorial = previous * (i + 1);
                factorials[i] = factorial;
                previous = factorial;
            }
            return factorials;
        }
    public:
        Factorial_registry(int size) :factorials_(calculate(size)) {}
        int get(size_t index) { return factorials_[index]; }
};

// Tailor series: sin x = x - x^3/3! + x^5/5! - x^7/7! + ...
template<typename T>
T sin_tailor(T angle) {
    static Factorial_registry factorial_registry(11);
    auto sin = angle;
    for (int i = 0; i < 3; ++i) {
        int power = (i + 1) * 2;
        auto entry = angle;
        for (int j = 0; j < power; ++j)
            entry *= angle;
        entry /= factorial_registry.get(power);
        if (i % 2 == 0) 
            sin -= entry;
        else
            sin += entry;
    }
    return sin;
}

