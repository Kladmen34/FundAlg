#include <stdio.h>
#include <math.h>
#include <locale.h>

double f(double x){
    return 2 * log(x) - sin(x) + 2 * pow(x, 2);
}

double Dhtm(double a, double b, int k){
    double epsylone = pow(10, -k);
    double c;
    while (fabs(b - a) > epsylone){
        c = (a + b) / 2;
        if (f(c) * f(a) > 0)
            a = c;
        else
            b = c;
    }
    return c;
}

int main(){
    setlocale(LC_ALL, "Russian");
    int a, b, k;
    printf("Введите границы отрезка: ");
    scanf("%d %d", &a, &b);
    printf("\nВведите точность, с которой необходимо вычислить корень: ");
    scanf("%d", &k);
    printf("x = %lf\n", Dhtm(a, b, k));
    return 0;
}
