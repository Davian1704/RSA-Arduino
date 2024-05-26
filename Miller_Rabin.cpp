#include "Miller_Rabin.h"

using namespace std;

uint32_t modular_pow(uint32_t base, uint32_t exponent, uint32_t mod) {
    uint32_t result = 1;
    base = base % mod;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % mod;
        }
        exponent = exponent >> 1;
        base = (base * base) % mod;
    }
    return result;
}

bool MillerRabinFct(uint32_t n) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    uint32_t t = n - 1;
    uint32_t seqL = 0;
    while (t % 2 == 0) {
        seqL++;
        t /= 2;
    }

    uint32_t AVector[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

    for (uint32_t a : AVector) {
        if (a > n - 2) continue; // Skip bases greater than n-2

        uint32_t x = modular_pow(a, t, n);

        if (x == 1 || x == n - 1) continue;

        bool isComposite = true;
        for (uint32_t i = 0; i < seqL - 1; i++) {
            x = (x * x) % n;
            if (x == n - 1) {
                isComposite = false;
                break;
            }
        }

        if (isComposite) {
            return false;
        }
    }

    return true;
}

uint32_t getMaxPrime(uint32_t n) {
    n += 2;
    while (n <= 2147483647) {
        if (MillerRabinFct(n)) {
            return n;
        }
        n += 2;
    }

    return -1;
}