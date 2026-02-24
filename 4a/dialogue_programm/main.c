#include <stdio.h>
#include <stdlib.h>
#include "dialogue.h"

int main(){
    ErrorCode error = SUCCESS;
    Bin_tree* tree = NULL;
    error = init_tree(&tree);
    if (error != SUCCESS)
        return error;
    error = dialogue(tree);
    if (error != SUCCESS)
        goto error_container;


    error_container:
        switch(error){
            default: break;
        }
    return SUCCESS;
}