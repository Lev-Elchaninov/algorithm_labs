#include <stdio.h>

int main() {
    int m = 8; 
    int n[] = {6, 6, 7, 2, 5, 10, 3, 2};
    int matrix[8][10] = {
        {1, 2, 3, 0, 0, 0},
        {0, 0, 0, 1, 2, 3},           
        {0, -1, 0, -2, -3, 6, -1},
        {-1, 2}, 
        {0, 0, 0, 0, 0},           
        {0, 0, 0, 0, 1, 2, 3, -2, -9, 7}, 
        {1, 2, 3}, 
        {0, 0}         
    };

    FILE *file = fopen("matrix.bin", "wb");
    if (file == NULL) {
        printf("Ошибка открытия файла");
        return 1;
    }

    fwrite(&m, sizeof(int), 1, file);

    int offset = sizeof(int) + (m * 2 * sizeof(int));
    for (int i = 0; i < m; i++) {
        fwrite(&offset, sizeof(int), 1, file);

        fwrite(&n[i], sizeof(int), 1, file);
        offset += n[i] * sizeof(int);
    }

    for (int i = 0; i < m; i++) {
        fwrite(matrix[i], sizeof(int), n[i], file);
    }

    fclose(file);

    printf("Матрица успешно записана в файл matrix.bin\n");
    return 0;
}