#include <stdio.h>
#include <math.h>

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
    }
    else{
        printf("Уравнение не имеет корней на заданном отрезке ");
        return -1;
    }
    return c;
}

int main(){
    double eps = 0.0000001;
    printf("x = %lf\n", Dhtm(0, 1, eps));
    return 0;
}