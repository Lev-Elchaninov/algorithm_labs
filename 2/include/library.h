#ifndef LIB_H
#define LIB_H

extern int error;

typedef struct Passanger{
    char* id;
    unsigned ta;
    unsigned ts;
}Passanger;

typedef struct input_info{//структура - информация, которая подается в функцию task (полный результат инпута)
    Passanger* storage;//массив пассажиров
    unsigned pass_amount;//количество пассажиров
    unsigned receptions_amount;//количество очередей
}Info;

typedef struct Node Node;
typedef struct List List;


typedef struct Queue Queue;


int task(Info*);
Queue* queues_initializtion(Info*);
int enqueue(Queue*, Passanger, unsigned);//внесение в очередь
int dequeue(Info*, Queue*, unsigned*, unsigned);//удаление из очереди
int current_passanger_count(Info*, unsigned);//счетчик пассажиров в одну секунду
Queue* min_queue(Info*, Queue*);//поиск минимальной очереди
Passanger fighter_is_ready(Info*, int, unsigned);//подготовка пассажира
void output_queue(Info*, Queue*, int);
void free_list(Info*, Queue*);
void free_vector(Info*, Queue*);
void pop(Queue*);
int push(Queue*, Passanger);

#endif