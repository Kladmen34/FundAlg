#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>

int ToDecimal(int c);
void to_left(char *str);
int len(char *str);
void shift(char *str);
void additionition(int base, char *res, char *num);
void summa(char *res, int base, int argc, ...);

int main()
{
    char result[90];
    summa(result, 10, 2, "108", "2567");
    printf("%s\n", result);
    return 0;
}

void addition(int base, char *res, char *num)
{
    char arr[36] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int lengthNum = len(num);
    int lengthRes = len(res);
    while (lengthRes <= lengthNum)
    {
        shift(res);
        lengthRes = len(res);
    }
    while (*res != '\0')
        res++;
    while (*num != '\0')
        num++;
    num--;
    res--;
    int remember = 0;
    for (int i = 0; (i < lengthNum); i++)
    {
        if (ToDecimal(*num) >= base)
        {
            printf("Wrong number\n");
            exit(1);
        }
        int sum = ToDecimal(*num) + ToDecimal(*res) + remember;
        if (sum >= base)
            remember = 1;
        else
            remember = 0;
        *res = arr[sum % base];
        res--;
        num--;
    }
    while (remember)
    {
        int sum = ToDecimal(*res) + remember;
        if (sum >= base)
            remember = 1;
        else
            remember = 0;
        *res = arr[sum % base];
        res--;
    }
    res++;
}

void summa(char *res, int base, int argc, ...)
{
    va_list ap;
    va_start(ap, argc);
    res[0] = '0';
    res[1] = '\0';
    for (int i = 0; i < argc; i++)
    {
        addition(base, res, va_arg(ap, char *));
    }
    while (res[0] == '0')
        to_left(res);
    va_end(ap);
}
int len(char *str)
{
    int len = 0;
    while (*str != '\0')
    {
        len++;
        str++;
    }
    return len;
}

int ToDecimal(int c)
{
    c = toupper(c);
    char arr[36] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 36; i++)
    {
        if (arr[i] == c)
            return i;
    }
}

void to_left(char *str)
{
    int length = len(str);
    char temp1;
    for (int i = 0; i < length; i++)
    {
        temp1 = *(str + 1);
        *str = temp1;
        str++;
    }
    *str = '\0';
}

void shift(char *str)
{
    int length = len(str);
    char temp1 = *str;
    char temp2;
    *str = '0';
    str++;
    for (int i = 0; i < length; i++)
    {
        temp2 = *str;
        *str = temp1;
        temp1 = temp2;
        str++;
    }
    *str = '\0';
}
