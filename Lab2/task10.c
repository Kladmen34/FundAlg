#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

int check_finite(double decimal, int base) {
    if(base == 10)
        return 1;
    int i = 0;
    double fraction = decimal - (int)decimal;
    while (fraction != 0) {
        fraction *= base;
        if (fraction >= 1) fraction -= 1;
        i++;
        if (i > log(INT_MAX)/log(base) + 1)
            return 0;
    }
    return 1;
}

double* fin_repr(int base, int count, int* fin_size, ...) {
    va_list args;
    va_start(args, fin_size);
    double* numbers = (double*)malloc(count * sizeof(double));
    if (numbers == NULL)
        return NULL;
    int index = 0;
    for(int i = 0; i < count; i++){
        double x = va_arg(args, double);
        if(check_finite(x, base)){
            numbers[index] = x;
            index++;
        }
    }
    va_end(args);
    *fin_size = index;
    return numbers;
}

int main() {
    int size = 0;
    int base = 2;
    double* result = fin_repr(base, 3, &size, 0.625, 0.1, 0.35);
    for(int i = 0; i < size; i++){
        printf("Number %lf is finite in base %d\n", result[i], base);
    }
    return 0;
}
