#ifndef LIB_H
#define LIB_H

#include "error.h"
#include <stdio.h>
#include <stdbool.h>
#include "stack.h"

//сами структуры лежат в hidden, они скрыты от пользователя
typedef struct ArrayElem ArrayElem;
typedef struct EdgeInfo EdgeInfo;
typedef struct ListNode ListNode;
typedef struct Mst_edge Mst_edge;

typedef struct Vertex{//информация о узлек
    char* hostname;
    size_t port;
}Vertex;


typedef struct Graph{//граф
    size_t capacity;
    size_t size;
    ArrayElem** all_vertices;//все исходные узлы
}Graph;

//структуры под DFS
typedef struct DFS{
    Vertex* vertex;
    bool achievable;
}DFS;


ErrorCode init_graph(Graph**);//инициализация графа
void erase_graph(Graph*);
DFS* init_DFS(const size_t);
void erase_DFS(DFS*, size_t);


ErrorCode func_add_vertex(Graph*, const char*, const size_t);
ErrorCode func_add_edge(Graph*, const size_t* array, const size_t size, const char* hostname1, const char* hostname2);
ErrorCode func_output(const Graph*, FILE*);
ErrorCode func_output_to_graphviz(const Graph*);
ErrorCode func_change_vertex_data(Graph*, const char* old_hostname, const char* new_hostname, const size_t* new_port);
ErrorCode func_change_edge_data(Graph*, const size_t* array, const size_t size, const char* hostname1, const char* hostname2);
ErrorCode func_delete_vertex(Graph*, const char* hostname);
ErrorCode func_delete_edge(Graph*, const char*, const char*);
ErrorCode func_DFS(const Graph*, const char* start_hostname, size_t target_port, DFS** result);
ErrorCode func_dijkstra(const Graph*, const char* start_host, const char* end_host, size_t required_port, char*** path, size_t* path_length);
ErrorCode func_find_MST(const Graph*, const size_t target_port, Graph**);
#endif