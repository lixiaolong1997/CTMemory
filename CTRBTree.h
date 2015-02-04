#ifndef __CTRBTREE_H__
#define __CTRBTREE_H__

#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"

struct CTRBTreeNode {
    struct CTRBTreeNode *childNode[2];
    struct CTRBTreeNode *parent;

    uint8_t height;
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

void cleanTree(struct CTRBTreeRoot *root, bool shouldPrint);

#endif
