#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main(){
    setlocale(LC_ALL, "Russian");
    int dim1, dim2;
    printf("Введите размерности массивов: ");
    scanf("%d %d", &dim1, &dim2);
    int *A, *B, *C;
    A = (int*)malloc(dim1 * sizeof(int));
    B = (int*)malloc(dim2 * sizeof(int));
    C = (int*)malloc(dim1 * sizeof(int));

    printf("Массив A: \n");
    for (int i = 0; i < dim1; i++){
        A[i] = rand() % 100;
        printf("%d ", A[i]);
    }

    printf("\nМассив B: \n");
    for (int i = 0; i < dim2; i++){
        B[i] = rand() % 100;
        printf("%d ", B[i]);
    }

    for (int i = 0; i < dim1; i++){
        if (i < dim2)
            C[i] = A[i] + B[i];
        else
            C[i] = A[i] + B[i - 1];
    }

    printf("\nНовый массив: \n");
    for (int i = 0; i < dim1; i++)
        printf("%d ", C[i]);

    free(A);
    free(B);
    free(C);

    return 0;
}
