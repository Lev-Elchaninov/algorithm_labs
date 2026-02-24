#include "talk.h"

#include <stdio.h>
#include <stdlib.h>
int main(){
    ErrorCode error = SUCCESS;
    Table* table = NULL;
    error = init_table(&table);
    if (error != SUCCESS)
        return error;
    
    table->keyspace = NULL;
    error = dialogue(table);
    if (error != SUCCESS)  goto error_container;
    error_container:
        switch(error){
            case(ERROR_EOF):
            case(ERROR_MEMORY_ALLOCATION):
            case(ERROR_NULL_POINTER):
            case(ERROR_NOT_UNSIGNED):
                free(table);
                return error;
            
            default:
                break;
        }
    free(table);
    return 0;
}