#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <readline/readline.h>
#include "dialogue.h"

Error_storage error_storage[] = {
    {"\n\t\t\tError: Tree is empty\n", ERROR_EMPTY_TREE},
    {"\n\t\t\tError: EOF\n", ERROR_EOF},
    {"\n\t\t\tError: Key match\n", ERROR_KEY_MATCH},
    {"\n\t\t\tError: Parent not found.\n", ERROR_PARENT_NOT_FOUND},
    {"\n\t\t\tError: Key not found.\n", ERROR_KEY_NOT_FOUND},
    {"\n\t\t\tError: Invalid read of size_t\n", ERROR_NOT_SIZE_T},
    {"\n\t\t\tError: File opening failed\n", ERROR_OPENING_FILE},
    {"\n\t\t\tError: Pre-order is empty\n", ERROR_EMPTY_ORDER}
};


void greeting(){
    printf(" ______________________________\n") ;
    printf("|Enter a number from 1 to 8    |\n");
    printf("|1. Insert                     |\n");
    printf("|2. Delete                     |\n");
    printf("|3. Pre-order in range         |\n");
    printf("|4. Output                     |\n");
    printf("|5. Find key                   |\n");
    printf("|6. Find the farest key        |\n");
    printf("|7. Load tree from txt         |\n");
    printf("|8. Graphviz output            |\n");
    printf(" ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n\n>>");
}


ErrorCode get_size_t(size_t *unsx){
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
            cycle = -2; return ERROR_EOF;
            }
		if (temp >= 0 && (size_t)temp <= __SIZE_MAX__){
			*unsx = (size_t)temp;

			while ((symbol = getchar()) != '\n' && symbol != EOF){
        		if((symbol < '0' || symbol > '9') && symbol != -1){
        			cycle = 1;
				}
        	}
			if (cycle == 1) printf("\n\t\t\tError: Incorrect input\n");
		}
		else if (temp < 0 || (size_t)temp > __SIZE_MAX__) {
            printf("\n\t\t\tError: Input exceeds size_t range\n");
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
    if (error >= 0 && error < 8)
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




ErrorCode input_action(size_t* action){
    while (*action < 1 || *action > 8){
        ErrorCode error = get_size_t(action);
		if (error == ERROR_EOF) return ERROR_EOF;
        if ((*action < 1 || *action > 8)) printf("\n\t\t\tError: Your number is not in the range from 1 to 8.\nTry again:");
    }
    return SUCCESS;
}


void pre_order_output(For_order* pre_order_struct, FILE* file){
    for (unsigned i = 0; i < pre_order_struct->capacity; i++){
        fprintf(file, "{%zu | %s}\n", pre_order_struct->array[i].key, pre_order_struct->array[i].info);
    }
    printf("\n\n");
}



void output_data(const Data* data){
    printf("|---Data output---|\n");
    for (size_t i = 0; i < data->size; i++){
        printf("{key = %zu | info = %s}\n", data->storage[i]->info_fieldes.key, data->storage[i]->info_fieldes.info);
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


ErrorCode entry(Data** data, Node* current){
    if ((*data)->storage == NULL){
        (*data)->storage = calloc((*data)->capacity, sizeof(Node*));
        if ((*data)->storage == NULL)
            return ERROR_MEMORY_ALLOCATION;
    }
    if ((*data)->size == (*data)->capacity){
        (*data)->capacity *= 2;

        Node** ptr = realloc((*data)->storage, sizeof(Node*) * (*data)->capacity);
        if (*ptr == NULL)
            return ERROR_MEMORY_ALLOCATION;
        (*data)->storage = ptr;
    }
    (*data)->storage[(*data)->size] = current;
    (*data)->size++;
    return SUCCESS;
}

