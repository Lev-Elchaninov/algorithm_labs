#include <stdlib.h>
#include <stdio.h>
#include "library.h"
#include "hidden_functions.h"
#include <string.h>
#include <math.h>
#include <stdint.h>


ErrorCode init_tree(Bin_tree** tree){//инициализация дерева
    if (*tree == NULL)
        *tree = malloc(sizeof(Bin_tree));
    if (*tree == NULL)
        return ERROR_MEMORY_ALLOCATION;
    (*tree)->number_of_elements = 0;
    (*tree)->root = NULL;
    return SUCCESS;
}


ErrorCode func_insert(Bin_tree* tree, const size_t key, const char* info){
    if (tree == NULL)
        return ERROR_EMPTY_TREE;
    if (info == NULL) 
        return ERROR_NULL_INFO;
    ErrorCode error = SUCCESS;
    if (tree->root == NULL){
        Node* current = NULL;
        error = node_init(&current, key, info);
        if (error != SUCCESS)//ERROR_MEMORY_ALLOCATION
            return error;
        tree->root = current;
        tree->number_of_elements++;
        return SUCCESS;
    }
    Node* parent_ptr = NULL;
    error = func_find_key(tree, key, &parent_ptr);//эта функция будет лежать в hidden
    if (error == ERROR_KEY_MATCH){
        free(parent_ptr->info_fieldes.info);
        parent_ptr->info_fieldes.info = strdup(info);
        return SUCCESS;
    }
    Node* current = NULL;
    error = node_init(&current, key, info);//должна быть в hidden
    if (error != SUCCESS)
        return error;

    
    if (key < parent_ptr->info_fieldes.key)
        parent_ptr->left = current;
    else
        parent_ptr->right = current;
        
    current->parent = parent_ptr;
    current->left = NULL;
    current->right = NULL;
    tree->number_of_elements++;
    return SUCCESS;
}


ErrorCode func_output(const Bin_tree* tree, FILE* file) {
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;

    Stack stack;
    stack_init(&stack);
    Output_info current;
    current.node = tree->root;
    current.level = 0;
    current.dir = 0;
    
    int process = 0;
    while (process == 0) {

        while (current.node != NULL) {

            Output_info* copy = malloc(sizeof(Output_info));
            if (copy == NULL) {
                erase_stack(&stack);
                return ERROR_MEMORY_ALLOCATION;
            }
            *copy = current;
            
            ErrorCode error = push(&stack, copy);
            if (error != SUCCESS) {
                free(copy);
                erase_stack(&stack);
                return error;
            }
            
            current.node = current.node->right;
            current.level++;
            current.dir = 0;
        }
        
        if (empty_stack(&stack))
            process = 1;

        if (process == 0) {
            void* ptr;
            ErrorCode error2 = pop(&stack, &ptr);
            if (error2 != SUCCESS)
                return error2;
                
            Output_info* ptr_node = (Output_info*)ptr;
            
            for (int i = 1; i < ptr_node->level; i++)
                fprintf(file, "│   ");
            
            if (ptr_node->level > 0)
                fprintf(file, ptr_node->dir ? "└── " : "├── ");

            fprintf(file, "%zu\n", ptr_node->node->info_fieldes.key);

            current.node = ptr_node->node->left;
            current.level = ptr_node->level + 1;
            current.dir = 1;
            
            free(ptr_node);
        }
    }
    
    return SUCCESS;
}


ErrorCode func_pre_order(const Bin_tree* tree, For_order** pre_order_struct){
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    (*pre_order_struct) = malloc(sizeof(For_order));
    if (*pre_order_struct == NULL)
        return ERROR_MEMORY_ALLOCATION;
    (*pre_order_struct)->array = malloc(sizeof(Info) * (tree->number_of_elements));
    (*pre_order_struct)->capacity = tree->number_of_elements;
    if ((*pre_order_struct)->array == NULL){
        free(*pre_order_struct);
        return ERROR_MEMORY_ALLOCATION;
    }
    
    Stack stack;
    stack_init(&stack);
    Node* current = tree->root;
    ErrorCode error = push(&stack, current);
    if (error != SUCCESS)
        return error;
    int i = 0;
    while (!empty_stack(&stack)){
        void* ptr = NULL;
        pop(&stack, &ptr);
        
        Node* current = (Node*)ptr;
        
        if (current != NULL){
            (*pre_order_struct)->array[i].info = strdup(current->info_fieldes.info);
            (*pre_order_struct)->array[i].key = current->info_fieldes.key;
            i++;
        }
        if (current != NULL){
            push(&stack, current->right); 
            push(&stack, current->left);  
        }
    }
    return SUCCESS;
}



