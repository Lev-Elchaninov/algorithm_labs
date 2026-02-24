#include "library.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>


typedef struct Queue{//из них будем создавать массив очередей
    unsigned length;//длина очереди
    unsigned head;//начало очереди
    unsigned tail;//конец очереди
    unsigned busy;//до какого времени очередь будет занята
    Passanger* queue;
}Queue;

#define N (5)


int task(Info* information){
    printf("<<vector implementation>>\n\n");
    Queue* vectors = NULL;
    int error = 0;
    int tracking = 0;//скольки людям осталось зайти в очередь в данную секунду
    printf("---");
    vectors = queues_initializtion(information); if(error == 1) return 1;
    unsigned done_passangers = 0; //счетчик пассажиров, полностью прошедших очередь
    unsigned time = 1;//счетчик времени от первой секунды. На нулевой секунде надо напечатать пустые очереди
    int indicator = 0;
    while (done_passangers != information->pass_amount){//пока все пассажиры не пройдут очередь
        indicator = dequeue(information, vectors, &done_passangers, time);//удаление пассажира из очереди
        int number_per_second = current_passanger_count(information, time);//узнает, сколько пассажиров приходит за одну секунду (Работает)
        tracking = number_per_second;
        while (tracking != 0){//это если пассажиров приходит > 1 за одну секунду
            int delta = number_per_second - tracking;
            Passanger current_passanger = fighter_is_ready(information, delta, time); if (error == 1) {free_vector(information, vectors); return 1;}
            Queue *minimal = min_queue(information, vectors);//поиск минимальной очереди (должна полностью работать)
            indicator = enqueue(minimal, current_passanger, time);/*вставка в эту минимальную очередь*/ if (error == 11) {free_vector(information, vectors); return 11;}
            tracking--;
        }
        if (indicator == 12 || indicator == 10)//или удаление из очереди или успешная запись в очередь
            output_queue(information, vectors, time);
        time++;
    }
    free_vector(information, vectors);
    return 0;
}


Queue* queues_initializtion(Info* information){
    Queue* vectors = calloc(information->receptions_amount, sizeof(Queue)); if(vectors == NULL) {free(vectors); error = 1; return NULL;}
    for (unsigned i = 0; i < information->receptions_amount; i++){
        vectors[i].queue = calloc(N, sizeof(Passanger)); 
        if (vectors[i].queue == NULL){
            for (unsigned j = 0; j < i; j++) {
                free(vectors[j].queue);
            }
            free(vectors); 
            error = 1;
            return NULL;
        }
    }
    return vectors;
}



int dequeue(Info* information, Queue* vectors, unsigned* done_passangers, unsigned time){
    unsigned x = *done_passangers;
    for (unsigned i = 0; i < information->receptions_amount; i++){
        if (vectors[i].busy == time){
            pop(&vectors[i]);
            if (vectors[i].length != 0)
                vectors[i].busy = time + vectors[i].queue[vectors->head].ts;
            (*done_passangers)++;
        }
    }
    if (x < *done_passangers)
        return 12;
    return 13;
}


int current_passanger_count(Info* information, unsigned time){
    int number_per_second = 0;
    for (unsigned i = 0; i < information->pass_amount; i++){
        if (information->storage[i].ta == time){
            number_per_second++;
        }
    }
    return number_per_second;
}


Passanger fighter_is_ready(Info* information, int delta, unsigned time){
    int counter = 0;
    Passanger empty = {NULL, 0, 0};
    for (unsigned i = 0; i < information->pass_amount; i++){
        if (information->storage[i].ta == time && delta == counter){
            return information->storage[i];
        }
        if (information->storage[i].ta == time && delta > counter)
            counter++;
    }
    error = 1;
    return empty;
}


Queue* min_queue(Info* information, Queue* vectors){
    unsigned min_length = INT_MAX;
    Queue* minimal = NULL;
    for (int i = information->receptions_amount - 1; i >= 0; i--){
        if (min_length >= vectors[i].length){
            minimal = &vectors[i];
            min_length = vectors[i].length;
        }        
    }
    return minimal;
}



int enqueue(Queue* minimal, Passanger current_passanger, unsigned time){
    int err = push(minimal, current_passanger);
    if (err == 11) return 11;

    if (minimal->length == 1){
        minimal->busy = minimal->queue[minimal->head].ts + time;
    }
    return 10;
}

void output_queue(Info* information, Queue* vectors, int time){
    int len = 0;
    int begin = 0;
    for (unsigned i = 0; i < information->receptions_amount; i++){
        printf("Queue %d | time %d |", (i+1), time);
        begin = vectors[i].head;
        len = vectors[i].length;
        while (len != 0){
            printf(" %s/%d/%d ", vectors[i].queue[begin].id, vectors[i].queue[begin].ta, vectors[i].queue[begin].ts);
            begin = (begin + 1) % N;
            len--;
        }
        printf("length = %d, head = %d, tail = %d, busy = %d", vectors[i].length, vectors[i].head, vectors[i].tail, vectors[i].busy);
        printf("\n");
    }
    printf("\n\n");
}

void free_vector(Info* information, Queue* vectors){
    for (unsigned i = 0; i < information->receptions_amount; i++){
        free(vectors[i].queue);
    }
    free(vectors);
}

int push(Queue* minimal, Passanger current_passanger){//вставка элемента в очередь
    minimal->queue[minimal->tail] = current_passanger;
    if (minimal->tail == minimal->head && minimal->length == N)
        {error = 11; return 11;}
    minimal->tail = (minimal->tail + 1) % N;
    minimal->length++;
    return 0;
}

void pop(Queue* queue){//достать элемент из очереди
    queue->head = (queue->head + 1) % N;
    queue->length--;
}














