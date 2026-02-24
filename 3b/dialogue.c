#include "dialogue.h"

#include <stdlib.h>
#include <stdio.h>


ErrorCode dialogue(Table* table){
    int action = 0;
    ErrorCode error = SUCCESS;
    while (error != ERROR_EOF){
        greeting();
        action = 0;
        error = input_action(&action); if (error != SUCCESS) goto error_container;
        switch(action){
            case(1)://сделано
                error = operation_input(&table);
                if (error != SUCCESS && error != ERROR_KEY_MATCH){
                    goto error_container;
                }
                else
                    errors_printf(error);
                break;
            case(2):
                error = operation_delete(table);
                if (error == ERROR_KEY_NOT_FOUND || error == ERROR_EMPTY_TABLE)
                    errors_printf(error);   
                else if(error != SUCCESS)
                    goto error_container;
                break;

            case(3):
                error = operation_search(table);
                if (error == ERROR_KEY_NOT_FOUND || error == ERROR_EMPTY_TABLE)
                    errors_printf(error);
                else if (error != SUCCESS)
                    goto error_container;
                break;
            
            case(4)://сделано
                error = func_output(table, stdout); 
                if (error == ERROR_EMPTY_TABLE) errors_printf(error);
                break;

            case(5)://сделано
               error = operation_input_bin(&table);
               errors_printf(error);
                if (error != SUCCESS && error != ERROR_OPENING_FILE){
                    goto error_container;
                }
                break;
            case(6)://сделано
                error = operation_output_bin(table);
                errors_printf(error);
                if (error != SUCCESS && error != ERROR_OPENING_FILE)
                    goto error_container;
                break;               
            }
    }
    error_container:
        switch(error){
            case(ERROR_EOF):
            case(ERROR_MEMORY_ALLOCATION):
            case(ERROR_NOT_UNSIGNED):
                erase_table(table);
                return error;
            default:
                break;
        }
    return SUCCESS;
}