void order_array_free(For_order* current){
    for (unsigned i = 0; i < current->capacity; i++)
        free(current->array[i].info);
    free(current->array);
    free(current);
    current = NULL;
}

ErrorCode func_pre_order_in_range(const Bin_tree* tree, size_t left_board, size_t right_board, For_order** result){
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    if (left_board > right_board){
        size_t ptr = 0;
        ptr = left_board;
        left_board = right_board;
        right_board = ptr;
    }
    (*result) = malloc(sizeof(For_order));
    if (*result == NULL)
        return ERROR_MEMORY_ALLOCATION;
    (*result)->array = malloc(sizeof(Info) * (tree->number_of_elements));
    (*result)->capacity = tree->number_of_elements;
    if ((*result)->array == NULL){
        free(*result);
        return ERROR_MEMORY_ALLOCATION;
    }
    Stack stack;
    stack_init(&stack);
    Node* current = tree->root;
    ErrorCode error = push(&stack, current);
    if (error != SUCCESS){
        order_array_free(*result);
        return error;
    }
    int i = 0;
    while (!empty_stack(&stack)){
        void* ptr = NULL;
        pop(&stack, &ptr);
        
        Node* current = (Node*)ptr;
        
        if ((current != NULL) && (current->info_fieldes.key < left_board || current->info_fieldes.key > right_board)){
            (*result)->array[i].info = strdup(current->info_fieldes.info);
            (*result)->array[i].key = current->info_fieldes.key;
            i++;
        }
        if (current != NULL){
            push(&stack, current->right);
            push(&stack, current->left);  
        }

    }
    (*result)->capacity = i;
    if ((*result)->capacity != 0){
        Info* ptr = realloc((*result)->array, (*result)->capacity * sizeof(Info));
        if (ptr == NULL){
            order_array_free(*result);
            return ERROR_MEMORY_ALLOCATION;
        }
        (*result)->array = ptr;
    }
    return SUCCESS;
}


ErrorCode func_find_key(const Bin_tree* tree, const size_t key, Node** parent_ptr){
    if (tree == NULL)
        return ERROR_EMPTY_TREE;
    Node* ptr = tree->root;
    while (ptr != NULL){
        (*parent_ptr) = ptr;
        if (key < ptr->info_fieldes.key)
            ptr = ptr->left;
        
        else if (key > ptr->info_fieldes.key)
            ptr = ptr->right;
        
        else if (ptr->info_fieldes.key == key){
            (*parent_ptr) = ptr;
            return ERROR_KEY_MATCH;
        }
    }
    return ERROR_KEY_NOT_FOUND;
}


ErrorCode func_find_most_dif_value(const Bin_tree* tree, const size_t key, Data** data){
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    ErrorCode error = data_init(data);
    if (error != SUCCESS)
        return error;
    Node* lowest_element = NULL;
    func_find_key(tree, 0, &lowest_element);
    Node* highest_element = NULL;
    func_find_key(tree, __SIZE_MAX__, &highest_element);

    size_t delta_w_min = 0;
    if (key > lowest_element->info_fieldes.key)
        delta_w_min = key - lowest_element->info_fieldes.key;
    else
        delta_w_min = lowest_element->info_fieldes.key - key;
    
    size_t delta_w_max = 0;
    if (highest_element->info_fieldes.key > key)
        delta_w_max = highest_element->info_fieldes.key - key;
    else
        delta_w_max = key - highest_element->info_fieldes.key;
    
    if (delta_w_min > delta_w_max){
        (*data)->storage[0] = lowest_element;
        (*data)->size++;
    }
    else if (delta_w_min < delta_w_max){
        (*data)->storage[0] = highest_element;
        (*data)->size++;
    }
    else if (delta_w_min == delta_w_max){
        (*data)->storage[0] = lowest_element;
        (*data)->storage[1] = highest_element;
        (*data)->size = 2;
    }
    return SUCCESS;
}



