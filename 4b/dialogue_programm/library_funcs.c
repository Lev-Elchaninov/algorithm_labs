#include <stdlib.h>
#include <stdio.h>
#include "library.h"
#include "hidden_functions.h"
#include <string.h>
#include <math.h>
#include <stdint.h>



static Node elist_storage = {{0, NULL, 0}, BLACK, NULL, NULL, NULL, NULL};
Node* const ELIST = &elist_storage;

ErrorCode init_tree(LLRB_tree** tree){
    if (*tree == NULL)
        *tree = malloc(sizeof(LLRB_tree));
    if (*tree == NULL)
        return ERROR_MEMORY_ALLOCATION;
    (*tree)->number_of_elements = 0;
    (*tree)->root = NULL;
    return SUCCESS;
}


ErrorCode func_output_to_graphviz(const LLRB_tree* tree){
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
    ErrorCode error = data_array_creation(&data, tree, ALL);
    if (error != SUCCESS){
        fclose(file);
        return error;
    }

    for (size_t i = 0; i < data->capacity; i++) {
        char* colour = (data->storage[i]->colour == RED) ? "#FF0000" : "#000000";
        char* font_colour = "white";
        fprintf(file, "    %zu [label=\"%zu\\n%s\n\", fillcolor=\"%s\", fontcolor=\"%s\", style=filled];\n",
                data->storage[i]->info_fieldes.key,
                data->storage[i]->info_fieldes.key,
                data->storage[i]->info_fieldes.info,
                colour,
                font_colour);
    }

    
    for (size_t i = 0; i < data->capacity; i++){
        if (data->storage[i]->left != ELIST)
            fprintf(file, "    %zu -> %zu [color=\"#B0C4DE\"];\n", data->storage[i]->info_fieldes.key, data->storage[i]->left->info_fieldes.key);
        if (data->storage[i]->right != ELIST)
            fprintf(file, "    %zu -> %zu [color=\"#F08080\"];\n", data->storage[i]->info_fieldes.key, data->storage[i]->right->info_fieldes.key);
        if (data->storage[i]->parent != ELIST)
            fprintf(file, "    %zu -> %zu [color=\"#00000080\"];\n", data->storage[i]->info_fieldes.key, data->storage[i]->parent->info_fieldes.key);
    }
    

    fprintf(file, "}\n");
    fclose(file);

    system("dot -Tpng Binary_tree.dot -o Binary_tree.png");
    system("xdg-open Binary_tree.png");

    erase_data(data);

    return SUCCESS;
}





void erase_data(Data* data){
    if (data != NULL){
        free(data->storage);
        free(data);
    }
}




ErrorCode func_output(const LLRB_tree* tree, FILE* file) {
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;

    Stack stack;
    stack_init(&stack);
    Output_info current;
    current.node = tree->root;
    current.level = 0;
    current.dir = 0;
    ErrorCode error = SUCCESS;
    int process = 0;
    while (process == 0) {
        while (current.node != ELIST) {

            Output_info* copy = malloc(sizeof(Output_info));
            if (copy == NULL) {
                erase_stack(&stack);
                return ERROR_MEMORY_ALLOCATION;
            }
            *copy = current;
            
            error = push(&stack, copy);
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
            if (ptr_node->node->colour == RED)
                fprintf(file, "%zu:R\n", ptr_node->node->info_fieldes.key);
            else if (ptr_node->node->colour == BLACK)
                fprintf(file, "%zu:B\n", ptr_node->node->info_fieldes.key);

            current.node = ptr_node->node->left;
            current.level = ptr_node->level + 1;
            current.dir = 1;
            
            free(ptr_node);
        }
    }
    Data* only_dups = NULL;
    error = data_array_creation(&only_dups, tree, ONLY_DUPLICATES);
    if (error != SUCCESS){
        erase_data(only_dups);
        return error;
    }
    if (only_dups->capacity != 0)
        fprintf(file, "\n\n ==Duplicates list==\n\n");
    for (unsigned i = 0; i < only_dups->capacity; i++){
        Node* ptr = only_dups->storage[i];
        fprintf(file, "• ");
        while (ptr != NULL){
            fprintf(file, "{key = %zu|info = %s|rel = %zu} -> ", 
                ptr->info_fieldes.key, 
                ptr->info_fieldes.info, 
                ptr->info_fieldes.release);
            ptr = ptr->duplicate_next;
        }
        fprintf(file, "\n");
    }
    erase_data(only_dups);
    return SUCCESS;
}







