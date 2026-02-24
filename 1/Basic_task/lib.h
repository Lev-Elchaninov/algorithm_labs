#ifndef LIB_H
#define LIB_H

typedef struct Line{//строка матрицы
    unsigned int n;
    int* arr;
}Line;

typedef struct Matrix{//сама матрица
    unsigned int lines;
    Line* stroki;
}Matrix;

typedef struct Border{
    int indexpos;
    int pos;
    int indexneg;   
    int neg;
}Border;

int getint_for_elements(int*);//ввод целого положительного числа для m, n
int getint_for_sizes(int*);//ввод целого положительного числа для m, n
int input(Matrix*, int*);//заполнение матрицы
void output(const Matrix*);//вывод матрицы
void erase(Matrix*);//освобождение памяти под матрицу
int task(Matrix*, Matrix*, int*);//индивиидуальное задание
int createline(Matrix*, Matrix*, int, Border*, int);//создание линии
void minmax(int*, int*, Border*);//поиск максимального и минимального элемента
void extra_erase(Matrix*);
int getint(int*);

#endif