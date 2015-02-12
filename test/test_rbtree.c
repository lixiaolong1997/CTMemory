#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "greatest.h"
#include "CTRBTree.h"

static struct CTRBTreeRoot *__sharedRoot;

void _createCTRBTreeRoot(void);
struct CTRBTreeRoot *_sharedRBTreeRoot(void);
struct CTRBTreeNode * createNode(uint64_t key);

SUITE(test_rbtree);

TEST test_findRBTreeNode()
{
    /* struct CTRBTreeNode *node = NULL; */

    /* struct CTRBTreeRoot *root = _sharedRBTreeRoot(); */
    /* ASSERT(root != NULL); */

    /* node = findCTRBTreeNode(2, root); */
    /* ASSERT(node == NULL); */

    /* node = (struct CTRBTreeNode *)malloc(sizeof(struct CTRBTreeNode)); */
    /* node->key = 2; */
    /* node->value = (void *)"test string"; */
    /* insertCTRBTreeNode(node, root); */

    /* struct CTRBTreeNode *foundNode = findCTRBTreeNode(2, root); */
    /* ASSERT(foundNode != NULL); */
    /* ASSERT(node == foundNode); */

    PASS();
}

TEST test_deleteRBTreeNode()
{
    /* struct CTRBTreeRoot *root = _sharedRBTreeRoot(); */
    /* ASSERT(root != NULL); */

    /* deleteCTRBTreeNode(3, root); */

    /* struct CTRBTreeNode *foundNode = NULL; */

    /* struct CTRBTreeNode *node = (struct CTRBTreeNode *)malloc(sizeof(struct CTRBTreeNode)); */
    /* node->key = 4; */
    /* node->value = (void *)"test string"; */

    /* insertCTRBTreeNode(node, root); */
    /* foundNode = findCTRBTreeNode(4, root); */
    /* ASSERT(foundNode != NULL); */

    /* deleteCTRBTreeNode(4, root); */
    /* foundNode = findCTRBTreeNode(4, root); */
    /* ASSERT(foundNode == NULL); */

    PASS();
}

