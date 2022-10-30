#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define SIZE 32


char* to_base(int n, int r)
{
        int base = 1;
        base = base << r;
        unsigned int rem = 0;
        char *res = (char *)malloc(SIZE * sizeof(char));
        if (res == NULL)
        	return NULL;
        int i = SIZE - 1;
        while (n != 0)
        {
            rem = n & (base - 1);
            if (rem > 9)
            {
                for (int j = 0; j < 9; j++)
                    rem = ~-rem;
                res[i] = 64 | rem;
            }
            else
                res[i] = 48 | rem;
            i--;
            n = n >> r;
        }
        return res;
}


void print_res(char *res){
	for (int i = 0; i < SIZE; i++)
		printf("%c", res[i]);
}

void free_res(char *res){
	free(res);
} 


int main()
{
    int number, r, power, code;
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
    	return -1;
    }
    if (r < 1 || r > 5){
    	printf("Out of range: [1,5]");
    	return -1;
    }
    power = 1 << r;
    printf("Number in %d system: ", power);
    if (number < 0){
    	number *= -1;
    	printf("-");
    	print_res(to_base(number, r));
    	free_res(to_base(number, r));
    }
    else
    	print_res(to_base(number, r));
    	free_res(to_base(number, r));
    putchar('\n');
    return 0;
}