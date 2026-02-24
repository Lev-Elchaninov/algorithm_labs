#include "hidden_functions.h"
#include "stack.h"
#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 1000//для fgets

ErrorCode node_init(Node** current, const size_t key, const char* info){//создание узла дерева
    (*current) = malloc(sizeof(Node));
    if ((*current) == NULL)
        return ERROR_MEMORY_ALLOCATION;
    (*current)->info_fieldes.info = strdup(info);
    (*current)->info_fieldes.key = key;
    (*current)->info_fieldes.release = 1;
    (*current)->colour = RED;
    (*current)->left = ELIST;
    (*current)->right = ELIST;
    (*current)->parent = ELIST;
    (*current)->duplicate_next = NULL;
    return SUCCESS;
}



ErrorCode char_input_txt(FILE* file, char** string){
    (*string) = malloc(MAX_SIZE * sizeof(char)); 
    if (*string == NULL){
        return ERROR_MEMORY_ALLOCATION;
    }

    if (fgets(*string, MAX_SIZE, file) == NULL){
        free(*string);  
        return ERROR_EOF;
    }
    if (*string == NULL)
        return ERROR_EOF;
    
    size_t len = strlen(*string);
    if (len > 0 && (*string)[len-1] == '\n')
        (*string)[len-1] = '\0';
        
    char* ptr_txt = realloc((*string), (strlen((*string)) + 1)); 
    if (ptr_txt == NULL) {
        free((*string));
        return ERROR_MEMORY_ALLOCATION;
    }
    (*string) = ptr_txt;
    return SUCCESS;
}


Node* rotate_right(Node* current, LLRB_tree* tree){
    Node* parent = current;
    Node* left_child = current->left;

    left_child->parent = parent->parent;
    if (parent->parent != ELIST){
        if (parent->parent->left == parent)
            parent->parent->left = left_child;

        else if (parent->parent->right == parent)
            parent->parent->right = left_child;
    }
    parent->left = left_child->right;
    if (left_child->right != ELIST)
        left_child->right->parent = parent;
    left_child->right = parent;
    if (tree->root == parent)
        tree->root = left_child;
    parent->parent = left_child;

    left_child->colour = parent->colour;
    parent->colour = RED;
    return left_child;
}


Node* rotate_left(Node* current, LLRB_tree* tree){
    Node* parent = current;
    Node* right_child = current->right;

    right_child->parent = parent->parent;
    if (parent->parent != ELIST){
        if (parent->parent->left == parent)
            parent->parent->left = right_child;

        else if (parent->parent->right == parent)
            parent->parent->right = right_child;
    }

    parent->right = right_child->left;
    if (right_child->left != ELIST)
        (right_child->left)->parent = parent;
    right_child->left = parent;
    if (tree->root == parent)
        tree->root = right_child;
    parent->parent = right_child;

    right_child->colour = parent->colour;
    parent->colour = RED;
    return right_child;
}



int restoring_the_properties(Node** parent_ptr, LLRB_tree* tree){
    int trigger = 0;
    if ((*parent_ptr) == tree->root && (*parent_ptr)->colour == RED)
        (*parent_ptr)->colour = BLACK;
    //случай 1. Если у родителя - правый - красный и левый - черный
    if ((*parent_ptr)->right->colour == RED && (*parent_ptr)->left->colour == BLACK){
        (*parent_ptr) = rotate_left(*parent_ptr, tree);
        (*parent_ptr) = (*parent_ptr)->left;
        if (tree->root->colour == RED)
            tree->root->colour = BLACK;
        trigger = 1;
    }
    //случай 2. Если есть левый красный и левый красный внук
    Node* temp = NULL;
    if ((*parent_ptr)->parent != ELIST){
        temp = (*parent_ptr);
        (*parent_ptr) = (*parent_ptr)->parent;
    }
    if ((*parent_ptr)->left != ELIST && (*parent_ptr)->left->colour == RED && (*parent_ptr)->left->left->colour == RED){
        (*parent_ptr) = rotate_right(*parent_ptr, tree);
        (*parent_ptr) = (*parent_ptr)->right;
        if (tree->root->colour == RED)
            tree->root->colour = BLACK;
        (*parent_ptr) = (*parent_ptr)->parent;
        trigger = 1;
    }
    else if (temp != NULL)
        (*parent_ptr) = temp;
    //случай 3. Оба потомка - красные
    if ((*parent_ptr)->left->colour == RED && (*parent_ptr)->right->colour == RED){
        colour_flip(*parent_ptr);
        trigger = 1;
    }
    (*parent_ptr) = (*parent_ptr)->parent;
    tree->root->colour = BLACK;
    return trigger;
}


