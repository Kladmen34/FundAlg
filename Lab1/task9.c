#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LEN 34

int main(){
	int base; 
	int max = 0;
	char numbers[LEN], maxnumber[LEN];
	printf("Введите систему счисления: ");
	scanf("%d", &base);
	if (base < 2 || base > 36){
		printf("Некорректный ввод!");
		return 1;
	}
	printf("Введите числа в этой системе: ");
	while (strcmp(numbers, "Stop")){
		scanf("%s", numbers);
		char *pb = numbers - 1;
		int result = 0;
		while (*++pb){
			if ((isdigit(*pb) && (*pb - '0' >= base) || isalpha(*pb) && *pb - 'A' + 10 >= base) && strcmp(numbers, "Stop")){
				printf("Некорректный ввод: %c", *pb);
				return 2;
			}
			result = result * base + (isdigit(*pb) ? *pb - '0' : *pb - 'A' + 10);
		}
		if (result > max && strcmp(numbers, "Stop")){
			max = result;
			strcpy(maxnumber, numbers);
		}
	}
	printf("Максимальное по модулю число в %d системе счисления: %s\n", base, maxnumber);
	printf("Это же число в десятичной системе: %d\n", max);
	
	char str[LEN];
	for (int i = 9; i <= 36; i += 9){
		int r;
		base = i;
		int temp = max;
		int j = 0;
		
		while (temp){
			r = temp % base;
			str[j] = (r > 9) ? (r - 10 + 'A') : (r + '0');
			temp /= base;
			j++;
		}

		printf("%d система счисления: ", base);
		for (int n = j - 1; n >= 0; n--){
			printf("%c", str[n]);
		}
		printf("\n");
	}

	return 0;
}