ErrorCode data_init(Data** data){
    if (*data != NULL && (*data)->storage != NULL)
        return SUCCESS;
    (*data) = malloc(sizeof(Data));
    if (*data == NULL)
        return ERROR_MEMORY_ALLOCATION;
    (*data)->capacity = 2;
    (*data)->size = 0;
    (*data)->storage = malloc(sizeof(Node*) * (*data)->capacity);
    if ((*data)->storage == NULL){
        free(*data);
        return ERROR_MEMORY_ALLOCATION;
    }
    return SUCCESS;
}


ErrorCode func_load_from_txt(Bin_tree** tree, const char* filename){
    FILE* file = fopen(filename, "r");
    if (file == NULL)
        return ERROR_OPENING_FILE;
    if (*tree != NULL){
        erase_tree(*tree);
        (*tree) = NULL;
    }
    ErrorCode error = init_tree(tree);
    if (error != SUCCESS){
        fclose(file);
        return error;
    }
    error = SUCCESS;
    while (error != ERROR_EOF){
        long long key = 0;
        int correct_fields = fscanf(file, "%lld\n", &key);
        if (key < 0 || correct_fields == 0){
            fclose(file);
            return ERROR_NOT_SIZE_T;
        }
        
        if (correct_fields == EOF){
            fclose(file);
            return SUCCESS;
        }
        char* info = NULL;
        error = char_input_txt(file, &info);
        if (error != SUCCESS){
            fclose(file);
            if (error == ERROR_EOF)
                error = SUCCESS;
            return error;
        }
        error = func_insert(*tree, (size_t)key, info);
        if (error != SUCCESS){
            free(info);
            fclose(file);
            if (error == ERROR_EOF)
                error = SUCCESS;
            return error;
        }
        free(info);
    }
    fclose(file);
    return SUCCESS;
}



ErrorCode erase_tree(Bin_tree* tree){
    if (tree == NULL){
        return ERROR_EMPTY_TREE;
    }
    if (tree->root == NULL){
        free(tree);
        tree = NULL;
        return ERROR_EMPTY_TREE; 
    }
    Data* data = NULL;
    ErrorCode error = data_array_creation(&data, tree);
    if (error != SUCCESS)
        return error;
    
    for (size_t i = 0; i < tree->number_of_elements; i++){
        free(data->storage[i]->info_fieldes.info);
        free(data->storage[i]);
    }
    free(data->storage);
    free(data);
    free(tree);
    tree = NULL;
    return SUCCESS;
}



ErrorCode func_delete(Bin_tree* tree, size_t key){//проверено
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    
    Node* parent = NULL;
    ErrorCode error = func_find_key(tree, key, &parent);
    if (error == ERROR_KEY_NOT_FOUND)
        return error;
    Bin_tree* sub_tree = malloc(sizeof(Bin_tree));
    if (sub_tree == NULL)
        return ERROR_MEMORY_ALLOCATION;
    sub_tree->root = parent;
    size_t children = 0;
    if (sub_tree->root->left != NULL)
        children++;
    if (sub_tree->root->right != NULL)
        children++;

    if (sub_tree->root == tree->root && tree->number_of_elements == 1){//если элемент один в дереве
        free(tree->root->info_fieldes.info);
        free(tree->root);
        tree->root = NULL;
        free(sub_tree);
        tree->number_of_elements--;
        return SUCCESS;
    }

    switch(children){
        case(0)://работает
            if ((sub_tree->root->parent)->left == sub_tree->root)
                (sub_tree->root->parent)->left = NULL;
            else
                (sub_tree->root->parent)->right = NULL;
            free(sub_tree->root->info_fieldes.info);
            free(sub_tree->root);
            break;
        case(1)://работает
            Node* child = NULL;
            if (sub_tree->root->left != NULL)
                child = sub_tree->root->left;
            else
                child = sub_tree->root->right;
            child->parent = sub_tree->root->parent;
            if (child->parent != NULL && (sub_tree->root->parent)->left == sub_tree->root)
                (sub_tree->root->parent)->left = child;
            else if (child->parent != NULL)
                (sub_tree->root->parent)->right = child;
            free(sub_tree->root->info_fieldes.info);
            free(sub_tree->root);
            sub_tree->root = NULL;
            if (child->parent == NULL)
                tree->root = child;
            break;
        case(2)://здесь надо найти минимальный элемент из правого поддерева
            Node* current = NULL;
            Node* ptr = sub_tree->root;
            sub_tree->root = sub_tree->root->right;
            error = func_find_key(sub_tree, 0, &current);//поиск минимального в правом поддереве
            sub_tree->root = ptr;
            if ((current->parent)->left == current){
                (current->parent)->left = current->right;
                if (current->right != NULL)
                    (current->right)->parent = current->parent;
            }
            else
                (current->parent)->right = current->right;
            current->parent = sub_tree->root->parent;
            if (current->parent != NULL){
                if ((sub_tree->root->parent)->left != NULL && (sub_tree->root->parent)->left->info_fieldes.key == key)
                    (sub_tree->root->parent)->left = current;
                else 
                    (sub_tree->root->parent)->right = current;
            }
            current->left = sub_tree->root->left;
            if (sub_tree->root->left != NULL)
                (sub_tree->root->left)->parent = current;
            current->right = sub_tree->root->right;
            if (sub_tree->root->right != NULL)
                (sub_tree->root->right)->parent = current;
            free(sub_tree->root->info_fieldes.info);
            free(sub_tree->root);
            sub_tree->root = NULL;
            if (current->parent == NULL)
                tree->root = current;
            break;
    }
    tree->number_of_elements--;
    free(sub_tree);
    return SUCCESS;
}


