#include <stdio.h>
#include <stdlib.h>

void filling_matrix(double **matrix, int rowscount, int columnscount){
    for (int i = 0; i < rowscount; i++){
        for (int j = 0; j < columnscount; j++){
            matrix[i][j] = rand() % 201 - 100;
        }
    }
}

void print_matrix(double **matrix, int rowscount, int columnscount){
    for (int i = 0; i < rowscount; i++){
        for (int j = 0; j < columnscount; j++){
            printf("%.1f ", matrix[i][j]);
        }
        printf("\n");
    }
}

double **mult(double **matrix1, double **matrix2, int l, int m, int n) {
    double **result =(double**)malloc(l * sizeof(double*));
    for (int M = 0; M < l; M++){
        result[M] = (double*)malloc(n * sizeof(double));
    }
    for (int i = 0; i < l; i++){
        for (int j = 0; j < n; j++){
            result[i][j] = 0;
            for (int r = 0; r < m; r++){
                result[i][j] += matrix1[i][r] * matrix2[r][j];
            }
        }
    }
    return result;
}

double determinant(double **a, int n){
    int i = 0, j = 0, k = 0, l = 0;
    double det;
    while (k < n - 1){
        while (i < n){
            det = a[i][j] / a[k][l];
            while (j < n){
                a[i][j] += (-det) * a[k][j];
                j++;
            }
            j = l;
            i++;
        }
        k++;
        l++;
        i = k + 1;
        j = l;
    }
    det = 1;
    i = 0;
    while (i < n){
        det = a[i][i] * det;
        i++;
    }
    return det;
}

void matrix_cleaning(double **a, int rowscount){
    for (int i = 0; i < rowscount; i++)
        free(a[i]);
    free(a);
}

int main(){
    int n;
    double **matrix1, **matrix2, **matr_mult;
    int rowscount1 = rand() % 10 + 1;
    int columnscount1 = rand() % 10 + 1;
    int rowscount2 = rand() % 10 + 1;
    int columnscount2 = rand() % 10 + 1;
    matrix1 = (double**)malloc(rowscount1 * sizeof(double*));
    for (int i = 0; i < rowscount1; i++)
        matrix1[i] = (double*)malloc(columnscount1 * sizeof(double));
    filling_matrix(matrix1, rowscount1, columnscount1);
    printf("Matrix 1:\n");
    print_matrix(matrix1, rowscount1, columnscount1);

    matrix2 = (double**)malloc(rowscount2 * sizeof(double*));
    for (int i = 0; i < rowscount2; i++)
        matrix2[i] = (double*)malloc(columnscount2 * sizeof(double));
    filling_matrix(matrix2, rowscount2, columnscount2);
    printf("Matrix 2:\n");
    print_matrix(matrix2, rowscount2, columnscount2);

    if (columnscount1 == rowscount2){
        matr_mult = mult(matrix1, matrix2, rowscount1, columnscount2, columnscount1);
        printf("Matrix 1 * 2:\n");
        print_matrix(matr_mult, rowscount1, columnscount2);
        matrix_cleaning(matr_mult, rowscount1);
    }
    else
        printf("Operation is not possible\n");


    if (rowscount1 == columnscount1){
        n = rowscount1 = columnscount1;
        printf("Determinant of matrix1 = %1.1f\n", determinant(matrix1, n));
    }
    else
        printf("Operation is not possible\n");

    if (rowscount2 == columnscount2){
        n = rowscount2 = columnscount2;
        printf("Determinant of matrix2 = %1.1f\n", determinant(matrix2, n));
    }
    else
        printf("Operation is not possible\n");

    matrix_cleaning(matrix1, rowscount1);
    matrix_cleaning(matrix2, rowscount2);
    return 0;
}
