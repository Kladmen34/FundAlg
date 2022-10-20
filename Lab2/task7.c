#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>


typedef struct koord{
    double x;
    double y;
}koord;

int convex(int count,...){

    int x1, x2, y1, y2, res, tmp_sign, tmp;
    double eps = 0.000001;
    int k = count / 2;
    koord buf[k];

    if (count % 2 == 0 && count > 5){
        va_list factor;
        va_start(factor, count);
        for (int i = 0; i < k; i++){
            buf[i].x = va_arg(factor, double);
            buf[i].y = va_arg(factor, double);
        }
        va_end(factor);

        for (int i = 0; i < k; i++){
            int ind_xy = i % k;
            int ind_xy1 = (i + 2) % k;
            int ind_xy2 = (i + 1) % k;

            x1 = buf[ind_xy2].x - buf[ind_xy].x;
            x2 = buf[ind_xy1].x - buf[ind_xy].x;

            y1 = buf[ind_xy2].y - buf[ind_xy].y;
            y2 = buf[ind_xy1].y - buf[ind_xy].y;

            res = (x1 * y2) - (x2 * y1);

            if (res > eps)
                tmp = 1;

            else
                tmp = 0;

            if (i == 0)
                tmp_sign = tmp;

            else if (tmp_sign != tmp)
                return 0;
        }
        return 1;
    }
    return -1;
}

double polynom(double x, int n, ...){

    va_list factor;
    double res = 0;
    va_start(factor, n);
    double coeff;
    for (int i = 0; i <= n; i++) {
        coeff = va_arg(factor, double);
        res += pow(x, i) * coeff;
    }
    va_end(factor);
    return res;
}

int main(){
    int result = convex(6, 1.0, 5.0, 2.0, 4.0, 7.0, 3.0);
    if (result)
        printf("Is convex!\n");
    else
        printf("Not convex\n");
    printf("%lf", polynom(4.0, 3, 2.0, 1.0, 5.0, 4.0));
    return 0;
}
