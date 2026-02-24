#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lib.h"



int input(Matrix* matr, int* nulls){
	printf("-Enter number of lines: ");
	unsigned zero_count = 0;
	int err = 0; int size, n = 0;
	err = getint_for_sizes(&size);
	unsigned int matr_size = (unsigned int)size;
	if (err == 2)
		return 2;
	if (matr_size == 0)
		{printf("Your matrix size = 0\n\n"); return 1;}
	if (matr_size > 0){
		matr->lines = matr_size;
		matr->stroki = malloc(matr->lines * sizeof(Line));//выделили память под массив структур
		if (matr->stroki == NULL)
			{printf("Memory allocation faled\n"); free(matr->stroki); return 1;}
		for(unsigned i = 0; i < matr->lines; i++){
			printf("-Enter size of line %d: ", i+1);
			n = 0;
			err = getint_for_sizes(&n);
			if (err == 2)
				{matr->lines = i; return 2;}
			unsigned int line_size = (unsigned int)n;
			matr->stroki[i].n = line_size;//пишу количество элементов в массиве строки
			int obhod = 0;
			if (matr->stroki[i].n != 0)
				matr->stroki[i].arr = malloc(matr->stroki[i].n * sizeof(int));//выделяю под массив строки память
			else{
				matr->stroki[i].arr = NULL;
				obhod = -5;
			}
			if (matr->stroki[i].arr == NULL && obhod != -5)
				{printf("Memory allocation faled\n"); erase(matr); return 1;}
			for (unsigned j = 0; j < matr->stroki[i].n; j++){
				printf("*Enter element %d of line %d: ", j+1, i+1);
				int elem = 0;
				err = getint_for_elements(&elem);
				if (err== 2)
					{matr->lines = (i+1); return 2;}
				if (elem == 0)
					zero_count++;
				matr->stroki[i].arr[j] = elem;
			}
			if (zero_count == matr->stroki[i].n || matr->stroki[i].n == 0)
				(*nulls)++;
			zero_count = 0;
		}
	}
	if (err != 2)
		return 0;
	else
		return 3;
}

void output(const Matrix* matr){
	for (unsigned i = 0; i < matr->lines; i++){
		printf("%d line: ", i+1);
		if (matr->stroki[i].n != 0){
			for (unsigned j = 0; j < matr->stroki[i].n; j++){
				printf("%d ", matr->stroki[i].arr[j]);
			}
		}
		printf("\n");
	}
}

void erase(Matrix* example){
	for (unsigned i = 0;  i < example->lines; i++){
		free(example->stroki[i].arr);
	}
	free(example->stroki);
}