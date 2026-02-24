#include "dialogue.h"

#include <stdio.h>
#include <stdlib.h>

int main(){
    ErrorCode error = SUCCESS;
    Table* table = NULL;
    error = init_table(&table);
    if (error != SUCCESS)
        return error;
    error = dialogue(table);
    if (error != SUCCESS)
        goto error_container;


    error_container:
        switch(error){
            case(ERROR_EOF):
            case(ERROR_MEMORY_ALLOCATION):
            case(ERROR_NOT_UNSIGNED):
                return error;
            
            default:
                break;
        }
    
    return SUCCESS;
}