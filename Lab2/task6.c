#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef struct found {
    char* filename;
    int count;
    int* string_numbers;
} found;

int finding_substring(char* str, char* substr)
{
    int len = strlen(substr);
    char* ref = substr;
    while (*str && *ref)
    {
        if (*str++ == *ref)
        {
            ref++;
        }
        if (!*ref)
        {
            return (1);
        }
        if (len == (ref - substr))
        {
            ref = substr;
        }
    }
    return 0;
}

found* search(char* substr, int num, ...)
{
    found *res = (found*)malloc(num * sizeof(found));
    if (res == NULL)
        return NULL;
    int k = 0, filename_length=0;
    va_list args;
    char* str[BUFSIZ];
    FILE* f;
    char* filename;
    va_start(args, num);
    filename = va_arg(args, char*);
    for(int i=0; i<num;i++)
    {
        filename_length = strlen(filename);
        res[i].filename = (char*)malloc(sizeof(char) * filename_length);
        if (res[i].filename == NULL)
        {
            for (int l = 0; l < i; l++)
            {
                free(res[l].string_numbers);
                free(res[l].filename);
            }
            return NULL;
        }
        res[i].count = 1;
        res[i].string_numbers = (int*)malloc(sizeof(int) * res[i].count);
        if (res[i].string_numbers == NULL)
        {
            for (int l = 0; l < i; l++)
            {
                free(res[l].string_numbers);
                free(res[l].filename);
            }
            free(res[i].filename);
            return NULL;
        }
        if ((f = fopen(filename, "r")) != NULL)
        {
            strcpy(res[i].filename, filename);
            int line = 1;
            while (fgets(str, sizeof(str), f) != NULL)
            {
                while (1)
                {
                    if (finding_substring(str, substr) != 0)
                    {
                        printf("Substring %s found in string number %d\n", substr, line);
                        res[i].count++;
                        res[i].string_numbers[k++] = line;
                        if (k == res[i].count)
                        {
                            res[i].count += 1;
                            res[i].string_numbers = (int*)realloc(res[i].string_numbers, res[i].count * sizeof(int));
                            if (res[i].string_numbers == NULL)
                            {
                                for (int l = 0; l < i; l++)
                                {
                                    free(res[l].string_numbers);
                                    free(res[l].filename);
                                }
                                free(res[i].filename);
                                return NULL;
                            }
                        }
                            
                    }
                    break;
                }
                line++;
            }
            fclose(f);
            k = 0;
            filename = va_arg(args, char*);
        }
    }
    va_end(args);
    return res;
}

void rfree(int num, found* res)
{
    for (int i = 0; i < num; i++)
    {
        free((res+i)->filename);
        free((res+i)->string_numbers);
    }
    
}

int main()
{
    int num = 2;
    found *res ;
    if ((res = search("Hello", num, "f61.txt", "f62.txt")) != NULL)
    {
        for (int i = 0; i < num; i++)
        {
            printf("%s\n", res[i].filename);
            if (res[i].count > 1)
                for (int j = 0; j < res[i].count-1; j++)
                {
                    printf("%d\n", res[i].string_numbers[j]);
                }
            else
                printf("No matches\n");
        }
        rfree(num, res);
        free(res);
    }
    else
    {
        printf("Error during search\n");
        free(res);
    }
    return 0;
}
