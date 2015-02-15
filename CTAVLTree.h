#ifndef __CTRBTREE_H__
#define __CTRBTREE_H__

#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"

#define CTAVLCMPRESULT_ASC 1
#define CTAVLCMPRESULT_DEC 0
#define CTAVLCMPRESULT_EQL -1

struct CTAVLTreeNode {
    int height;
    struct CTAVLTreeNode *parent;
    struct CTAVLTreeNode *childNode[2];

    void * key;
    void *value;
};

struct CTAVLTreeRoot {
    struct CTAVLTreeNode *rootNode;
    int (* compare)(void * value1, void * value2);
};

struct CTAVLTreeNode * findCTAVLTreeNode(void * key, struct CTAVLTreeRoot *root);
void deleteCTAVLTreeNode(void * key, struct CTAVLTreeRoot *root);
void insertCTAVLTreeNode(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root);

void cleanTree(struct CTAVLTreeRoot *root, bool shouldPrint);

#endif
