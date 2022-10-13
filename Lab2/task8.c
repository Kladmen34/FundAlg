#include <stdio.h>
#include <math.h>
#include <locale.h>

double f(double x){
    return 2 * log(x) - sin(x) + 2 * pow(x, 2);
}

double Dhtm(double a, double b, double epsylone){
    double c;
    if (f(a) * f(b) < 0){
        while (fabs(b - a) > epsylone){
            c = (a + b) / 2;
            if (f(c) * f(a) > 0)
                a = c;
            else
                b = c;
        }
        return c;
    }
}

int main(){
    setlocale(LC_ALL, "Russian");
    int a, b;
    double eps;
    printf("Введите границы отрезка: ");
    scanf("%d %d", &a, &b);
    printf("\nВведите точность, с которой необходимо вычислить корень: ");
    scanf("%lf", &eps);
    printf("x = %lf\n", Dhtm(a, b, eps));
    return 0;
}
