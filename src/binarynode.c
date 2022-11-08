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
