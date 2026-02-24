#include <stdlib.h>
#include <stdio.h>
#include "library.h"
#include "hidden_functions.h"
#include <string.h>
#include <math.h>
#include <stdint.h>
#include "dialogue.h"


ErrorCode init_graph(Graph** ptr){
    if (*ptr == NULL){
        (*ptr) = malloc(sizeof(Graph));
        if (*ptr == NULL)
            return ERROR_MEMORY_ALLOCATION;
        (*ptr)->capacity = 5;
        (*ptr)->size = 0;
        (*ptr)->all_vertices = malloc(sizeof(ArrayElem*) * (*ptr)->capacity);
        if ((*ptr)->all_vertices == NULL){
            free(*ptr);
            return ERROR_MEMORY_ALLOCATION;
        }
        return SUCCESS;
    }    
    if ((*ptr) != NULL && (*ptr)->all_vertices == NULL){
        if ((*ptr)->capacity == 0) 
            (*ptr)->capacity = 5;
        (*ptr)->all_vertices = malloc(sizeof(ArrayElem*) * (*ptr)->capacity);
        if ((*ptr)->all_vertices == NULL){
            free(*ptr);
            return ERROR_MEMORY_ALLOCATION;
        }
        (*ptr)->size = 0;
        return SUCCESS;
    }
    return SUCCESS;
}


void erase_graph(Graph* graph){//возможно не работает
    for (size_t i = 0; i < graph->size; i++){
        ListNode* ptr = graph->all_vertices[i]->head;
        while (ptr != NULL){
            graph->all_vertices[i]->head = ptr->next;
            if (ptr->edge != NULL){
                ptr->edge->reference_count--;
                if (ptr->edge->reference_count == 0){
                    free(ptr->edge->allowed_ports);
                    free(ptr->edge);
                }
            }
            free(ptr);
            ptr = graph->all_vertices[i]->head;
        }
        free(graph->all_vertices[i]->vertex->hostname);
        free(graph->all_vertices[i]->vertex);
        free(graph->all_vertices[i]);
    }
    free(graph->all_vertices);
    free(graph);
}


ErrorCode func_add_vertex(Graph* graph, const char* hostname, const size_t port){
    if (!graph || !graph->all_vertices)
        return ERROR_EMPTY_GRAPH;
    if (!hostname)
        return ERROR_INVALID_PARAMETER;
    ErrorCode error = is_vertex_exist(graph, hostname);
    if (error != SUCCESS)//если вершина уже существует
        return error;//ERROR_VERTEX_EXIST
    if (graph->size == graph->capacity){
        ErrorCode error = graph_expansion(graph);
        if (error != SUCCESS)
            return error;
    }
    ArrayElem* elem = malloc(sizeof(ArrayElem));
    if (elem == NULL)
        return ERROR_MEMORY_ALLOCATION;
    Vertex* new_vertex = malloc(sizeof(Vertex));
    if (new_vertex == NULL){
        free(elem);
        return ERROR_MEMORY_ALLOCATION;
    }
    new_vertex->hostname = strdup(hostname);
    if (new_vertex->hostname == NULL)
        return ERROR_MEMORY_ALLOCATION;
    new_vertex->port = port;
    if (new_vertex == NULL)
        return ERROR_MEMORY_ALLOCATION;
    graph->all_vertices[graph->size] = elem;
    graph->all_vertices[graph->size]->vertex = new_vertex;
    graph->all_vertices[graph->size]->head = NULL;
    graph->all_vertices[graph->size]->index = graph->size;
    graph->size++;
    return SUCCESS;
}


