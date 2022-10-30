#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SIZE 50

int main(int argc, char* argv[]){

    char flag;
    short fl = 0;
    FILE *in, *out;
    if (argv[1][0] == '-' || argv[1][0] == '/'){
        flag = (argv[1][1] == 'n') ? argv[1][2] : argv[1][1];
        char new_name[SIZE] = "out_";
        if (argv[1][1] == 'n')
            strcpy(new_name, argv[3]);
        else
            strcat(new_name, argv[2]);

        out = fopen(new_name, "w");

        if (out == NULL){
            printf("Can't open file!");
            return -1;
        }
    }
    else{
        printf("Wrong flag symbol");
        return -1;
    }

    in = fopen(argv[2], "r");
    if (in == NULL){
        printf("Can't open file!");
        fclose(out);
        return -1;
    }

    char input,old_input = ' ';
    int counter = 0;

    while (!feof(in)){
        input = fgetc(in);
        switch(flag){

        case 'd':
            if (!isdigit(input) && input != EOF)
                fputc(input, out);
            break;

        case 'i':
            if (isalpha(input))
                counter++;

            else if (input == '\n'){
                fprintf(out, "%d\n", counter);
                counter = 0;
            }
            else if (input == EOF)
                fprintf(out, "%d", counter);
            break;

        case 's':
            if (input == '\n'){
                    fprintf(out, "%d\n", ++counter);
                    counter = 0;
            }
            else if (input == EOF){
                fprintf(out, "%d", ++counter);
            }
            else if ((!isalnum(input)) && (!(input == ' ')))
                    counter++;
            break;

        case 'a':
            if (input == '\n')
                fputc('\n', out);
            else if ((!isdigit(input)) && (input != EOF))
                fprintf(out, "%d", input);
            else if ((isdigit(input)) && (input != EOF))
                fprintf(out, "%c", input);
            break;

        case 'f':
            if ((isalnum(input)) && (!isalnum(old_input)))
                    counter++;

            if ((counter % 2 == 0) && (counter % 5 != 0) && (isalnum(input))){
                fputc(tolower(input), out);
                fl = 1;
            }

            if ((counter % 5 == 0) && (counter % 2 != 0) && (isalnum(input))){
                fprintf(out, "%d", input);
                fl = 1;
            }

            if ((counter % 2 == 0) && (counter % 5 == 0) && (isalnum(input))){
                fprintf(out, "%d", tolower(input));
                fl = 1;
            }

            if ((fl == 0) && (input != EOF)){
                fputc(input, out);
            }
            else
                fl = 0;

            old_input = input;
            break;

        default:
            printf("Incorrect flag");
            fclose(in);
            fclose(out);
            break;
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}
