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
    } else {
        struct CTRBTreeNode *parent = nodeToDelete->parent;
        struct CTRBTreeNode *left = nodeToDelete->left;
        struct CTRBTreeNode *right = nodeToDelete->right;

        if (nodeToDelete->key > parent->key) {
            // node is right node of parent
            parent->right = right;
            if (left != NULL) {
                insertCTRBTreeNode(left, root);
            }
        }

        if (nodeToDelete->key < parent->key) {
            // node is left node of parent
            parent->left = left;
            if (right != NULL) {
                insertCTRBTreeNode(right, root);
            }
        }
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