ErrorCode data_array_creation(Data** data, const LLRB_tree* tree, int duplicates_or_all){
    ErrorCode error = data_init(data);
    if (error != SUCCESS)
        return error;
    (*data)->capacity = tree->number_of_elements;
    (*data)->storage = realloc((*data)->storage, 10 * tree->number_of_elements * sizeof(Node*));
    

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

        switch(duplicates_or_all){
            case(ONLY_DUPLICATES):
                if (current != ELIST && current->duplicate_next != NULL){
                    (*data)->storage[i] = current;
                    i++;
                }
                break;
            case(ALL):
                if (current != ELIST){
                    (*data)->storage[i] = current;
                    i++;
                }
                break;
        }
        if (current != ELIST && current->right != ELIST) push(&stack, current->right); 
        if (current != ELIST && current->left != ELIST) push(&stack, current->left); 
    }
    (*data)->capacity = i;
    if (i != 0)
        (*data)->storage = realloc((*data)->storage, (*data)->capacity * sizeof(Node*));
    return SUCCESS;
}



//случай 1: ключ совпал и он в левом поддереве, есть родитель
ErrorCode key_math_in_left_tree_parent_exists(Node* ptr, Node** result){
    if (ptr->right == ELIST)
        (*result) = ptr->parent;
    else if (ptr->right != ELIST)
        *result = min(ptr->right);
    return SUCCESS;
}

//случай 2: ключ не совпал и он меньше, чем его родитель
ErrorCode key_not_found_and_less_than_parent(Node* ptr, Node** result){
    if (ptr->right == ELIST)
        (*result) = ptr;
    else if (ptr->right != ELIST)
        (*result) = min(ptr->right);
    return SUCCESS;
}

//случай 2 или 3: ключ совпал и это корень
ErrorCode key_match_and_is_root(Node* ptr, Node** result){     
    if (ptr->right != ELIST)
        *result = min(ptr->right);
    return SUCCESS;
}

//случай 4: ключ совпал и он в правом поддереве его родителя
ErrorCode key_math_in_right_tree_parent_exists(Node* ptr, Node** result){
    if (ptr->right != ELIST)
        *result = min(ptr->right);
    else if (ptr->right == ELIST){
        Node* up = ptr->parent;
        while (up->parent != ELIST && up->info_fieldes.key < ptr->info_fieldes.key)//поднимаемся до того момента пока родитель не превысит наш ключ
            up = up->parent;
        (*result) = up;
    }
    return SUCCESS;
}


//случай 5: ключ не совпал и он должен быть в правом поддереве от своего родителя
ErrorCode key_not_found_and_more_than_parent(Node* ptr, Node** result, const size_t key){
    Node* up = ptr;
    while (up->parent != ELIST && up->info_fieldes.key < key)//подъем по родителям
        up = up->parent;
    (*result) = up;
    return SUCCESS;
}


void replace_node(Node** new_node, Node** old_node){//для дубликатов
    (*new_node)->left = (*old_node)->left;
    (*new_node)->right = (*old_node)->right;
    (*new_node)->colour = (*old_node)->colour;
    (*new_node)->parent = (*old_node)->parent;
    if (((*old_node)->parent != ELIST) && ((*old_node)->parent)->left == (*old_node))
        ((*old_node)->parent)->left = (*new_node);
    else if(((*old_node)->parent != ELIST) && ((*old_node)->parent)->right == (*old_node))
        ((*old_node)->parent)->right = (*new_node);
    if (((*old_node)->left)->parent != ELIST)
        ((*old_node)->left)->parent = (*new_node);
    if (((*old_node)->right)->parent != ELIST)
        ((*old_node)->right)->parent = (*new_node);
    (*new_node)->duplicate_next = (*old_node);
    (*old_node)->left = ELIST;
    (*old_node)->right = ELIST;
    (*old_node)->parent = NULL;    
}






