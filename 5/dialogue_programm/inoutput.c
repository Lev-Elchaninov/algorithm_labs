#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <readline/readline.h>
#include "dialogue.h"

Error_storage error_storage[] = {
    {"\n\t\t\tError: Graph is empty\n", ERROR_EMPTY_GRAPH},
    {"\n\t\t\tError: EOF\n", ERROR_EOF},
    {"\n\t\t\tError: Vertex with this hostname already exists\n", ERROR_VERTEX_EXIST},
    {"\n\t\t\tError: Unknown vertex.\n", ERROR_UNKNOWN_VERTEX},
    {"\n\t\t\tError: Invalid read of size_t\n", ERROR_NOT_SIZE_T},
    {"\n\t\t\tError: File opening failed\n", ERROR_OPENING_FILE},
    {"\n\t\t\tError: Edge info is empty\n", ERROR_EMPTY_EDGE},
    {"\n\t\t\tError: Hostname = NULL\n", ERROR_NULL_HOST},
    {"\n\t\t\tError: Edge already exists\n", ERROR_EDGE_EXIST},
    {"\n\t\t\tError: Unknown edge\n", ERROR_UNKNOWN_EDGE},
    {"\n\t\t\tError: No path\n", ERROR_NO_PATH},
    {"\n\t\t\tError: Cant build MST\n", ERROR_CANT_BUILD_MST}
};


void greeting(){
    printf(" ______________________________\n") ;
    printf("|Enter a number from 1 to 11   |\n");
    printf("|1. Add vertex                 |\n");
    printf("|2. Add edge                   |\n");
    printf("|3. Delete vertex              |\n");
    printf("|4. Delete edge                |\n");
    printf("|5. Change vertex data         |\n");
    printf("|6. Change edge data           |\n");
    printf("|7. Adjacency list output      |\n");
    printf("|8. Graphviz output            |\n");
    printf("|9. DFS                        |\n");
    printf("|10. Shortest_Path             |\n");
    printf("|11. MST                       |\n");
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
    if (error >= 0 && error < 12)
        if (error != 5)
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
    while (*action < 1 || *action > 11){
        ErrorCode error = get_size_t(action);
		if (error == ERROR_EOF) return ERROR_EOF;
        if ((*action < 1 || *action > 11)) printf("\n\t\t\tError: Your number is not in the range from 1 to 11.\nTry again:");
    }
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


ErrorCode entry(size_t** array, size_t* size, size_t* capacity, size_t port){
    if (*array == NULL){
        *capacity = 5;
        *size = 0;
        *array = malloc(*capacity * sizeof(size_t));
        if (*array == NULL)
            return ERROR_MEMORY_ALLOCATION;
    }
    if (*size == *capacity){
        *capacity *= 2;
        size_t* ptr = realloc(*array, *capacity * sizeof(size_t));
        if (ptr == NULL)
            return ERROR_MEMORY_ALLOCATION;
        *array = ptr;
    }
    (*array)[*size] = port;
    return SUCCESS;
}




ErrorCode ports_entring(size_t** array, size_t* size){
    ErrorCode error = ERROR_NOT_ALL;
    *size = 0;
    size_t capacity = 0;
    while (error != SUCCESS){
        size_t port = 0;
        fprintf(stdout, "Enter the port %zu: ", (*size) + 1);
        error = get_size_t(&port);
        if (error != SUCCESS){
            free(*array);
            *array = NULL;
            return error;
        }
        if (error == SUCCESS)
            error = ERROR_NOT_ALL;
        if (error == ERROR_NOT_ALL){
            error = entry(array, size, &capacity, port);//не библиотечная функция
            if (error != SUCCESS){
                free(*array);
                *array = NULL;
                return error;
            }
            if (error == SUCCESS)
                error = ERROR_NOT_ALL;
            error = not_all();//срашиваем у пользователя, все ли он ввел
            if (error == ERROR_EOF){
                free(*array);
                *array = NULL;
                return error;
            }
            (*size)++;
        }
    }
    return SUCCESS;
}


void output_bool_array(const Graph* graph, DFS* current){
    fprintf(stdout, "\nAll valid hostnames:\n");
    for (size_t i = 0; i < graph->size; i++){
        if (current[i].achievable == true)
            fprintf(stdout, "%s (port %zu)\n", current[i].vertex->hostname, current[i].vertex->port);
    }
}


void output_path(const size_t size, char** path){
    fprintf(stdout, "\nLength = %zu\n%s", size, path[0]);
    for (size_t i = 1; i < size; i++){
        fprintf(stdout, " -> %s", path[i]);
    }
    fprintf(stdout, "\n");
}