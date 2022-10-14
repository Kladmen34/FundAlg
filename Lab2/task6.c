#include <stdio.h>
#include <stdarg.h>

char* find_substr(char *str, char *substr){
    int l = strlen(substr);
    char *tmp = substr;
    while (*str && *tmp){

        if (*str++ == *tmp)
            tmp++;

        if (!*tmp)
            return (str - l);

        if (l == (tmp - substr))
            tmp = substr;
    }
    return NULL;
}


void search(char *substr, ...){
    va_list factor;
    char str[BUFSIZ];
    FILE *f;
    char *file;
    va_start(factor, str);
    file = va_arg(factor, char *);
    while ((f = fopen(file, "r")) != NULL){
        printf("%s\n", file);
        int line = 1;
        while (fgets(str, sizeof(str), f) != NULL){
            while (1){
                if (find_substr(str, substr) != NULL){
                    printf("Substring %s in string %d\n", substr, line);
                    line = 1;
                    break;
                }
                else{
                    printf("In string %d no find substring %s!!!\n", line, substr);
                    break;
                }
            }
            line++;
        }
        fclose(f);
        file = va_arg(factor, char *);
    }
    va_end(factor);
}

int main(){
    search("bebra", "f1.txt", "f2.txt");
    return 0;
}
