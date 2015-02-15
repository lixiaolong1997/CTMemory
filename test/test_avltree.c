#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "greatest.h"
#include "CTAVLTree.h"

static struct CTAVLTreeRoot *__sharedRoot;

void _createCTAVLTreeRoot(void);
struct CTAVLTreeRoot *_sharedAVLTreeRoot(void);
struct CTAVLTreeNode * createNode(uint64_t key);
static int compare(void *value1, void *value2);

SUITE(test_avltree);

TEST test_insertAVLTreeNode()
{
    struct CTAVLTreeRoot *root = _sharedAVLTreeRoot();
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
    /* struct CTAVLTreeNode *testNode = createNode(4); */
    /* testNode = createNode(7); */
    /* insertCTAVLTreeNode(testNode, root); */
    /* testNode = createNode(10); */
    /* insertCTAVLTreeNode(testNode, root); */
    /* testNode = createNode(9); */
    /* insertCTAVLTreeNode(testNode, root); */

    /* ASSERT((uint64_t)root->rootNode->key == 9); */
    /* ASSERT((uint64_t)root->rootNode->childNode[0]->key == 7); */
    /* ASSERT((uint64_t)root->rootNode->childNode[1]->key == 10); */

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

    struct CTAVLTreeNode *nodeToTest = NULL;

    /*
     *
     * 4
     *
     * */
    struct CTAVLTreeNode *node = createNode(4);
    insertCTAVLTreeNode(node, root);
    ASSERT((uint64_t)root->rootNode->key == 4);

    /* 
     *   4
     *  /
     * 2
     *
     * */
    node = createNode(2);
    insertCTAVLTreeNode(node, root);
    nodeToTest = root->rootNode->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 2);
    ASSERT((uint64_t)root->rootNode->key == 4);

    /*
     *
     *   2
     *  / \
     * 1   4
     *
     * */
    node = createNode(1);
    insertCTAVLTreeNode(node, root);
    ASSERT((uint64_t)root->rootNode->key == 2);
    nodeToTest = root->rootNode->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 4);

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
    insertCTAVLTreeNode(node, root);
    ASSERT((uint64_t)root->rootNode->key == 2);
    nodeToTest = root->rootNode->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 4);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 3);

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
    insertCTAVLTreeNode(node, root);
    ASSERT((uint64_t)root->rootNode->key == 2);
    nodeToTest = root->rootNode->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 4);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 3);
    nodeToTest = root->rootNode->childNode[1]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 6);

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
    insertCTAVLTreeNode(node, root);
    ASSERT((uint64_t)root->rootNode->key == 4);
    nodeToTest = root->rootNode->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 2);
    nodeToTest = root->rootNode->childNode[0]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[0]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 3);

    nodeToTest = root->rootNode->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 6);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 5);


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
    insertCTAVLTreeNode(node, root);
    ASSERT((uint64_t)root->rootNode->key == 4);
    nodeToTest = root->rootNode->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 2);
    nodeToTest = root->rootNode->childNode[0]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[0]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 3);

    nodeToTest = root->rootNode->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 6);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 5);
    nodeToTest = root->rootNode->childNode[1]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 7);



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
    insertCTAVLTreeNode(node, root);
    node = createNode(9);
    insertCTAVLTreeNode(node, root);

    ASSERT((uint64_t)root->rootNode->key == 4);
    nodeToTest = root->rootNode->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 2);
    nodeToTest = root->rootNode->childNode[0]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[0]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 3);

    nodeToTest = root->rootNode->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 6);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 5);
    nodeToTest = root->rootNode->childNode[1]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 9);
    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 7);
    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 10);


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
    insertCTAVLTreeNode(node, root);

    nodeToTest = root->rootNode->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 9);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 6);
    nodeToTest = root->rootNode->childNode[1]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 10);
    nodeToTest = root->rootNode->childNode[1]->childNode[0]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 5);
    nodeToTest = root->rootNode->childNode[1]->childNode[0]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 7);
    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 50);

    node = createNode(60);
    insertCTAVLTreeNode(node, root);
    node = createNode(70);
    insertCTAVLTreeNode(node, root);
    node = createNode(80);
    insertCTAVLTreeNode(node, root);
    node = createNode(90);
    insertCTAVLTreeNode(node, root);
    node = createNode(100);
    insertCTAVLTreeNode(node, root);
    node = createNode(75);
    insertCTAVLTreeNode(node, root);
    node = createNode(78);
    insertCTAVLTreeNode(node, root);

    nodeToTest = root->rootNode;
    ASSERT((uint64_t)nodeToTest->key == 9);

    nodeToTest = root->rootNode->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 4);
    nodeToTest = root->rootNode->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 70);

    nodeToTest = root->rootNode->childNode[0]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 2);
    nodeToTest = root->rootNode->childNode[0]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 6);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 50);
    nodeToTest = root->rootNode->childNode[1]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 90);

    nodeToTest = root->rootNode->childNode[0]->childNode[0]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[0]->childNode[0]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 3);
    nodeToTest = root->rootNode->childNode[0]->childNode[1]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 5);
    nodeToTest = root->rootNode->childNode[0]->childNode[1]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 7);
    nodeToTest = root->rootNode->childNode[1]->childNode[0]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 10);
    nodeToTest = root->rootNode->childNode[1]->childNode[0]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 60);
    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 78);
    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 100);

    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[0]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 75);
    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[0]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 80);

    PASS();
}

