#include <stdio.h>
#include <stdlib.h>

#define LEN 200
int main(int argc, char* argv[]){
	FILE *in, *out, *files;
	char c, str[LEN], act;
	int flag = 1;
	if (argv[1][0] == '-'){
		if (argv[1][1] == 'f' && argv[1][2] == 'i'){
			act = 'f';
		}
		else if (argv[1][1] == 'c' && argv[1][2] == 'i' && argv[1][3] == 'n'){
			act = 'c';
		}
		else if (argv[1][1] == 'a' && argv[1][2] == 'r' && argv[1][3] == 'g'){
			act = 'a';
		}
	}
	switch(act){
		
	case 'f':
		if (argc != 3){
			printf("Неверное число аргументов!");
			return 1;
		}
		out = fopen("out.txt", "w");
		files = fopen(argv[2], "r");
		if (!files){
			printf("Невозможно открыть файл!");
			return 2;
		}
		while (!feof(files)){
			fscanf(files, "%s", str);
			in = fopen(str, "r");
			if (!in){
				printf("Невозможно открыть файл!");
				return 3;
			}
			while ((c = fgetc(in)) != EOF)
				fputc(c, out);
			fclose(in);
		}
		fclose(out);	
		fclose(files);
		
		break;
			
	case 'c':
		
		out = fopen("out.txt", "w");
                scanf("%s", str);
                if (str[0] == 's')
                    flag = 0;
                while(flag){ 
                    in = fopen(str, "r");
                    if (!in){
                        printf("Невозможно открыть файл!");
                        return 4;
                    }
                    while ((c = fgetc(in)) != EOF)
				fputc(c, out);
                        fclose(in);
                    
                    scanf("%s", str);
                    if (str[0] == 's')
                        flag = 0;
                }
                fclose(out);
                break;
			
			
	case 'a':
		if (argc < 3){
			printf("Неверное число аргументов!");
			return 5;
		}
		out = fopen("out.txt", "w");
		for (int i = 2; i < argc; i++){
			in = fopen(argv[i], "r");
			if (!in){
				printf("Невозможно открыть файл!");
				return 6;
			}
			while ((c = fgetc(in)) != EOF)
				fputc(c, out);
			fclose(in);
		}
		fclose(out);
		break;
	}	
	return 0;
}
				
	
		
