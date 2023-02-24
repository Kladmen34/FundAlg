#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define EPSILON 1e-7


enum{
    DONE = 0,
    MEMORY = 1,
    SYNTAX = 2,
    COUNT = 3
};

typedef struct list{
    int id;
    char* name;
    char* surname;
    int course;
    char* group;
    int* marks;
}list;

int reallocation(list** students, int* k){
    *k *= 2;
    list* p = (list*)realloc(*students, *k * sizeof(list));
    if(!p){
        return MEMORY;
    }
    *students = p;
    return DONE;
}

int free_list(list* students, int counter_of_persons){
    printf("\n\n [ Clearing list... ]\n");
    for(counter_of_persons; counter_of_persons > 0; counter_of_persons--){
        if((students + counter_of_persons - 1)->name != NULL) free((students + counter_of_persons - 1)->name);
        if((students + counter_of_persons - 1)->surname != NULL) free((students + counter_of_persons - 1)->surname);
        if((students + counter_of_persons - 1)->group != NULL) free((students + counter_of_persons - 1)->group);
        if((students + counter_of_persons - 1)->marks != NULL) free((students + counter_of_persons - 1)->marks);
    }
    free(students);
    return DONE;
}

int read_str(FILE* fin, char** str){
    if(feof(fin)) return COUNT;
    char c;
    int k = 4;
    *str = (char*)malloc(k*sizeof(char));
    if(!*str) return MEMORY;
    char* p_str = *str;
    while((c = (char)fgetc(fin)) != ' '){
        if(c == EOF){
            free (*str);
            return COUNT;
        }
        if(!isalpha(c)){
            free (*str);
            return SYNTAX;
        }
        if(p_str - *str == (k-1)){
            k *= 2;
            char* p = realloc(*str, k*sizeof(char));
            if(!p){
                return 2;
            }
            *str = p;
            p_str = *str + (k/2-1);
        }
        *p_str++ = c;
    }
    *p_str = 0;
    return DONE;
}

int read_str_no_valid(FILE* fin, char** str){
    if(feof(fin)) return COUNT;
    char c;
    int k = 4;
    *str = (char*)malloc(k*sizeof(char));
    if(!*str) return MEMORY;
    char* p_str = *str;
    while((c = (char)fgetc(fin)) != ' '){
        if(c == EOF){
            free (*str);
            return COUNT;
        }
        if(p_str - *str == (k-1)){
            k *= 2;
            char* p = realloc(*str, k*sizeof(char));
            if(!p){
                return 2;
            }
            *str = p;
            p_str = *str + (k/2-1);
        }
        *p_str++ = c;
    }
    *p_str = 0;
    return DONE;
}

int read_marks(FILE* fin, int** marks){
    int count = 0;
    char c;
    if(feof(fin)) return COUNT;
    *marks = (int*)malloc(5 * sizeof(int));
    if(!*marks) return MEMORY;
    while(count != 5){
        (*marks)[count] = 0;
        if(count < 4){
            while((c = (char)fgetc(fin)) != ' '){
                if(c == EOF){
                    free(*marks);
                    return COUNT;
                }
                if(!isdigit(c)){
                    free(*marks);
                    return SYNTAX;
                }
                (*marks)[count] = (*marks)[count] * 10 + (c - '0');
            }
            if((*marks)[count] < 2 || (*marks)[count] > 5){
                free(*marks);
                return SYNTAX;
            }
            count++;
        }else{
            while((c = (char)fgetc(fin)) != '\n'){
                if(c == EOF){
                    break;
                }
                if(c == ' '){
                    free(*marks);
                    return COUNT;
                }
                if(!isdigit(c)){
                    free(*marks);
                    return SYNTAX;
                }
                (*marks)[count] = (*marks)[count] * 10 + (c - '0');
            }
            if((*marks)[count] < 2 || (*marks)[count] > 5){
                free(*marks);
                return SYNTAX;
            }
            count++;
        }
    }
    return DONE;
}

int read_int(FILE* fin, int* num){
    if(feof(fin)) return COUNT;
    *num = 0;
    char c;
    while((c = (char)fgetc(fin)) != ' '){
        if(c == EOF) return COUNT;
        if(!isdigit(c)){
            return SYNTAX;
        }
        *num = *num * 10 + (c - '0');
    }
    return DONE;
}

