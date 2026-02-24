#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../dialogue_programm/library.h"
#include <readline/readline.h>
#include <string.h>


ErrorCode task(LLRB_tree*);
ErrorCode read_from_file(LLRB_tree*, FILE*, char*);
ErrorCode get_size_t(size_t*);
void output(char**, int, size_t);
ErrorCode infinite_cycle(LLRB_tree*, char*);
ErrorCode if_key_exists(size_t, Node*);



ErrorCode read_from_file(LLRB_tree* tree, FILE* file, char* filename){
    char line[1024];
    int line_number = 0;
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        char* token = strtok(line, ",");
        int position_number = 0;

        while (token) {
            position_number++;
            char* endptr;
            size_t key = strtoul(token, &endptr, 10);

            if (endptr != token) {
                // Вставка в дерево
                char info_str[256];
                snprintf(info_str, sizeof(info_str), "%s:%d:%d", filename, line_number, position_number);

                ErrorCode error = func_insert(tree, key, info_str);
                if (error != SUCCESS) {
                    fclose(file);
                    return error;
                }
            }

            token = strtok(NULL, ",");
        }
    }
    return SUCCESS;
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


void output(char** result, int size, size_t key){
    fprintf(stdout, "key = %zu\n", key);
    for (int i = 0; i < size; i++)
        fprintf(stdout, "%s\n", result[i]);
}

ErrorCode if_key_exists(size_t key, Node* current){
    int size = 10, i = 0;
    char** result = malloc(sizeof(char*) * size);
    if (result == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }
    Node* ptr = current;
    while (ptr != NULL) {
        if (i >= size - 1) {
            size *= 2;
            char** new_result = realloc(result, sizeof(char*) * size);
            if (!new_result) {
                for (int j = 0; j < i; j++) free(result[j]);
                free(result);
                return ERROR_MEMORY_ALLOCATION;
            }
            result = new_result;
        }
        result[i++] = strdup(ptr->info_fieldes.info);
        ptr = ptr->duplicate_next;
    }
    output(result, i, key);
    for (int j = 0; j < i; j++) free(result[j]);
        free(result);
    return SUCCESS;
}




ErrorCode infinite_cycle(LLRB_tree* tree, char* filename){
    while (1) {
        fprintf(stdout, "Enter the key to find: ");
        size_t key = 0;
        ErrorCode error = get_size_t(&key);
        if (error != SUCCESS) {
            free(filename);
            return error;
        }

        Node* current = NULL;
        error = func_find_key(tree, key, &current);
        if (error == ERROR_KEY_NOT_FOUND) {
            printf("Key %zu not found.\n", key);
        }
        else if (error == ERROR_KEY_MATCH) {
            error = if_key_exists(key, current);
            if (error != SUCCESS){
                free(filename);
                return error;
            }
        }
        char* end = readline("Do you want to continue, (y/n): ");
        if (end == NULL || strcmp(end, "n") == 0){
            if (end != NULL){
                free(end);
                return SUCCESS;
            }
            else{
                free(filename);
                return ERROR_EOF;
            }
        }
        free(end);
    }
    return SUCCESS;
}


ErrorCode task(LLRB_tree* tree) {
    char* filename = readline("Enter the name of file: ");
    if (filename == NULL) return ERROR_MEMORY_ALLOCATION;

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        free(filename);
        perror("Error opening file");
        return ERROR_OPENING_FILE;
    }

    ErrorCode error = read_from_file(tree, file, filename);
    fclose(file);
    if (error != SUCCESS) {
        free(filename);
        return error;
    }
    error = infinite_cycle(tree, filename);
    if (error != SUCCESS)
        return error;
    free(filename);
    return SUCCESS;
}



int main(){
    LLRB_tree* tree = NULL;
    ErrorCode error = init_tree(&tree);
    if (error != SUCCESS){
        erase_tree(tree);
        return error;
    }
    task(tree);
    erase_tree(tree);
    return SUCCESS;
}



