#include "dialogue.h"

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>

ErrorCode operation_insert(Bin_tree* tree){
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


ErrorCode operation_pre_order(Bin_tree* tree){
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    For_order* pre_order_struct = NULL;
    size_t left_board = 0;
    size_t right_board = 0;
    printf("Enter left board: ");
    ErrorCode error = get_size_t(&left_board);
    if (error != SUCCESS)
        return error;
    printf("Enter right board: ");
    error = get_size_t(&right_board);
    if (error != SUCCESS)
        return error;
    error = func_pre_order_in_range(tree, left_board, right_board, &pre_order_struct);
    if (error != SUCCESS)
        return error;
    
    pre_order_output(pre_order_struct, stdout);
    order_array_free(pre_order_struct);
    return SUCCESS;
}

ErrorCode operation_delete(Bin_tree* tree){
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




ErrorCode operation_search(Bin_tree* tree){
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
            error = entry(&data, required_element);//не библиотечная функция
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


ErrorCode operation_find_most_dif_value(Bin_tree* tree){
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    printf("Enter the key: ");
    size_t key = 0;
    ErrorCode error = get_size_t(&key);
    Data* data = NULL;
    error = func_find_most_dif_value(tree, key, &data);
    if (error != SUCCESS){
        return error;
    }
    output_data(data);
    erase_data(data);
    return SUCCESS;
}


ErrorCode operation_load_from_txt(Bin_tree** tree){
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


ErrorCode operation_output_to_graphviz(Bin_tree* tree){
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    ErrorCode error = func_output_to_graphviz(tree);
    if (error != SUCCESS)
        return error;
    return SUCCESS;
}