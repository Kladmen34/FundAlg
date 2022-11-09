#include <stdio.h>
#include <stdlib.h>


int add(int a, int b){
    int res = 0, carry = 0;
    res = a ^ b;
    carry = (a & b) << 1;
    while (carry){
        int tmp = res;
        res = res ^ carry;
        carry = (tmp & carry) << 1;
    }
    return res;
}

int subtraction(int a, int b) {
    return add(a, add(~b, 1));
}

int count(int n, int base){
    int k = 0;
    while(n > 0){
        k = add(k, 1);
        n >>= base;
    }
    return k;
}

char* to_base(int n, int r){
    int base = 1;
    base = base << r;
    unsigned int rem = 0;
    int size = count(n, r);
    size = add(size, 1);
    char* res = (char*)malloc(size * sizeof(char));
    if (res == NULL)
        return NULL;

    int i = subtraction(size, 1);
    res[i] = '\0';
    i = subtraction(i, 1);
    base = subtraction(base, 1);
    while (n != 0){
        rem = n & base;
        if (rem > 9){
            rem = subtraction(rem, 9);
            res[i] = 64 | rem;
        }
        else
            res[i] = 48 | rem;
        i = subtraction(i, 1);
        n = n >> r;
    }

    return res;
}

int main(){
    int number, r, power, code;
    char* buf;
    printf("Number in decimal system: ");
    code = scanf("%d", &number);
    if (code != 1){
    	printf("Incorrect input");
    	return -1;
    }
    printf("For the construction of 2^r input r: ");
    code = scanf("%d", &r);
    if (code != 1){
    	printf("Incorrect input");
    	return -2;
    }
    if (r < 1 || r > 5){
    	printf("Out of range: [1,5]");
    	return -3;
    }
    power = 1 << r;
    printf("Number in %d system: ", power);
    buf = to_base(abs(number), r);
    if (number < 0){
        number = -number;
        printf("-");
        printf("%s", buf);
        free(buf);
    }
    else{
        printf("%s", buf);
        free(buf);
    }
    putchar('\n');
    return 0;
}
