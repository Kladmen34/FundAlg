#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum ERRORS{
    INCORRECT_INPUT = -1,
    NO_MEMORY = -2
};

long long int factorial(int num){
    if (num == 0 || num == 1)
        return 1;
    return num * factorial(num - 1);
}

int comb(int l, int k){
    long long a = factorial(k), b = factorial(l), c = factorial(k - l);
    return (int) (a / (b * c));
}

int check_bits(int l, int n, int straight){
    int count = 0;
    if (!straight){
        while(n != 0){
            count += (n & 1);
            n >>= 1;
        }
    }
    else{
        while(n != 0){
            if (!(n & 1)){
                if (count == l)
                    break;
            count = 0;
            }
            else
                count++;
            n >>= 1;
        }
    }
    return count == l;
}

void search_one(int k, int l, int** mas, int *len){
    int lim = 2 << (k - 1);
    int size = comb(l, k);
    *mas = (int*)malloc(size * sizeof(int));
    if (!*mas)
        return;

    for (int i = 0; i < lim; i++){
        if(check_bits(l, i, 0))
            (*mas)[(*len)++] = i;
    }
}

void finding_duplicate_one(int k, int l, int** mas, int *len){
    int lim = 2 << (k - 1);
    int pred = 2 << (l - 1) - 1;
    int size = lim - pred;
    *mas = (int*)malloc(size * sizeof(int));
    if (!*mas)
        return;

    for (int i = pred; i < lim; i++){
        if(check_bits(l, i, 1))
            (*mas)[(*len)++] = i;
    }
}


void print_res(int *mas, int len){
    for (int i = 0; i < len; i++)
        printf("%d ", mas[i]);
    printf("\n");
}

int main(){
    int k, l, num, len1 = 0, len2 = 0, code, *res1 = NULL, *res2 = NULL;
    printf("Enter the number of bits: ");
    code = scanf("%d", &k);
    if (code != 1){
        printf("Incorrect input!");
        return INCORRECT_INPUT;
    }

    printf("Enter the number of units: ");
    code = scanf("%d", &l);
    if (code != 1){
        printf("Incorrect input!");
        return INCORRECT_INPUT;
    }
    if (l > k || k < 0 || l < 0){
       printf("Incorrect input!");
       return INCORRECT_INPUT;
    }
    search_one(k, l, &res1, &len1);
    printf("Numbers containing %d bits and %d units:\n", k, l);
    print_res(res1, len1);
    free(res1);
    finding_duplicate_one(k, l, &res2, &len2);
    printf("Numbers containing %d bits and exactly %d consecutive units:\n", k, l);
    print_res(res2, len2);
    free(res2);
    return 0;
}