ErrorCode func_add_edge(Graph* graph, const size_t* array, const size_t size, const char* hostname1, const char* hostname2){
    if (!graph || !graph->all_vertices)
        return ERROR_EMPTY_GRAPH;
    if (!hostname1 || !hostname2 || !array)
        return ERROR_INVALID_PARAMETER;

    size_t vertex1_index = graph->capacity + 1, vertex2_index = graph->capacity + 1;
    for (size_t i = 0; i < graph->size; i++){
        if (strcmp(graph->all_vertices[i]->vertex->hostname, hostname1) == 0)
            vertex1_index = i;
        if (strcmp(graph->all_vertices[i]->vertex->hostname, hostname2) == 0)
            vertex2_index = i;
    }
    if (vertex1_index > graph->capacity || vertex2_index > graph->capacity)
        return ERROR_UNKNOWN_VERTEX;
    ListNode* ptr = graph->all_vertices[vertex1_index]->head;
    while (ptr != NULL){
        const char* edge_host1 = ptr->edge->connected_vertex_1->vertex->hostname;
        const char* edge_host2 = ptr->edge->connected_vertex_2->vertex->hostname;
    
        bool direct_match = (strcmp(edge_host1, hostname1) == 0 && strcmp(edge_host2, hostname2) == 0);

        bool reverse_match = (strcmp(edge_host1, hostname2) == 0 && strcmp(edge_host2, hostname1) == 0);
    
        if (direct_match || reverse_match)
            return ERROR_EDGE_EXIST;
        ptr = ptr->next;
    }
    EdgeInfo* current = malloc(sizeof(EdgeInfo));
    if (current == NULL)
        return ERROR_MEMORY_ALLOCATION;
    current->allowed_ports = malloc(size * sizeof(size_t));
    current->size = size;
    if (current->allowed_ports == NULL){
        free(current);
        return ERROR_MEMORY_ALLOCATION;
    }

    for (size_t j = 0; j < size; j++)
        current->allowed_ports[j] = array[j];

    ErrorCode error = create_edge(graph, vertex1_index, vertex2_index, current);
    if (error != SUCCESS){
        free(current);
        return error;
    }
    return SUCCESS;
}


ErrorCode func_output(const Graph* graph, FILE* file){
    if (!graph || !graph->all_vertices || graph->size == 0 || graph->capacity == 0)
        return ERROR_EMPTY_GRAPH;
    if (!file)
        return ERROR_INVALID_PARAMETER;
    for (size_t i = 0; i < graph->size; i++){
        fprintf(file, "[%s]:    ", graph->all_vertices[i]->vertex->hostname);
        ListNode* ptr = graph->all_vertices[i]->head;
        while (ptr != NULL){
            if (strcmp(ptr->edge->connected_vertex_1->vertex->hostname, graph->all_vertices[i]->vertex->hostname) == 0)
                fprintf(file, "{%s}  ", ptr->edge->connected_vertex_2->vertex->hostname);
            else
                fprintf(file, "{%s}  ", ptr->edge->connected_vertex_1->vertex->hostname);
            ptr = ptr->next;
        }
        fprintf(file, "\n");
    }
    return SUCCESS;
}

