#include "CTRBTree.h"

void _printNode(struct CTRBTreeNode *node);
struct CTRBTreeNode * _maxNode(struct CTRBTreeNode *rootNode);

struct CTRBTreeNode * _rotate(struct CTRBTreeNode *node, struct CTRBTreeRoot *root, bool *isRotated);
void _maintain(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);

struct CTRBTreeNode * _rotateLL(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
struct CTRBTreeNode * _rotateRR(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
struct CTRBTreeNode * _rotateLR(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
struct CTRBTreeNode * _rotateRL(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);

int _numberOfChild(struct CTRBTreeNode *node);
int _nodeIndex(struct CTRBTreeNode *node);
struct CTRBTreeNode * _deleteLeafNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
struct CTRBTreeNode * _deleteOneChildNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
struct CTRBTreeNode * _deleteTwoChildrenNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
void ctFreeNode(struct CTRBTreeNode *node);





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

    int numberOfChild = _numberOfChild(nodeToDelete);
    struct CTRBTreeNode *newTopNode = NULL;
    if (numberOfChild == 0) {
        newTopNode = _deleteLeafNode(nodeToDelete, root);
    }
    if (numberOfChild == 1) {
        newTopNode = _deleteOneChildNode(nodeToDelete, root);
    }
    if (numberOfChild == 2) {
        newTopNode = _deleteTwoChildrenNode(nodeToDelete, root);
    }
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

void _maintain(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *iterator = node;

    while (iterator->parent) {
        /*
         * 插入10，9之后，经过一次旋转，应该不需要再往上跑了
         * */
        if (iterator == iterator->parent->childNode[1]) {
            iterator->parent->balance++;
        }
        if (iterator == iterator->parent->childNode[0]) {
            iterator->parent->balance--;
        }

        if (iterator->parent->balance == 0) {
            break;
        }

        bool isRotated;
        iterator = _rotate(iterator->parent, root, &isRotated);
        if (isRotated) {
            break;
        }
    }
}

struct CTRBTreeNode * _rotate(struct CTRBTreeNode *node, struct CTRBTreeRoot *root, bool *isRotated)
{
    *isRotated = false;
    struct CTRBTreeNode *newTopNode = node;
    if (node->balance == 2) {
        if (node->childNode[1]->balance == 1) {
            newTopNode = _rotateLL(node, root);
            *isRotated = true;
            return newTopNode;
        }
        if (node->childNode[1]->balance == -1) {
            newTopNode = _rotateLR(node, root);
            *isRotated = true;
            return newTopNode;
        }
    }

    if (node->balance == -2) {
        if (node->childNode[0]->balance == 1) {
            newTopNode = _rotateRL(node, root);
            *isRotated = true;
            return newTopNode;
        }
        if (node->childNode[0]->balance == -1) {
            newTopNode = _rotateRR(node, root);
            *isRotated = true;
            return newTopNode;
        }
    }
    return newTopNode;
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
 *      \
 *       \             \         bNode
 *      bNode       ----\        /   \
 *         \        ----/       /     \
 *          \          /     aNode   cNode
 *         cNode
 *
 * */
struct CTRBTreeNode * _rotateLL(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *aNode = node;
    struct CTRBTreeNode *bNode = node->childNode[1];
    struct CTRBTreeNode *parentNode = node->parent;

    if (parentNode) {
        parentNode->childNode[(parentNode->key > aNode->key)?0:1] = bNode;
        bNode->parent = parentNode;
    } else {
        root->rootNode = bNode;
        bNode->parent = NULL;
    }

    aNode->childNode[1] = bNode->childNode[0];
    if (aNode->childNode[1]) {
        aNode->childNode[1]->parent = aNode;
    }

    bNode->childNode[0] = aNode;
    aNode->parent = bNode;

    aNode->balance-=2;
    bNode->balance--;

    return bNode;
}

/*
 *
 *
 *         aNode
 *          /
 *         /          \        bNode
 *      bNode      ----\       /   \
 *       /         ----/      /     \
 *      /             /    cNode   aNode
 *   cNode
 *
 * */
struct CTRBTreeNode * _rotateRR(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *aNode = node;
    struct CTRBTreeNode *bNode = node->childNode[0];
    struct CTRBTreeNode *parentNode = node->parent;

    if (parentNode) {
        parentNode->childNode[(parentNode->key > aNode->key)?0:1] = bNode;
        bNode->parent = parentNode;
    } else {
        root->rootNode = bNode;
        bNode->parent = NULL;
    }

    aNode->childNode[0] = bNode->childNode[1];
    if (aNode->childNode[0]) {
        aNode->childNode[0]->parent = aNode;
    }

    bNode->childNode[1] = aNode;
    aNode->parent = bNode;

    aNode->balance+=2;
    bNode->balance++;

    return bNode;
}

/*
 *
 *   aNode
 *      \
 *       \           \         cNode
 *      bNode     ----\        /   \
 *       /        ----/       /     \
 *      /            /     aNode   bNode
 *   cNode
 *
 * */
struct CTRBTreeNode * _rotateLR(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *aNode = node;
    struct CTRBTreeNode *bNode = node->childNode[1];
    struct CTRBTreeNode *cNode = bNode->childNode[0];

    aNode->childNode[1] = cNode;
    cNode->parent = aNode;

    bNode->childNode[0] = cNode->childNode[1];
    if (cNode->childNode[1]) {
        cNode->childNode[1]->parent = bNode;
    }

    cNode->childNode[1] = bNode;
    bNode->parent = cNode;

    bNode->balance++;
    cNode->balance++;

    return _rotateLL(node, root);
}

/*
 *
 *      aNode
 *       /
 *      /           \         bNode
 *   bNode       ----\        /   \
 *      \        ----/       /     \
 *       \          /     cNode   aNode
 *      cNode
 *
 * */
struct CTRBTreeNode * _rotateRL(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *aNode = node;
    struct CTRBTreeNode *bNode = node->childNode[0];
    struct CTRBTreeNode *cNode = bNode->childNode[1];

    aNode->childNode[0] = cNode;
    cNode->parent = aNode;

    bNode->childNode[1] = cNode->childNode[0];
    if (cNode->childNode[0]) {
        cNode->childNode[0]->parent = bNode;
    }

    cNode->childNode[0] = bNode;
    bNode->parent = cNode;

    cNode->balance--;
    bNode->balance--;

    return _rotateRR(node, root);
}

int _numberOfChild(struct CTRBTreeNode *node)
{
    int result = 0;
    if (node->childNode[0] || node->childNode[1]) {
        result++;
        if (node->childNode[0] && node->childNode[1]) {
            result++;
        }
    }
    return result;
}

struct CTRBTreeNode * _deleteLeafNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    if (node->parent == NULL) {
        root->rootNode = NULL;
        ctFreeNode(node);
        return NULL;
    }

    int nodeIndex = _nodeIndex(node);
    node->parent->childNode[nodeIndex] = NULL;
    if (nodeIndex == 1) {
        node->parent->balance--;
    }
    if (nodeIndex == 0) {
        node->parent->balance++;
    }

    ctFreeNode(node);
    return node->parent;
}

struct CTRBTreeNode * _deleteOneChildNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *child = node->childNode[0] ? node->childNode[0] : node->childNode[1];
    struct CTRBTreeNode *parent = node->parent;
    if (parent == NULL) {
        root->rootNode = childNode;
        childNode->parent = NULL;
        ctFreeNode(node);
        return childNode;
    }

    int nodeIndex = _nodeIndex(node);
    child->parent = parent;
    parent->childNode[nodeIndex] = child;

    if (nodeIndex == 1) {
        parent->balance--;
    }
    if (nodeIndex == 0) {
        parent->balance++;
    }

    ctFreeNode(node);
    return node->parent;
}

struct CTRBTreeNode * _deleteTwoChildrenNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
}

int _nodeIndex(struct CTRBTreeNode *node)
{
    int index = 0;
    if (node == node->parent->childNode[1]) {
        index = 1;
    }
    return index;
}

void ctFreeNode(struct CTRBTreeNode *node)
{
}
