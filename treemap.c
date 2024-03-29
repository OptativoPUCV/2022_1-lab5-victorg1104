#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap* new = (TreeMap *) malloc(sizeof(TreeMap));
    new->root = new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {

    TreeNode* aux = tree->root;

    while ((aux->left !=  NULL) && (aux->right != NULL))
    {
        if (is_equal(tree, aux->pair->key, key))
            break;
        else if (tree->lower_than(key, aux->pair->key))
        {
            aux = aux->left;
        }        
        else
        {
            aux = aux->right;
        }
    }
    
    if (is_equal(tree, aux->pair->key, key) == 0)
    {
        
        if (aux->right == NULL)
        {
            TreeNode* new = createTreeNode(key, value);
            aux->right = new;
            new->parent = aux;
            tree->current = new;
        }

        if (aux->left == NULL)
        {
            TreeNode* new = createTreeNode(key, value);
            aux->left = new;
            new->parent = aux;
            tree->current = new;
        }
    }
}

TreeNode * minimum(TreeNode * x){

    while (x->left != NULL)
    {
        x = x->left;
    }

    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {

    TreeNode* padre = node->parent;

    if ((node->left == NULL) && (node->right == NULL))
    {
        if (padre->left == node)
        {
            padre->left = NULL;
            free(node);
        }
        else
        {
            padre->right = NULL;
            free(node);
        }
        
    }
    else if ((node->left != NULL) && (node->right != NULL))
    {
        TreeNode* minimo = minimum(node->right);

        node->pair->key = minimo->pair->key;
        node->pair->value = minimo->pair->key;

        removeNode(tree, minimo);
    }    
    else if ((node->left != NULL) || (node->right != NULL))
    {
        if (padre->right == node)
        {
            if(node->right != NULL)
            {
                node->right->parent = padre;
                padre->right = node->right;
                free(node);
            }
            if (node->left != NULL)
            {
                node->left->parent = padre;
                padre->right = node->left;
                free(node);
            }
        }
        else if (padre->left == node)
        {
            if(node->right != NULL)
            {
                node->right->parent = padre;
                padre->left = node->right;
                free(node);
            }
            if(node->left != NULL)
            {
                node->left->parent = padre;
                padre->left = node->left;
                free(node);
            }
        }
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {

    TreeNode* aux = (TreeNode*) malloc(sizeof(TreeNode));
    aux = tree->root;

    while (aux != NULL)
    {
        if(tree->lower_than(key, aux->pair->key))
            aux = aux->left;
        else if (is_equal(tree, key, aux->pair->key))
        {
            tree->current = aux;
            return aux->pair;
        }
        else
            aux = aux->right;
    }
    
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    Pair* ub_node = searchTreeMap(tree, key);
    Pair* aux = firstTreeMap(tree);

    if(ub_node != NULL) return ub_node;
    
    while ((aux != NULL) && (tree->lower_than(aux->key, key)))
    {
       aux = nextTreeMap(tree);
    }

    return aux;
}

Pair * firstTreeMap(TreeMap * tree) {
    tree->current = minimum(tree->root);
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {

    if (tree->current->right != NULL)
    {
        tree->current = minimum(tree->current->right);
        return tree->current->pair;
    }
    else
    {
        TreeNode * padre = tree->current->parent;

        while (tree->lower_than(padre->pair->key, tree->current->pair->key))
        {
            if(is_equal(tree, tree->root->pair->key, padre->pair->key)) return NULL;
            padre = padre->parent;
        }

        tree->current = padre;
        return tree->current->pair;
    }
}
