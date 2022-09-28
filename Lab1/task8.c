#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int mas1[128], new_mas[64];
    int k = 0, len = 0, sum = 0, len1 = 0, len2 = 0;
    int max = 0, min = 10000000;
    int action, indx;
    FILE *fi = fopen("input.txt", "r");
    if (!fi){
        perror("Can't open file");
        return 1;
    }
    
    while (fscanf(fi, "%d", mas1) != EOF)
        len++;
    
    printf("Меню:\n");
    printf("1.Переписать числа, стоящие на нечетных позициях\n");
    printf("2.Переписать четные числа\n");
    printf("3.Для текущего элемента: наиболее далёкое (по значению) от него значение из этого же массива\n");
    printf("4.Для текущего элемента: сумму элементов, которые ему предшествуют\n");
    printf("5.Для текущего элемента: сумму элементов, которые меньше него\n");
    
    scanf("%d", &action);
    
    switch(action){
        case 1:
            for (int i = 0; i < len / 2; i++){
            	new_mas[i] = mas1[2 * i + 1];
            	printf("%d ", new_mas[i]);
        }
        break;
        
        case 2:
            for (int i = 0; i < len; i++){
                if (mas1[i] % 2 == 0){
                    new_mas[k] = mas1[i];
                    printf("%d ", new_mas[k]);
                    k++;
                }
            }
        break;
        
        case 3:
            printf("Введите индекс текущего элемента:\n");
            scanf("%d", &indx);
            for (int i = 0; i < len; i++){
            	if (mas1[i] > max){
            		max = mas1[i];
            	}
            	if (mas1[i] < min){
            		min = mas1[i];
            	}
            }
         
            if (mas1[indx] == max){
            	new_mas[k] = min;
            	printf("%d", new_mas[k]);
            	break;
            }
            
            if (mas1[indx] == min){
            	new_mas[k] = max;
            	printf("%d", new_mas[k]);
            	break;
            }
            
            len1 = max - mas1[indx];
            len2 = mas1[indx] - min;
            
            if (len1 > len2){
            	new_mas[k] = max;
            	printf("%d", new_mas[k]);
            }
            else{
            	new_mas[k] = min;
            	printf("%d", new_mas[k]);
            }
            
            break;
            
        case 4:
            printf("Введите индекс текущего элемента:\n");
            scanf("%d", &indx);
            for (int i = 0; i < indx; i++){
                sum += mas1[i];
            }
            
            new_mas[k] = sum;
            printf("%d", new_mas[k]);
            break;
            
        case 5:
            printf("Введите индекс текущего элемента:\n");
            scanf("%d", &indx);
            for (int i = 0; i < len; i++){
                if (mas1[i] < mas1[indx]){
                    sum += mas1[i];
                }
            }
            
            new_mas[k] = sum;
            printf("%d", new_mas[k]);
            break;
            
        default:
            printf("Неверный пункт меню!");
            break;
    }
    fclose(fi);
    return 0;
}
