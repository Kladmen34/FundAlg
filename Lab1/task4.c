#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAXSTRING 150

int main(){
    FILE *f1, *f2;
    char lex1[MAXSTRING], lex2[MAXSTRING], lex3[MAXSTRING];
    f1 = fopen("in.txt","r");
    f2 = fopen("out.txt","w");

    if (!f1){
        perror("Can't open file");
        return 1;
  }

  if (!f2){
    perror("Can't open file");
    return 2;
  }

  while (fscanf(f1, "%s%s%s", lex1, lex2, lex3) != EOF){
    fprintf(f2, "%s %s %s%s", lex3,lex1,lex2,"\n");
  }
  fclose(f1);
  fclose(f2);

  f1 = fopen("in.txt","w");
  f2 = fopen("out.txt","r");

  if (!f1){
    perror("Can't open file");
    return 3;
  }

  if (!f2){
    perror("Can't open file");
    return 4;
  }

  char c;
  while ((c = fgetc(f2)) != EOF)
    fputc(c, f1);
  fclose(f2);
  fclose(f1);
  remove("out.txt");
  return 0;
}