ErrorCode func_output_to_graphviz(const Graph* graph) {
    if (!graph || !graph->all_vertices || graph->size == 0)
        return ERROR_EMPTY_GRAPH;

    FILE* file = fopen("Graph.dot", "w");
    if (file == NULL)
        return ERROR_OPENING_FILE;
    fprintf(file, "graph Network {\n");
    fprintf(file, "    rankdir=\"LR\";\n");
    fprintf(file, "    node [shape=circle, fontname=\"Arial\"];\n");
    fprintf(file, "    edge [fontname=\"Arial\"];\n\n");

    for (size_t i = 0; i < graph->size; i++) {
        fprintf(file, "    \"%s_%zu\" [label=\"%s\\nPort: %zu\"];\n",
                graph->all_vertices[i]->vertex->hostname,
                graph->all_vertices[i]->vertex->port,
                graph->all_vertices[i]->vertex->hostname,
                graph->all_vertices[i]->vertex->port);
    }
    fprintf(file, "\n");

    for (size_t i = 0; i < graph->size; i++) {
        ListNode* current = graph->all_vertices[i]->head;
        char* host = NULL;
        size_t port = 0;
        while (current != NULL) {
            if (strcmp(graph->all_vertices[i]->vertex->hostname, current->edge->connected_vertex_1->vertex->hostname) == 0){
                host = current->edge->connected_vertex_2->vertex->hostname;
                port = current->edge->connected_vertex_2->vertex->port;
            }
            else{
                host = current->edge->connected_vertex_1->vertex->hostname; 
                port = current->edge->connected_vertex_1->vertex->port;
            }
            if (strcmp(graph->all_vertices[i]->vertex->hostname, host) == 0) {
                fprintf(file, "    \"%s_%zu\" -- \"%s_%zu\" [label=\"",
                        graph->all_vertices[i]->vertex->hostname, 
                        graph->all_vertices[i]->vertex->port, 
                        host, port);
                for (size_t j = 0; j < current->edge->size; j++) {
                    fprintf(file, "%zu", current->edge->allowed_ports[j]);
                    if (j < current->edge->size - 1) fprintf(file, ",");
                }
                fprintf(file, "\", dir=\"none\"];\n");
            }

            if (strcmp(graph->all_vertices[i]->vertex->hostname, host) < 0) {
                fprintf(file, "    \"%s_%zu\" -- \"%s_%zu\" [label=\"",
                        graph->all_vertices[i]->vertex->hostname,
                        graph->all_vertices[i]->vertex->port,
                        host, port);
                for (size_t j = 0; j < current->edge->size; j++) {
                    fprintf(file, "%zu", current->edge->allowed_ports[j]);
                    if (j < current->edge->size - 1) fprintf(file, ",");
                }
                fprintf(file, "\"];\n");
            }
            current = current->next;
        }
    }
    fprintf(file, "}\n");
    fclose(file);

    system("dot -Tpng Graph.dot -o Graph.png 2>/dev/null");
    system("xdg-open Graph.png 2>/dev/null");

    return SUCCESS;
}


ErrorCode func_change_vertex_data(Graph* graph, const char* old_hostname, const char* new_hostname, const size_t* new_port){
    if (!graph || !graph->all_vertices || graph->size == 0 || graph->capacity == 0)
        return ERROR_EMPTY_GRAPH;
    if (!old_hostname)
        return ERROR_INVALID_PARAMETER;
    size_t right_index = graph->size + 1;
    for (size_t i = 0; i < graph->size; i++){
        if (strcmp(graph->all_vertices[i]->vertex->hostname, old_hostname) == 0)
            right_index = i;
        if (new_hostname != NULL && strcmp(graph->all_vertices[i]->vertex->hostname, new_hostname) == 0)
            return ERROR_VERTEX_EXIST;
    }
    if (right_index > graph->size)
        return ERROR_UNKNOWN_VERTEX;
    
    change_vertex_info(graph->all_vertices[right_index]->vertex, new_hostname, new_port);
    
    return SUCCESS;
}


ErrorCode func_change_edge_data(Graph* graph, const size_t* array, const size_t size, const char* hostname1, const char* hostname2){
    if (!graph || !graph->all_vertices || graph->size == 0 || graph->capacity == 0)
        return ERROR_EMPTY_GRAPH;
    if (!hostname1 || !hostname2)
        return ERROR_INVALID_PARAMETER;
    size_t* array_copy = malloc(size * sizeof(size_t));
    if (array_copy == NULL)
        return ERROR_MEMORY_ALLOCATION;
    for (size_t t = 0; t < size; t++)
        array_copy[t] = array[t];
    
    for (size_t i = 0; i < graph->size; i++){
        if (strcmp(graph->all_vertices[i]->vertex->hostname, hostname1) == 0){
            ErrorCode error = change_edge_info(*graph->all_vertices[i], hostname2, array_copy, size);
            if (error != SUCCESS)
                free(array_copy);
            return error;
        }
    }
    free(array_copy);
    return ERROR_UNKNOWN_EDGE;
}




