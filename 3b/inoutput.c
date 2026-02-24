#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <readline/readline.h>
#include "dialogue.h"

Error_storage error_storage[] = {
    {"\n\t\t\tError: Table is empty\n", ERROR_EMPTY_TABLE},
    {"\n\t\t\tError: EOF\n", ERROR_EOF},
    {"\n\t\t\tError: Key match\n", ERROR_KEY_MATCH},
    {"\n\t\t\tError: Parent not found.\n", ERROR_PARENT_NOT_FOUND},
    {"\n\t\t\tError: Key not found.\n", ERROR_KEY_NOT_FOUND},
    {"\n\t\t\tError: Invalid read of unsigned\n", ERROR_NOT_UNSIGNED},
    {"\n\t\t\tError: File opening failed\n", ERROR_OPENING_FILE}
};


void greeting(){
    printf(" ______________________________\n");
    printf("|Enter a number from 1 to 6    |\n");
    printf("|1. Insert                     |\n");
    printf("|2. Delete                     |\n");
    printf("|3. Search                     |\n");
    printf("|4. Output                     |\n");
    printf("|5. Import from binary         |\n");
    printf("|6. Export to binary           |\n");
    printf(" ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n\n>>");
}



ErrorCode get_int(int* intx){
	int scanf_value = 0;
	int symbol = 0;
	int cycle = 1;
	int tryagain = 0;
	long long temp = 0;
	while(scanf_value != 1  ||  cycle != 0){ 
		temp = 0;
		cycle = 0;
		if (tryagain == 1)
			printf("Try again: ");
		scanf_value = scanf("%lld", &temp);
		
		if (scanf_value == EOF && symbol != EOF){//EOF
            printf("\n\t\t\tError: EOF\n"); cycle = -2; return ERROR_EOF;
            }
		if (temp <= INT_MAX && temp >= INT_MIN){
			*intx = (int)temp;

			while ((symbol = getchar()) != '\n' && symbol != EOF){
        		if((symbol < '0' || symbol > '9') && symbol != -1){
        			cycle = 1;
				}
        	}
			if (cycle == 1) printf("\n\t\t\tError: Incorrect input\n");
		}
		else if (temp > INT_MAX || temp < INT_MIN) {
            printf("\n\t\t\tError: Input exceeds INT range\n");
            cycle = 5;
		}
        else if (scanf_value == 0 || cycle != 0){//некорректный ввод 2147483647
            printf("\n\t\t\tError: Incorrect input\n");
        }
		tryagain = 1;       
            
    }
	return SUCCESS;
}



ErrorCode get_unsigned(unsigned *unsx){
	int scanf_value = 0;
	int symbol = 0;
	int cycle = 1;
	int tryagain = 0;
	long long temp = 0;
	while(scanf_value != 1  ||  cycle != 0){ 
		temp = 0;
		cycle = 0;
		if (tryagain == 1)
			printf("Try again: ");
		scanf_value = scanf("%lld", &temp);
		
		if (scanf_value == EOF && symbol != EOF){//EOF
            printf("\n\t\t\tError: EOF\n"); cycle = -2; return ERROR_EOF;
            }
		if (temp <= UINT_MAX && temp >= 0){
			*unsx = (unsigned)temp;

			while ((symbol = getchar()) != '\n' && symbol != EOF){
        		if((symbol < '0' || symbol > '9') && symbol != -1){
        			cycle = 1;
				}
        	}
			if (cycle == 1) printf("\n\t\t\tError: Incorrect input\n");
		}
		else if (temp > UINT_MAX || temp < 0) {
            printf("\n\t\t\tError: Input exceeds UINT range\n");
            cycle = 5;
		}
        else if (scanf_value == 0 || cycle != 0){//некорректный ввод 2147483647
            printf("\n\t\t\tError: Incorrect input\n");
        }
		tryagain = 1;       
            
    }
	return SUCCESS;
}



void errors_printf(ErrorCode error){
    if (error >=0 && error < 7)
        if (error != 6)
            printf("%s", error_storage[error].string);
        else
            perror("\n\t\t\tError: File opening failed: ");
    else
        return;
}


char* input_filename(){
    char* filename = readline("Enter the name of file: ");
    return filename;
}




ErrorCode input_action(int* action){
    while (*action < 1 || *action > 6){
        ErrorCode error = get_int(action);
		if (error == ERROR_EOF) return ERROR_EOF;
        if ((*action < 1 || *action > 6)) printf("\n\t\t\tError: Your number is not in the range from 1 to 6.\nTry again:");
    }
    return SUCCESS;
}




void output_data(const Data* data){
    printf("---Data output---\n");
    for (unsigned i = 0; i < data->size; i++){
        printf("{key = %u | info = %u}\n", data->array[i].key, data->array[i].information);
    }
}

void erase_data(Data* data){
    if (data != NULL){
        free(data->array);
        free(data);
    }
}



ErrorCode not_all(){
    char* word = readline("\nIf you want to finish, enter 'end': ");
    printf("\n");
    if (word == NULL) return ERROR_EOF;
    if (strcmp(word, "end") != 0) {free(word); return ERROR_NOT_ALL;}
    free(word);
    return SUCCESS;
}


ErrorCode entry(Data** data, Data_element element){
    if ((*data)->array == NULL){
        (*data)->array = calloc((*data)->capacity, sizeof(Data_element));
        if ((*data)->array == NULL)
            return ERROR_MEMORY_ALLOCATION;
    }
    if ((*data)->size == (*data)->capacity){
        (*data)->capacity *= 2;
    
        Data_element* ptr = realloc((*data)->array, sizeof(Data_element) * (*data)->capacity);
        if (ptr == NULL)
            return ERROR_MEMORY_ALLOCATION;
        (*data)->array = ptr;
    }
    (*data)->array[(*data)->size].information = element.information;
    (*data)->array[(*data)->size].key = element.key;
    (*data)->size++;
    return SUCCESS;
}