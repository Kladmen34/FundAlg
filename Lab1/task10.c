#include <stdio.h>
#include <stdlib.h>

#define EPS 0.0000001

double **generate_matrix(int *rows, int *columns) {
    *rows = rand() % 10 + 1;
    *columns = rand() % 10 + 1;
    double **matrix = (double**)malloc(sizeof(double*) * *rows);
    if(matrix == NULL) {
        return NULL;
    }

    for(int i =0; i < *rows; i++) {
        matrix[i] = (double*)malloc(sizeof(double) * *columns);
        if(matrix[i] == NULL) {
            return NULL;
        }
        for(int j=0; j < *columns; j++) {
            matrix[i][j] = rand() % 201 - 100;
        }
    }
    return matrix;
}

void print_matrix(double **matrix, int rowscount, int columnscount){
    for (int i = 0; i < rowscount; i++){
        for (int j = 0; j < columnscount; j++){
            printf("%.1f ", matrix[i][j]);
        }
        printf("\n");
    }
}

double **matr_mult(double **matrix1, double **matrix2, int rows_m1, int columns_m1, int rows_m2, int columns_m2) {
    double **new_matrix = NULL;
    if(columns_m1 == rows_m2) {
        new_matrix = (double**)malloc(sizeof(double*) * rows_m1);
        if(new_matrix == NULL) {
            return NULL;
        }
        for(int i = 0; i < rows_m1; i++) {
            new_matrix[i] = (double*)malloc(sizeof(double) * columns_m2);
            if(new_matrix[i] == NULL) {
                return NULL;
            }
        }

        for(int i = 0; i < rows_m1; i++) {
            for(int j = 0; j < columns_m2; j++) {
                new_matrix[i][j] = 0;
                for(int k = 0; k < columns_m1; k++) {
                    new_matrix[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }
    }
    return new_matrix;
}


int determinant(double **matrix, int rows, int columns) {
    if(rows != columns) {
        return -1;
    } else {
        int det = 1;
        double* tmp;
        for (int i = 0; i < rows; i++) {
            int k = i;
            for (int j = i + 1; j < rows; j++)
                if (fabs(matrix[j][i]) > fabs(matrix[k][i]))
                    k = j;
            if (fabs(matrix[k][i]) < EPS) {
                det = 0;
                break;
            }
            tmp = matrix[i];
            matrix[i] = matrix[k];
            matrix[k] = tmp;
            if (i != k)
                det = -det;
            det *= matrix[i][i];
            for (int j = i + 1; j < rows; j++)
                matrix[i][j] /= matrix[i][i];
            for (int j = 0; j < rows; ++j)
                if (j != i && fabs(matrix[j][i]) > EPS)
                    for (int k = i + 1; k < rows; k++)
                        matrix[j][k] -= matrix[i][k] * matrix[j][i];
        }
        return det;
    }
}


void matrix_cleaning(double ***a, int rowscount){
    for (int i = 0; i < rowscount; i++)
        free(a[i]);
    free(a);
    a = NULL;
}

int main(){
    int rows_of_matrix1 = 0, columns_of_matrix1 = 0;
    int rows_of_matrix2 = 0, columns_of_matrix2 = 0;
    int determinant_of_matrix1, determinant_of_matrix2;
    double **matrix1 = generate_matrix(&rows_of_matrix1, &columns_of_matrix1);

    if(matrix1 != NULL) {
        printf("First matrix: \n");
        print_matrix(matrix1, rows_of_matrix1, columns_of_matrix1);
    } else {
        printf("Failed to generate 1 matrix");
    }

    double **matrix2 = generate_matrix(&rows_of_matrix2, &columns_of_matrix2);
    if(matrix1 != NULL) {
        printf("Second matrix: \n");
        print_matrix(matrix2, rows_of_matrix2, columns_of_matrix2);
    } else {
        printf("Failed to generate 2 matrix");
    }

    double **new_matrix = matr_mult(matrix1, matrix2, rows_of_matrix1, columns_of_matrix1, rows_of_matrix2, columns_of_matrix2);
    if(new_matrix != NULL) {
        printf("After multiplying 2 matrices: \n");
        print_matrix(new_matrix, rows_of_matrix1, columns_of_matrix2);
        matrix_cleaning(&new_matrix, rows_of_matrix1);
    } else {
        printf("Failed to multiply 2 matrices\n");
    }

    determinant_of_matrix1 = determinant(matrix1, rows_of_matrix1, columns_of_matrix1);
    determinant_of_matrix2 = determinant(matrix2, rows_of_matrix2, columns_of_matrix2);

    if(determinant_of_matrix1 != -1) {
        printf("Determinant of first matrix: %d\n", determinant_of_matrix1);
    } else {
        printf("Failed to find determinant of first matrix\n");
    }

    if(determinant_of_matrix2 != -1) {
        printf("Determinant of second matrix: %d\n", determinant_of_matrix2);
    } else {
        printf("Failed to find determinant of second matrix\n");
    }

    matrix_cleaning(&matrix1, rows_of_matrix1);
    matrix_cleaning(&matrix2, rows_of_matrix2);

    return 0;
}
