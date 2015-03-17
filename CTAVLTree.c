#include "CTAVLTree.h"

struct CTAVLTreeNode * _maxNode(struct CTAVLTreeNode *rootNode);

struct CTAVLTreeNode * _rotate(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root, bool *isRotated);
void _maintain(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root);

struct CTAVLTreeNode * _rotateLL(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root);
struct CTAVLTreeNode * _rotateRR(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root);
struct CTAVLTreeNode * _rotateLR(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root);
struct CTAVLTreeNode * _rotateRL(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root);

int _heightOfNode(struct CTAVLTreeNode *node);
int _balanceOfNode(struct CTAVLTreeNode *node);
int _numberOfChild(struct CTAVLTreeNode *node);
int _nodeIndex(struct CTAVLTreeNode *node);
struct CTAVLTreeNode * _rightMinNode(struct CTAVLTreeNode *node);

struct CTAVLTreeNode * _deleteLeafNode(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root);
struct CTAVLTreeNode * _deleteOneChildNode(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root);
struct CTAVLTreeNode * _deleteTwoChildrenNode(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root);

void _ctNodeFree(struct CTAVLTreeNode *node);
void _printNode(struct CTAVLTreeNode *node);




struct CTAVLTreeNode * findCTAVLTreeNode(void * key, struct CTAVLTreeRoot *root)
{
    struct CTAVLTreeNode *foundedNode = root->rootNode;
    bool founded = false;
    while (true) {
        if (foundedNode == NULL) {
            break;
        }

        int result;
        if (root->compare == NULL) {
            if ((uint64_t)key > (uint64_t)foundedNode->key) {
                result = CTAVLCMPRESULT_ASC;
            }
            if ((uint64_t)key < (uint64_t)foundedNode->key) {
                result = CTAVLCMPRESULT_DEC;
            }
            if ((uint64_t)key == (uint64_t)foundedNode->key) {
                result = CTAVLCMPRESULT_EQL;
            }
        } else {
            result = (* root->compare)(key, foundedNode->key);
        }

        if (result == CTAVLCMPRESULT_EQL) {
            founded = true;
            break;
        }
        foundedNode = foundedNode->childNode[result];
    }

    if (!founded) {
        foundedNode = NULL;
    }

    return foundedNode;
}

void deleteCTAVLTreeNode(void * key, struct CTAVLTreeRoot *root)
{
    struct CTAVLTreeNode *nodeToDelete = findCTAVLTreeNode(key, root);
    if (nodeToDelete == NULL) {
        return;
    }

    int numberOfChild = _numberOfChild(nodeToDelete);
    struct CTAVLTreeNode *iterator = NULL;

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

void insertCTAVLTreeNode(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root)
{
    if (node == NULL) {
        return;
    }

    if (root->rootNode == NULL) {
        root->rootNode = node;
        return;
    }

    struct CTAVLTreeNode *positionNode = root->rootNode;
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

void cleanTree(struct CTAVLTreeRoot *root, bool shouldPrint)
{
    while (root->rootNode) {
        if (shouldPrint) {
            _printNode(root->rootNode);
        }
        deleteCTAVLTreeNode(root->rootNode->key, root);
    }
}

/*********** private methods ***********/

void _maintain(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root)
{
    struct CTAVLTreeNode *iterator = node;

    while (iterator) {
        bool isRotated;
        iterator = _rotate(iterator, root, &isRotated);
        if (isRotated == false) {
            iterator = iterator->parent;
        }
    }
}

struct CTAVLTreeNode * _rotate(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root, bool *isRotated)
{
    *isRotated = false;
    node->height = _heightOfNode(node);

    struct CTAVLTreeNode *newTopNode = node;
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

struct CTAVLTreeNode * _rightMinNode(struct CTAVLTreeNode *node)
{
    if (node == NULL) {
        return NULL;
    }

    struct CTAVLTreeNode *foundedNode = node;
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
struct CTAVLTreeNode * _rotateLL(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root)
{
    struct CTAVLTreeNode *aNode = node;
    struct CTAVLTreeNode *bNode = node->childNode[1];
    struct CTAVLTreeNode *parentNode = node->parent;

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
struct CTAVLTreeNode * _rotateRR(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root)
{
    struct CTAVLTreeNode *aNode = node;
    struct CTAVLTreeNode *bNode = node->childNode[0];
    struct CTAVLTreeNode *parentNode = node->parent;

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
struct CTAVLTreeNode * _rotateLR(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root)
{
    struct CTAVLTreeNode *aNode = node;
    struct CTAVLTreeNode *bNode = node->childNode[1];
    struct CTAVLTreeNode *cNode = bNode->childNode[0];

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
struct CTAVLTreeNode * _rotateRL(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root)
{
    struct CTAVLTreeNode *aNode = node;
    struct CTAVLTreeNode *bNode = node->childNode[0];
    struct CTAVLTreeNode *cNode = bNode->childNode[1];

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

int _numberOfChild(struct CTAVLTreeNode *node)
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

int _nodeIndex(struct CTAVLTreeNode *node)
{
    int index = 0;
    if (node == node->parent->childNode[1]) {
        index = 1;
    }
    return index;
}

int _heightOfNode(struct CTAVLTreeNode *node)
{
    if (node == NULL) {
        return 0;
    }

    struct CTAVLTreeNode *leftNode = node->childNode[0];
    struct CTAVLTreeNode *rightNode = node->childNode[1];

    int leftHeight = leftNode ? leftNode->height : 0;
    int rightHeight = rightNode ? rightNode->height : 0;

    return ((leftHeight > rightHeight ? leftHeight : rightHeight) + 1);
}

int _balanceOfNode(struct CTAVLTreeNode *node)
{
    if (node == NULL) {
        return 0;
    }

    int leftHeight = _heightOfNode(node->childNode[0]);
    int rightHeight = _heightOfNode(node->childNode[1]);
    return rightHeight - leftHeight;
}

void _ctNodeFree(struct CTAVLTreeNode *node)
{
    _heightOfNode(node);
}

struct CTAVLTreeNode * _deleteLeafNode(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root)
{
    struct CTAVLTreeNode *parent = node->parent;

    if (parent == NULL) {
        root->rootNode = NULL;
    } else {
        int nodeIndex = _nodeIndex(node);
        parent->childNode[nodeIndex] = NULL;
    }

    _ctNodeFree(node);
    return parent;
}

struct CTAVLTreeNode * _deleteOneChildNode(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root)
{
    struct CTAVLTreeNode *parent = node->parent;

    struct CTAVLTreeNode *childNode = node->childNode[0];
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

struct CTAVLTreeNode * _deleteTwoChildrenNode(struct CTAVLTreeNode *node, struct CTAVLTreeRoot *root)
{
    struct CTAVLTreeNode *parent = node->parent;
    struct CTAVLTreeNode *leftNode = node->childNode[0];
    struct CTAVLTreeNode *rightNode = node->childNode[1];
    struct CTAVLTreeNode *rightMinNode = _rightMinNode(rightNode);

    struct CTAVLTreeNode *iteratorNode = NULL;

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

void _printNode(struct CTAVLTreeNode *node)
{
    printf("\n=============\n");
    printf("key is %llu", (uint64_t)node->key);
    printf("\n=============\n");
}
