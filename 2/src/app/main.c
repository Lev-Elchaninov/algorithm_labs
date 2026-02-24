#include "applied.h"


#include <stdlib.h>
#include <stdio.h>


int main(){
    int err = 0;
    Info* information = NULL;
    while (1){
        information = malloc(sizeof(Info)); if (information == NULL) {printf("Allocation failed\n"); return 1;}
        err = input(information); if (err != 0) goto error_container;
        output(information);
        err = task(information); if (err != 0) goto error_container;
        free_array(information);
        free(information);
    }
    error_container:
        switch(err){
            case 1:
                printf("Allocation failed\n");
                free(information);
                return 1;
            case 3:
                printf("End of file\n");
                free(information);
                return 3;
            case 11:
                printf("All queues are overflowed\n");
                free_array(information);
                free(information);
                return 11;
        }
    free_array(information);
    free(information);
    return 0;
}


