#include "CTRBTree.h"

void _printNode(struct CTRBTreeNode *node);
struct CTRBTreeNode * _maxNode(struct CTRBTreeNode *rootNode);

bool _rotate(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
void _maintain(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);

void _rotateLL(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
void _rotateRR(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
void _rotateLR(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
void _rotateRL(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);

struct CTRBTreeNode * findCTRBTreeNode(uint64_t key, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *foundedNode = root->rootNode;
    bool founded = false;
    while (true) {
        if (foundedNode == NULL) {
            break;
        }
        if (foundedNode->key == key) {
            founded = true;
            break;
        }
        foundedNode = foundedNode->childNode[(key > foundedNode->key)?1:0];
    }

    if (!founded) {
        foundedNode = NULL;
    }

    return foundedNode;
}

void deleteCTRBTreeNode(uint64_t key, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *nodeToDelete = findCTRBTreeNode(key, root);
    if (nodeToDelete == NULL) {
        return;
    }

    struct CTRBTreeNode *parent = nodeToDelete->parent;
    struct CTRBTreeNode *leftMaxNode = _maxNode(nodeToDelete->childNode[0]);
    struct CTRBTreeNode *rightNode = nodeToDelete->childNode[1];

    if (nodeToDelete == parent->childNode[0]) {
        parent->balance++;
        parent->childNode[0] = leftMaxNode ? leftMaxNode : rightNode;
    }
    if (nodeToDelete == parent->childNode[1]) {
        parent->balance--;
        parent->childNode[1] = leftMaxNode ? leftMaxNode : rightNode;
    }

    _rotate(parent, root);
    free(nodeToDelete);
}

void insertCTRBTreeNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    if (node == NULL) {
        return;
    }

    if (root->rootNode == NULL) {
        root->rootNode = node;
        return;
    }

    struct CTRBTreeNode *positionNode = root->rootNode;
    int childNodeIndex = 0;
    while (true) {
        childNodeIndex = (node->key > positionNode->key)?1:0;
        if (positionNode->childNode[childNodeIndex] == NULL) {

            positionNode->childNode[childNodeIndex] = node;
            node->parent = positionNode;

            node->balance = 0;
            node->childNode[0] = NULL;
            node->childNode[1] = NULL;

            _maintain(node, root);
            break;
        } else {
            positionNode = positionNode->childNode[childNodeIndex];
        }
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

/* check balance from leaf */
void _maintain(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *iterator = node;
    while (iterator->parent) {
        if (iterator == iterator->parent->childNode[1]) {
            iterator->parent->balance++;
        }
        if (iterator == iterator->parent->childNode[0]) {
            iterator->parent->balance--;
        }
        iterator = iterator->parent;
        if (_rotate(iterator, root)) {
            break;
        }
    }
}

bool _rotate(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    bool isRotated = false;
    if (node->balance == 2) {
        if (node->childNode[1]->balance == 1) {
            _rotateLL(node, root);
        }
        if (node->childNode[1]->balance == -1) {
            _rotateLR(node, root);
        }
        node->balance = 0;
        node->childNode[1]->balance = 0;
        isRotated = true;
    }

    if (node->balance == -2) {
        if (node->childNode[0]->balance == 1) {
            _rotateRL(node, root);
        }
        if (node->childNode[0]->balance == -1) {
            _rotateRR(node, root);
        }
        node->balance = 0;
        node->childNode[0]->balance = 0;
        isRotated = true;
    }
    return isRotated;
}

struct CTRBTreeNode * _maxNode(struct CTRBTreeNode *rootNode)
{
    if (rootNode == NULL) {
        return NULL;
    }

    struct CTRBTreeNode *foundedNode = rootNode;
    while (foundedNode->childNode[1]) {
        foundedNode = foundedNode->childNode[1];
    }
    return foundedNode;
}

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
void _rotateLL(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *aNode = node;
    struct CTRBTreeNode *bNode = node->childNode[1];
    struct CTRBTreeNode *parentNode = node->parent;

    if (parentNode) {
        parentNode->childNode[(parentNode->key > aNode->key)?0:1] = bNode;
    } else {
        root->rootNode = bNode;
    }

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
void _rotateRR(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *aNode = node;
    struct CTRBTreeNode *bNode = node->childNode[0];
    struct CTRBTreeNode *parentNode = node->parent;

    if (parentNode) {
        parentNode->childNode[(parentNode->key > aNode->key)?0:1] = bNode;
    } else {
        root->rootNode = bNode;
    }

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
void _rotateLR(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    _rotateRR(node->childNode[1], root);
    _rotateLL(node, root);
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
void _rotateRL(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    _rotateLL(node->childNode[0], root);
    _rotateRR(node, root);
}
