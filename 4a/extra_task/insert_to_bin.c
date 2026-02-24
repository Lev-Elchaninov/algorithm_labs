#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_and_write_size_t_array(char* filename, size_t max_count, size_t max_value) {
    srand(time(NULL));
    size_t count = 1 + rand() % max_count;
    size_t* numbers = malloc(count * sizeof(size_t));
    if (numbers == NULL)
        return;
    for (size_t i = 0; i < count; i++)
        numbers[i] = rand() % (max_value + 1);

    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        free(numbers);
        return;
    }

    fwrite(numbers, sizeof(size_t), count, file);

    fclose(file);
    free(numbers);
}


