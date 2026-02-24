#include "dialogue.h"

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>

ErrorCode operation_add_vertex(Graph* graph){
    char* hostname = readline("Enter the hostname: ");
    if (hostname == NULL)
        return ERROR_EOF;
    
    printf("Enter the port: ");
    size_t port = 0;
    ErrorCode error = get_size_t(&port);
    if (error != SUCCESS){
        free(hostname);
        return error;
    }
    
    error = func_add_vertex(graph, hostname, port);
    if (error != SUCCESS){
        free(hostname);
        return error;
    }
    free(hostname);
    return SUCCESS;
}

ErrorCode operation_add_edge(Graph* graph){
    char* hostname1 = readline("Enter the hostname_1: ");
    if (hostname1 == NULL)
        return ERROR_EOF;
    char* hostname2 = readline("Enter the hostname_2: ");
    if (hostname2 == NULL){
        free(hostname1);
        return ERROR_EOF;
    }
    size_t* array = NULL;
    size_t size = 0;
    ErrorCode error = ports_entring(&array, &size);
    if (error != SUCCESS){
        free(hostname1);
        free(hostname2);
        return error;
    }
    error = func_add_edge(graph, array, size, hostname1, hostname2);//надо будет чуть переделать структуру
    free(array);
    free(hostname1);
    free(hostname2);
    return error;
}
    
ErrorCode operation_change_vertex_data(Graph* graph){
    if (!graph || !graph->all_vertices || graph->size == 0 || graph->capacity == 0)
        return ERROR_EMPTY_GRAPH;
    char* old_host = readline("Enter the host you want to change: ");
    if (old_host == NULL)
        return ERROR_EOF;
    char* variant = readline("Do you want to change hostname? (y/n): ");
    if (variant == NULL){
        free(old_host);
        return ERROR_EOF;
    }
    char* new_host = NULL;
    if (strcmp(variant, "n") != 0){
        new_host = readline("Enter new host: ");
        if (new_host == NULL){
            free(old_host);
            free(variant);
            return ERROR_EOF;
        }
    }
    free(variant);
    variant = NULL;
    variant = readline("Do you want to change port? (y/n): ");
    if (variant == NULL){
        free(old_host);
        free(new_host);
        return ERROR_EOF;
    }
    ErrorCode error = SUCCESS;
    if (strcmp(variant, "n") != 0){
        fprintf(stdout, "Enter new port: ");
        size_t new_port = 0;
        error = get_size_t(&new_port);
        if (error != SUCCESS){
            free(old_host);
            free(variant);
            free(new_host);
            return ERROR_EOF;
        }
        error = func_change_vertex_data(graph, old_host, new_host, &new_port);
    }
    else if (strcmp(variant, "n") == 0)
        error = func_change_vertex_data(graph, old_host, new_host, NULL);
    free(old_host);
    free(new_host);
    free(variant);
    return error;
}

ErrorCode operation_change_edge_data(Graph* graph){
    if (!graph || !graph->all_vertices || graph->size == 0 || graph->capacity == 0)
        return ERROR_EMPTY_GRAPH;
    char* hostname1 = readline("Enter the hostname 1: ");
    if (hostname1 == NULL)
        return ERROR_EOF;
    char* hostname2 = readline("Enter the hostname 2: ");
    if (hostname2 == NULL){
        free(hostname1);
        return ERROR_EOF;;
    }
    size_t* array = NULL;
    size_t size = 0;
    ErrorCode error = ports_entring(&array, &size);
    if (error != SUCCESS){
        free(hostname1);
        free(hostname2);
        return error;
    }
    
    error = func_change_edge_data(graph,array, size, hostname1, hostname2);
    free(hostname1);
    free(hostname2);
    free(array);
    return error;
}


ErrorCode operation_delete_vertex(Graph* graph){
    if (!graph || !graph->all_vertices || graph->size == 0 || graph->capacity == 0)
        return ERROR_EMPTY_GRAPH;
    char* hostname = readline("Enter the vertex you want to delete: ");
    if (hostname == NULL)
        return ERROR_EOF;
    ErrorCode error = func_delete_vertex(graph, hostname);
    free(hostname);
    return error;
}

ErrorCode operation_delete_edge(Graph* graph){
    if (!graph || !graph->all_vertices || graph->size == 0 || graph->capacity == 0)
        return ERROR_EMPTY_GRAPH;
    char* hostname1 = readline("Enter 1st hostname: ");
    if (hostname1 == NULL)
        return ERROR_EOF;
    char* hostname2 = readline("Enter 2nd hostname: ");
    if (hostname2 == NULL){
        free(hostname1);
        return ERROR_EOF;
    }
    ErrorCode error = func_delete_edge(graph, hostname1, hostname2);
    free(hostname1);
    free(hostname2);
    return error;
}


ErrorCode operation_DFS(const Graph* graph){
    if (!graph || !graph->all_vertices || graph->size == 0 || graph->capacity == 0)
        return ERROR_EMPTY_GRAPH;
    char* start_hostname = readline("Enter the start hostname: ");
    if (start_hostname == NULL)
        return ERROR_EOF;
    fprintf(stdout, "Enter the required service: ");
    size_t target_port = 0;
    ErrorCode error = get_size_t(&target_port);
    if (error != SUCCESS){
        free(start_hostname);
        return ERROR_EOF;
    }
    DFS* result = NULL;
    error = func_DFS(graph, start_hostname, target_port, &result);
    free(start_hostname);
    if (error == SUCCESS)
        output_bool_array(graph, result);
    erase_DFS(result, graph->size);
    return error;
}


ErrorCode operation_dijkstra(const Graph* graph){
    if (!graph || !graph->all_vertices || graph->size == 0 || graph->capacity == 0)
        return ERROR_EMPTY_GRAPH;
    char* start_host = readline("Enter the start vertex hostname: ");
    if (!start_host)
        return ERROR_EOF;
    char* end_host = readline("Enter the end vertex hostname: ");
    if (!end_host){
        free(start_host);
        return ERROR_EOF;
    }
    size_t requared_port;
    fprintf(stdout, "Enter the requared port: ");
    ErrorCode error = get_size_t(&requared_port);
    if (error != SUCCESS){
        free(start_host);
        free(end_host);
        return error;
    }
    size_t path_lenght = 0;
    char** path = NULL;
    error = func_dijkstra(graph, start_host, end_host, requared_port, &path, &path_lenght);
    if (error == SUCCESS){
        output_path(path_lenght, path);
    }
    free(start_host);
    free(end_host);
    for (size_t i = 0; i < path_lenght; i++)
        free(path[i]);
    free(path);
    return error;
}


ErrorCode operation_MST(const Graph* graph){
    if (!graph || !graph->all_vertices || graph->size == 0 || graph->capacity == 0)
        return ERROR_EMPTY_GRAPH;
    fprintf(stdout, "Enter the requared port: ");
    size_t required_port = 0;
    ErrorCode error = get_size_t(&required_port);
    if (error != SUCCESS)
        return error;
    
    Graph* result_graph = NULL;
    error = func_find_MST(graph, required_port, &result_graph);
    if (error == SUCCESS)
        func_output_to_graphviz(result_graph);
    if (result_graph)
        erase_graph(result_graph);
    return error;
}