#ifndef __CTRBTREE_H__
#define __CTRBTREE_H__

#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"

struct CTRBTreeNode {
    struct CTRBTreeNode *left;
    struct CTRBTreeNode *right;
    struct CTRBTreeNode *parent;
    uint64_t key;
    void *value;
};

struct CTRBTreeRoot {
    bool isEmpty;
    struct CTRBTreeNode *rootNode;
};

struct CTRBTreeNode * findCTRBTreeNode(uint64_t key, struct CTRBTreeRoot *root);
void deleteCTRBTreeNode(uint64_t key, struct CTRBTreeRoot *root);
void insertCTRBTreeNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);

#endif