void pre_order_insert_in_array(Node** current, For_order* data, size_t* max_key, int* i){//функция для декомпозиции
    if (max_key == NULL || (*current)->info_fieldes.key <= *max_key){
        Node* ptr = *current;
        while (ptr != NULL){
            data->array[*i].info = strdup(ptr->info_fieldes.info);
            data->array[*i].key = ptr->info_fieldes.key;
            data->array[*i].release = ptr->info_fieldes.release;
            (*i)++;
            ptr = ptr->duplicate_next;
        }
    }
}

void colour_flip(Node* current){//перекраска если два потомка - красные
    if (current == ELIST)
        return;
    if (current->left->colour == RED)
        current->left->colour = BLACK;
    else if (current->left != ELIST)
        current->left->colour = RED;

    if (current->right->colour == RED)
        current->right->colour = BLACK;
    else if (current->right != ELIST)
        current->right->colour = RED;

    if (current->colour == BLACK)
        current->colour = RED;
    else if (current->colour == RED)
        current->colour = BLACK;
}


Node* move_red_left(Node* h, LLRB_tree* tree){
    if (h == ELIST)
        return h;
    colour_flip(h);
    if (h != ELIST && h->right != ELIST && h->right->left->colour == RED){
        h->right = rotate_right(h->right, tree);
        h = rotate_left(h, tree);
        colour_flip(h);
    }
    return h;
}

Node* move_red_right(Node* h, LLRB_tree* tree){
    colour_flip(h);
    if (h != ELIST && h->left != ELIST && h->left->left->colour == RED){
        h = rotate_right(h, tree);
        colour_flip(h);
    }
    return h;
}


Node* min(Node* h){
    while (h->left != ELIST)
        h = h->left;
    return h;
}

Node* fix_up(Node* h, LLRB_tree* tree){
    if (h == ELIST)
        return h;
    if (h->left->colour == RED && h->right->colour == RED)
        colour_flip(h);
    if (h->right->colour == RED && h->left->colour == BLACK)
        h = rotate_left(h, tree);
    if (h->left->colour == RED && h->left->left->colour == RED)
        h = rotate_right(h, tree);
    tree->root->colour = BLACK;
    return h;
}



Node* delete_min(Node* h, LLRB_tree* tree){
    if (h->left == ELIST){
        free(h->info_fieldes.info);
        free(h);
        return ELIST;
    }
    if (h->left->colour == BLACK && h->left->left->colour == BLACK)
        h = move_red_left(h, tree);
    h->left = delete_min(h->left, tree);
    return fix_up(h, tree);
}




Node* recursive_delete(LLRB_tree* tree, Node* h, const size_t key){
    if (h == ELIST)
        return ELIST;
    if (key < h->info_fieldes.key){
        if (h->left->colour == BLACK && h->left->left->colour == BLACK)
            h = move_red_left(h, tree);
        h->left = recursive_delete(tree, h->left, key);
    }
    else if(key >= h->info_fieldes.key){
        if (h->left->colour == RED)
            h = rotate_right(h, tree);
        if (key == h->info_fieldes.key && h->right == ELIST){
            free(h->info_fieldes.info);
            free(h);
            return ELIST;
        }
        if (h->right->colour == BLACK && h->right->left->colour == BLACK)
            h = move_red_right(h, tree);
        if (key == h->info_fieldes.key){
            Node* minim = min(h->right);
            h->info_fieldes.key = minim->info_fieldes.key;
            free(h->info_fieldes.info);
            h->info_fieldes.info = strdup(minim->info_fieldes.info);
            h->duplicate_next = minim->duplicate_next;
            h->right = delete_min(h->right, tree);
        }
        else
            h->right = recursive_delete(tree, h->right, key);
    }
    return fix_up(h, tree);
}
