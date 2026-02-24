#include <stdlib.h>
#include <stdio.h>
#include "lib.h"


void minmax(int* begin, int* end, Border* border){
    if (border->indexneg < border->indexpos)
        {*begin = border->indexneg; *end = border->indexpos;}
    else
        {*begin = border->indexpos; *end = border->indexneg;}
}



int createline(Matrix* matr, Matrix* newmatr, int line, Border* border, int n){
    int begin = 0; int end = 0;
    minmax(&begin, &end, border);
    newmatr->stroki[line].n = (end - begin) + 1;
    if(((end - begin) + 1) > 0){
        newmatr->stroki[line].arr = malloc(newmatr->stroki[line].n * sizeof(int));
        int k = begin;
        if (newmatr->stroki[line].arr == NULL)
		    {printf("Memory allocation faled\n"); free(newmatr->stroki[line].arr); return 1;}

        for (unsigned i = 0; i < newmatr->stroki[line].n; i++){
            newmatr->stroki[line].arr[i] = matr->stroki[n].arr[k];
            k++;
        }
    }
    return 0;
}




int task(Matrix* newmatr, Matrix* matr, int* nulls){
    Border* border = malloc(1 * sizeof(Border));
    if (border == NULL)
        {free(border); return 1;}
    newmatr->lines = matr->lines - *nulls;
    int line = 0;
    int err = 0;
    newmatr->stroki = malloc((matr->lines - *nulls) * sizeof(Line));
    if (newmatr->stroki == NULL)
		    {printf("Memory allocation faled\n"); free(newmatr->stroki); return 1;}
    for (unsigned i = 0; i < matr->lines; i++){
        border->pos = 0; border->neg = 0; border->indexneg = 0; border->indexpos = 0;
        for (unsigned j = 0; j < matr->stroki[i].n; j++){
            if (matr->stroki[i].arr[j] < 0 && border->neg == 0){
                border->neg = matr->stroki[i].arr[j];
                border->indexneg = j;
            }
            if (matr->stroki[i].arr[j] > 0 && border->pos == 0){
                border->pos = matr->stroki[i].arr[j];
                border->indexpos  = j;
            }
            if (border->pos != 0 && border->neg != 0){
                err = createline(matr, newmatr, line, border, i);
                if (err == 1){
                    extra_erase(newmatr);
                    return 1;
                }
                line++;
                j = matr->stroki[i].n;
            }
            if (((border->pos != 0 && border->neg == 0) || (border->pos == 0 && border->neg != 0)) && (j == matr->stroki[i].n - 1)){
                border->indexpos = 0;
                border->indexneg = j;
                err = createline(matr, newmatr, line, border, i);
                if (err == 1){
                    extra_erase(newmatr);
                    return 1;
                }
                line++;
            }
        }
    }
    free(border);
    return 0;
}

void extra_erase(Matrix* example){
    for (unsigned i = 0; i < example->lines; i++){
        free(example->stroki[i].arr);
    }
    free(example);
}