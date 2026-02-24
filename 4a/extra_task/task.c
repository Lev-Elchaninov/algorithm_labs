#include <stdlib.h>
#include "../dialogue_programm/library.h"
#include <limits.h>
#include <string.h>
#include <readline/readline.h>

ErrorCode task(Bin_tree* tree, char* input_file){
    if (tree == NULL)
        return ERROR_EMPTY_TREE;
    FILE* file = fopen(input_file, "rb");
    if (file == NULL)
        return ERROR_OPENING_FILE;
    ErrorCode error = SUCCESS;
    ErrorCode trigger = SUCCESS;
    size_t key = 0;
    while (fread(&key, sizeof(size_t), 1, file)){
        Node* ptr = NULL;
        trigger = func_find_key(tree, key, &ptr);
        if (trigger == ERROR_KEY_NOT_FOUND)
            func_insert(tree, key, "1");
        else if (trigger == ERROR_KEY_MATCH){
            char* ptr_str = NULL;
            size_t count = strtoull(ptr->info_fieldes.info, &ptr_str, 10);
            count++;
            char buf[11];
            snprintf(buf, sizeof(buf), "%zu", count);
            free(ptr->info_fieldes.info);
            ptr->info_fieldes.info = NULL;
            ptr->info_fieldes.info = strdup(buf);
        }
    }
    fclose(file);
    For_order* result = NULL;
    error = func_pre_order_in_range(tree, 0, 0, &result);
    Node* zero = NULL;
    if (func_find_key(tree, 0, &zero) == ERROR_KEY_MATCH){
        result->array = realloc(result->array, (result->capacity + 1) * sizeof(Info));
        result->array[result->capacity].info = strdup(zero->info_fieldes.info);
        result->array[result->capacity].key = zero->info_fieldes.key;
        result->capacity++;
    }
    if (error != SUCCESS){
        printf("error = %d\n", error);
        return error;
    }
    char* out_file = readline("Enter output file: ");
    if (out_file == NULL){
        order_array_free(result);
        return ERROR_EOF;
    }
    FILE* output_file = fopen(out_file, "w");
    if (out_file == NULL){
        order_array_free(result);
        fclose(output_file);
        return ERROR_OPENING_FILE;
    }
    for (size_t i = 0; i < result->capacity; i++)
        fprintf(output_file, "%zu --- %s\n", result->array[i].key, result->array[i].info);
    free(out_file);
    order_array_free(result);
    fclose(output_file);
    return SUCCESS;
}