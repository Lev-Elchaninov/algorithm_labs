#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <readline/readline.h>
#include "talk.h"

Error_storage error_storage[] = {
    {"\n\t\t\tError: Table is empty\n", ERROR_EMPTY_TABLE},
    {"\n\t\t\tError: EOF\n", ERROR_EOF},
    {"\n\t\t\tError: Key match\n", ERROR_KEY_MATCH},
    {"\n\t\t\tError: Parent not found.\n", ERROR_PARENT_NOT_FOUND},
    {"\n\t\t\tError: Key not found.\n", ERROR_KEY_NOT_FOUND},
    {"\n\t\t\tError: Invalid read of unsigned\n", ERROR_NOT_UNSIGNED},
    {"\n\t\t\tError: File opening failed\n", ERROR_OPENING_FILE},
    {"\n\t\t\tError: Forbidden key\n", ERROR_FORBIDDEN_KEY},
};


void greeting(){
	printf(" ______________________________\n");
    printf("|Enter a number from 1 to 7    |\n");
    printf("|1. Insert                     |\n");
    printf("|2. Delete                     |\n");
    printf("|3. Search                     |\n");
    printf("|4. Output                     |\n");
    printf("|5. Import from file           |\n");
    printf("|6. Delete (individual)        |\n");
	printf("|7. Search (individual)        |\n");
	printf(" ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n\n>>");
}


ErrorCode input_action(int* action){
    while (*action < 1 || *action > 7){
        ErrorCode error = getint(action);
		if (error == ERROR_EOF) return ERROR_EOF;
        if ((*action < 1 || *action > 7)) printf("\n\t\t\tError: Your number is not in the range from 1 to 7.\nTry again:");
    }
    return SUCCESS;
}

ErrorCode getint(int* intx){
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




ErrorCode get_unsigned(int *intx){
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
		printf("--");
		if (scanf_value == EOF && symbol != EOF){//EOF
            cycle = -2; return ERROR_EOF;
            }
		if (temp <= INT_MAX && temp >= INT_MIN){
			*intx = (int)temp;
			if ((*intx) * (-1) > 0){
                printf("\n\t\t\tError: A number less than zero\n");
				cycle = -1;
            }
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
            printf("\t\t\t\tError: Incorrect input\n");
        }
		tryagain = 1;       
            
    }
	printf("\t\t\t\tSuccessful input\n\n"); 
	return SUCCESS;
}



void errors_printf(ErrorCode error){
    if (error >=0 && error < 8)
        if (error != 6)
            printf("%s", error_storage[error].string);
        else
            perror("\n\t\t\tError: File opening failed: ");
    else
        return;
}



ErrorCode key_input(char** key){
    *key = readline("Enter the key: "); 
    if (*key == NULL) return ERROR_EOF;
    return SUCCESS;
}


ErrorCode par_input(const Table* table, KeySpace** current){
    ErrorCode error = ERROR_PARENT_NOT_FOUND;
    while (error != SUCCESS){
        (*current)->par = readline("Enter the parent (if no parent, print 'null'): ");
        if ((*current)->par == NULL) {error = ERROR_EOF; return error; }
       
        error = parent_check(table, (*current)->par);
        if(error == ERROR_PARENT_NOT_FOUND) {free((*current)->par); errors_printf(error);}
    }
    return SUCCESS;
}


ErrorCode info_input(KeySpace* current){
    unsigned* current_info = calloc(1, sizeof(unsigned)); if (current_info == NULL) return ERROR_MEMORY_ALLOCATION;

    printf("Enter the information: ");
    int information = 0;
    ErrorCode error = get_unsigned(&information); if (error != SUCCESS) {free(current_info); return error;}
    *current_info = (unsigned)information;
    current->information = current_info;
    return SUCCESS;
}



ErrorCode element_entry(const Table* table, KeySpace* current){
    ErrorCode error = key_input(&current->key);
    if (error != SUCCESS) {free(current->key); return error;}

    error = individuality(table, current->key);
    if (error != SUCCESS) {errors_printf(error); free(current->key); free(current); return error;}

    error = par_input(table, &current);
    if (error != SUCCESS) {free(current->key); free(current->par); return error;}

    error = info_input(current);
    if (error != SUCCESS) 
    {free(current->key); free(current->par); return error;}//тут кондишинал джамп

    return SUCCESS;
}


ErrorCode not_all(){
    char* word = readline("\nIf you want to finish, enter 'end': ");
    printf("\n");
    if (word == NULL) return ERROR_EOF;
    if (strcmp(word, "end") != 0) {free(word); return ERROR_NOT_ALL;}
    free(word);
    return SUCCESS;
}


char* input_filename(){
    char* filename = readline("Enter the name of file: ");
    return filename;
}


ErrorCode borders_input(char** border_1, char** border_2){
    *border_1 = readline("Input 1st border line: "); if (*border_1 == NULL) return ERROR_EOF;
    *border_2 = readline("Input 2nd border line: "); if (*border_2 == NULL) {free(*border_1); return ERROR_EOF;}  
    char* buffer = *border_1;
    if (strcmp(*border_1, *border_2) >= 0) {*border_1 = *border_2; *border_2 = buffer;}
    return SUCCESS;
}




void out_data(const Data* data){
    if (data == NULL) return;
    printf("\n--Data output--\n");
    for (unsigned i = 0; i < data->size; i++){
        printf("[key = %s, info = %u]\n", data->storage[i].key, *data->storage[i].information);
    }
    printf("\n");
}




void erase_data(Data* data){//тут все норм
    if (data == NULL) return;
    for (unsigned i = 0; i < data->size; i++){
        free(data->storage[i].information);
        free(data->storage[i].key);
    }
    free(data->storage);
    free(data);
}



ErrorCode entry(Data** data, const KeySpace* required_key){
    if ((*data) == NULL){
        (*data) = malloc(sizeof(Data)); if ((*data) == NULL) return ERROR_MEMORY_ALLOCATION;
        (*data)->capacity = N;
        (*data)->size = 0;
        (*data)->storage = malloc((*data)->capacity * sizeof(Element)); if ((*data)->storage == NULL) return ERROR_MEMORY_ALLOCATION;
    }
    if ((*data)->size == (*data)->capacity){
        (*data)->capacity += N;
        Element* ptr = realloc((*data)->storage, (*data)->capacity * sizeof(Element));
        if (ptr == NULL)
            return ERROR_MEMORY_ALLOCATION;
        (*data)->storage = ptr;
        
    }
   
    (*data)->storage[(*data)->size].information = malloc(sizeof(unsigned)); 
    if ((*data)->storage[(*data)->size].information == NULL) {return ERROR_MEMORY_ALLOCATION;};
   
    
    (*data)->storage[(*data)->size].key = malloc((strlen(required_key->key)+1) * sizeof(char)); 
    if ((*data)->storage[(*data)->size].key == NULL) {return ERROR_MEMORY_ALLOCATION;}
    strcpy((*data)->storage[(*data)->size].key, required_key->key);
 
    *(*data)->storage[(*data)->size].information = *required_key->information;
    (*data)->size++;
    return SUCCESS;
}


ErrorCode empty_table_check(const Table* table){
    if (table->keyspace == NULL) 
        return ERROR_EMPTY_TABLE;
    return SUCCESS;
}