int read_file(list** students, int* k, FILE* fin, int* counter_of_persons){
    int error = 0;
    while(!feof(fin)){
        if(*counter_of_persons == *k){
            if(reallocation(students, k)) return MEMORY;
        }
        error = read_int(fin, &((*(*students + *counter_of_persons)).id));
        if(error == COUNT){
            return COUNT;
        }else if(error == SYNTAX || (*students + *counter_of_persons)->id < 1){
            return SYNTAX;
        }
        error = read_str(fin, &((*(*students + *counter_of_persons)).name));
        if(error == MEMORY){
            return MEMORY;
        }else if(error == SYNTAX || strlen((*students + *counter_of_persons)->name) < 1){
            return SYNTAX;
        }else if(error == COUNT){
            return COUNT;
        }
        error = read_str(fin, &((*(*students + *counter_of_persons)).surname));
        if(error == MEMORY){
            return MEMORY;
        }else if(error == SYNTAX || strlen((*students + *counter_of_persons)->surname) < 1){
            return SYNTAX;
        }else if(error == COUNT){
            return COUNT;
        }
        error = read_int(fin, &((*students + *counter_of_persons)->course));
        if(error == COUNT){
            return COUNT;
        }else if(error == SYNTAX || (*students + *counter_of_persons)->course < 1 || (*students + *counter_of_persons)->course > 4){
            return SYNTAX;
        }
        error = read_str_no_valid(fin, &((*(*students + *counter_of_persons)).group));
        if(error == MEMORY){
            return MEMORY;
        }else if(error == SYNTAX || strlen((*students + *counter_of_persons)->surname) < 1){
            return SYNTAX;
        }else if(error == COUNT){
            return COUNT;
        }
        error = read_marks(fin, &((*students + *counter_of_persons)->marks));
        if(error == MEMORY){
            return MEMORY;
        }else if(error == SYNTAX){
            return SYNTAX;
        }else if(error == COUNT){
            return COUNT;
        }
        (*counter_of_persons)++;
    }
    return DONE;
}

double find_stud_average(int* marks){
    double sum = 0.0;
    for(int i = 0; i < 5; i++){
        sum += marks[i];
    }
    return sum/5;
}

int print_all_students(list* students, int counter_of_persons){
    printf("\n");
    if(counter_of_persons == 0){
        printf("List if empty!\n");
        return DONE;
    }
    for(int i = 0; i < counter_of_persons; i++){
        printf("%d %s %s ", students[i].id, students[i].name, students[i].surname);
        printf("%d %s ", students[i].course, students[i].group);
        for(int l = 0; l < 5; l++){
            printf("%d ", students[i].marks[l]);
        }
        printf("(average: %f)\n", find_stud_average(students[i].marks));
    }
    return DONE;
}

int print_certain_student(list* students, int i){
    printf("%d %s %s ", students[i].id, students[i].name, students[i].surname);
    printf("%d %s ", students[i].course, students[i].group);
    for(int l = 0; l < 5; l++){
        if(l == 4) printf("%d\n", students[i].marks[l]);
        else printf("%d ", students[i].marks[l]);
    }
    return DONE;
}

int input_str(char** str){
    fflush(stdin);
    char c;
    int k = 2;
    *str = (char*)malloc(k*sizeof(char));
    char* p_str = *str;
    if(!*str) return MEMORY;
    while((c = getchar()) != '\n'){
        if(p_str - *str == k-1){
            k*=2;
            char* p = (char*)realloc(*str, k*sizeof(char));
            if(!p){
                free(*str);
                return MEMORY;
            }else{
                *str = p;
                p_str = *str + (k/2)-1;
            }
        }
        *p_str++ = c;
    }
    *p_str = 0;
    fflush(stdin);
    return 0;
}

int find_name(list* students, int counter_of_persons){
    char* name;
    int error = 0;
    error = input_str(&name);
    if(error) return MEMORY;
    error = 0;
    for(int i = 0; i < counter_of_persons; i++){
        if(!strcmp(students[i].name, name)){
            error++;
            print_certain_student(students, i);
        }
    }
    if(!error) printf("\nStudents with entered parameter are not found\n\n");
    else printf("\n");
    free(name);
    return DONE;
}