TEST test_insertRBTreeNode()
{
    struct CTRBTreeRoot *root = _sharedRBTreeRoot();
    cleanTree(root, false);
    root->rootNode = NULL;

    /*
     * 7 10 9
     *
     *  7
     *   \            9
     *   10  ---->   / \
     *   /          7  10
     *  9
     *
     * */
    /* struct CTRBTreeNode *testNode = createNode(4); */
    /* testNode = createNode(7); */
    /* insertCTRBTreeNode(testNode, root); */
    /* testNode = createNode(10); */
    /* insertCTRBTreeNode(testNode, root); */
    /* testNode = createNode(9); */
    /* insertCTRBTreeNode(testNode, root); */

    /* ASSERT(root->rootNode->key == 9); */
    /* ASSERT(root->rootNode->childNode[0]->key == 7); */
    /* ASSERT(root->rootNode->childNode[1]->key == 10); */

    /* PASS(); */

    /*
     * 4 2 1 3 6 5 7 should be
     *
     *      4
     *    /   \
     *   2     6
     *  / \   / \
     * 1   3 5   7
     *
     * */

    struct CTRBTreeNode *nodeToTest = NULL;

    /*
     *
     * 4
     *
     * */
    struct CTRBTreeNode *node = createNode(4);
    insertCTRBTreeNode(node, root);
    ASSERT(root->rootNode->key == 4);

    /* 
     *   4
     *  /
     * 2
     *
     * */
    node = createNode(2);
    insertCTRBTreeNode(node, root);
    nodeToTest = root->rootNode->childNode[0];
    ASSERT(nodeToTest->key == 2);
    ASSERT(root->rootNode->key == 4);

    /*
     *
     *   2
     *  / \
     * 1   4
     *
     * */
    node = createNode(1);
    insertCTRBTreeNode(node, root);
    ASSERT(root->rootNode->key == 2);
    nodeToTest = root->rootNode->childNode[0];
    ASSERT(nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[1];
    ASSERT(nodeToTest->key == 4);

    /*
     *
     *    2
     *   / \
     *  1   4
     *     /
     *    3
     *
     * */
    node = createNode(3);
    insertCTRBTreeNode(node, root);
    ASSERT(root->rootNode->key == 2);
    nodeToTest = root->rootNode->childNode[0];
    ASSERT(nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[1];
    ASSERT(nodeToTest->key == 4);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT(nodeToTest->key == 3);

    /*
     *
     *    2
     *   / \
     *  1   4
     *     / \
     *    3   6
     *
     * */
    node = createNode(6);
    insertCTRBTreeNode(node, root);
    ASSERT(root->rootNode->key == 2);
    nodeToTest = root->rootNode->childNode[0];
    ASSERT(nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[1];
    ASSERT(nodeToTest->key == 4);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT(nodeToTest->key == 3);
    nodeToTest = root->rootNode->childNode[1]->childNode[1];
    ASSERT(nodeToTest->key == 6);

    /*
     *
     *       4
     *     /   \
     *    2     6
     *   / \   /
     *  1   3 5
     *
     * */
    node = createNode(5);
    insertCTRBTreeNode(node, root);
    ASSERT(root->rootNode->key == 4);
    nodeToTest = root->rootNode->childNode[0];
    ASSERT(nodeToTest->key == 2);
    nodeToTest = root->rootNode->childNode[0]->childNode[0];
    ASSERT(nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[0]->childNode[1];
    ASSERT(nodeToTest->key == 3);

    nodeToTest = root->rootNode->childNode[1];
    ASSERT(nodeToTest->key == 6);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT(nodeToTest->key == 5);


    /*
     *
     *       4
     *     /   \
     *    2     6
     *   / \   / \
     *  1   3 5   7
     *
     * */
    node = createNode(7);
    insertCTRBTreeNode(node, root);
    ASSERT(root->rootNode->key == 4);
    nodeToTest = root->rootNode->childNode[0];
    ASSERT(nodeToTest->key == 2);
    nodeToTest = root->rootNode->childNode[0]->childNode[0];
    ASSERT(nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[0]->childNode[1];
    ASSERT(nodeToTest->key == 3);

    nodeToTest = root->rootNode->childNode[1];
    ASSERT(nodeToTest->key == 6);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT(nodeToTest->key == 5);
    nodeToTest = root->rootNode->childNode[1]->childNode[1];
    ASSERT(nodeToTest->key == 7);



    /*
     * add 10, then add 9, to test LR rotation
     *
     *       4
     *     /   \
     *    2     6
     *   / \   / \
     *  1   3 5   9
     *           / \
     *          7  10
     *
     * */
    node = createNode(10);
    insertCTRBTreeNode(node, root);
    node = createNode(9);
    insertCTRBTreeNode(node, root);

    ASSERT(root->rootNode->key == 4);
    nodeToTest = root->rootNode->childNode[0];
    ASSERT(nodeToTest->key == 2);
    nodeToTest = root->rootNode->childNode[0]->childNode[0];
    ASSERT(nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[0]->childNode[1];
    ASSERT(nodeToTest->key == 3);

    nodeToTest = root->rootNode->childNode[1];
    ASSERT(nodeToTest->key == 6);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT(nodeToTest->key == 5);
    nodeToTest = root->rootNode->childNode[1]->childNode[1];
    ASSERT(nodeToTest->key == 9);
    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[0];
    ASSERT(nodeToTest->key == 7);
    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[1];
    ASSERT(nodeToTest->key == 10);


    /*
     * add 50, 60, 70, 80, 90, 100, 75, 78 to test RL rotation
     *
     *             9
     *       /           \
     *      4            70
     *    /   \       /      \
     *   2     6     50      90
     *  / \   / \   /  \    /  \
     * 1   3 5   7 10  60  78  100
     *                    /  \
     *                   75  80
     *
     * */
    node = createNode(50);
    insertCTRBTreeNode(node, root);

    nodeToTest = root->rootNode->childNode[1];
    ASSERT(nodeToTest->key == 9);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT(nodeToTest->key == 6);
    nodeToTest = root->rootNode->childNode[1]->childNode[1];
    ASSERT(nodeToTest->key == 10);
    nodeToTest = root->rootNode->childNode[1]->childNode[0]->childNode[0];
    ASSERT(nodeToTest->key == 5);
    nodeToTest = root->rootNode->childNode[1]->childNode[0]->childNode[1];
    ASSERT(nodeToTest->key == 7);
    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[1];
    ASSERT(nodeToTest->key == 50);

    node = createNode(60);
    insertCTRBTreeNode(node, root);
    node = createNode(70);
    insertCTRBTreeNode(node, root);
    node = createNode(80);
    insertCTRBTreeNode(node, root);
    node = createNode(90);
    insertCTRBTreeNode(node, root);
    node = createNode(100);
    insertCTRBTreeNode(node, root);
    node = createNode(75);
    insertCTRBTreeNode(node, root);
    node = createNode(78);
    insertCTRBTreeNode(node, root);

    nodeToTest = root->rootNode;
    ASSERT(nodeToTest->key == 9);

    nodeToTest = root->rootNode->childNode[0];
    ASSERT(nodeToTest->key == 4);
    nodeToTest = root->rootNode->childNode[1];
    ASSERT(nodeToTest->key == 70);

    nodeToTest = root->rootNode->childNode[0]->childNode[0];
    ASSERT(nodeToTest->key == 2);
    nodeToTest = root->rootNode->childNode[0]->childNode[1];
    ASSERT(nodeToTest->key == 6);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT(nodeToTest->key == 50);
    nodeToTest = root->rootNode->childNode[1]->childNode[1];
    ASSERT(nodeToTest->key == 90);

    nodeToTest = root->rootNode->childNode[0]->childNode[0]->childNode[0];
    ASSERT(nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[0]->childNode[0]->childNode[1];
    ASSERT(nodeToTest->key == 3);
    nodeToTest = root->rootNode->childNode[0]->childNode[1]->childNode[0];
    ASSERT(nodeToTest->key == 5);
    nodeToTest = root->rootNode->childNode[0]->childNode[1]->childNode[1];
    ASSERT(nodeToTest->key == 7);
    nodeToTest = root->rootNode->childNode[1]->childNode[0]->childNode[0];
    ASSERT(nodeToTest->key == 10);
    nodeToTest = root->rootNode->childNode[1]->childNode[0]->childNode[1];
    ASSERT(nodeToTest->key == 60);
    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[0];
    ASSERT(nodeToTest->key == 78);
    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[1];
    ASSERT(nodeToTest->key == 100);

    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[0]->childNode[0];
    ASSERT(nodeToTest->key == 75);
    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[0]->childNode[1];
    ASSERT(nodeToTest->key == 80);

    PASS();
}

SUITE(test_rbtree) {
    RUN_TEST(test_findRBTreeNode);
    RUN_TEST(test_deleteRBTreeNode);
    RUN_TEST(test_insertRBTreeNode);
}

/******************** private methods ********************/

struct CTRBTreeNode * createNode(uint64_t key)
{
    struct CTRBTreeNode *node = (struct CTRBTreeNode *)malloc(sizeof(struct CTRBTreeNode));
    node->key = key;
    node->parent = NULL;
    node->childNode[0] = NULL;
    node->childNode[1] = NULL;
    node->value = NULL;
    return node;
}

void _createCTRBTreeRoot()
{
    __sharedRoot = (struct CTRBTreeRoot *)malloc(sizeof(struct CTRBTreeRoot));
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
