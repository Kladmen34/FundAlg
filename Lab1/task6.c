#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int detect_system(char *x){
	int k, max = 0;
  	for(int i = 0; x[i]; i++){
    		char d =  x[i];
    		if ('0' <= d && d <= '9') k = d - '0';
    		else if ('A' <= d && d<='Z') k = d - 'A' + 10;
    		if (k > max) max = k;
  	}
	return max + 1;
}

int convert_to_dec(char *x, int base){
	int n = 0, k;
    	for(int i = 0; x[i]; i++){
      		char d = x[i];
      		if ('0' <= d && d <= '9') k = d - '0';
      		else if ('A' <= d && d <='Z') k = d - 'A' + 10;
      		n = base * n + k;
    	}
    	return n;
}

int main(){
  	FILE *f_in, *f_out;
  	char str[50];
  	int base = 0, n = 0;
  	f_in=fopen("in.txt", "r");
  	f_out=fopen("out.txt", "w");
	if (!f_in){
		perror("Can't open file");
		return -1;
	}

	if (!f_out){
		perror("Can't open file");
		return -2;
	}

  	while (!feof(f_in)){
    		fscanf(f_in, "%s", str);
    		base = detect_system(str);
    		n = convert_to_dec(str, base);
    		fprintf(f_out, "%s %d %d\n", str, base, n);
  	}
  	fclose(f_in);
  	fclose(f_out);
	return 0;
}
