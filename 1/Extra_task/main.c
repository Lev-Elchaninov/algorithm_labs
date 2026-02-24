#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "lib.h"

int main(){
    Matrix matr = {0, NULL};
    int nulls = 0;
    int err = 0;
    err = start(&matr);//задание файлов
    if (err == 1)
        return 1;
    
    err = linesinfo(&matr);//узнать информацию о строках (смещения и длины)
    if (err == 1)
        return 1;

    printf("\n-Old matrix\n");
    err = output(&matr);//вывод старой матрицы на экран
    if (err == 1)
        return 1;
    
    err = typelines(&matr);//узнать, какого типа строки
    if (err == 1)
        return 1;
    
    err = sizeofnewlines(&matr, &nulls);//пишем размеры каждой новой строки
    if (err == 1)
        return 1;
    

    err = writenewinfo(&matr, &nulls);//пишем в бинарь инфо о новых стоках (смещение + длина)
    if (err == 1)
        return 1;


   return 0;
}



