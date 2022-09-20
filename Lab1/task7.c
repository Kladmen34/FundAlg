#include <stdio.h>
#include <stdlib.h>

int main(){
    
    int n;
    printf("Введите размерность массива: \n");
    scanf("%d", &n);
    int mas[n];
    
    printf("Введенный массив: \n");
    for(int i = 0; i < n; ++i){
        mas[i] = rand() % 1000;
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
    
    mas[indxmax] = min;
    mas[indxmin] = max;
    
    printf("\nНовый массив: \n");
    for(int i = 0; i < n; ++i){
        printf("%d ", mas[i]);
    }
    
    return 0;
}