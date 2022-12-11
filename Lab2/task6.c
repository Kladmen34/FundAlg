#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


enum {
    FILE_NOT_FOUND = -1,
    SUBSTR_FOUND = -2,
    SUBSTR_NOT_FOUND = -3,
    MEMORY_ERROR = -4,
    SUCCESSFULLY = -5
};

int find_str(char *str, char *substr) {
    int i, j, str_len, substr_len;
	str_len = strlen(str);
	substr_len = strlen(substr);
	int *prefix = (int*)malloc(substr_len * sizeof(int));
    if (!prefix){
        free(*prefix);
        return MEMORY_ERROR;
    }

	prefix[0] = 0;
	for(i = 1, j = 0; i < substr_len; i++) {
		while(j > 0 && substr[j] != substr[i])
			j = prefix[j-1];
		if(substr[j] == substr[i])
			j++;
		prefix[i] = j;
	}


	for(i = 0, j = 0; i < str_len; i++){
		while(j > 0 && substr[j] != str[i])
			j = prefix[j - 1];
		if(substr[j] == str[i]) {
            j++;
        }
		if(j == substr_len) {
		    free(prefix);
            return SUBSTR_FOUND;
        }
	}
    free(prefix);
	return SUBSTR_NOT_FOUND;
}


int check_files(char *substr, int num_of_files, ...) {
    FILE *fin;
    char *file_name;
    va_list p;
    va_start(p, num_of_files);
    int is_found = 0, line = 1;
    char str[BUFSIZ];
    while(num_of_files != 0) {
        line = 1;
        num_of_files--;
        file_name = va_arg(p, char*);
        fin = fopen(file_name, "r");
        if(!fin) {
            return FILE_NOT_FOUND;
        }
        while(fgets(str, sizeof(str), fin) != NULL) {
            is_found = find_str(str, substr);
            if(is_found == SUBSTR_FOUND) {
                printf("Substr %s found in file: %s, in line=%d\n", substr, file_name, line);
            } else if(is_found == SUBSTR_NOT_FOUND) {
                printf("Substr %s NOT found in file: %s, in line=%d\n", substr, file_name, line);
            } else if(is_found == MEMORY_ERROR) {
                printf("Problems with memory...\n");
                return MEMORY_ERROR;
            }
            line++;
        }
        fclose(fin);
    }
    va_end(p);
    return SUCCESSFULLY;
}

int main(int argc, char * argv[]) {
    check_files("Hello", 2, "f1.txt","f2.txt");
}
