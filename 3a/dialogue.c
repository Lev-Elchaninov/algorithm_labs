#include "talk.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>



ErrorCode dialogue(Table* table){
    int action = 0;
    ErrorCode error = SUCCESS;
    while (error != ERROR_EOF){
        greeting();
        action = 0;
        error = input_action(&action); if (error != SUCCESS) goto error_container;
        switch(action){
            case(1):
                error = operation_input(table);
                if (error != SUCCESS)
                    goto error_container;
                break;

            case(2):
                error = operation_delete(table);
                if (error != SUCCESS && error != ERROR_EMPTY_TABLE)
                    goto error_container;
                break;

            case(3):
                error = operation_search(table);
                if (error != SUCCESS && error != ERROR_EMPTY_TABLE)
                    goto error_container;
                break;
            
            case(4):
                error = output(table, stdout); 
                if (error == ERROR_EMPTY_TABLE) errors_printf(error);
                break;

            case(5):
                error = operation_file_input(table);
                if (error != SUCCESS)
                    goto error_container;
                break;

            case(6):
                error = operation_delete_elem_and_kids_null(table);
                if (error != SUCCESS && error != ERROR_EMPTY_TABLE)
                    goto error_container;
                break;
                
            case(7):
                error = operation_search_in_range(table);
                if (error != SUCCESS && error != ERROR_EMPTY_TABLE)
                    goto error_container;
                break;
            }
    }
    error_container:
        switch(error){
            case(ERROR_EOF):
            case(ERROR_NULL_POINTER):
            case(ERROR_MEMORY_ALLOCATION):
            case(ERROR_NOT_UNSIGNED):
                erase_table(table);
                return error;
            default:
                break;
        }
    return SUCCESS;
}
