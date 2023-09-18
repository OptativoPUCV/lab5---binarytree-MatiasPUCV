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

TreeNode *searchTreeNode(TreeMap *tree, TreeNode *node, void *key) {

  if (node == NULL || is_equal(tree, node->pair->key, key))
    return node;

  if (tree->lower_than(node->pair->key, key))
    return searchTreeNode(tree, node->right, key);

  return searchTreeNode(tree, node->left, key);
}

void insertTreeNode(TreeMap *tree, TreeNode *node, TreeNode *new) {
  if (tree->lower_than(node->pair->key, new->pair->key)) {
    if (node->right == NULL) {
      node->right = new;
      new->parent = node;
      return;
    } else {
      insertTreeNode(tree, node->right, new);
    }
  }

  else if (tree->lower_than(node->pair->key, new->pair->key) != 1) {
    if (node->left == NULL) {
      node->left = new;
      new->parent = node;
      return;
    } else {
      insertTreeNode(tree, node->left, new);
    }
  }
}

void insertTreeMap(TreeMap *tree, void *key, void *value) {
  if (tree == NULL)
    return;

  TreeNode *temp = searchTreeNode(tree, tree->root, key);
  if (temp != NULL)
    return;

  TreeNode *new = createTreeNode(key, value);
  tree->current = new;

  if (tree->root == NULL) {
    tree->root = new;
    return;
  }

  insertTreeNode(tree, tree->root, new);
}

TreeNode *minimum(TreeNode *x) {

  TreeNode *current = x;

  while (current != NULL) {
    if (current->left != NULL)
      current = current->left;
    else
      return current;
  }

  return NULL;
}

void removeNodeHelper(TreeMap *tree, TreeNode *node, void *val) {

  if (node->parent->right == NULL)
    return;

  if (node->parent->left == NULL)
    return;

  if (is_equal(tree, node->parent->right->pair->key, node->pair->key))
    node->parent->right = val;
  else if (is_equal(tree, node->parent->left->pair->key, node->pair->key))
    node->parent->left = val;

  free(node);
}

void removeNode(TreeMap *tree, TreeNode *node) {
  if (node == NULL)
    return;

  if (searchTreeMap(tree, node->pair->key) == NULL)
    return;

  // Sin hijos
  if (node->right == NULL && node->left == NULL)
    removeNodeHelper(tree, node, NULL);

  // Un hijo
  else if (node->right != NULL && node->left == NULL) {
    node->right->parent = node->parent;
    removeNodeHelper(tree, node, node->right);
  } else if (node->right == NULL && node->left != NULL) {
    node->left->parent = node->parent;
    removeNodeHelper(tree, node, node->left);
  }

  // Dos hijos
  else if (node->right != NULL && node->left != NULL) {
    TreeNode *min = minimum(node->right);
    node->pair->key = min->pair->key;
    node->pair->value = min->pair->value;
    removeNode(tree, min);
  }
}

void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || tree->root == NULL)
    return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL)
    return NULL;

  TreeNode *result = searchTreeNode(tree, tree->root, key);
  tree->current = result;

  if (result == NULL)
    return NULL;

  return result->pair;
}

TreeNode *sdearchTreeNode(TreeMap *tree, TreeNode *node, void *key) {

  if (node == NULL || is_equal(tree, node->pair->key, key))
    return node;

  if (tree->lower_than(node->pair->key, key))
    return searchTreeNode(tree, node->right, key);

  return searchTreeNode(tree, node->left, key);
}

Pair *upperBound(TreeMap *tree, void *key) {

  TreeNode *ub = tree->root;
  TreeNode *current = tree->root;

  while (current != NULL)
  {
    if(tree->lower_than(current->pair->key, ub->pair->key))
      ub = current;

    if(is_equal(tree, current->pair->key, key))
      return current->pair;

    else if (tree->lower_than(current->pair->key, key))
      current = current->right;
    else if (tree->lower_than(current->pair->key, key) != 1)
      current = current->left;
    
  }

  return ub->pair;
}

Pair *firstTreeMap(TreeMap *tree) {
  TreeNode *current = tree->root;

  while (current != NULL && current->left != NULL) {
    if (tree->lower_than(current->left->pair->key, current->pair->key))
      current = current->left;
    else {
      return current->pair;
    }
  }

  return current->pair;
}

Pair *nextTreeMap(TreeMap *tree) {
  if (tree == NULL || tree->current == NULL)
    return NULL;

  if (tree->current->right != NULL) {
    TreeNode *result = minimum(tree->current->right);
    tree->current = result;
    return result->pair;
  } else {

    TreeNode *temp = tree->current->parent;
    while (temp != NULL) {
      if (tree->lower_than(tree->current->pair->key, temp->pair->key)) {
        tree->current = temp;
        return temp->pair;
      } else {
        temp = temp->parent;
      }
    }
  }

  return NULL;
}
