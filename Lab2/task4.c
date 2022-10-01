#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

double geom_mean(int n, ...){
    double result = 1.0;
    va_list factor;
    va_start(factor, n);
    for (int i = 0; i < n; i++){
        result *= va_arg(factor, int);
    }
    va_end(factor);
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
    printf("%lf\n", geom_mean(2, 5, 7, 1));
    printf("%lf\n", power(2, 0));
    printf("%lf\n", power(5, 1));
    printf("%lf\n", power(9, 3));
    printf("%lf\n", power(4, -3));
    return 0;
}
