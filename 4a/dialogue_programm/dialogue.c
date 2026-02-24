#include "dialogue.h"

#include <stdlib.h>
#include <stdio.h>


ErrorCode dialogue(Bin_tree* tree){
    size_t action = 0;
    ErrorCode error = SUCCESS;
    while (error != ERROR_EOF){
        greeting();
        action = 0;
        error = input_action(&action); 
        if (error != SUCCESS){
            errors_printf(error); goto error_container;
        }
        switch(action){
            case(1):
                error = operation_insert(tree);//проверено на утечки
                if (error != SUCCESS){
                    errors_printf(error);
                    goto error_container;
                }
                break;
            case(2)://проверено на утечки
                error = operation_delete(tree);
                if (error == ERROR_EOF)
                    errors_printf(error);
                if (error == ERROR_KEY_NOT_FOUND || error == ERROR_EMPTY_TREE)
                    errors_printf(error);   
                else if(error != SUCCESS)
                    goto error_container;
                break;
            case(3)://pre-order в диапазоне, проверено на утечки
                error = operation_pre_order(tree);
                if (error == ERROR_EOF)
                    errors_printf(error);
                if (error != SUCCESS && error != ERROR_EMPTY_TREE && error != ERROR_EMPTY_ORDER)
                    goto error_container;
                else
                    errors_printf(error);
                break;
            
            case(4)://вывод в stdout, проверено на утечки
                error = func_output(tree, stdout); 
                if (error == ERROR_EMPTY_TREE) 
                    errors_printf(error);
                else if (error != SUCCESS)
                    goto error_container;
                break;
            
            case(5)://поиск, проверено на утечки
               error = operation_search(tree);
                if (error == ERROR_EOF)
                    errors_printf(error);
                if (error == ERROR_EOF || error == ERROR_MEMORY_ALLOCATION)
                    goto error_container;
                else if(error != SUCCESS)
                    errors_printf(error);
                break;
            
            case(6)://поиск максимально отдаленного, проверено на утечки
                error = operation_find_most_dif_value(tree);
                if (error != SUCCESS && error != ERROR_EMPTY_TREE && error != ERROR_KEY_NOT_FOUND)
                    goto error_container;
                else if (error != SUCCESS)
                    errors_printf(error);
                break; 
            case(7)://проверено на утечки
                error = operation_load_from_txt(&tree);
                if (error != SUCCESS && error != ERROR_EOF && error != ERROR_OPENING_FILE && error != ERROR_NOT_SIZE_T)
                    goto error_container;
                else
                    errors_printf(error);
                break;
            case(8)://проверено на утечки
                error = operation_output_to_graphviz(tree);
                if (error == ERROR_EMPTY_TREE || error == ERROR_OPENING_FILE) 
                    errors_printf(error);
                else if (error != SUCCESS)
                    goto error_container;
                break;
        }
    }
    error_container:
        switch(error){
            case(ERROR_EOF):
            case(ERROR_MEMORY_ALLOCATION):
            case(ERROR_NOT_SIZE_T):
                erase_tree(tree);
                return error;
            default:
                break;
        }
    erase_tree(tree);
    return SUCCESS;
}
