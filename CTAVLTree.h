#ifndef __CTRBTREE_H__
#define __CTRBTREE_H__

#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"

struct CTAVLTreeNode {
    int height;
    struct CTAVLTreeNode *parent;
    struct CTAVLTreeNode *childNode[2];

    uint64_t key;
    void *value;
};

struct CTAVLTreeRoot {
    struct CTAVLTreeNode *rootNode;
};

struct CTAVLTreeNode * findCTAVLTreeNode(uint64_t key, struct CTAVLTreeRoot *root);
void deleteCTAVLTreeNode(uint64_t key, struct CTAVLTreeRoot *root);
void insertCTAVLTreeNode(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root);

void cleanTree(struct CTAVLTreeRoot *root, bool shouldPrint);

#endif
