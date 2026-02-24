#include "applied.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <readline/readline.h>

int error = 0;
int input(Info* information){
    char* string = readline("Введите строку: "); if (string == NULL) {free(string); return 3;}
    char* buffer1 = strdup(string); if (buffer1 == NULL) {free(buffer1); return 1; }
    information->receptions_amount = 0;
    int err = correct(buffer1);
    char* buffer2 = NULL; char* buffer3 = NULL;
    int total_check = 0;
    while (total_check == 0){
        while (err != 0){
            free(string);
            free(buffer1);
            string = readline("\n\nIncorrect input, try again: "); if (string == NULL) {free(string); return 3;}
            buffer1 = strdup(string); if (buffer1 == NULL) {free(buffer1); return 1; }
            err = correct(buffer1); if (err == 3) return 3;
        }
        buffer2 = strdup(string); if (buffer2 == NULL) return 1;
        information->pass_amount = Passanger_count(buffer2);//вычисляем количество людей
        information->storage = malloc(information->pass_amount * sizeof(Passanger)); if (information->storage == NULL) {free(string); free(buffer1); return 1;}
        initialization(information);
        buffer3 = strdup(string); if (buffer3 == NULL) return 1;
        error = entering(information->storage, buffer3, &information->receptions_amount);//занесение пасажиров в массив
        if (error == 1) {free(string); free(buffer1); free(buffer2); free(buffer3); free_array(information); return 1;}
        if (error == -3){
            printf("Error: non-consecutive arrival time for passengers\n"); free(buffer2); free(buffer3); free_array(information); err = -3;
        }
        if (error == 0) total_check = 1; 
    }

    free(string);
    free(buffer1);
    free(buffer2);
    free(buffer3); 
    return 0;
}

int correct(char* buffer){
    const char delimiters[] = " /";
    if (buffer == NULL)
        return 3;//EOF
    char* token = strtok(buffer, delimiters);
    if (token != NULL){
        int err = is_number(token);
        printf("\n");
        if (err != 0){
            if (err == -4) {printf("%s - Beyond the int size\n", token); return -4;}
            if (err == -2) {printf("Incorrect queues value\n"); return -1;}
        }
    }
    while (token != NULL){
        token = strtok(NULL, delimiters);
        token = strtok(NULL, delimiters);
        if (token != NULL){
            int err = is_number(token);
            if (err != 0){
                if (err == -4) {printf("%s - Beyond the int size\n", token); return -4;}
                if (err == -2) {printf("%s - Incorrect ta value\n", token); return -1;}
            }
        }
        token = strtok(NULL, " ");
        if (token != NULL){
            int err = is_number(token);
            if (err != 0){
                if (err == -4) {printf("%s - Beyond the int size\n", token); return 1;}
                if (err == -2) {printf("%s - Incorrect ts value\n", token); return 1;}
            }
        }
    }
    return 0;
}



int is_number(char* token){
    long long num = 0;
    if (isdigit(*token) == 0)
        return -2;//не число
    if ((*token - '0') == 0)
        return -2;//начинается с 0 или просто 0
    token++;
    while (*token != '\0'){
        if (isdigit(*token) == 0)
            return -2;//первое поле - не число
        num = num * 10 + (*token - '0');
        if (num > INT_MAX || num < INT_MIN)
            return -4;//за пределами инта
        token++;//переход на следующий символ
    }
    return 0;
}


int Passanger_count(char* buffer){
    unsigned int passanger_amount = 0;
    const char delimiters[] = " /";
    char* token = strtok(buffer, delimiters);
    while (token != NULL){
        token = strtok(NULL, delimiters);
        token = strtok(NULL, delimiters);
        token = strtok(NULL, " ");
        passanger_amount++;
    }
    passanger_amount--;
    return passanger_amount;
}




int entering(Passanger* Passanger_storage, char* buffer, unsigned int* queue_amount){
    const char delimiters[] = " /";
    char* token = strtok(buffer, delimiters);
    *queue_amount = writing_int(token);
    int i = 0;
    while (token != NULL){
        token = strtok(NULL, delimiters);
        if (token != NULL){
            Passanger_storage[i].id = strdup(token); if (Passanger_storage[i].id == NULL) return 0; 
        }


        token = strtok(NULL, delimiters);
        if (token != NULL){
            Passanger_storage[i].ta = writing_int(token);
            if (i > 0 && Passanger_storage[i-1].ta > Passanger_storage[i].ta)//проверка, чтобы пользователи записывались по времени прибывания по возрастанию
                {error = -3; return -3;}
                
        }

        token = strtok(NULL, " ");
        if (token != NULL)
            Passanger_storage[i].ts = writing_int(token);
        i++;
    }
    return 0;
}


int writing_int(char* token){
    int number= 0;
    while (*token != '\0'){
        isdigit(token);
        number = number * 10 + (*token - '0');
        token++;//переход на следующий символ
    }
    return number;
}


void output(Info* information){
    for (unsigned i = 0; i < information->pass_amount; i++){
        printf("Passanger %d --> %s/%d/%d\n", (i+1), information->storage[i].id, information->storage[i].ta, information->storage[i].ts);
    }
    printf("\n");
}

void initialization(Info* information){
    for (unsigned i = 0; i < information->pass_amount; i++){
        information->storage[i].id = NULL;
        information->storage[i].ta = 0;
        information->storage[i].ts = 0;
    }
}

void free_array(Info* information){
    for (unsigned i = 0; i < information->pass_amount; i++){
        free(information->storage[i].id);
    }
    free(information->storage);
}

