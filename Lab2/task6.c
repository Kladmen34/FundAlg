#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>


int search_substring(const char* str, const char* substr) {
    int i, j;
    for (i = 0; str[i]; i++) {
        for (j = 0; substr[j]; j++) {
            if (str[i + j] != substr[j]) {
                break;
            }
        }
        if (!substr[j]) {
            return i;
        }
    }
    return 0;
}

int* search_in_files(const char* substr, int file_count, ...) {
    va_list files;
    va_start(files, file_count);
    char* buff = NULL;
    size_t buff_size = 0;
    int i;
    for (i = 0; i < file_count; i++) {
        FILE* file = va_arg(files, FILE*);
        int line_number = 1;
        while (getline(&buff, &buff_size, file) != -1) {
            int sym = search_substring(buff, substr);
            if (sym) {
                int *result = (int*)malloc(3 * sizeof(int));
                if (!result) {
                    fprintf(stderr, "malloc() failed: insufficient memory!\n");
                    return NULL;
                }
                result[0] = i + 1;
                result[1] = line_number;
                result[2] = sym;
                return result;
            }
            line_number++;
            free(buff);
        }
        rewind(file);
    }
    va_end(files);
    free(buff);
    return NULL;
}

int main() {
    FILE* file1 = fopen("file1.txt", "r");
    if((file1 = fopen("file1.txt", "r"))==NULL){
        printf("File opening failed\n");
        return -1;
    }
    FILE* file2 = fopen("file2.txt", "r");
    if((file2 = fopen("file2.txt", "r"))==NULL){
        printf("File opening failed\n");
        fclose(file1);
        return -2;
    }
    int *res = search_in_files("bruh", 2, file1, file2);
    if(res){
        printf("Substring found in file %d in line %d in position %d\n", res[0], res[1], res[2]);
        free(res);
    }
    fclose(file1);
    fclose(file2);
    return 0;
}
