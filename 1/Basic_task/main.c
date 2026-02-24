#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "lib.h"

int main(){
    Matrix matr = {0, NULL};
    int nulls = 0, err = 0;
    int err1 = input(&matr, &nulls);
    if (err1 == 1){
        return 1;
    }
    else if (err1 == 2){
        printf("----");
        erase(&matr);
        return 2;
    }
    Matrix newmatr = {0, NULL};
    err = task(&newmatr, &matr, &nulls);
    if (err == 1)
        return 1;
    if (err == 0 && err1 != 3){
        printf ("-Old matrix\n");
        output(&matr);
        printf("\n");
        printf("-New matrix");
        if (nulls != 0) 
            printf("\t(%d zero lines were removed)\n", nulls);
        else
            printf("\n");
        output(&newmatr);
        printf("\n");
    }
    erase(&matr);
    erase(&newmatr);
    
    return 0;
}



