#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <stdarg.h>

double geom_mean(int n, ...){
    double result = 1.0, eps = 0.00001;
    va_list factor;
    va_start(factor, n);
    for (int i = 0; i < n; i++){
        result *= va_arg(factor, double);
    }
    va_end(factor);
    if ((n % 2 == 0) && (result < eps)){
        return -1;
    }
    return pow(result, 1.0 / n);
}


double power(double num, int base){
    if (base == 0)
        return 1;
    if (base == 1)
        return num;
    if (base > 0)
        return num * power(num, base - 1);
    else
        return 1.0 / power(num, -base);
}

int main(){
    setlocale(LC_ALL, "Russian");
    printf("%lf\n", geom_mean(4, 5.0, 7.0, 1.0, 6.0));
    printf("%lf\n", geom_mean(2, 2.0, -1.0));
    printf("%lf\n", power(2, 0));
    printf("%lf\n", power(5, 1));
    printf("%lf\n", power(9, 3));
    printf("%lf\n", power(4, -3));
    return 0;
}
