#include "library.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>


typedef struct Node{//узел
    Passanger data;
    struct Node* next;
}Node;

typedef struct List{//описание очереди на списке
    Node* head;
    Node* tail;
}List;

typedef struct Queue{//из них будем создавать массив очередей
    unsigned length;//длина очереди
    unsigned busy;//до какого времени очередь будет занята
    List* queue;
}Queue;


int task(Info* information){
    printf("<<list implementation>>\n\n");
    Queue* vectors = NULL;
    int error = 0;
    int tracking = 0;//скольки людям осталось зайти в очередь в данную секунду
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
            Passanger current_passanger = fighter_is_ready(information, delta, time); if (error == 1) {free_list(information, vectors); return 1;}
            Queue *minimal = min_queue(information, vectors);//поиск минимальной очереди (должна полностью работать)
            indicator = enqueue(minimal, current_passanger, time);/*вставка в эту минимальную очередь*/ 
            if (error == 1) {free_list(information, vectors); return 1;}
            tracking--;
        }
        if (indicator == 12 || indicator == 10)//или удаление из очереди или успешная запись в очередь
            output_queue(information, vectors, time);
        time++;
    }
    free_list(information, vectors);
    return 0;
}


Queue* queues_initializtion(Info* information){
    Queue* vectors = calloc(information->receptions_amount, sizeof(Queue)); if(vectors == NULL) {free(vectors); error = 1; return NULL;}
    for (unsigned i = 0; i < information->receptions_amount; i++){
        vectors[i].queue = malloc(sizeof(List));
        if (vectors[i].queue == NULL){
            for (unsigned j = 0; j < i; j++) {
                free(vectors[j].queue);
            }
            free(vectors); 
            error = 1;
            return NULL;
        }
        else
            {vectors[i].queue->head = NULL; vectors[i].queue->tail = NULL; }
    }
    return vectors;
}



int dequeue(Info* information, Queue* vectors, unsigned* done_passangers, unsigned time){
    unsigned x = *done_passangers;
    for (unsigned i = 0; i < information->receptions_amount; i++){
        if (vectors[i].busy == time){
            pop(&vectors[i]);
            if (vectors[i].length != 0)
                vectors[i].busy = time + vectors[i].queue->head->data.ts;
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
    if (err == 1) return 1;
    if (minimal->length == 1){
        minimal->busy = minimal->queue->head->data.ts + time;
    }
    return 10;
}

void output_queue(Info* information, Queue* vectors, int time){
    int len = 0;
    for (unsigned i = 0; i < information->receptions_amount; i++){
        printf("Queue %d | time %d |", (i+1), time);
        Node* ptr_head = NULL;
        if (vectors[i].queue->head != NULL)
            ptr_head = vectors[i].queue->head;
        len = vectors[i].length;
        while (ptr_head != NULL && len != 0){
            printf(" %s/%d/%d ", ptr_head->data.id, ptr_head->data.ta, ptr_head->data.ts);
            ptr_head = ptr_head->next;
            len--;
        }
        printf("length = %d, busy = %d", vectors[i].length, vectors[i].busy);
        printf("\n");
    }
    printf("\n\n");
}

void free_list(Info* information, Queue* vectors){
    for (unsigned i = 0; i < information->receptions_amount; i++){
        Node* ptr = vectors[i].queue->head;
        Node* next = NULL;
        while (ptr != NULL){
            next = ptr->next;
            free(ptr);
            ptr = next;
        }
        free(vectors[i].queue);
    }
    free(vectors);
}

void pop(Queue* current){//достать элемент из очереди
    Node* ptr = current->queue->head;
    current->queue->head = current->queue->head->next;
    free(ptr);
    current->length--;
}


int push(Queue* minimal, Passanger current_passanger){//вставка в очередь
    if (minimal->length == 0){
        minimal->queue->head = calloc(1, sizeof(Node)); if (minimal->queue->head == NULL) {error = 1; return 1;}
        minimal->queue->head->data = current_passanger;
        minimal->queue->tail = minimal->queue->head;
    }
    else{
        minimal->queue->tail->next = malloc(sizeof(Node)); if (minimal->queue->tail->next == NULL) {error = 1; return 1;}
        minimal->queue->tail = minimal->queue->tail->next;
        minimal->queue->tail->data = current_passanger;
        minimal->queue->tail->next = NULL;
    }
    minimal->length++;
    return 0;
}