#include "CTRBTree.h"

void _printNode(struct CTRBTreeNode *node);

void _rotateLL(struct CTRBTreeNode *node);
void _rotateRR(struct CTRBTreeNode *node);
void _rotateLR(struct CTRBTreeNode *node);
void _rotateRL(struct CTRBTreeNode *node);

struct CTRBTreeNode * findCTRBTreeNode(uint64_t key, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *foundNode = root->rootNode;
    bool founded = false;
    while (true) {
        if (foundNode == NULL) {
            break;
        }
        if (foundNode->key == key) {
            founded = true;
            break;
        }
        foundNode = foundNode->childNode[(key > foundNode->key)?1:0];
    }

    if (!founded) {
        foundNode = NULL;
    }

    return foundNode;
}

void deleteCTRBTreeNode(uint64_t key, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *nodeToDelete = findCTRBTreeNode(key, root);
    if (nodeToDelete == NULL) {
        return;
    } else {
        struct CTRBTreeNode *parent = nodeToDelete->parent;
        if (parent != NULL) {
            int childNodeIndex = (nodeToDelete->key > parent->key)?1:0;
            parent->childNode[childNodeIndex] = nodeToDelete->childNode[childNodeIndex];
            insertCTRBTreeNode(nodeToDelete->childNode[(nodeToDelete->key > parent->key)?0:1], root);
        } else {
            root->isEmpty = true;
            root->isEmpty = true;
            root->rootNode = NULL;
        }
        free(nodeToDelete);
    }
}

void insertCTRBTreeNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    if (node == NULL) {
        return;
    }

    if (root->isEmpty) {
        root->rootNode = node;
        root->isEmpty = false;
        return;
    }

    struct CTRBTreeNode *positionNode = root->rootNode;
    int childNodeIndex = 0;
    while (true) {
        childNodeIndex = (node->key > positionNode->key)?1:0;
        if (positionNode->childNode[childNodeIndex] == NULL) {
            positionNode->childNode[childNodeIndex] = node;
            break;
        } else {
            positionNode = positionNode->childNode[childNodeIndex];
        }
    }

    positionNode = node;
    while (true) {

    }
}

void _printNode(struct CTRBTreeNode *node)
{
    printf("\n===========================\n");
    printf("node->key = %llu", node->key);
    printf("\n===========================\n");
}

void cleanTree(struct CTRBTreeRoot *root, bool shouldPrint)
{
    struct CTRBTreeNode * nodeStack[50];
    int stackIndex = 0;
    struct CTRBTreeNode *node = root->rootNode;
    struct CTRBTreeNode *childNode = NULL;
    while (true) {
        if (node == NULL) {
            if (stackIndex == 0) {
                break;
            }
            stackIndex--;
            node = nodeStack[stackIndex];
        }

        if (shouldPrint) {
            _printNode(node);
        }

        if (node->childNode[1] != NULL) {
            nodeStack[stackIndex] = node->childNode[1];
            stackIndex++;
        }

        childNode = node->childNode[0];
        free(node);
        node = childNode;
    }
}

/*********** private methods ***********/

/*
 *
 *   aNode (node)
 *     \
 *      \                       bNode
 *     bNode       ----\        /   \
 *        \        ----/       /     \
 *         \                aNode   cNode
 *        cNode
 *
 * */
void _rotateLL(struct CTRBTreeNode *node)
{
    struct CTRBTreeNode *aNode = node;
    struct CTRBTreeNode *bNode = node->childNode[1];
    struct CTRBTreeNode *parentNode = node->parent;

    parentNode->childNode[(parentNode->key > aNode->key)?0:1] = bNode;
    aNode->childNode[1] = bNode->childNode[0];
    bNode->childNode[0] = aNode;
}

/*
 *
 *
 *         aNode
 *          /
 *         /                   bNode
 *      bNode      ----\       /   \
 *       /         ----/      /     \
 *      /                  cNode   aNode
 *   cNode
 *
 * */
void _rotateRR(struct CTRBTreeNode *node)
{
    struct CTRBTreeNode *aNode = node;
    struct CTRBTreeNode *bNode = node->childNode[0];
    struct CTRBTreeNode *parentNode = node->parent;

    parentNode->childNode[(parentNode->key > aNode->key)?0:1] = bNode;
    aNode->childNode[0] = bNode->childNode[1];
    aNode->childNode[0] = aNode;
}

/*
 *
 *   aNode
 *      \
 *       \                     cNode
 *      bNode     ----\        /   \
 *       /        ----/       /     \
 *      /                  aNode   bNode
 *   cNode
 *
 * */
void _rotateLR(struct CTRBTreeNode *node)
{
    _rotateRR(node->childNode[1]);
    _rotateLL(node);
}

/*
 *
 *      aNode
 *       /
 *      /                     bNode
 *   bNode       ----\        /   \
 *      \        ----/       /     \
 *       \                cNode   aNode
 *      cNode
 *
 * */
void _rotateRL(struct CTRBTreeNode *node)
{
    _rotateLL(node->childNode[0]);
    _rotateRR(node);
}
