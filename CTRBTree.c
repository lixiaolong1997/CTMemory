#include "CTRBTree.h"

struct CTRBTreeNode * findCTRBTreeNode(uint64_t key, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *foundNode = root->rootNode;
    while (true) {
        if (foundNode == NULL) {
            break;
        }
        if (foundNode->key == key) {
            break;
        }
        if (key > foundNode->key) {
            foundNode = foundNode->right;
        } else {
            foundNode = foundNode->left;
        }
    }
    return foundNode;
}

void deleteCTRBTreeNode(uint64_t key, struct CTRBTreeRoot *root)
{
    struct CTRBTreeNode *nodeToDelete = findCTRBTreeNode(key, root);
    if (nodeToDelete == NULL) {
        return;
    }
}

void insertCTRBTreeNode(struct CTRBTreeNode *node, struct CTRBTreeRoot *root)
{
    if (root->isEmpty) {
        root->rootNode = node;
        root->isEmpty = false;
        return;
    }

    struct CTRBTreeNode *positionNode = root->rootNode;
    while (true) {
        if (node->key > positionNode->key) {
            if (positionNode->right == NULL) {
                positionNode->right = node;
                node->parent = positionNode;
                break;
            } else {
                positionNode = positionNode->right;
            }
        }

        if (node->key < positionNode->key) {
            if (positionNode->left == NULL){
                positionNode->left = node;
                node->parent = positionNode;
                break;
            } else {
                positionNode = positionNode->left;
            }
        }
    }
}
