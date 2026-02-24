#ifndef DIA_H
#define DIA_H

#include "error.h"
#include "library.h"

ErrorCode dialogue(Graph*);//диалог
void greeting();//приглащение

void errors_printf(ErrorCode);//функция вывода ошибок
char* input_filename();



ErrorCode not_all();//может быть и не нужна



ErrorCode get_size_t(size_t*);//считка size_t
ErrorCode input_action(size_t*);//диалоговая
void erase_data(EdgeInfo*);
ErrorCode entry(size_t**, size_t*, size_t*, size_t);
ErrorCode ports_entring(size_t**, size_t*);
void output_bool_array(const Graph*, DFS*);
void output_path(const size_t, char**);

ErrorCode operation_add_vertex(Graph*);
ErrorCode operation_add_edge(Graph*);
ErrorCode operation_change_vertex_data(Graph*);
ErrorCode operation_change_edge_data(Graph*);
ErrorCode operation_delete_vertex(Graph*);
ErrorCode operation_delete_edge(Graph*);
ErrorCode operation_DFS(const Graph*);
ErrorCode operation_dijkstra(const Graph*);
ErrorCode operation_MST(const Graph*);
#endif