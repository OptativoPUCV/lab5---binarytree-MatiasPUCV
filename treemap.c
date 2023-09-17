#include "treemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
  Pair *pair;
  TreeNode *left;
  TreeNode *right;
  TreeNode *parent;
};

struct TreeMap {
  TreeNode *root;
  TreeNode *current;
  int (*lower_than)(void *key1, void *key2);
};

int is_equal(TreeMap *tree, void *key1, void *key2) {
  if (tree->lower_than(key1, key2) == 0 && tree->lower_than(key2, key1) == 0)
    return 1;
  else
    return 0;
}

TreeNode *createTreeNode(void *key, void *value) {
  TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));
  if (new == NULL)
    return NULL;
  new->pair = (Pair *)malloc(sizeof(Pair));
  new->pair->key = key;
  new->pair->value = value;
  new->parent = new->left = new->right = NULL;
  return new;
}

TreeMap *createTreeMap(int (*lower_than)(void *key1, void *key2)) {

  TreeMap *new = malloc(sizeof(TreeMap));
  new->root = NULL;
  new->current = NULL;

  new->lower_than = lower_than;
  
  return new;
}

TreeNode* searchTreeNode(TreeMap* tree, TreeNode* node, void* key)
{
  if (node == NULL)
    return NULL;

  if (is_equal(tree, node->pair->key, key) == 1)
    return node;

  if (tree->lower_than(node->pair->key, key) == 1)
    return searchTreeNode(tree, node->left, key);

  if (tree->lower_than(node->pair->key, key) != 1)
    return searchTreeNode(tree, node->right, key);

  // Error
  return NULL;
}

void insertNode(TreeMap* tree, TreeNode* node, TreeNode* new)
{
  if (node == NULL)
    node = new;

  if (tree->lower_than(node->pair->key < new->pair->key) == 1)
    insertNode(tree, node->left, new);

  if (tree->lower_than(node->pair->key, new->pair->key) != 1)
    insertNode(tree, node->right, new);
}

void insertTreeMap(TreeMap *tree, void *key, void *value)
{
  if (tree == NULL)
    return;
  
  TreeNode* temp = searchTreeNode(tree, tree->root, key);
  if(temp == NULL)
    return;

  TreeNode* new = createTreeNode(key, value);

  insertNode(tree, tree->root, new);
  tree->current = new;

}

TreeNode *minimum(TreeNode *x) { return NULL; }

void removeNode(TreeMap *tree, TreeNode *node) {}

void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || tree->root == NULL)
    return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key)
{
  if (tree == NULL)
    return  NULL;

  TreeNode* result = searchTreeNode(tree, tree->root, key);
  tree->current = result;

  if(result == NULL)
    return NULL;
  
  return result->pair;
}

Pair *upperBound(TreeMap *tree, void *key) { return NULL; }

Pair *firstTreeMap(TreeMap *tree) { return NULL; }

Pair *nextTreeMap(TreeMap *tree) { return NULL; }