TEST test_deleteAVLTreeNode()
{
    struct CTAVLTreeRoot *root = _sharedAVLTreeRoot();
    struct CTAVLTreeNode *nodeToTest = NULL;
    /*
     *
     * before:
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
     *
     *
     * after:
     *
     *             9
     *       /           \
     *      4            70
     *    /   \       /      \
     *   2     6     50      78
     *  / \   / \   /  \    /  \
     * 1   3 5   7 10  60  75  90
     *                         /
     *                        80
     * */

    deleteCTAVLTreeNode((void *)100, root);

    nodeToTest = root->rootNode;
    ASSERT((uint64_t)nodeToTest->key == 9);

    nodeToTest = root->rootNode->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 4);
    nodeToTest = root->rootNode->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 70);

    nodeToTest = root->rootNode->childNode[0]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 2);
    nodeToTest = root->rootNode->childNode[0]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 6);
    nodeToTest = root->rootNode->childNode[1]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 50);
    nodeToTest = root->rootNode->childNode[1]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 78);

    nodeToTest = root->rootNode->childNode[0]->childNode[0]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 1);
    nodeToTest = root->rootNode->childNode[0]->childNode[0]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 3);
    nodeToTest = root->rootNode->childNode[0]->childNode[1]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 5);
    nodeToTest = root->rootNode->childNode[0]->childNode[1]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 7);
    nodeToTest = root->rootNode->childNode[1]->childNode[0]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 10);
    nodeToTest = root->rootNode->childNode[1]->childNode[0]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 60);
    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 75);
    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[1];
    ASSERT((uint64_t)nodeToTest->key == 90);

    nodeToTest = root->rootNode->childNode[1]->childNode[1]->childNode[1]->childNode[0];
    ASSERT((uint64_t)nodeToTest->key == 80);

    cleanTree(root, true);

    PASS();
}

TEST test_findAVLTreeNode()
{
    PASS();
}


SUITE(test_avltree) {
    RUN_TEST(test_insertAVLTreeNode);
    RUN_TEST(test_deleteAVLTreeNode);
    RUN_TEST(test_findAVLTreeNode);
}

/******************** private methods ********************/
static int compare(void *value1, void *value2)
{
    uint64_t v1 = (uint64_t)value1;
    uint64_t v2 = (uint64_t)value2;

    int result = v1 - v2;

    if (result > 0) {
        return CTAVLCMPRESULT_ASC;
    }
    if (result < 0) {
        return CTAVLCMPRESULT_DEC;
    }
    if (result == 0) {
        return CTAVLCMPRESULT_EQL;
    }

    return 0;
}

struct CTAVLTreeNode * createNode(uint64_t key)
{
    struct CTAVLTreeNode *node = (struct CTAVLTreeNode *)malloc(sizeof(struct CTAVLTreeNode));
    node->key = (void *)key;
    node->parent = NULL;
    node->childNode[0] = NULL;
    node->childNode[1] = NULL;
    node->value = NULL;
    return node;
}

void _createCTAVLTreeRoot()
{
    __sharedRoot = (struct CTAVLTreeRoot *)malloc(sizeof(struct CTAVLTreeRoot));
    __sharedRoot->rootNode = NULL;
    __sharedRoot->compare = compare;
}

struct CTAVLTreeRoot *_sharedAVLTreeRoot()
{
    if (__sharedRoot == NULL) {
        static pthread_once_t onceToken = PTHREAD_ONCE_INIT;
        pthread_once(&onceToken, _createCTAVLTreeRoot);
    }
    return __sharedRoot;
}
