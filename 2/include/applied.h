#ifndef AP_H
#define AP_H

#include "library.h"


int input(Info*);//функция ввода данных
int correct(char*);//функция проверки на корректность
int is_number(char*);//проверяет на число
int Passanger_count(char*);//считает число пассажиров
int entering(Passanger*, char*, unsigned int*);//занесение информации о пассажирах
int writing_int(char*);//перевод символов в числа
void output(Info*);////вывод очередей
void free_array(Info*);//освобождение
void initialization(Info*);


#endif