ErrorCode func_output_to_graphviz(const Bin_tree* tree){
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    FILE* file = fopen("Binary_tree.dot", "w");
    if (file == NULL)
        return ERROR_OPENING_FILE;
    
    fprintf(file, "digraph BinatyTree {\n");
    fprintf(file, "    rankdir=\"TB\"; // Top to Bottom\n");
    fprintf(file, "    node [shape=circle, fontcolor=\"black\", width=1.2];\n");
    fprintf(file, "    edge [penwidth=2];\n");


    Data* data = NULL;
    ErrorCode error = data_array_creation(&data, tree);
    if (error != SUCCESS){
        fclose(file);
        return error;
    }

    for (size_t i = 0; i < data->capacity; i++) {
        fprintf(file, "    %zu [label=\"%zu\\n%s\"];\n", 
                data->storage[i]->info_fieldes.key,
                data->storage[i]->info_fieldes.key,
                data->storage[i]->info_fieldes.info);
    }


    for (size_t i = 0; i < data->capacity; i++){
        if (data->storage[i]->left != NULL)
            fprintf(file, "    %zu -> %zu [color=\"#B0C4DE\"];\n", data->storage[i]->info_fieldes.key, data->storage[i]->left->info_fieldes.key);
        if (data->storage[i]->right != NULL)
            fprintf(file, "    %zu -> %zu [color=\"#F08080\"];\n", data->storage[i]->info_fieldes.key, data->storage[i]->right->info_fieldes.key);
    }



    fprintf(file, "}\n");
    fclose(file);

    system("dot -Tpng Binary_tree.dot -o Binary_tree.png");
    system("xdg-open Binary_tree.png");

    free(data->storage);
    free(data);

    return SUCCESS;
}





ErrorCode data_array_creation(Data** data, const Bin_tree* tree){
    ErrorCode error = data_init(data);
    if (error != SUCCESS)
        return error;
    (*data)->capacity = tree->number_of_elements;
    (*data)->storage = realloc((*data)->storage, tree->number_of_elements * sizeof(Node*));
    

    Stack stack;
    stack_init(&stack);
    Node* current = tree->root;
    error = push(&stack, current);
    if (error != SUCCESS)
        return error;
    int i = 0;
    while (!empty_stack(&stack)){
        void* ptr = NULL;
        error = pop(&stack, &ptr);
        if (error != SUCCESS)
            return error;
        Node* current = (Node*)ptr;

        if (current != NULL){
            (*data)->storage[i] = current;
            if (current->right) push(&stack, current->right); 
            if (current->left) push(&stack, current->left);  
        }
        i++;
    }
    return SUCCESS;
}





void erase_data(Data* data){
    if (data != NULL){
        free(data->storage);
        free(data);
    }
}














