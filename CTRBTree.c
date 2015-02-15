#include "CTRBTree.h"

struct CTRBTreeNode * _maxNode(struct CTRBTreeNode *rootNode);

struct CTRBTreeNode * _rotate(struct CTRBTreeNode *node, struct CTRBTreeRoot *root, bool *isRotated);
void _maintain(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);

struct CTRBTreeNode * _rotateLL(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
struct CTRBTreeNode * _rotateRR(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
struct CTRBTreeNode * _rotateLR(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
struct CTRBTreeNode * _rotateRL(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);

int _heightOfNode(struct CTRBTreeNode *node);
int _balanceOfNode(struct CTRBTreeNode *node);
int _numberOfChild(struct CTRBTreeNode *node);
int _nodeIndex(struct CTRBTreeNode *node);
struct CTRBTreeNode * _rightMinNode(struct CTRBTreeNode *node);

struct CTRBTreeNode * _deleteLeafNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
struct CTRBTreeNode * _deleteOneChildNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);
struct CTRBTreeNode * _deleteTwoChildrenNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root);

void _ctNodeFree(struct CTRBTreeNode *node);
void _printNode(struct CTRBTreeNode *node);




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
    struct CTRBTreeNode *iterator = NULL;

    if (numberOfChild == 0) {
        iterator = _deleteLeafNode(nodeToDelete, root);
    }

    if (numberOfChild == 1) {
        iterator = _deleteOneChildNode(nodeToDelete, root);
    }

    if (numberOfChild == 2) {
        iterator = _deleteTwoChildrenNode(nodeToDelete, root);
    }

    _maintain(iterator, root);
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

            node->childNode[0] = NULL;
            node->childNode[1] = NULL;

            _maintain(node, root);
            break;
        } else {
            positionNode = positionNode->childNode[childNodeIndex];
        }
    }
}

void cleanTree(struct CTRBTreeRoot *root, bool shouldPrint)
{
    while (root->rootNode) {
        if (shouldPrint) {
            _printNode(root->rootNode);
        }
        deleteCTRBTreeNode(root->rootNode->key, root);
    }
}

/*********** private methods ***********/

void _maintain(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *iterator = node;

    while (iterator) {
        bool isRotated;
        iterator = _rotate(iterator, root, &isRotated);
        if (isRotated == false) {
            iterator = iterator->parent;
        }
    }
}

struct CTRBTreeNode * _rotate(struct CTRBTreeNode *node, struct CTRBTreeRoot *root, bool *isRotated)
{
    *isRotated = false;
    node->height = _heightOfNode(node);

    struct CTRBTreeNode *newTopNode = node;
    int balance = _balanceOfNode(node);

    if (balance == 2) {

        balance = _balanceOfNode(node->childNode[1]);

        if (balance >= 0) {
            newTopNode = _rotateLL(node, root);
            *isRotated = true;
            return newTopNode;
        }
        if (balance == -1) {
            newTopNode = _rotateLR(node, root);
            *isRotated = true;
            return newTopNode;
        }
    }



    if (balance == -2) {

        balance = _balanceOfNode(node->childNode[0]);

        if (balance == 1) {
            newTopNode = _rotateRL(node, root);
            *isRotated = true;
            return newTopNode;
        }
        if (balance <= 0) {
            newTopNode = _rotateRR(node, root);
            *isRotated = true;
            return newTopNode;
        }
    }
    return newTopNode;
}

struct CTRBTreeNode * _rightMinNode(struct CTRBTreeNode *node)
{
    if (node == NULL) {
        return NULL;
    }

    struct CTRBTreeNode *foundedNode = node;
    while (foundedNode->childNode[0]) {
        foundedNode = foundedNode->childNode[0];
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

    aNode->height = _heightOfNode(aNode);
    bNode->height = _heightOfNode(bNode);

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

    aNode->height = _heightOfNode(aNode);
    bNode->height = _heightOfNode(bNode);

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

    bNode->height = _heightOfNode(bNode);
    cNode->height = _heightOfNode(cNode);

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

    bNode->height = _heightOfNode(bNode);
    cNode->height = _heightOfNode(cNode);

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

int _nodeIndex(struct CTRBTreeNode *node)
{
    int index = 0;
    if (node == node->parent->childNode[1]) {
        index = 1;
    }
    return index;
}

int _heightOfNode(struct CTRBTreeNode *node)
{
    if (node == NULL) {
        return 0;
    }

    struct CTRBTreeNode *leftNode = node->childNode[0];
    struct CTRBTreeNode *rightNode = node->childNode[1];

    int leftHeight = leftNode ? leftNode->height : 0;
    int rightHeight = rightNode ? rightNode->height : 0;

    return ((leftHeight > rightHeight ? leftHeight : rightHeight) + 1);
}

int _balanceOfNode(struct CTRBTreeNode *node)
{
    if (node == NULL) {
        return 0;
    }

    int leftHeight = _heightOfNode(node->childNode[0]);
    int rightHeight = _heightOfNode(node->childNode[1]);
    return rightHeight - leftHeight;
}

void _ctNodeFree(struct CTRBTreeNode *node)
{
    _heightOfNode(node);
}

struct CTRBTreeNode * _deleteLeafNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *parent = node->parent;

    if (parent == NULL) {
        root->rootNode = NULL;
    } else {
        int nodeIndex = _nodeIndex(node);
        parent->childNode[nodeIndex] = NULL;
    }

    _ctNodeFree(node);
    return parent;
}

struct CTRBTreeNode * _deleteOneChildNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *parent = node->parent;

    struct CTRBTreeNode *childNode = node->childNode[0];
    if (childNode == NULL) {
        childNode = node->childNode[1];
    }

    if (parent == NULL) {
        root->rootNode = childNode;
        childNode->parent = NULL;
        _ctNodeFree(node);
        return NULL;
    }

    int nodeIndex = _nodeIndex(node);
    parent->childNode[nodeIndex] = childNode;
    childNode->parent = parent;

    _ctNodeFree(node);
    return childNode;
}

struct CTRBTreeNode * _deleteTwoChildrenNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *parent = node->parent;
    struct CTRBTreeNode *leftNode = node->childNode[0];
    struct CTRBTreeNode *rightNode = node->childNode[1];
    struct CTRBTreeNode *rightMinNode = _rightMinNode(rightNode);

    struct CTRBTreeNode *iteratorNode = NULL;

    if (parent == NULL) {
        root->rootNode = rightMinNode;
    }

    if (rightNode == rightMinNode) {
        rightNode->childNode[0] = leftNode;
        leftNode->parent = rightNode;
        rightNode->parent = parent;
        iteratorNode = rightNode;
    } else {
        iteratorNode = rightMinNode->parent;


        rightMinNode->parent->childNode[_nodeIndex(rightMinNode)] = rightMinNode->childNode[1];
        if (rightMinNode->childNode[1]) {
            rightMinNode->childNode[1]->parent = rightMinNode->parent;
        }

        rightMinNode->childNode[0] = leftNode;
        leftNode->parent = rightMinNode;

        rightMinNode->childNode[1] = rightNode;
        rightNode->parent = rightMinNode;

        rightMinNode->parent = node->parent;
    }

    _ctNodeFree(node);
    return iteratorNode;
}

void _printNode(struct CTRBTreeNode *node)
{
    printf("\n=============\n");
    printf("key is %llu", node->key);
    printf("\n=============\n");
}
