#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int flag_q(int a, int b, int c) {
	if (a == 0) {
		printf("Неверный ввод!\n");
		return 1;
	}
	float D;
	float x1, x2;
	D = pow(b, 2) - (4 * a * c);
	printf("При a = %d, b = %d, c = %d:\n", a, b, c);
	if (D < 0) {
		printf("Вещественных решений нет\n");
		return -1;
	}
	else {
		x1 = (-b + sqrt(D)) / ((2 * a));
		x2 = (-b - sqrt(D)) / ((2 * a));
		printf("Корни - %f %f\n", x1, x2);
		return 0;
	}
	
}



int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	if (strlen(argv[1]) != 2) {
		printf("Неверная длина флага!\n");
	}
	int a, b, c;
	int num1, num2, num3;
	if (argv[1][0] == '-' || argv[1][0] == '/') {
		switch (argv[1][1]) {
		case 'q':
			if (argc != 5) {
				printf("Неверное число параметров!\n");
				break;
			}
			a = char_to_int(argv[2]);
			b = char_to_int(argv[3]);
			c = char_to_int(argv[4]);
			flag_q(a, b, c);
			flag_q(a, c, b);
			flag_q(b, a, c);
			flag_q(b, c, a);
			flag_q(c, a, b);
			flag_q(c, b, a);
			break;
			
		case 'm':
			if (argc != 4) {
				printf("Неверное число параметров!\n");
				break;
			}
			num1 = char_to_int(argv[2]);
			num2 = char_to_int(argv[3]);

			if (num1 > 0 && num2 > 0) {
				if (num1 % num2 == 0) {
					printf("Первое число кратно второму\n");
				}
				else {
					printf("Числа не кратны\n");
				}
			}
			else {
				printf("Некорректный ввод!\n");
			}
			
			break;
		case 't':

			if (argc != 5) {
				printf("Неверное число параметров!\n");
				break;
			}
			num1 = char_to_int(argv[2]);
			num2 = char_to_int(argv[3]);
			num3 = char_to_int(argv[4]);

			num1 = pow(num1, 2);
			num2 = pow(num2, 2);
			num3 = pow(num3, 2);
			if (num1 > 0 && num2 > 0 && num3 > 0) {
				if (num1 == num2 + num3 || num2 == num1 + num3 || num3 == num1 + num2) {
					printf("Это стороны прямоугольного треугольника\n");
				}
				else {
					printf("Введенные значения не могут быть сторонами прямоугольного треугольника\n");
				}
			}
			else {
				printf("Некорректный ввод!\n");
			}
			break;
		}	
	}
	return 0;
}
