#ifndef HID_H
#define HID_H

#include "library.h"
#include <stdio.h>

typedef struct EdgeInfo{//информация о ребре
    size_t* allowed_ports;
    size_t size;
    size_t reference_count;//счетчик ссылок
    struct ArrayElem* connected_vertex_1;
    struct ArrayElem* connected_vertex_2;
}EdgeInfo;



typedef struct ArrayElem{//структура элемента который содержится непосредственно в массиве
    Vertex* vertex;//текущая вершина
    struct ListNode* head;//голова на списко смежных вершин к текущей вершине
    size_t index;
}ArrayElem;


typedef struct ListNode{//узел в списке смежности
    EdgeInfo* edge;
    struct ListNode* next;
}ListNode;


typedef struct MST_edge{//структура для получания минимального остовного дерева
    size_t from_index;
    size_t to_index;
}MST_edge;

typedef struct Graph Graph;
typedef struct DFS DFS;

ErrorCode graph_expansion(Graph*);//расширение графа если массив заполнен
ErrorCode is_vertex_exist(const Graph*, const char*);//проверка на существование вершины
ErrorCode create_edge(Graph*, const size_t, const size_t, EdgeInfo*);//два size_t - два индекса по которым вершины
void change_vertex_info(Vertex*, const char*, const size_t*);//поменять информацию о вершине
ErrorCode change_edge_info(const ArrayElem, const char*, size_t*, const size_t);//поменять информацию о ребре
void go_to_adjacent_and_delete_vertex_from_there(ArrayElem*, const char*);//функция которая удаляет нужную вершину из списка смежности смежной с ней вершины
void replace_pointers(Graph*, size_t);//переместить указатели на нувую ячейку массива при удалении
ErrorCode delete_from_adjacent_list(ArrayElem*, const char*);//удаление узла вершины, смежной с нашей вершиной
void delete_node(ArrayElem**, ListNode**, ListNode**);//удаление узла
bool is_port_allowed(const EdgeInfo*, const size_t);


//вспомогательные функции для декомпозиции кода в DFS обходе
ErrorCode until_the_stack_is_empty(ArrayElem* start_vertex, bool* already_visited, DFS* all_correct_vertexes, size_t target_port);
ErrorCode suitable_vertex(Stack* stack, ArrayElem* current_elem, bool* already_visited, DFS* all_correct_vertexes, size_t target_port, size_t current_index);



//вспомогательные функции для дейкстры
void main_dijkstra(const Graph* graph, size_t vertex_count, bool* visited, size_t* distances, size_t* previous, size_t end_index, size_t required_port);
void working_w_edges_in_dijkstra(const Graph* graph, bool* visited, size_t* distances, size_t* previous, size_t current, size_t required_port);
void erase_arrays_in_dijkstra_and_mst(size_t*, size_t*, bool*);
void init_arrays_in_dijkstra_and_mst(size_t, size_t**, size_t**, bool**);
ErrorCode length_entry_in_dijkstra(const Graph*, size_t* previous, char*** path, size_t* path_length, size_t end_index);

//вспомогательные функции для MST
ErrorCode build_MST_graph(const Graph*, const MST_edge* mst_edges, size_t edge_count, Graph**);
void main_MST(const Graph* graph, size_t* min_edge, bool* in_tree, size_t* parent, MST_edge* needed_edges, size_t* res_edge_count, size_t target_port);
void working_w_edges_in_MST(const Graph* graph, size_t current, size_t target_port, bool* in_tree, size_t* min_edge, size_t* parent);
ErrorCode add_vertex_in_MST(const Graph* original_graph, Graph* result_graph);
ErrorCode add_edge_in_MST(Graph** result_graph, ArrayElem* vertex_1, ArrayElem* vertex_2, EdgeInfo* original_edge);


#endif