#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <limits.h>

void swap(int *n1, int *n2){
    int tmp = *n2;
    *n2 = *n1;
    *n1 = tmp;
}

int main(){

    setlocale(LC_ALL, "Russian");
    int *mas, *new_mas, dim, action, range;
    int *max = INT_MIN, *min = INT_MAX;
    int new_dim = 0, flag = 0;
    printf("Введите размерность массива: ");
    scanf("%d", &dim);
    mas = (int*)malloc(dim * sizeof(int));
    new_mas = (int*)malloc(dim * sizeof(int));
    printf("\nВведите диапазон: ");
    scanf("%d", &range);
    printf("\nИсходный массив: ");

    for (int i = 0; i < dim; i++){
        mas[i] = rand() % range;
        printf("%d ", mas[i]);
    }

    printf("\nМеню:\n");
    printf("1.Поменять местами максимальный и минимальный элементы\n");
    printf("2.Создать новый массив, который содержит уникальные элементы исходного массива в порядке, определённом исходным массивом\n");

    scanf("%d", &action);

    switch(action){
    case 1:
        max = &mas[0], min = &mas[0];
        for (int i = 1; i < dim; i++){
            if (mas[i] > *max){
                max = &mas[i];
            }
            if (mas[i] < *min){
                min = &mas[i];
            }
        }

        printf("\nМаксимум = %d; Минимум = %d\n", *max, *min);
        swap(min, max);
        printf("Измененный масив: ");
        for (int i = 0; i < dim; i++){
            printf("%d ", mas[i]);
        }
        break;

    case 2:
        for (int i = 0; i < dim; i++){
            flag = 0;
            for (int j = 0; j < dim; j++){
                if (i == j){
                    continue;
                }
                else{
                    if (mas[i] == mas[j]){
                        flag = 0;
                        break;
                    }
                    else{
                        flag = 1;
                    }
                }
            }
            if (flag == 1){
                new_mas[new_dim] = mas[i];
                new_dim++;
            }
        }
        printf("Новый массив: ");
        for (int i = 0; i < new_dim; i++){
            printf("%d ", new_mas[i]);
        }
        break;
    }
    free(mas);
    free(new_mas);
    return 0;
}
