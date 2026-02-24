#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "lib.h"


int getint_for_elements(int *intx){
	int err = getint(intx);
	if (err == 2) return 2;
	printf("\t\t\t\tSuccessful input\n\n"); 
	return 0;
}


int getint_for_sizes(int *intx){
	int cycle = 1;
	int err = 0;
	while (cycle != 0){
		err = getint(intx);
		if (err == 2) return 2;
		if ((*intx) >= 0)
			cycle = 0;
		else
			{printf("\t\t\t\tIncorrent input\n"); printf("Try again: ");}
	}
	printf("\t\t\t\tSuccessful input\n\n"); 
	return 0;
}


int getint(int* intx){
	int scanf_value = 0;
	int symbol = 0;
	int cycle = 1;
	int tryagain = 0;
	long long temp = 0;
	while(scanf_value != 1  ||  cycle != 0){ 
		temp = 0;
		cycle = 0;
		if (tryagain == 1)
			printf("Try again: ");
		scanf_value = scanf("%lld", &temp);
		printf("--");
		if (scanf_value == EOF && symbol != EOF){//EOF
            printf("\n\t\t\t\tEnd of File\n"); cycle = -2; return 2;
            }
		if (temp <= INT_MAX && temp >= INT_MIN){
			*intx = (int)temp;

			while ((symbol = getchar()) != '\n' && symbol != EOF){
        		if((symbol < '0' || symbol > '9') && symbol != -1){
        			cycle = 1;
				}
        	}
		}
		else if (temp > INT_MAX || temp < INT_MIN) {
            printf("\t\t\t\tInput exceeds INT range\n");
            cycle = 5;
		}
        if (scanf_value == 0 || cycle != 0){//некорректный ввод 2147483647
            printf("\t\t\t\tIncorrect input\n");
        }
		tryagain = 1;       
            
    }
	return 0;
}