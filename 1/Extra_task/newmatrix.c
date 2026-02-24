#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "lib.h"



int sizeofnewlines(Matrix* matr, int* nulls){//узнаем длины новыс строк
	int err = 0;
	err = fseek(matr->file, 0, SEEK_SET);
	int elem = 0;
	if (err != 0)
		{printf("Error with seeking file\n"); fclose(matr->file); return 1;}
	for (int i = 0; i < matr->lines; i++){
		int zero_count = 0; 
		int newsize = 0; int if_output = 0;
		err = fseek(matr->file, matr->stroki[i].offset, SEEK_SET);
		if (err != 0)
			{printf("Error with seeking file\n"); fclose(matr->file); return 1;}
		int pos = 0; int neg = 0; elem = 0;
		for (int j = 0; j < matr->stroki[i].size; j++){
			err = fread(&elem, sizeof(int), 1, matr->file);
			if (err != 1)
				{printf("Error with reading file\n"); fclose(matr->file); return 1;}
			if (elem > 0)
				pos = 1;
			if (elem < 0)
				neg = -1;
			if (pos != 1 || neg != -1){
				if ((pos != 0 && neg == 0) || (pos == 0 && neg != 0)){
					newsize++;
				}
				else if (pos == 0 && neg == 0 && matr->stroki[i].correct == 0){
					newsize++;
				}
			}
			if (pos == 1 && neg == -1 && if_output == 0){//d - для выхода их цикла
				newsize++; if_output = 1;
			}
			if (elem == 0)
				zero_count++;
		}
		if (zero_count == matr->stroki[i].size || matr->stroki[i].size == 0){
			(*nulls)++;
			matr->stroki[i].size = 0;
		}
		matr->stroki[i].newsize = newsize;
	}
	return 0;
}





int writenewinfo(Matrix* matr, int* nulls){
	int err = 0;
	int n = 0;
	err = fseek(matr->file, 0, SEEK_SET);
	if (err != 0)
		{printf("Error with seeking file\n"); fclose(matr->file); return 1;}
	printf("\n\n");

	char fileoutput[300];
	printf("Inter the name of file to write(output): ");
	n = scanf("%255s", fileoutput);
	if (n == EOF){
		printf("EOF\n"); fclose(matr->file); free(matr->stroki); return 1;
	}
	FILE* file = fopen(fileoutput, "wb");
	while (file == NULL){
		printf("Name of file is incorrect, try again: ");
    	n = scanf("%255s", fileoutput);
		if (n == EOF){
			printf("EOF\n"); fclose(matr->file); free(matr->stroki); fclose(file); return 1;
		}
		file = fopen(fileoutput, "wb");
	}

	printf("\n-New matrix");
	int newm = matr->lines - (*nulls);
	err = fwrite(&newm, sizeof(int), 1, file);
	if (err != 1)
		{printf("Error with writing in file\n"); fclose(file); return 1;} 
	
	int newoffset = sizeof(int) + (newm * 2 * sizeof(int)); 
	
	if ((*nulls) != 0)
		printf("\t(%d zero lines were removed)\n", (*nulls));
	else
		printf("\n");
	for (int i = 0; i < matr->lines; i++){
		if (matr->stroki[i].size != 0){
			err = fwrite(&newoffset, sizeof(int), 1, file);
			if (err != 1)
				{printf("Error with writing in file\n"); fclose(file); return 1;}

			err = fwrite(&matr->stroki[i].newsize, sizeof(int), 1, file);
			if (err != 1)
				{printf("Error with writing in file\n"); fclose(file); return 1;}
			newoffset += matr->stroki[i].newsize * sizeof(int);
		}
	}
	err = outputnew_bin_stdout(matr, file);
	if (err == 1)
		return 1;
	return 0;
}


int outputnew_bin_stdout(Matrix* matr, FILE* file){
	int err= 0;
	int k = 0;
	for (int i = 0; i < matr->lines; i++){
		if (matr->stroki[i].size != 0){
			err = fseek(matr->file, matr->stroki[i].offset, SEEK_SET);
			if (err != 0)
				{printf("Error with seeking file\n"); fclose(matr->file); return 1;}
			int pos = 0; int neg = 0;
			int elem = 1;
			printf("Line %d: ", (k+1));
			for (int j = 0; j < matr->stroki[i].size; j++){
				err = fread(&elem, sizeof(int), 1, matr->file);
				if (err != 1)
					{printf("Error with reading file\n"); fclose(matr->file); return 1;}
				if (elem > 0)
					pos = 1;
				if (elem < 0)
					neg = -1;
				if (pos != 1 || neg != -1){
					if ((pos != 0 && neg == 0) || (pos == 0 && neg != 0)){
						printf("%d ", elem);
						err = fwrite(&elem, sizeof(int), 1, file);
						if (err != 1)
							{printf("Error with writing in file\n"); fclose(file); return 1;}
					}
					else if (pos == 0 && neg == 0 && matr->stroki[i].correct == 0){
						printf("%d ", elem);
						err = fwrite(&elem, sizeof(int), 1, file);
						if (err != 1)
							{printf("Error with writing in file\n"); fclose(file); return 1;}
					}
				}
				if (pos == 1 && neg == -1){
					printf("%d", elem); 
					j = matr->stroki[i].size;
					err = fwrite(&elem, sizeof(int), 1, file);
					if (err != 1)
						{printf("Error with writing in file\n"); fclose(file); return 1;}
				}
	
			}
			printf("\n");
			k++;
		}
	}
	
	fclose(file);
	free_matrix(matr);
	return 0;
}

void free_matrix(Matrix* matr){
	fclose(matr->file);
	free(matr->stroki);
}