#include <stdio.h>
#include <stdlib.h>
#include "dialogue.h"

int main(){
    ErrorCode error = SUCCESS;
    Graph* graph = NULL;
    error = init_graph(&graph);
    if (error != SUCCESS)
        return error;
    error = dialogue(graph);
    if (error != SUCCESS)
        goto error_container;


    error_container:
        switch(error){
            default: break;
        }
    return SUCCESS;
}