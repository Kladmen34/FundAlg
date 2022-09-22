#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
	if (number > 1){
		for (int i = 2; i < number; i++) {
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


void flag_h(int number) {
	int flag = 0;
	for (int i = 1; i <= 100; ++i) {
		if (number % i == 0) {
			printf("%d ", i);
			flag++;
		}
		
	}
	if (flag == 0) {
		printf("Таких чисел нет\n");
	}
}

void flag_p(int number) {
	if (prime(number)) {
		printf("Число простое\n");
	}
	else {
		printf("Число составное\n");
	}
}

void flag_s(int number) {
	int result;
	while (number != 0) {
		result = number % 10;
		printf("%d ", result);
		number /= 10;
	}

}

void flag_e(int number) {
	if (number <= 10) {
		int result = 1;
		for (int i = 1; i <= number; ++i) {
			result *= number;
			printf("%d ", result);
		}
	}
	else {
		printf("Число больше 10!\n");
	}
}

void flag_a(int number) {
	int sum = 0;
	for (int i = 1; i <= number; ++i) {
		sum += i;
	}
	printf("%d", sum);
}

int flag_f(int number) {
	if (number == 0 || number == 1) {
		return 1;
	}
	return number * flag_f(number - 1);
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

	int fact, num = char_to_int(argv[1]);
	if (num > 0) {
		if (argv[2][0] == '-' || argv[2][0] == '/') {
			switch (argv[2][1]) {

			case 'h':
				flag_h(num);
				break;

			case 'p':
				flag_p(num);
				break;

			case 's':
				flag_s(num);
				break;

			case 'e':
				flag_e(num);
				break;

			case 'a':
				flag_a(num);
				break;

			case 'f':
				fact = flag_f(num);
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