int find_surname(list* students, int counter_of_persons){
    char* surname;
    int error = 0;
    error = input_str(&surname);
    if(error) return MEMORY;
    error = 0;
    for(int i = 0; i < counter_of_persons; i++){
        if(!strcmp(students[i].surname, surname)){
            error++;
            print_certain_student(students, i);
        }
    }
    if(!error) printf("\nStudents with entered parameter are not found\n\n");
    else printf("\n");
    free(surname);
    return DONE;
}

int find_group(list* students, int counter_of_persons){
    char* group;
    int error = 0;
    error = input_str(&group);
    if(error) return MEMORY;
    error = 0;
    for(int i = 0; i < counter_of_persons; i++){
        if(!strcmp(students[i].group, group)){
            error++;
            print_certain_student(students, i);
        }
    }
    if(!error) printf("\nStudents with entered parameter are not found\n\n");
    else printf("\n");
    free(group);
    return DONE;
}

int find_id(list* students, int counter_of_persons){
    char* id_str;
    int error = 0;
    error = input_str(&id_str);
    if(error) return MEMORY;
    int id = atoi(id_str);
    error = 0;
    for(int i = 0; i < counter_of_persons; i++){
        if(students[i].id == id){
            error++;
            print_certain_student(students, i);
        }
    }
    if(!error) printf("\nStudents with entered parameter are not found\n\n");
    else printf("\n");
    free(id_str);
    return DONE;
}

int find_course(list* students, int counter_of_persons){
    char* course_str;
    int error = 0;
    error = input_str(&course_str);
    if(error) return MEMORY;
    int course = atoi(course_str);
    error = 0;
    for(int i = 0; i < counter_of_persons; i++){
        if(students[i].course == course){
            error++;
            print_certain_student(students, i);
        }
    }
    if(!error) printf("\nStudents with entered parameter are not found\n\n");
    else printf("\n");
    free(course_str);
    return DONE;
}

int compare_id(const void* x_void, const void* y_void){
    int x = (*(list*)x_void).id;
    int y = (*(list*)y_void).id;
    return x-y;
}

int compare_course(const void* x_void, const void* y_void){
    int x = (*(list*)x_void).course;
    int y = (*(list*)y_void).course;
    return x-y;
}

int compare_name(const void* x_void, const void* y_void){
    char* x = (*(list*)x_void).name;
    char* y = (*(list*)y_void).name;
    return strcmp(x, y);
}

int compare_surname(const void* x_void, const void* y_void){
    char* x = (*(list*)x_void).surname;
    char* y = (*(list*)y_void).surname;
    return strcmp(x, y);
}

int compare_group(const void* x_void, const void* y_void){
    char* x = (*(list*)x_void).group;
    char* y = (*(list*)y_void).group;
    return strcmp(x, y);
}

char* make_path(char* old_path, int num){
    char* path = (char*)malloc((strlen(old_path)+3)*sizeof(char));
    if(!path){
        return NULL;
    }
    int id = strlen(old_path)+2;
    strcpy(path, old_path);
    while(path[id] != '.'){
        id--;
    }
    path[id++] = '_';
    path[id++] = (char)(48 + num);
    path[id++] = '.';
    path[id++] = 't';
    path[id++] = 'x';
    path[id++] = 't';
    path[id] = 0;
    return path;
}

double find_average(list* students, int counter_of_persons){
    int sum;
    double avr = 0.0;
    for(int i = 0; i < counter_of_persons; i++){
        sum = 0;
        for(int l = 0; l < 5; l++){
            sum += students[i].marks[l];
        }
        avr = avr + (sum/5);
    }
    return avr/counter_of_persons;
}

void write_student(FILE* fout, list* students, int l){
    fprintf(fout, "%d %s %s ", students[l].id, students[l].name, students[l].surname);
    fprintf(fout, "%d %s ", students[l].course, students[l].group);
    for(int i = 0; i < 5; i++){
        if(i == 4) fprintf(fout, "%d\n", students[l].marks[i]);
        else fprintf(fout, "%d ", students[l].marks[i]);
    }
}