ErrorCode func_load_from_txt(LLRB_tree** tree, const char* filename){
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



void order_array_free(For_order* current){
    for (unsigned i = 0; i < current->capacity; i++)
        free(current->array[i].info);
    free(current->array);
    free(current);
    current = NULL;
}




ErrorCode func_find_key(const LLRB_tree* tree, const size_t key, Node** parent_ptr){
    if (tree == NULL)
        return ERROR_EMPTY_TREE;
    Node* ptr = tree->root;
    while (ptr != ELIST){
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



ErrorCode func_insert(LLRB_tree* tree, const size_t key, const char* info){
    if (tree == NULL)
        return ERROR_EMPTY_TREE;
    if (info == NULL)
        return ERROR_NULL_INFO;
    ErrorCode error = SUCCESS;
    if (tree->root == NULL){
        Node* current = NULL;
        error = node_init(&current, key, info);
        if (error != SUCCESS)
            return error;
        current->colour = BLACK;
        tree->root = current;
        tree->number_of_elements++;
        return SUCCESS;
    }
    Node* parent_ptr = NULL;
    error = func_find_key(tree, key, &parent_ptr);
    Node* current = NULL;
    ErrorCode error_ptr = node_init(&current, key, info);
    if (error_ptr != SUCCESS)
        return error_ptr;
    if (error == ERROR_KEY_MATCH){
        replace_node(&current, &parent_ptr);
        if (parent_ptr == tree->root)
            tree->root = current;
        current->info_fieldes.release = (current->duplicate_next)->info_fieldes.release + 1;
        return SUCCESS;
    }

    
    if (key < parent_ptr->info_fieldes.key)
        parent_ptr->left = current;
    else
        parent_ptr->right = current;
    
    current->parent = parent_ptr;
    current->left = ELIST;
    current->right = ELIST;
    int trigger = 1;
    while (parent_ptr != ELIST && trigger == 1){
        trigger = restoring_the_properties(&parent_ptr, tree);
    }
    tree->number_of_elements++;
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



ErrorCode erase_tree(LLRB_tree* tree){
    if (tree == NULL){
        return ERROR_EMPTY_TREE;
    }
    if (tree->root == NULL){
        free(tree);
        tree = NULL;
        return ERROR_EMPTY_TREE; 
    }
    Data* data = NULL;
    ErrorCode error = data_array_creation(&data, tree, ALL);
    if (error != SUCCESS)
        return error;
    
    for (size_t i = 0; i < data->capacity; i++){
        while (data->storage[i]->duplicate_next != NULL){
            Node* ptr = data->storage[i]->duplicate_next;
            data->storage[i]->duplicate_next = data->storage[i]->duplicate_next->duplicate_next;
            free (ptr->info_fieldes.info);
            free(ptr);
        }
        free(data->storage[i]->info_fieldes.info);
        free(data->storage[i]);
    }
    free(data->storage);
    free(data);
    free(tree);
    tree = NULL;
    return SUCCESS;
}



ErrorCode func_delete(LLRB_tree* tree, const size_t key){
    if (tree == NULL || tree->root == NULL || tree->root == ELIST)
        return ERROR_EMPTY_TREE;
    Node* ptr = NULL;
    ErrorCode error = func_find_key(tree, key, &ptr);
    if (error == ERROR_KEY_NOT_FOUND)
        return error;
    if (ptr->duplicate_next != NULL){
        Node* current = ptr;
        Node* prev = NULL;
        while(current->duplicate_next != NULL){
            prev = current;
            current = current->duplicate_next;
        }
        free(current->info_fieldes.info);
        free(current);
        prev->duplicate_next = NULL;
        tree->number_of_elements--;
        return SUCCESS;
    }
    Node* test = recursive_delete(tree, tree->root, key);
    if (test == ELIST)
        tree->root = NULL;
    tree->number_of_elements--;
    return SUCCESS;
}





ErrorCode func_pre_order(const LLRB_tree* tree, For_order** pre_order_struct, size_t* max_key){
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    (*pre_order_struct) = malloc(sizeof(For_order));
    if (*pre_order_struct == NULL)
        return ERROR_MEMORY_ALLOCATION;
    (*pre_order_struct)->array = malloc(sizeof(Info) * (tree->number_of_elements) * 10);
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
        if (current != ELIST){

            pre_order_insert_in_array(&current, *pre_order_struct, max_key, &i);
        
        }
        if (current != ELIST && max_key != NULL){
            if (current->right != ELIST)
                push(&stack, current->right);
            if (current->left != ELIST)
                push(&stack, current->left);  
        }
        else if(current != ELIST && max_key == NULL){
            push(&stack, current->right);
            push(&stack, current->left);  
        }
    }
    (*pre_order_struct)->capacity = i;
    if (i != 0){
        Info* ptr = realloc((*pre_order_struct)->array, i * sizeof(Info));
        if (ptr == NULL){
            order_array_free(*pre_order_struct);
            return ERROR_MEMORY_ALLOCATION;
        }
        (*pre_order_struct)->array = ptr;
    }
    return SUCCESS;
}


ErrorCode func_search_min_exceeding(const LLRB_tree* tree, const size_t key, Node** result){
    if (tree == NULL || tree->root == NULL)
        return ERROR_EMPTY_TREE;
    Node* maximum = NULL;
    func_find_key(tree, __SIZE_MAX__, &maximum);
    if (maximum->info_fieldes.key <= key)
        return ERROR_KEY_NOT_FOUND;
    Node* ptr = NULL;
    ErrorCode error = func_find_key(tree, key, &ptr);
    //случай 1: ключ совпал и он в левом поддереве, есть родитель (поиск наименьшего ключа в его правом поддереве)
    if (error == ERROR_KEY_MATCH && (ptr->parent) != ELIST && (ptr->parent)->left == ptr){
        error = key_math_in_left_tree_parent_exists(ptr, result);
        return error;
    }
    //случай 2: ключ не совпал и он меньше, чем его родитель (от родителя заходим в правое поддерево и ищем там минимум)
    if (error == ERROR_KEY_NOT_FOUND && ptr->info_fieldes.key > key){
        error = key_not_found_and_less_than_parent(ptr, result);
        return error;
    }
    //случай 3: ключ совпал и это корень (идем в его правое поддерево и ищем там минимум)
    if (error == ERROR_KEY_MATCH && (ptr->parent) == ELIST){
        error = key_match_and_is_root(ptr, result);
        return error;
    }
    //случай 4: ключ совпал и он в правом поддереве его родителя
    if (error == ERROR_KEY_MATCH && (ptr->parent) != ELIST && (ptr->parent)->right == ptr){
        error = key_math_in_right_tree_parent_exists(ptr, result);
        return error;
    }
    //случай 5: ключ не совпал и он должен быть в правом поддереве от своего родителя
    if (error == ERROR_KEY_NOT_FOUND && ptr->info_fieldes.key < key){
        error = key_not_found_and_more_than_parent(ptr, result, key);
        return error;
    }
    return SUCCESS;
}