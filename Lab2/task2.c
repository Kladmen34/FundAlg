#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

int len(char *str){
    int l = 0;
    while(*str != '\0'){
        l++;
        str++;
    }
    return l;
}

void reverse(char *str){
    char *beg = str;
    char *end = beg + len(beg) - 1;

    char temp;
    while (beg < end)
    {
        temp = *end;
        *end = *beg;
        *beg = temp;
        beg++;
        end--;
    }
}

void regstr(char *str){
    int k = 0;
    while(*str != '\0'){
        if (k % 2 == 1){
            *str = toupper(*str);
        }
        k++;
        str++;
    }
}

void transform(char *str){

    char tmp[100];
    int i = 0, fl = 0, length = 0;

    while (1){
        if (*str == '\0'){
            fl++;
            str -= length;
        }

        if (!fl)
            length++;

        if (fl > 2)
            break;

        if (fl == 0 && isdigit(*str)){
            tmp[i] = *str;
            i++;
        }
        else if (fl == 1 && isalpha(*str)){
            tmp[i] = *str;
            i++;
        }
        else if (fl == 2 && !isalnum(*str)){
            tmp[i] = *str;
            i++;
        }
        str++;
    }

    i = 0;

    while (*str != '\0'){
        *str = tmp[i];
        i++;
        str++;
    }
}

char* concatenation(char *str1, char *str2){

    int len1 = len(str1), len2 = len(str2);
    for (int i = len1; i < len1 + len2; i++){
        str1[i] = str2[i - len1];
    }
    str1[len1+len2] = '\0';
    return str1;
}

int main(int argc, char* argv[]){
    setlocale(LC_ALL, "Russian");
    char *string;
    string = argv[1];
    if(argv[2][0] == '-'){
        switch(argv[2][1]){
        case 'l':
            printf("%d", len(string));
            break;

        case 'r':
            reverse(string);
            printf("%s", string);
            break;

        case 'u':
            regstr(string);
            printf("%s", string);
            break;

        case 'n':
            transform(string);
            printf("%s", string);
            break;

        case 'c':
            if (argc != 4){
                printf("Неверное число параметров!");
                break;
            }
            char* new_string = argv[3];
            concatenation(string, new_string);
            printf("%s", string);
        }
    }
    return 0;
}