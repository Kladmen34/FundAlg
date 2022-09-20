#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>

int char_to_int(char* string) {
	int result = 0;
	while (*string) {
		result = result * 10 + (*string - '0');
		string++;
	}
	return result;
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	if (strlen(argv[1]) != 2) {
		printf("Неверная длина флага!\n");
	}
	int a, b, c, D;
	double x1, x2;
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

			D = pow(b, 2) - 4 * a * c;

			if (D > 0) {
				x1 = (-b + sqrt(D)) / (2 * a);
				x2 = (-b - sqrt(D)) / (2 * a);
				printf("Корни - %lf %lf", x1, x2);
			}
			else if (D == 0) {
				x1 = -(-b) / (2 * a);
				printf("Корень - %lf", x1);
			}
			else {
				printf("Корней нет");
			}
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

			if (num1 > 0 && num2 > 0 && num3 > 0) {
				num1 = pow(num1, 2);
				num2 = pow(num2, 2);
				num3 = pow(num3, 2);

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
