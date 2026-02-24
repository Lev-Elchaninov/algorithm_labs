#include "hidden_functions.h"
#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#define MAX_SIZE 1000//для fgets

ErrorCode graph_expansion(Graph* graph){//расширение графа, если он уже существует
    graph->capacity *= 2;
    ArrayElem** ptr = realloc(graph->all_vertices, graph->capacity * sizeof(ArrayElem*));
    if (*ptr == NULL){
        erase_graph(graph);
        return ERROR_MEMORY_ALLOCATION;
    }
    graph->all_vertices = ptr;
    return SUCCESS;
}


ErrorCode is_vertex_exist(const Graph* graph, const char* hostname){
    for (size_t i = 0; i < graph->size; i++){
        if (strcmp(graph->all_vertices[i]->vertex->hostname, hostname) == 0)
            return ERROR_VERTEX_EXIST;
    }
    return SUCCESS;
}



ErrorCode create_edge(Graph* graph, const size_t vertex1_index, const size_t vertex2_index, EdgeInfo* current){
    ListNode* for_vertex1 = malloc(sizeof(ListNode));
    if (for_vertex1 == NULL)
        return ERROR_MEMORY_ALLOCATION;

    current->connected_vertex_1 = graph->all_vertices[vertex1_index];
    current->connected_vertex_2 = graph->all_vertices[vertex2_index];
    
    for_vertex1->edge = current;
    for_vertex1->next = graph->all_vertices[vertex1_index]->head;
    graph->all_vertices[vertex1_index]->head = for_vertex1;
    
    if (vertex1_index == vertex2_index){
        current->reference_count = 1;
        return SUCCESS;
    }

    ListNode* for_vertex2 = malloc(sizeof(ListNode));
    if (for_vertex2 == NULL){
        free(for_vertex1->edge->allowed_ports);
        free(for_vertex1->edge);
        free(for_vertex1);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    for_vertex2->edge = current;
    for_vertex2->next = graph->all_vertices[vertex2_index]->head;
    graph->all_vertices[vertex2_index]->head = for_vertex2;
    current->reference_count = 2;
    return SUCCESS;
}


void change_vertex_info(Vertex* vertex, const char* new_hostname, const size_t* new_port){
    if (new_hostname != NULL){
        free(vertex->hostname);
        vertex->hostname = strdup(new_hostname);
    }
    if (new_port != NULL)
        vertex->port = *new_port;
}


ErrorCode change_edge_info(const ArrayElem needed, const char* hostname2, size_t* new_array, const size_t new_capacity){
    ListNode* ptr = needed.head;
    while (ptr != NULL){
        char* host1 = ptr->edge->connected_vertex_1->vertex->hostname;
        char* host2 = ptr->edge->connected_vertex_2->vertex->hostname;

        bool is_direct = strcmp(needed.vertex->hostname, host1) == 0 && strcmp(hostname2, host2) == 0;
        bool is_reverse = strcmp(needed.vertex->hostname, host2) == 0 && strcmp(hostname2, host1) == 0;
        bool is_loop = strcmp(needed.vertex->hostname, host1) == 0 && strcmp(hostname2, host2) == 0 && strcmp(host1, host2) == 0;

        if (is_direct || is_reverse || is_loop){
            free(ptr->edge->allowed_ports);
            ptr->edge->allowed_ports = new_array;
            ptr->edge->size = new_capacity;
            return SUCCESS;
        }
        ptr = ptr->next;
    }
    return ERROR_UNKNOWN_EDGE;
}


void go_to_adjacent_and_delete_vertex_from_there(ArrayElem* current, const char* hostname){
    ListNode* ptr = NULL;
    ListNode* prev = NULL;
    if (strcmp(current->head->edge->connected_vertex_1->vertex->hostname, hostname) == 0){
        ptr = current->head->edge->connected_vertex_2->head;
        prev = current->head->edge->connected_vertex_2->head;
    }
    else{
        ptr = current->head->edge->connected_vertex_1->head;
        prev = current->head->edge->connected_vertex_1->head; 
    }
    while (ptr != NULL){
        char* host1 = ptr->edge->connected_vertex_1->vertex->hostname;
        char* host2 = ptr->edge->connected_vertex_2->vertex->hostname;
        if ((strcmp(host1, hostname) == 0 || strcmp(host2, hostname) == 0)){
            //если найденный элемент - первый в списке смежных узлов
            if (current->head != NULL && prev == current->head->edge->connected_vertex_1->head
            && current->head->edge->connected_vertex_1->head->next == NULL){
                current->head->edge->connected_vertex_1->head = NULL;
            }
            else if (current->head != NULL && prev == current->head->edge->connected_vertex_2->head 
            && current->head->edge->connected_vertex_2->head->next == NULL){
                current->head->edge->connected_vertex_2->head = NULL;
            }


            if (prev != ptr){
                prev->next = ptr->next;
            }
            //перемещаем голову если попали удаляем с начала списка
            else if (current->head != NULL && prev == current->head->edge->connected_vertex_2->head 
            && current->head->edge->connected_vertex_2->head != NULL){
                current->head->edge->connected_vertex_2->head = current->head->edge->connected_vertex_2->head->next;
            }
            else if (current->head != NULL && prev == current->head->edge->connected_vertex_1->head 
            && current->head->edge->connected_vertex_1->head != NULL){
                current->head->edge->connected_vertex_1->head = current->head->edge->connected_vertex_1->head->next;
            }
            
            if (strcmp(host1, host2) == 0){
                free(ptr->edge->allowed_ports);
                free(ptr->edge);
                return;
            }
            free(ptr->edge->allowed_ports);
            free(ptr->edge);
            free(ptr);
            return;
        }
        prev = ptr;
        ptr = ptr->next;
    }
}


void replace_pointers(Graph* graph, size_t new_index){
    ListNode* ptr = graph->all_vertices[graph->size - 1]->head;

    while (ptr != NULL){
        if (ptr->edge->connected_vertex_1 == graph->all_vertices[graph->size - 1])
            ptr->edge->connected_vertex_1 = graph->all_vertices[new_index];
        if (ptr->edge->connected_vertex_2 == graph->all_vertices[graph->size - 1])
           ptr->edge->connected_vertex_2 = graph->all_vertices[new_index]; 
        ptr = ptr->next;
    }
}



ErrorCode delete_from_adjacent_list(ArrayElem* current, const char* hostname_adjacent){
    ListNode* ptr = current->head;
    ListNode* prev = NULL;
    while (ptr != NULL){
        char* host1 = ptr->edge->connected_vertex_1->vertex->hostname;
        char* host2 = ptr->edge->connected_vertex_2->vertex->hostname;
        char* current_host = current->vertex->hostname;

        bool is_normal_edge = 
            (strcmp(host1, hostname_adjacent) == 0 && strcmp(host2, current_host) == 0) ||
            (strcmp(host2, hostname_adjacent) == 0 && strcmp(host1, current_host) == 0);

        bool is_loop_edge = 
            (strcmp(host1, host2) == 0 && 
            (strcmp(host1, hostname_adjacent) == 0));

        if (is_normal_edge || is_loop_edge) {
            delete_node(&current, &prev, &ptr);
            return SUCCESS;
        }

        prev = ptr;
        ptr = ptr->next;
    }
    return ERROR_UNKNOWN_EDGE;
}

void delete_node(ArrayElem** current, ListNode** prev, ListNode** ptr){
    if (*prev == NULL)
        (*current)->head = (*current)->head->next;
    else if (*prev != NULL)
        (*prev)->next = (*ptr)->next;
    if ((*ptr)->edge->reference_count == 1){
        free((*ptr)->edge->allowed_ports);
        free((*ptr)->edge);
    }
    else if ((*ptr)->edge->reference_count == 2)
        (*ptr)->edge->reference_count--;
    free(*ptr);
}


bool is_port_allowed(const EdgeInfo* edge, const size_t port){
    for(size_t i = 0; i < edge->size; i++){
        if (edge->allowed_ports[i] == port)
            return true;
    }
    return false;
}



ErrorCode until_the_stack_is_empty(ArrayElem* start_vertex, bool* already_visited, DFS* all_correct_vertexes, size_t target_port){
    Stack stack;
    stack_init(&stack);
    ErrorCode error = push(&stack, start_vertex);
    if (error != SUCCESS){
        erase_stack(&stack);
        return ERROR_MEMORY_ALLOCATION;
    }

    while (!empty_stack(&stack)){
        ArrayElem* current_elem;
        if (pop(&stack, (void**)&current_elem) != SUCCESS){
            erase_stack(&stack);
            return ERROR_MEMORY_ALLOCATION;
        }
        size_t current_index = current_elem->index;

        if (!already_visited[current_index]){
            error = suitable_vertex(&stack, current_elem, already_visited, all_correct_vertexes, target_port, current_index);
            if (error != SUCCESS)
                return error;
        }
    }
    return SUCCESS;
}


ErrorCode suitable_vertex(Stack* stack, ArrayElem* current_elem, bool* already_visited, DFS* all_correct_vertexes, size_t target_port, size_t current_index){
    already_visited[current_index] = true;
    all_correct_vertexes[current_index].vertex->port = current_elem->vertex->port;
    all_correct_vertexes[current_index].vertex->hostname = strdup(current_elem->vertex->hostname);
    if (current_elem->vertex->port == target_port)
        all_correct_vertexes[current_index].achievable = true;
    else
        all_correct_vertexes[current_index].achievable = false;
            
    ListNode* ptr = current_elem->head;
    while (ptr != NULL){
        EdgeInfo* edge = ptr->edge;
        ArrayElem* neighbor = (edge->connected_vertex_1 == current_elem) ? edge->connected_vertex_2 : edge->connected_vertex_1;

        if (is_port_allowed(edge, target_port) && !already_visited[neighbor->index]){
            if (push(stack, neighbor) != SUCCESS) return ERROR_MEMORY_ALLOCATION;
        }
        ptr = ptr->next;
    
    }
    return SUCCESS;
}

DFS* init_DFS(const size_t size){
    DFS* ptr = malloc(size * sizeof(DFS));
    if (ptr == NULL)
        return NULL;
    for (size_t i = 0; i < size; i++){
        ptr[i].achievable = false;
        ptr[i].vertex = malloc(sizeof(Vertex));
        if (ptr[i].vertex == NULL){
            erase_DFS(ptr, i+1);
            return NULL;
        }
        ptr[i].vertex->hostname = NULL;
        ptr[i].vertex->port = 0;
    }
    return ptr;
}

void erase_DFS(DFS* ptr, size_t size){
    for (size_t i = 0; i < size; i++){
        free(ptr[i].vertex->hostname);
        free(ptr[i].vertex);
    }
    free(ptr);
}



void main_dijkstra(const Graph* graph, size_t vertex_count, bool* visited, 
size_t* distances, size_t* previous, size_t end_index, size_t required_port){
    for (size_t i = 0; i < vertex_count; i++){
        size_t current = __SIZE_MAX__;
        size_t min_distance = __SIZE_MAX__;

        for (size_t j = 0; j < vertex_count; j++){
            if (!visited[j] && distances[j] < min_distance){
                min_distance = distances[j]; 
                current = j;
            }
        }
        if (current == __SIZE_MAX__ || current == end_index)//если нет вершин для обработки или достиг конца
            i = vertex_count;
        else
            working_w_edges_in_dijkstra(graph, visited, distances, previous, current, required_port);
    }
}


void working_w_edges_in_dijkstra(const Graph* graph, bool* visited, size_t* distances, size_t* previous, size_t current, size_t required_port){
    visited[current] = true;//пометка что вершину посетили
    ListNode* ptr = graph->all_vertices[current]->head;//заходим в список смежности
    while (ptr != NULL){
        EdgeInfo* edge = ptr->edge;
        bool allowed_port = is_port_allowed(edge, required_port);
        if (allowed_port){//если у текущей смежной вершины разрешена передача
            size_t neightbor_index = (edge->connected_vertex_1 == graph->all_vertices[current])
                                ? edge->connected_vertex_2->index
                                : edge->connected_vertex_1->index;
            size_t temp = distances[current] + 1;//тут рассчет нового расстояния

            if (temp < distances[neightbor_index]){
                distances[neightbor_index] = temp;//путь до соседа это от текущего + 1
                previous[neightbor_index] = current;//теперь current - это сосед
            }
        }

        ptr = ptr->next;
    }
}

void init_arrays_in_dijkstra_and_mst(size_t size, size_t** array_1, size_t** array_2, bool** b_array_3){
    (*array_1) = malloc(size * sizeof(size_t));
    (*array_2) = malloc(size * sizeof(size_t));
    (*b_array_3) = malloc(size * sizeof(bool));
}

void erase_arrays_in_dijkstra_and_mst(size_t* array_1, size_t* array_2, bool* array_3){
    free(array_2);
    free(array_1);
    free(array_3);
}



ErrorCode length_entry_in_dijkstra(const Graph* graph, size_t* previous, char*** path, size_t* path_length, size_t end_index){
    size_t lengh = 0;
    for (size_t i = end_index; i != __SIZE_MAX__; i = previous[i])
        lengh++;
        
    (*path) = calloc(lengh, sizeof(char*));
    if (*path == NULL){
        return ERROR_MEMORY_ALLOCATION;
    }

    size_t pos = lengh - 1;
    for (size_t i = end_index; i != __SIZE_MAX__; i = previous[i]){
        (*path)[pos] = strdup(graph->all_vertices[i]->vertex->hostname);
        pos--;
    }
    *path_length = lengh;
    return SUCCESS;
}



ErrorCode build_MST_graph(const Graph* original_graph, const MST_edge* mst_edges, size_t edge_count, Graph** result_graph){
    *result_graph = NULL;
    ErrorCode error = init_graph(result_graph);
    if (error != SUCCESS){
        erase_graph(*result_graph);
        *result_graph = NULL;
        return error;
    }

    error = add_vertex_in_MST(original_graph, *result_graph);
    if (error != SUCCESS){
        erase_graph(*result_graph);
        return error;
    }
    for (size_t i = 0; i < edge_count; i++){
        MST_edge temp = mst_edges[i];
        ArrayElem* vertex_1 = original_graph->all_vertices[temp.from_index];
        ArrayElem* vertex_2 = original_graph->all_vertices[temp.to_index];
        
        EdgeInfo* original_edge = NULL;
        ListNode* ptr = vertex_1->head;
        while (ptr != NULL){
            EdgeInfo* ee = ptr->edge;
            size_t neighbor = (ee->connected_vertex_1 == vertex_1)
                            ? ee->connected_vertex_2->index : ee->connected_vertex_1->index;
            if (neighbor == temp.to_index){
                original_edge = ee;
                break;
            }
            ptr = ptr->next;
        }

        error = add_edge_in_MST(result_graph, vertex_1, vertex_2, original_edge);
        if (error != SUCCESS){
            erase_graph(*result_graph);
            *result_graph = NULL;
            return error;
        }
    }
    return SUCCESS;
}




void main_MST(const Graph* graph, size_t* min_edge, bool* in_tree, size_t* parent, MST_edge* edges, size_t* res_edge_count, size_t target_port){
    for (size_t i = 0; i < graph->size; i++) {
        size_t u = __SIZE_MAX__;
        size_t min_w = __SIZE_MAX__;

        for (size_t j = 0; j < graph->size; j++){
            if (!in_tree[j] && min_edge[j] < min_w) {
                min_w = min_edge[j];
                u = j;
            }
        }
        
        if (u == __SIZE_MAX__) 
            i = graph->size + 1;
        else{
            in_tree[u] = true;
            
            // Добавляем ребро в MST
            if (parent[u] != __SIZE_MAX__) {
                edges[*res_edge_count].from_index = parent[u];
                edges[*res_edge_count].to_index = u;
                (*res_edge_count)++;
            }
            working_w_edges_in_MST(graph, u, target_port, in_tree, min_edge, parent);
        }
    }
}






void working_w_edges_in_MST(const Graph* graph, size_t current, size_t target_port, bool* in_tree, size_t* min_edge, size_t* parent){
    ListNode* ptr = graph->all_vertices[current]->head;
    while (ptr != NULL) {
        EdgeInfo* edge = ptr->edge;
            
        bool valid_port = false;
        for (size_t k = 0; k < edge->size; k++) {
            if (edge->allowed_ports[k] == target_port) {
                valid_port = true;
                k = edge->size + 1;
            }
        }
            
        if (valid_port) {
            size_t neighbor = (edge->connected_vertex_1 == graph->all_vertices[current])
                     ? edge->connected_vertex_2->index
                     : edge->connected_vertex_1->index;
                
            size_t weight = 1;
            if (!in_tree[neighbor] && weight < min_edge[neighbor]) {
                min_edge[neighbor] = weight;
                parent[neighbor] = current;
            }
        }
        ptr = ptr->next;
    }
}


ErrorCode add_vertex_in_MST(const Graph* original_graph, Graph* result_graph){
    for (size_t i = 0; i < original_graph->size; i++){
        char* hostname = original_graph->all_vertices[i]->vertex->hostname;
        size_t port = original_graph->all_vertices[i]->vertex->port;
        ErrorCode error = func_add_vertex(result_graph, hostname, port);
        if (error != SUCCESS && error != ERROR_VERTEX_EXIST)
            return error;
    }
    return SUCCESS;
}

ErrorCode add_edge_in_MST(Graph** result_graph, ArrayElem* vertex_1, ArrayElem* vertex_2, EdgeInfo* original_edge){
    size_t* allowed_ports = malloc(original_edge->size * sizeof(size_t));
    if (!allowed_ports)
        return ERROR_MEMORY_ALLOCATION;
    memcpy(allowed_ports, original_edge->allowed_ports, original_edge->size * sizeof(size_t));
    size_t size = original_edge->size;

    ErrorCode error = func_add_edge(*result_graph, allowed_ports, size, vertex_1->vertex->hostname, vertex_2->vertex->hostname);
    free(allowed_ports);
    if (error == ERROR_UNKNOWN_VERTEX || error == ERROR_UNKNOWN_EDGE)
        error = SUCCESS;
    return error;
}
