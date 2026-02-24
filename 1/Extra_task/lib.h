#ifndef LIB_H
#define LIB_H

typedef struct Line{//строка матрицы
    int offset;
    int size;
    int correct;//поле для типофикации строк
    int newsize;
}Line;


typedef struct Matrix{//сама матрица
    int lines;
    FILE* file;
    Line* stroki;
}Matrix;

typedef struct Border{
    int indexpos;
    int pos;
    int indexneg;
    int neg;
}Border;

int start(Matrix*);//задание файлов
int linesinfo(Matrix*);//считывание информации о строках
int getElement(Matrix*, int*);//считываем элемент
int typelines(Matrix*);//узнать какого типа строки
int sizeofnewlines(Matrix*, int*);//узнаем длинны новых строк
int writenewinfo(Matrix*, int*);//пишем информацию о строках (смещение + длинна)
int outputnew_bin_stdout(Matrix*, FILE*);//запись финальной матрицы в бинарь + на экран
int output(Matrix*);//вывод старой матрицы на экран
void free_matrix(Matrix*);


#endif