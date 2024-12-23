#include <math.h>

// Объявление функции factorial
int factorial(int n);

float E(int x) {
    float result = 0.0;
    for (int n = 0; n <= x; n++) {
        result += 1.0 / factorial(n);
    }
    return result;
}

int factorial(int n) {
    if (n == 0) return 1;
    return n * factorial(n - 1);
}
