#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main(){
    setlocale(LC_ALL, "Russian");
    int n;
    printf("Введите размерность массива: \n");
    scanf("%d", &n);
    int mas[n];

    printf("Введенный массив: \n");
    for(int i = 0; i < n; ++i){
        mas[i] = rand() % 100;
        printf("%d ", mas[i]);
    }

    int max = 0, min = 10000000, indxmax, indxmin;

    for(int i = 0; i < n; ++i){
        if (mas[i] > max){
            max = mas[i];
            indxmax = i;
        }
        if (mas[i] < min){
            min = mas[i];
            indxmin = i;
        }
    }

    printf("\nМаксимум = %d; Минимум = %d\n", max, min);

    mas[indxmin] = mas[indxmax];
    mas[indxmax] = mas[indxmin];

    printf("\nНовый массив: \n");
    for(int i = 0; i < n; ++i){
        printf("%d ", mas[i]);
    }

    return 0;
}
