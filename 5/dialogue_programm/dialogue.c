#include "dialogue.h"

#include <stdlib.h>
#include <stdio.h>


ErrorCode dialogue(Graph* graph){
    size_t action = 0;
    ErrorCode error = SUCCESS;
    while (error != ERROR_EOF){
        greeting();
        action = 0;
        error = input_action(&action); 
        if (error != SUCCESS){
            errors_printf(error); goto error_container;
        }
        switch(action){
            case(1)://добавление вершины
                error = operation_add_vertex(graph);
                if (error == ERROR_EMPTY_GRAPH || error == ERROR_VERTEX_EXIST)
                    errors_printf(error);
                else if(error != SUCCESS){
                    errors_printf(error);
                    goto error_container;
                }
                break;
            case(2)://добавление ребра
                error = operation_add_edge(graph);
                if (error == ERROR_EOF || error == ERROR_MEMORY_ALLOCATION){
                    errors_printf(error);
                    goto error_container;
                }
                else if (error != SUCCESS)
                    errors_printf(error);   
                break;
            case(3):
                error = operation_delete_vertex(graph);
                if (error == ERROR_UNKNOWN_VERTEX || error == ERROR_EMPTY_GRAPH || error == ERROR_EOF)
                    errors_printf(error);
                else if (error != SUCCESS)
                    goto error_container;
                break;
            case(4):
                error = operation_delete_edge(graph);
                if (error != SUCCESS) 
                    errors_printf(error);
                else if (error == ERROR_MEMORY_ALLOCATION || error == ERROR_EOF)
                    goto error_container;
                break;
            case(5)://изменение информации о вершине
               error = operation_change_vertex_data(graph);
                if (error == ERROR_EOF){
                    errors_printf(error);
                    goto error_container;
                }
                else if(error != SUCCESS)
                    errors_printf(error);
                break;
            
            case(6)://поиск минимального, превышающего заданный
                error = operation_change_edge_data(graph);
                if (error != SUCCESS && error != ERROR_EMPTY_GRAPH && error != ERROR_UNKNOWN_EDGE){
                    errors_printf(error);
                    goto error_container;
                }
                else if (error != SUCCESS)
                    errors_printf(error);
                break; 
            case(7)://вывод в виде списка смежности
                ErrorCode error = func_output(graph, stdout);
                if (error != SUCCESS)
                    errors_printf(error);
                break;
            case(8)://вывод в графвиз
                error = func_output_to_graphviz(graph);
                if (error == ERROR_EMPTY_GRAPH || error == ERROR_OPENING_FILE) 
                    errors_printf(error);
                else if (error != SUCCESS)
                    goto error_container;
                break;
            case(9)://DFS
                error = operation_DFS(graph);
                if (error != SUCCESS && error != ERROR_UNKNOWN_VERTEX && error != ERROR_EMPTY_GRAPH){
                    errors_printf(error);
                    goto error_container;
                }
                else if (error != SUCCESS)
                    errors_printf(error);
                break;
            case(10)://dijkstra
                error = operation_dijkstra(graph);
                if (error != SUCCESS && error != ERROR_NO_PATH && error != ERROR_UNKNOWN_VERTEX && error != ERROR_EMPTY_GRAPH){
                    errors_printf(error);
                    goto error_container;
                }
                else if (error != SUCCESS)
                    errors_printf(error);
                break;
            case(11)://MST
                error = operation_MST(graph);
                if (error != SUCCESS && error != ERROR_CANT_BUILD_MST && error != ERROR_EMPTY_GRAPH){
                    errors_printf(error);
                    goto error_container;
                }
                else if (error != SUCCESS)
                    errors_printf(error);
                break;
        }
    }
    error_container:
        switch(error){
            case(ERROR_EOF):
            case(ERROR_MEMORY_ALLOCATION):
            case(ERROR_NOT_SIZE_T):
                erase_graph(graph);
                return error;
            default:
                break;
        }
    erase_graph(graph);
    return SUCCESS;
}
