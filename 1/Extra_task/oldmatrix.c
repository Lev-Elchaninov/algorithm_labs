#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "lib.h"



int start(Matrix* matr){//начальные условия
	char fileinput[300];

    printf("Inter the name of file to read(input): ");
	int scanf_value = 0;
	scanf_value = scanf("%255s", fileinput);
	if (scanf_value == EOF){
		printf("EOF\n"); return 1;
	}
	matr->file = fopen(fileinput, "rb");
	while (matr->file == NULL){
		printf("Name of file is incorrect, try again: ");
    	scanf_value = scanf("%255s", fileinput);
		if (scanf_value == EOF){
			printf("EOF\n"); fclose(matr->file); return 1;
		}
		matr->file = fopen(fileinput, "rb");
	}
	

	
	int err = 0;
	err = fread(&matr->lines, sizeof(int), 1, matr->file);
	if (err != 1)
		{printf("Error with reading file\n"); fclose(matr->file); return 1;}
	if (matr->lines == 0)
		{printf("Your matrix size = 0\n\n"); fclose(matr->file); return 1;}
	matr->stroki = malloc(matr->lines * sizeof(Line));
	if (matr->stroki == NULL){
		printf("Memory allocation failed\n");
		return 1;
	}
	return 0;
}



int linesinfo(Matrix* matr){
	int err = 0;
	for (int i = 0; i < matr->lines; i++){//считали информацию по каждой строке
		err = fread(&matr->stroki[i].offset, sizeof(int), 1, matr->file);//смещение
		if (err != 1)
			{printf("Error with reading file\n"); fclose(matr->file); return 1;}
		err = fread(&matr->stroki[i].size, sizeof(int), 1, matr->file);//длинна
		if (err != 1)
			{printf("Error with reading file\n"); fclose(matr->file); return 1;}
	}
	return 0;
}

int getElement(Matrix* matr, int* elem){
	int err = fread(elem, sizeof(int), 1, matr->file);
	return err;
}

int typelines(Matrix* matr){
	int err = 0;
	for (int i = 0; i < matr->lines; i++){//цикл, чтобы узнать, верные строки или нет
		matr->stroki[i].correct = 2;
		err = fseek(matr->file, matr->stroki[i].offset, SEEK_SET);
		if (err != 0)
			{printf("Error with seeking file\n"); fclose(matr->file); return 1;}
		int pos = 0; int neg = 0;
		int elem = 1;
		for (int j = 0; j < matr->stroki[i].size; j++){
			err = getElement(matr, &elem);
			if (err != 1)
				{printf("Error with reading file\n"); fclose(matr->file); return 1;}
			if (elem > 0)
				pos = 1;
			if (elem < 0)
				neg = -1;
		}
		if (pos != 0 && neg != 0)
			matr->stroki[i].correct = 1;//строка верная
		else if ((pos != 0 && neg == 0) || (pos == 0 && neg != 0))
			matr->stroki[i].correct = 0;//отсутствует выполнение одного условия
	}
	return 0;
}


int output(Matrix* matr){
	int err = 0;
	err = fseek(matr->file, 0, SEEK_SET);
	int elem = 0;
	if (err != 0)
		{printf("Error with seeking file\n"); fclose(matr->file); return 1;}
	for (int i = 0; i < matr->lines; i++){
		err = fseek(matr->file, matr->stroki[i].offset, SEEK_SET);
		if (err != 0)
			{printf("Error with seeking file\n"); fclose(matr->file); return 1;}
		elem = 0;
		printf("Line %d: ", (i+1));
		for (int j = 0; j < matr->stroki[i].size; j++){
			err = fread(&elem, sizeof(int), 1, matr->file);
			if (err != 1)
				{printf("Error with reading file\n"); fclose(matr->file); return 1;}
			printf("%d ", elem);
		}
		printf("\n");
	}
	return 0;
}