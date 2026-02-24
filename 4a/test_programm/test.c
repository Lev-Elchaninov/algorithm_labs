#include "../dialogue_programm/library.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define MIN_ELEMS 5000
#define MAX_ELEMS 2000000
#define STEP 1000
#define MAX_KEY 1000000000
#define MIN_INFO_LEN 4
#define MAX_INFO_LEN 6
#define ITERATIONS 10

// Глобальные переменные для тайминга
clock_t start, end;
double find_summary;

void shuffle_info_array(Info* array, size_t size) {
    srand(time(NULL));
    for (size_t i = size - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        Info temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}
/*
char* generate_random_string(size_t length) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char* str = malloc(length + 1);
    if (str == NULL) return NULL;

    for (size_t i = 0; i < length; i++) {
        int index = rand() % (sizeof(charset) - 1);
        str[i] = charset[index];
    }
    str[length] = '\0';
    return str;
}
*/
void generate_data(Info* array, int* used_keys, size_t num_elems) {
    memset(used_keys, 0, MAX_KEY * sizeof(int));
    size_t entries_generated = 0;
    
    while (entries_generated < num_elems) {
        size_t key = rand() % MAX_KEY + 1;
        
        if (used_keys[key - 1] == 0) {
            used_keys[key - 1] = 1;
            /*size_t info_len = MIN_INFO_LEN + rand() % (MAX_INFO_LEN - MIN_INFO_LEN + 1);
            char* info = generate_random_string(info_len);
            if (info == NULL) continue;*/
            
            array[entries_generated].key = key;
            array[entries_generated].info = "a";
            entries_generated++;
        }
    }
}

void process_iteration(Bin_tree** tree, Info* array, size_t num_elems) {
    // Инициализация дерева
    init_tree(tree);
    
    // Замер времени вставки
    for (size_t i = 0; i < num_elems; i++) {
        func_insert(*tree, array[i].key, array[i].info);
    }
    
    // Перемешиваем массив для случайного порядка удаления
    /*shuffle_info_array(array, num_elems);*/
    double average_time = 0;
    start = clock();
    for (size_t i = num_elems / 2; i < (num_elems / 2) + 1000;  i++){
        func_delete(*tree, array[i].key);
    }
    end = clock();
    long double find_time = ((long double)(end - start) * 1000000) / (CLOCKS_PER_SEC);
    average_time  = find_time / 1000;
    find_summary += average_time;
    
    // Запись результатов в файл
    
    // Очистка дерева
    erase_tree(*tree);
    (*tree) = NULL;
}

int main() {
    srand(time(NULL));
    FILE* output_file = fopen("time_results.txt", "w");
    if (output_file == NULL) {
        perror("Failed to open output file");
        return EXIT_FAILURE;
    }

    // Заголовок файла
    fprintf(output_file, "Elements InsertTime(μs) DeleteTime(μs)\n");
    
    Info* array = malloc(MAX_ELEMS * sizeof(Info));
    int* used_keys = calloc(MAX_KEY, sizeof(int));
    Bin_tree* tree = NULL;
    
    if (array == NULL || used_keys == NULL) {
        perror("Memory allocation failed");
        fclose(output_file);
        free(array);
        free(used_keys);
        return EXIT_FAILURE;
    }

    for (size_t num_elems = MIN_ELEMS; num_elems <= MAX_ELEMS; num_elems += STEP) {
        find_summary = 0;
        
        for (int i = 0; i < ITERATIONS; i++) {
            generate_data(array, used_keys, num_elems);
            process_iteration(&tree, array, num_elems);
            
            // Освобождаем память для строк
        }
        
        // Вывод средних значений
        printf("Elements: %zu\n", num_elems);
        printf("  Avg Delete Time: %.2f μs\n", find_summary / ITERATIONS);
        find_summary = find_summary / ITERATIONS;
        fprintf(output_file, "%zu %.2f\n", num_elems, find_summary);
        fflush(output_file);
    }
    
    fclose(output_file);
    free(array);
    free(used_keys);
    return EXIT_SUCCESS;
}