ErrorCode func_delete_vertex(Graph* graph, const char* hostname){
    if (!graph || !graph->all_vertices || graph->size == 0 || graph->capacity == 0)
        return ERROR_EMPTY_GRAPH;
    if (!hostname)
        return ERROR_INVALID_PARAMETER;
    for (size_t i = 0; i < graph->size; i++){
        if (strcmp(graph->all_vertices[i]->vertex->hostname, hostname) == 0){
            ListNode* ptr = graph->all_vertices[i]->head;
            while (graph->all_vertices[i]->head != NULL){
                go_to_adjacent_and_delete_vertex_from_there(graph->all_vertices[i], hostname);//удаление этой вершины из списка смежности смежной с ней вершины
                
                graph->all_vertices[i]->head = ptr->next;
                free(ptr);
                ptr = graph->all_vertices[i]->head;

            }
            graph->all_vertices[i]->head = NULL;
            free(graph->all_vertices[i]->vertex->hostname);
            if (i != graph->size - 1){
                graph->all_vertices[i]->vertex->hostname = strdup(graph->all_vertices[graph->size - 1]->vertex->hostname);
                graph->all_vertices[i]->vertex->port = graph->all_vertices[graph->size - 1]->vertex->port;
                graph->all_vertices[i]->head = graph->all_vertices[graph->size - 1]->head;
                replace_pointers(graph, i);
                free(graph->all_vertices[graph->size-1]->vertex->hostname);
                free(graph->all_vertices[graph->size-1]->vertex);
                free(graph->all_vertices[graph->size-1]);
                graph->all_vertices[graph->size-1] = NULL;
            }
            else if (i == graph->size - 1){
                free(graph->all_vertices[i]->vertex);
                free(graph->all_vertices[i]);
            }
            graph->size--;
            return SUCCESS;
        }
    }
    
    return ERROR_UNKNOWN_VERTEX;
}


ErrorCode func_delete_edge(Graph* graph, const char* hostname1, const char* hostname2){
    if (!graph || !graph->all_vertices || graph->size == 0 || graph->capacity == 0)
        return ERROR_EMPTY_GRAPH;
    if (!hostname1 || !hostname2)
        return ERROR_INVALID_PARAMETER;
    ErrorCode error = SUCCESS;
    int removed_count = 0;
    for (size_t i = 0; i < graph->size; i++){
        if (strcmp(graph->all_vertices[i]->vertex->hostname, hostname1) == 0){
            error = delete_from_adjacent_list(graph->all_vertices[i], hostname2);
            if (error != SUCCESS || strcmp(hostname1, hostname2) == 0)
                return error;
            removed_count++;
        }
        if (strcmp(graph->all_vertices[i]->vertex->hostname, hostname2) == 0){
            error = delete_from_adjacent_list(graph->all_vertices[i], hostname1);
            if (error != SUCCESS || strcmp(hostname1, hostname2) == 0)
                return error;
            removed_count++;
        }
    }
    if (removed_count != 2)
        return ERROR_UNKNOWN_EDGE;
    return SUCCESS;
}

//already_visited - массив в котором кажджому индексу дается знак - посещен/непосещен
//all_corrent_vertexes - все нужные нам вершины
ErrorCode func_DFS(const Graph* graph, const char* start_hostname, size_t target_port, DFS** result){
    if (!graph || !graph->all_vertices || graph->size == 0 || graph->capacity == 0)//добавить проверку на валидность параметров
        return ERROR_EMPTY_GRAPH;
    if (!start_hostname)
        return ERROR_INVALID_PARAMETER;
    bool* already_visited = calloc(graph->size, sizeof(bool));
    if (already_visited == NULL)
        return ERROR_MEMORY_ALLOCATION;
    (*result) = init_DFS(graph->size);
    if (*result == NULL){
        free(already_visited);
        return ERROR_MEMORY_ALLOCATION;
    }
    ArrayElem* start_vertex = NULL;
    int trigger = 0;
    size_t i = 0; 
    while (trigger == 0 && i <= graph->size - 1){
        if (strcmp(start_hostname, graph->all_vertices[i]->vertex->hostname) == 0){
            start_vertex = graph->all_vertices[i];
            trigger = 1;
        }
        i++;
    }
    if (trigger == 0){
        free(already_visited);
        erase_DFS(*result, graph->size);
        return ERROR_UNKNOWN_VERTEX;
    }
    
    ErrorCode error = until_the_stack_is_empty(start_vertex, already_visited, *result, target_port);
    if (error != SUCCESS){
        free(already_visited);
        erase_DFS(*result, graph->size);
        return error;
    }
    free(already_visited);
    return SUCCESS;
}



