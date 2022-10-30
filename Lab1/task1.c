#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <locale.h>

int char_to_int(char* string) {
	int result = 0;
	int sign = 1;
	if (*string == '-') {
		sign = -1;
		string++;
	}
	while (*string) {
		result = result * 10 + (*string - '0');
		string++;
	}
	return result * sign;
}

bool prime(int number) {
	if (number > 1) {
		for (int i = 2; i <= sqrt(number); i++) {
			if (number % i == 0) {
				return false;
			}
		}
		return true;
	}
	else {
		return false;
	}
}



int factorial(int number) {
	if (number == 0 || number == 1) {
		return 1;
	}
	return number * factorial(number - 1);
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	if (argc != 3) {
		printf("Неверное число аргументов!\n");
		return 1;
	}

	if (strlen(argv[2]) != 2) {
		printf("Неверная длина флага\n");
	}

	int num = char_to_int(argv[1]);
	if (num > 0) {
		if (argv[2][0] == '-' || argv[2][0] == '/') {
			switch (argv[2][1]) {

			case 'h':
				int flag = 0;
				for (int i = num; i <= 100; i += num) {
					printf("%d ", i);
					flag++;
				}

				}
				if (flag == 0) {
					printf("Таких чисел нет\n");
				}
				break;

			case 'p':
				if (prime(num)) {
					printf("Число простое\n");
				}
				else {
					printf("Число составное\n");
				}
				break;

			case 's':
				int result;
				while (num != 0) {
					result = num % 10;
					printf("%d ", result);
					num /= 10;
				}
				break;

			case 'e':
				if (num <= 10) {
					long long int result = 1;
					for (int i = 1; i <= 10; ++i) {
						result = i;
						for (int j = 1; j <= num; j++){
							result *= i;
							printf("%ld ", result);
						}
						printf("\n");
					}
					printf("\n");	
				}
				else {
					printf("Число больше 10!\n");
				}
				break;

			case 'a':
				int sum = (num * (num + 1)) / 2;
				printf("%d", sum);
				break;

			case 'f':
				int fact = factorial(num);
				printf("%d", fact);
				break;

			default:
				printf("Введен неверный флаг!\n");
				break;
			}
		}
	}
	else {
		printf("Введено неверное число\n");
	}
	return 0;
}