void print_groups(list** courses, int c1, int c2, int c3, int c4){
    int k;
    for(int i = 0; i < 4; i++){
        if(courses[i] != NULL){
            printf("\n-- %d course: --\n", i+1);
            if(i == 0) k = c1;
            else if(i == 1) k = c2;
            else if(i == 2) k = c3;
            else if(i == 3) k = c4;
            for(int l = 0; l < k; l++){
                printf("%d %s %s ", (courses[i])[l].id, (courses[i])[l].name, (courses[i])[l].surname);
                printf("%d %s ", (courses[i])[l].course, (courses[i])[l].group);
                for(int s = 0; s < 5; s++){
                    if(s == 4) printf("%d\n", (courses[i])[l].marks[s]);
                    else printf("%d ", (courses[i])[l].marks[s]);
                }
            }
            printf("\n");
        }
    }
}

list** group_students(list* students, int counter_of_persons, int* c1, int* c2, int* c3, int* c4){
    list** courses = (list**)malloc(4*sizeof(list*));
    if(!courses){
        return NULL;
    }
    for(int i = 0; i < counter_of_persons; i++){
        if(students[i].course == 1) (*c1)++;
        else if(students[i].course == 2) (*c2)++;
        else if(students[i].course == 3) (*c3)++;
        else (*c4)++;
    }
    list* course1 = (list*)malloc((*c1)*sizeof(list));
    if(!course1){
        free(courses);
        return NULL;
    }
    if((*c1) != 0) courses[0] = course1;
    else{
        free(course1);
        course1 = NULL;
        courses[0] = course1;
    }
    list* course2 = (list*)malloc((*c2)*sizeof(list));
    if(!course2){
        free(course1);
        free(courses);
        return NULL;
    }
    if((*c2) != 0) courses[1] = course2;
    else{
        free(course2);
        course2 = NULL;
        courses[1] = course2;
    }
    list* course3 = (list*)malloc((*c3)*sizeof(list));
    if(!course3){
        free(course1);
        free(course2);
        free(courses);
        return NULL;
    }
    if((*c3) != 0) courses[2] = course3;
    else{
        free(course3);
        course3 = NULL;
        courses[2] = course3;
    }
    list* course4 = (list*)malloc((*c4)*sizeof(list));
    if(!course4){
        free(course1);
        free(course2);
        free(course3);
        free(courses);
        return NULL;
    }
    if((*c4) != 0) courses[3] = course4;
    else{
        free(course4);
        course4 = NULL;
        courses[3] = course4;
    }
    (*c1) = 0; (*c2) = 0; (*c3) = 0; (*c4) = 0;
    for(int i = 0; i < counter_of_persons; i++){
        if(students[i].course == 1){
            course1[(*c1)] = students[i];
            (*c1)++;
        }
        else if(students[i].course == 2){
            course2[(*c2)] = students[i];
            (*c2)++;
        }
        else if(students[i].course == 3){
            course3[(*c3)] = students[i];
            (*c3)++;
        }
        else if(students[i].course == 4){
            course4[(*c4)] = students[i];
            (*c4)++;
        }
    }
    return courses;
}

