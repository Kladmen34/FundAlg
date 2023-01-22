#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

enum ERRORS {
    NO_MEMORY = -1,
    INPUT_ERROR = -2,
    DONE = 0,
    FILE_ERROR = -3,
    HANDLED = -4
};


typedef struct message {
    size_t id;
    char* str;
    size_t len;
    size_t size;
}message;

int init_message(message** mess) {
    *mess = (message*)malloc(sizeof(message));
    if (!*mess) {
        return NO_MEMORY;
    }
    (*mess)->id = 1;
    (*mess)->size = 2;
    (*mess)->str = (char*)malloc((*mess)->size * sizeof(char));
    if (!(*mess)->str) {
        return NO_MEMORY;
    }
    (*mess)->len = 0;
    return DONE;
}

int renew_mess(message** mess) {
    free((*mess)->str);
    (*mess)->id++;
    (*mess)->size = 2;
    (*mess)->str = (char*)malloc((*mess)->size * sizeof(char));
    if (!(*mess)->str) {
        return NO_MEMORY;
    }
    (*mess)->len = 0;
    return DONE;
}

int gen_symb(char** filename, int len) {
    *filename = (char*)calloc(len, sizeof(char));
    if (!*filename)
        return NO_MEMORY;
    int ver = 0, init = len - 1;
    char symb;
    for (int i = 0; i < init; i++) {
        ver = rand() & 1;
        switch (ver) {
        case 0:
            (*filename)[i] = rand() % 26 + 97;
            break;
        case 1:
            (*filename)[i] = rand() % 10 + 48;
            break;
        }
    }
    (*filename)[init] = '\0';
    return DONE;
}

int add_to_file(FILE* filename, message** mess, char* stop_word, int* size) {
    int flag = 1;
    char c;
    while (flag) {
        c = getchar();
        if (c != '\n') {
            if ((*mess)->size == (*mess)->len) {
                (*mess)->size <<= 1;
                (*mess)->str = (char*)realloc((*mess)->str, (*mess)->size);
                if ((*mess)->str == NULL) {
                    return NO_MEMORY;
                }
            }
            (*mess)->str[(*mess)->len++] = c;
        }
        else {
            if ((*mess)->size == (*mess)->len) {
                (*mess)->size += 1;
                (*mess)->str = (char*)realloc((*mess)->str, (*mess)->size);
                if ((*mess)->str == NULL) {
                    return NO_MEMORY;
                }
            }
            (*mess)->str[(*mess)->len] = '\0';
            if (!strcmp((*mess)->str, stop_word)) {
                flag = 0;
                *size = (*mess)->id;
                free((*mess)->str);
                free((*mess));
                if (init_message(mess) == NO_MEMORY)
                    return NO_MEMORY;
                break;
            }
            fprintf(filename, "%d,", (*mess)->id);
            fprintf(filename, "\"%s\"\n", (*mess)->str);
            if (renew_mess(mess) == NO_MEMORY)
                return NO_MEMORY;
        }
    }
    return DONE;
}

int from_file_to_msg(FILE* filename, message*** mess, int size) {
    int del = 1, bf_size = 2, curr = 0, ln = 0, num = 0;
    char c;
    char* buf = NULL;
    *mess = (message**)malloc(size * sizeof(message*));
    if (!*mess){
      return NO_MEMORY;
    }
    while ((c = fgetc(filename)) != EOF) {
        if (c == ',' && del) {
            init_message(&(*mess)[curr]);
            if (bf_size == ln) {
                bf_size += 1;
                buf = (char*)realloc(buf, bf_size);
                if (!buf)
                    return NO_MEMORY;
            }
            buf[ln] = '\0';
            num = atoi(buf);
            (*mess)[curr]->id = num;
            free(buf);
            buf = NULL;
            bf_size = 2;
            ln = 0;
            del = 0;
            fgetc(filename);
        }
        else if (del) {
            if (!buf) {
                buf = (char*)malloc(bf_size * sizeof(char));
                if (!buf) {
                    return NO_MEMORY;
                }
            }
            if (bf_size == ln) {
                bf_size <<= 1;
                char* temp;
                if (!(temp = (char*)realloc(buf, bf_size * sizeof(char))))
                {
                  free(buf);
                    return NO_MEMORY;
                }
                buf = temp;
            }
            buf[ln++] = c;
        }
        else if (!del && c != '\n') {
            if ((*mess)[curr]->size - 1 == (*mess)[curr]->len) {
                (*mess)[curr]->size <<= 1;
                (*mess)[curr]->str = (char*)realloc((*mess)[curr]->str, (*mess)[curr]->size * sizeof(char));
                if (!(*mess)[curr]->str)
                    return NO_MEMORY;
            }
            (*mess)[curr]->str[(*mess)[curr]->len++] = c;
        }
        else if (c == '\n') {
            (*mess)[curr]->str[(*mess)[curr]->len - 1] = '\0';
            curr++;
            del = 1;
        }
    }
    return DONE;
}

int prinr_messages(message*** mess, int len) {
    for (int i = 0; i < len; i++) {
        printf("id: %d, message: %s\n", (*mess)[i]->id, (*mess)[i]->str);
    }
    return DONE;
}

int free_messages(message*** mess, int len) {
    for (int i = 0; i < len; i++) {
        free((*mess)[i]->str);
        free((*mess)[i]);
    }
    free(*mess);
    *mess = NULL;
    return DONE;
}


int main(int argc, char* argv[]) {
    srand(time(NULL));
    char chr, del;
    message* mess = NULL;
    message** post = NULL;
    int code, tries = 0, ids = 0;
    char suffix[] = ".CSV";
    char* filename = NULL;
    if (argc != 2) {
       printf("You need to start programm with one stop word!\n");
       return INPUT_ERROR;
    }
    code = gen_symb(&filename, 10);
    if (code == NO_MEMORY) {
        printf("No memory error, try again later!\n");
        return NO_MEMORY;
    }
    strcat(filename, suffix);
    code = init_message(&mess);
    if (code == NO_MEMORY) {
        printf("No memory, sry!\n");
        return NO_MEMORY;
    }
    else {
        printf("\nPlease, enter messages, if you want to stop, enter \'%s\'\n", argv[1]);
        FILE* file = fopen(filename, "w");
        if (file == NULL) {
            return FILE_ERROR;
        }
        code = add_to_file(file, &mess, argv[1], &ids);
        fclose(file);
        file = fopen(filename, "r");
        code = from_file_to_msg(file, &post, ids);
        if (code == DONE) {
            printf("\nTyped messages\n");
            fclose(file);
            prinr_messages(&post, ids);
            free_messages(&post, ids);
        }
        else {
            printf("\nError\n");
            return code;
        }
    }
    return 0;
}
