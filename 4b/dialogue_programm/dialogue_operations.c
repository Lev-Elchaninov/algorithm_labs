#include "dialogue.h"

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>

ErrorCode operation_insert(LLRB_tree* tree){
    printf("Enter the key: ");
    size_t key = 0;
    ErrorCode error = get_size_t(&key);
    if (error != SUCCESS)
        return error;


    char* info = readline("Enter the information: ");
    if (info == NULL)
        return ERROR_EOF;

    
    error = func_insert(tree, key, info);
    if (error != SUCCESS){
        free(info);
        return error;
    }
    free(info);
    return SUCCESS;
}


ErrorCode operation_pre_order(LLRB_tree* tree){
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    For_order* pre_order_struct = NULL;
    size_t* max_key = malloc(sizeof(size_t));
    ErrorCode error = SUCCESS;
    char* answer = readline("Do you want to have max_key? (y/n): ");
    fprintf(stdout, "\n");
    if (answer == NULL){
        free(max_key);
        return ERROR_EOF;
    }
    if (strcmp(answer, "n") != 0){
        fprintf(stdout, "Enter the max allowed key: ");
        size_t ptr = 0;
        error = get_size_t(&ptr);
        *max_key = ptr;
        if (error != SUCCESS){
            free(answer);
            free(max_key);
            return error;
        }
    }
    else{
        free(max_key);
        max_key = NULL;
    }
    
    error = func_pre_order(tree, &pre_order_struct, max_key);
    if (error != SUCCESS){
        free(answer);
        free(max_key);
        return error;
    }
    pre_order_output(pre_order_struct, stdout);
    order_array_free(pre_order_struct);
    free(answer);
    free(max_key);
    return SUCCESS;
}


ErrorCode operation_delete(LLRB_tree* tree){
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    size_t key = 0;
    printf("Enter the key: ");
    ErrorCode error = get_size_t(&key);
    if (error != SUCCESS)
        return error;
    error = func_delete(tree, key);
    if (error != SUCCESS)
        return error;
    return SUCCESS;
}




ErrorCode operation_search(LLRB_tree* tree){
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    size_t key = 0;
    Data* data = NULL;
    ErrorCode error = data_init(&data);
    if (error != SUCCESS)
        return error;
    error = ERROR_NOT_ALL;
    while (error != SUCCESS){
        printf("Enter the key: ");
        error = get_size_t(&key);
        if (error != SUCCESS){
            erase_data(data);
            return error;
        }
        Node* required_element = NULL;
        error = func_find_key(tree, key, &required_element);
        if (error == ERROR_EMPTY_TREE){
            erase_data(data);
            return error;
        }
        else if (error == ERROR_KEY_NOT_FOUND)
            errors_printf(error);
        if (error == ERROR_KEY_MATCH){
            Node* ptr = required_element;
            while (ptr != NULL){
                error = entry(&data, ptr);//не библиотечная функция
                ptr = ptr->duplicate_next;
            }
            error = not_all();//срашиваем у пользователя, все ли он ввел
            if (error == ERROR_EOF){
                erase_data(data);
                return error;
            }
        }
    }
    output_data(data);
    erase_data(data);
    return SUCCESS;    
}


ErrorCode operation_search_min_exceeding(LLRB_tree* tree){
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    printf("Enter the key: ");
    size_t key = 0;
    ErrorCode error = get_size_t(&key);
    Node* result = NULL;
    error = func_search_min_exceeding(tree, key, &result);
    if (error != SUCCESS)
        return error;
    Data* founded = NULL;
    error = data_init(&founded);
    if (error != SUCCESS)
        return error;
    Node* ptr = result;
    while (ptr != NULL){
        error = entry(&founded, ptr);//не библиотечная функция
        ptr = ptr->duplicate_next;
    }
    output_data(founded);
    erase_data(founded);
    return SUCCESS;
}



ErrorCode operation_load_from_txt(LLRB_tree** tree){
    char* filename = readline("Enter the filename: ");
    if (filename == NULL)
        return ERROR_EOF;
    ErrorCode error = func_load_from_txt(tree, filename);
    if (error != SUCCESS){
        free(filename);
        return error;
    }
    free(filename);
    return SUCCESS;
}


ErrorCode operation_output_to_graphviz(LLRB_tree* tree){
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    ErrorCode error = func_output_to_graphviz(tree);
    if (error != SUCCESS)
        return error;
    return SUCCESS;
}