int main(int argc, char *argv[]){
    int counter_of_persons = 0;
    int comm_error;
    int k = 2;
    if(argc != 2){
        printf("Wrong number of arguments\n");
        return -1;
    }
    printf("\n[ Reading file... ]\n\n");
    FILE* fin = fopen(argv[1], "r");
    if(!fin){
        printf("Could not open a file\n");
        return -2;
    }
    list* students = (list*)malloc(k * sizeof(list));
    if(!students){
        printf("Memory allocation error\n");
        return -3;
    }
    comm_error = read_file(&students, &k, fin, &counter_of_persons);
    if(comm_error == MEMORY){
        printf("Memory allocation error\n");
        free_list(students, counter_of_persons);
        fclose(fin);
        return -4;
    }else if(comm_error == SYNTAX){
        printf("Wrong syntax\n");
        free_list(students, counter_of_persons);
        fclose(fin);
        return -5;
    }else if(comm_error == COUNT) {
        printf("Wrong amount of line properties\n");
        free_list(students, counter_of_persons);
        fclose(fin);
        return -6;
    }
    fclose(fin);
    if(counter_of_persons == 0){
        printf("\n[ Warning! List is empty! ]\n");
    }
    int action = 0;
    while(action != 6){
        printf("\n--- Choose an action: ---\n");
        printf("1) Find\n");
        printf("2) Sort\n");
        printf("3) Print\n");
        printf("4) Group\n");
        printf("5) Save (above)\n");
        printf("6) Exit\n");
        if(!scanf("%d", &action)){
            printf("\nInvalid command\n\n");
            fflush(stdin);
            continue;
        }
        if(action == 1){
            if(counter_of_persons == 0){
                printf("\nList is empty!\n\n");
                continue;
            }
            printf("\nChoose a parameter to find:\n");
            printf("1) ID\n");
            printf("2) Name\n");
            printf("3) Surname\n");
            printf("4) Course\n");
            printf("5) Group\n");
            int n = 0;
            scanf("%d", &n);
            if(n < 1 || n > 5){
                printf("\nInvalid number of parameter\n\n");
                fflush(stdin);
                continue;
            }
            if(n == 1){
                printf("\nID:\n");
                if(find_id(students, counter_of_persons)){
                    printf("Memory allocation error!\n\n");
                    break;
                }
            }else if(n == 2){
                printf("\nName:\n");
                if(find_name(students, counter_of_persons)){
                    printf("Memory allocation error!\n\n");
                    break;
                }
            }else if(n == 3){
                printf("\nSurname:\n");
                if(find_surname(students, counter_of_persons)){
                    printf("Memory allocation error!\n\n");
                    break;
                }
            }else if(n == 4){
                printf("\nCourse:\n");
                if(find_course(students, counter_of_persons)){
                    printf("Memory allocation error!\n\n");
                    break;
                }
            }else if(n == 5){
                printf("\nGroup:\n");
                if(find_group(students, counter_of_persons)){
                    printf("Memory allocation error!\n\n");
                    break;
                }
            }
            printf("[ Find ---> done! ]\n\n");
            action = 0;
        }else if(action == 2){
            printf("\nChoose a parameter to sort:\n");
            printf("1) ID\n");
            printf("2) Name\n");
            printf("3) Surname\n");
            printf("4) Course\n");
            printf("5) Group\n");
            int n = 0;
            scanf("%d", &n);
            if(n < 1 || n > 5){
                printf("\nInvalid number of parameter\n\n");
                fflush(stdin);
                continue;
            }
            if(n == 1){
                qsort(students, counter_of_persons, sizeof(list), compare_id);
            }else if(n == 2){
                qsort(students, counter_of_persons, sizeof(list), compare_name);
            }else if(n == 3){
                qsort(students, counter_of_persons, sizeof(list), compare_surname);
            }else if(n == 4){
                qsort(students, counter_of_persons, sizeof(list), compare_course);
            }else if(n == 5){
                qsort(students, counter_of_persons, sizeof(list), compare_group);
            }
            printf("\n[ Sort ---> done! ]\n\n");
            action = 0;
        }else if(action == 3){
            if(counter_of_persons == 0) printf("\nList is empty!\n\n");
            else print_all_students(students, counter_of_persons);
            printf("\n[ Print ---> done! ]\n\n");
            action = 0;
        }else if(action == 4){
            int c1 = 0, c2 = 0, c3 = 0, c4 = 0;
            list** courses = group_students(students, counter_of_persons, &c1, &c2, &c3, &c4);
            if(courses == NULL) break;
            print_groups(courses, c1, c2, c3, c4);
            free(courses[0]);
            free(courses[1]);
            free(courses[2]);
            free(courses[3]);
            free(courses);
            printf("\n[ Group ---> done! ]\n\n");
            action = 0;
        }else if(action == 5){
            double avr = find_average(students, counter_of_persons);
            for(int i = 1; i < 5; i++){
                char* path = make_path(argv[1], i);
                if(!path){
                    printf("Memory allocation error!\n\n");
                    break;
                }
                FILE* fout = fopen(path, "w");
                if(!fout){
                    printf("Could not create a file %s", path);
                    free(path);
                    break;
                }
                for(int l = 0; l < counter_of_persons; l++){
                    if(students[l].course == i){
                        if((find_stud_average(students[l].marks)-avr)>EPSILON){
                            printf("\n%d %s %f ---> %d course\n", students[l].id, students[l].name, find_stud_average(students[l].marks), i);
                            write_student(fout, students, l);
                        }
                    }
                }
                fclose(fout);
                free(path);
            }
            printf("\n[ Average ---> %f ]\n\n", avr);
            printf("\n[ Save ---> done! ]\n\n");
            action = 0;
        }else if(action == 6){
            break;
        }else{
            printf("\nInvalid command\n\n");
            fflush(stdin);
            continue;
        }
    }

    free_list(students, counter_of_persons);
    return 0;
}
