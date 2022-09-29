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

void flag_q(int a, int b, int c) {
	if (a == 0) {
		printf("Неверный ввод!\n");
		return 1;
	}
	int D;
	float x1, x2;
	D = pow(b, 2) - (4 * a * c);
	printf("При a = %d, b = %d, c = %d:\n", a, b, c);
	if (D > 0) {
		x1 = (-b + sqrt(D)) / ((2 * a));
		x2 = (-b - sqrt(D)) / ((2 * a));
		printf("Корни - %f %f\n", x1, x2);
	}
	else if (D == 0) {
		x1 = (-b) / (2 * a);
		printf("Корень - %f\n", x1);
	}
	else {
		printf("Корней нет\n");
	}
}

void flag_m(int n1, int n2) {
	if (n1 > 0 && n2 > 0) {
		if (n1 % n2 == 0) {
			printf("Первое число кратно второму\n");
		}
		else {
			printf("Числа не кратны\n");
		}
	}
	else {
		printf("Некорректный ввод!\n");
	}
}

void flag_t(int n1, int n2, int n3) {
	if (n1 > 0 && n2 > 0 && n3 > 0) {
		n1 = pow(n1, 2);
		n2 = pow(n2, 2);
		n3 = pow(n3, 2);

		if (n1 == n2 + n3 || n2 == n1 + n3 || n3 == n1 + n2) {
			printf("Это стороны прямоугольного треугольника\n");
		}
		else {
			printf("Введенные значения не могут быть сторонами прямоугольного треугольника\n");
		}
	}
	else {
		printf("Некорректный ввод!\n");
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

			flag_m(num1, num2);
			break;
		case 't':
			if (argc != 5) {
				printf("Неверное число параметров!\n");
				break;
			}

			num1 = char_to_int(argv[2]);
			num2 = char_to_int(argv[3]);
			num3 = char_to_int(argv[4]);

			flag_t(num1, num2, num3);
			break;
		}	
	}
	return 0;
}
