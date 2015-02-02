#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

#include "greatest.h"
#include "CTRBTree.h"

static struct CTRBTreeRoot *__sharedRoot;

void _createCTRBTreeRoot(void);
struct CTRBTreeRoot *_sharedRBTreeRoot(void);

/*
struct CTRBTreeNode {
    struct CTRBTreeNode *left;
    struct CTRBTreeNode *right;
    uint64_t key;
    void *value;
};

struct CTRBTreeRoot {
    bool isEmpty;
    struct CTRBTreeNode *root;
};

struct CTRBTreeNode * createCTRBTreeNode(uint64_t key, void *value);
struct CTRBTreeNode * findCTRBTreeNode(uint64_t key, struct CTRBTreeRoot *root);
void deleteCTRBTreeNode(uint64_t key, struct CTRBTreeRoot *root);
void insertCTRBTreeNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
void updateCTRBTreeNode(uint64_t oldKey, struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
*/

SUITE(test_rbtree);

TEST test_findRBTreeNode()
{
    struct CTRBTreeNode *node = NULL;

    struct CTRBTreeRoot *root = _sharedRBTreeRoot();
    ASSERT(root != NULL);

    node = findCTRBTreeNode(2, root);
    ASSERT(node == NULL);

    node = (struct CTRBTreeNode *)malloc(sizeof(struct CTRBTreeNode));
    node->key = 2;
    node->value = (void *)"test string";
    insertCTRBTreeNode(node, root);

    struct CTRBTreeNode *foundNode = findCTRBTreeNode(2, root);
    ASSERT(foundNode != NULL);
    ASSERT(node == foundNode);

    PASS();
}

TEST test_deleteRBTreeNode()
{
    struct CTRBTreeRoot *root = _sharedRBTreeRoot();
    ASSERT(root != NULL);

    deleteCTRBTreeNode(3, root);

    struct CTRBTreeNode *foundNode = NULL;

    struct CTRBTreeNode *node = (struct CTRBTreeNode *)malloc(sizeof(struct CTRBTreeNode));
    node->key = 4;
    node->value = (void *)"test string";

    insertCTRBTreeNode(node, root);
    foundNode = findCTRBTreeNode(4, root);
    ASSERT(foundNode != NULL);

    deleteCTRBTreeNode(4, root);
    foundNode = findCTRBTreeNode(4, root);
    ASSERT(foundNode == NULL);

    PASS();
}

TEST test_insertRBTreeNode()
{
    struct CTRBTreeRoot *root = _sharedRBTreeRoot();
    ASSERT(root != NULL);

    struct CTRBTreeNode *heapNode = (struct CTRBTreeNode *)malloc(sizeof(struct CTRBTreeNode));
    heapNode->key = 5;
    heapNode->value = (void *)"test_insertRBTreeNode";

    struct CTRBTreeNode *foundNode = NULL;
    insertCTRBTreeNode(heapNode, root);
    foundNode = findCTRBTreeNode(5, root);
    ASSERT(foundNode != NULL);

    PASS();
}

SUITE(test_rbtree) {
    RUN_TEST(test_findRBTreeNode);
    RUN_TEST(test_deleteRBTreeNode);
    RUN_TEST(test_insertRBTreeNode);
}

/******************** private methods ********************/

void _createCTRBTreeRoot()
{
    __sharedRoot = (struct CTRBTreeRoot *)malloc(sizeof(struct CTRBTreeRoot));
    __sharedRoot->isEmpty = true;
    __sharedRoot->rootNode = NULL;
}

struct CTRBTreeRoot *_sharedRBTreeRoot()
{
    if (__sharedRoot == NULL) {
        static pthread_once_t onceToken = PTHREAD_ONCE_INIT;
        pthread_once(&onceToken, _createCTRBTreeRoot);
    }
    return __sharedRoot;
}