ErrorCode func_dijkstra(const Graph* graph, const char* start_host, const char* end_host, size_t required_port, char*** path, size_t* path_length){
    if (!graph || !graph->all_vertices || graph->size == 0 || graph->capacity == 0)//добавить проверку на валидность параметров
        return ERROR_EMPTY_GRAPH;
    if (!start_host || !end_host)
        return ERROR_INVALID_PARAMETER;
    size_t vertex_count = graph->size;
    size_t* distances = NULL; size_t* previous = NULL; bool* visited = NULL;

    init_arrays_in_dijkstra_and_mst(vertex_count, &distances, &previous, &visited);
    if (!distances || !previous || !visited){
        erase_arrays_in_dijkstra_and_mst(previous, distances, visited);
        return ERROR_MEMORY_ALLOCATION;
    }

    size_t start_index = vertex_count + 1;
    size_t end_index = vertex_count + 1;
    for (size_t i = 0; i < vertex_count; i++){
        distances[i] = __SIZE_MAX__;//inf
        previous[i] = __SIZE_MAX__;//inf
        visited[i] = false;//не посетил
        if (strcmp(graph->all_vertices[i]->vertex->hostname, start_host) == 0)
            start_index = i;
        if (strcmp(graph->all_vertices[i]->vertex->hostname, end_host) == 0)
            end_index = i;
    }
    if (start_index == vertex_count + 1 || end_index == vertex_count + 1){
        erase_arrays_in_dijkstra_and_mst(previous, distances, visited);
        return ERROR_UNKNOWN_VERTEX;
    }
    distances[start_index] = 0;//помечаем путь от начала как 0
    
    main_dijkstra(graph, vertex_count, visited, distances, previous, end_index, required_port);
    if (distances[end_index] == __SIZE_MAX__){
        erase_arrays_in_dijkstra_and_mst(previous, distances, visited);
        return ERROR_NO_PATH;
    }
    ErrorCode error = length_entry_in_dijkstra(graph, previous, path, path_length, end_index);
    erase_arrays_in_dijkstra_and_mst(previous, distances, visited);
    return error;
}









ErrorCode func_find_MST(const Graph* graph, const size_t target_port, Graph** new_graph){
    if (!graph || !graph->all_vertices)
        return ERROR_EMPTY_GRAPH;
    *new_graph = NULL;
    bool* in_tree = NULL; size_t* parent = NULL; size_t* min_edge = NULL;
    init_arrays_in_dijkstra_and_mst(graph->size, &parent, &min_edge, &in_tree);
    if (!in_tree || !parent || !min_edge) {
        free(in_tree); free(parent); free(min_edge);
        return ERROR_MEMORY_ALLOCATION;
    }
    for (size_t i = 0; i < graph->size; i++){
        min_edge[i] = __SIZE_MAX__;
        parent[i] = __SIZE_MAX__;
        in_tree[i] = false;
    }
    
    size_t start = 0;
    min_edge[start] = 0;
    MST_edge* edges = NULL;
    if (graph->size -1 != 0){
        edges = malloc((graph->size - 1) * sizeof(MST_edge));//возможные ребра
        if (edges == NULL){
            free(edges); free(in_tree); free(parent); free(min_edge);
            return ERROR_MEMORY_ALLOCATION;
        }
    }
    else
        edges = NULL;
    size_t res_edge_count = 0;
    main_MST(graph, min_edge, in_tree, parent, edges, &res_edge_count, target_port);
    
    for (size_t i = 0; i < graph->size; i++) {//проверка на достижимость всех вершин
        if (!in_tree[i]){
            free(edges);  free(in_tree); free(parent); free(min_edge);
            return ERROR_CANT_BUILD_MST;
        }
    }

    ErrorCode error = build_MST_graph(graph, edges, res_edge_count, new_graph);
    erase_arrays_in_dijkstra_and_mst(min_edge, parent, in_tree);
    free(edges);
    return error;
}   