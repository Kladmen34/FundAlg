#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 20

enum ERRORS{
    INCORRECT_INPUT = -1,
    NO_MEMORY = -2
};

void search_one(int k, int l, int* mas, int *len){
    *len = 0;
    int count = 0, i = 0, rem, n, num = 1 << (k - 1), last_num = 1 << k;
    for (num; num < last_num; num++){
        n = num;
        while (num != 0){
            rem = num & 1;
            if (rem == 1)
                count++;
            num >>= 1;
        }
        if (count == l){
            mas[i] = n;
            i++;
        }
        num = n;
        count = 0;
    }
    *len = i;
}

void finding_duplicate_one(int k, int l, int* mas, int *len){
    *len = 0;
    int count = 0, i = 0, rem, n, num = 1 << (k - 1), last_num = 1 << k;
    for (num; num < last_num; num++){
        n = num;
        while (num != 0){
            rem = num & 1;
            if (rem == 1){
                count++;
            }
            else{
                if (count == l) break;
                count = 0;
            }
            num >>= 1;
        }
        if (count == l){
            mas[i] = n;
            i++;
        }
        num = n;
        count = 0;
    }
    *len = i;
}

void print_res(int *mas, int len){
    for (int i = 0; i < len; i++){
        if (mas[i] == 0)
            break;
        printf("%d ", mas[i]);
    }
    printf("\n");
}

int main()
{
    int k, l, num, *len, code;
    int *res = (int*)malloc(SIZE * sizeof(int));
    if (res == NULL)
        return NO_MEMORY;
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
    if (l > k){
       printf("The condition l <= k is not met");
       return INCORRECT_INPUT;
    }
    search_one(k, l, res, &len);
    printf("Length of the 1st mas: %d\n", len);
    printf("Numbers containing %d bits and %d units:\n", k, l);
    print_res(res, len);
    free(res);
    finding_duplicate_one(k, l, res, &len);
    printf("Length of the 2nd mas: %d\n", len);
    printf("Numbers containing %d bits and exactly %d consecutive units:\n", k, l);
    print_res(res, len);
    free(res);
    return 0;
}
