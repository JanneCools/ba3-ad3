#include <string.h>
#include <malloc.h>
#include <stdbool.h>

typedef struct BinaryNode {
    char character;
    char* string;
    char* skip;
    struct BinaryNode* left;
    struct BinaryNode* right;
    struct BinaryNode* equals;
    struct BinaryNode* parent;
} BinaryNode;

// bij het initialiseren van de top wordt een booleaanse waarde "skip" meegegeven die zegt of er geheugen
// geälloceerd moet worden voor de variabele skip. Zo ja, dan krijgt de variabele gewoon waarde "\0".
// Zo niet, dan worden paden niet gecomprimeerd of wordt de skip handmatig geälloceerd.
BinaryNode* binarynode_init(bool skip) {
    BinaryNode* node = calloc(1, sizeof(BinaryNode));
    if (skip) {
        node->skip = calloc(1,sizeof(char));
    }
    return node;
}

void binarynode_free(BinaryNode* node) {
    if (node->skip != NULL) {
        free(node->skip);
    }
    if (node->string != NULL) {
        free(node->string);
    }
    BinaryNode* left = node->left;
    BinaryNode* right = node->right;
    BinaryNode* equals = node->equals;
    if (node->left != NULL) {
        node->left = NULL;
        left->parent = NULL;
        binarynode_free(left);
    }
    if (node->right != NULL) {
        node->right = NULL;
        right->parent = NULL;
        binarynode_free(right);
    }
    if (node->equals != NULL) {
        node->equals = NULL;
        equals->parent = NULL;
        binarynode_free(equals);
    }
    free(node);
}

bool binarynode_search(BinaryNode* node, const char* string) {
    int index = 0;
    while (node != NULL && node->string == NULL) {
        if (string[index] < node->character) {
            node = node->left;
        } else if (string[index] > node->character) {
            node = node->right;
        } else {
            if (node->skip != NULL) {
                index += (int) strlen(node->skip);
            }
            index ++;
            node = node->equals;
        }
    }
    return node != NULL && strcmp(node->string, string) == 0;
}

void rearrange_trie(BinaryNode* node) {
    if (node->left != NULL) {
        BinaryNode* right = node->right;
        BinaryNode* left = node->left;
        node->character = left->character;
        node->left = left->left;
        node->right = left->right;
        node->equals = left->equals;
        if (node->skip != NULL) {
            free(node->skip);
            node->skip = left->skip;
        }
        if (left->left != NULL) {
            left->left->parent = node;
        }
        if (left->right != NULL) {
            left->right->parent = node;
        }
        left->equals->parent = node;
        left->left = NULL;
        left->right = NULL;
        left->equals = NULL;
        left->parent = NULL;
        left->skip = NULL;
        free(left);
        if (right != NULL) {
            BinaryNode* newRight = node->right;
            if (newRight == NULL) {
                newRight = node;
            }
            while (newRight->right != NULL) {
                newRight = newRight->right;
            }
            newRight->right = right;
            right->parent = newRight;
        }
    } else if (node->right != NULL) {
        BinaryNode* right = node->right;
        node->character = right->character;
        node->left = right->left;
        node->right = right->right;
        node->equals = right->equals;
        if (node->skip != NULL) {
            free(node->skip);
            node->skip = right->skip;
        }
        if (right->left != NULL) {
            right->left->parent = node;
        }
        if (right->right != NULL) {
            right->right->parent = node;
        }
        right->equals->parent = node;
        right->left = NULL;
        right->right = NULL;
        right->equals = NULL;
        right->parent = NULL;
        right->skip = NULL;
        free(right);
    }
}
