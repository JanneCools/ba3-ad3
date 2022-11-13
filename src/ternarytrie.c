#include <malloc.h>
#include "../include/ternarytrie.h"
#include "binarynode.c"
#include "string.h"

typedef struct ttrie {
    BinaryNode* root;
    size_t size;
} TernaryTrie;

TernaryTrie* ternarytrie_init() {
    TernaryTrie* trie = calloc(1, sizeof(TernaryTrie));
    trie->root = binarynode_init(false);
    return trie;
}

void ternarytrie_free(TernaryTrie* trie) {
    binarynode_free(trie->root);
    trie->root = NULL;
    free(trie);
}

bool ternarytrie_search(TernaryTrie* trie, const char* string) {
    return binarynode_search(trie->root, string);
}

bool ternarytrie_add(TernaryTrie* trie, const char* string) {
    BinaryNode* node = trie->root;
    if (node->character == '\0') {
        // de boom is nog volledig leeg
        node->character = string[0];
        BinaryNode* leaf = binarynode_init(false);
        leaf->string = calloc(strlen(string)+1,sizeof(char));
        strcpy(leaf->string, string);
        leaf->parent = node;
        node->equals = leaf;
        trie->size ++;
        return true;
    }
    if (ternarytrie_search(trie, string)) {
        return false;
    }
    bool trie_changed = false;
    size_t index = 0;
    BinaryNode* parent = node;
    // de while-lus wordt uitgevoerd uit zolang we geen NULL-pointer of blad tegenkomen
    while (node != NULL && node->string == NULL) {
        if (string[index] < node->character) {
            parent = node;
            node = node->left;
        } else if (string[index] > node->character) {
            parent = node;
            node = node->right;
        } else {
            parent = node;
            node = node->equals;
            index ++;
        }
    }
    if (node == NULL) {
        // hier nieuwe top aanmaken
        node = binarynode_init(false);
        node->character = string[index];
        if (node->character > parent->character) {
            parent->right = node;
        } else {
            parent->left = node;
        }
        node->parent = parent;
        BinaryNode* leaf = binarynode_init(false);
        leaf->string = malloc((strlen(string)+1) * sizeof(char));
        strcpy(leaf->string, string);
        leaf->parent = node;
        node->equals = leaf;
        trie_changed = true;
    } else if (strcmp(node->string, string) == 0) {
        trie_changed = false;
    } else {
        BinaryNode* binaryNode;
        while (string[index] == node->string[index]) {
            binaryNode = binarynode_init(false);
            binaryNode->character = string[index];
            parent->equals = binaryNode;
            binaryNode->parent = parent;
            parent = binaryNode;
            index ++;
        }
        binaryNode = binarynode_init(false);
        BinaryNode* child = binarynode_init(false);
        BinaryNode* newleaf = binarynode_init(false);
        binaryNode->character = node->string[index];
        child->character = string[index];
        newleaf->string = malloc((strlen(string)+1)*sizeof(char));
        strcpy(newleaf->string, string);
        if (child->character < binaryNode->character) {
            binaryNode->left = child;
        } else {
            binaryNode->right = child;
        }
        binaryNode->equals = node;
        child->equals = newleaf;
        parent->equals = binaryNode;
        child->parent = binaryNode;
        node->parent = binaryNode;
        newleaf->parent = child;
        binaryNode->parent = parent;
        trie_changed = true;
    }
    if (trie_changed) {
        trie->size ++;
    }
    return trie_changed;
}

void rearrange_trie(BinaryNode* node) {
    if (node->left != NULL) {
        BinaryNode* right = node->right;
        BinaryNode* left = node->left;
        node->character = left->character;
        node->left = left->left;
        node->right = left->right;
        node->equals = left->equals;
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
        free(right);
    }
}

bool ternarytrie_remove(TernaryTrie* trie, const char* string) {
    // de string zoeken in de boom
    BinaryNode* leaf = trie->root;
    int index = 0;
    while (leaf != NULL && leaf->string == NULL) {
        if (string[index] < leaf->character) {
            leaf = leaf->left;
        } else if (string[index] > leaf->character) {
            leaf = leaf->right;
        } else {
            leaf = leaf->equals;
            index ++;
        }
    }
    if (leaf == NULL || strcmp(leaf->string, string) != 0) {
        return false; // de string zit niet in de boom
    }
    free(leaf->string);
    BinaryNode* node = leaf->parent;
    leaf->parent = NULL;
    node->equals = NULL;
    free(leaf);
    bool finished = false;
    while (! finished) {
        if (node->equals == NULL) {
            if (node->left == NULL && node->right == NULL) {
                if (node->parent == NULL) {
                    // de top is de wortel dus het karakter wordt omgezet naar '\0'
                    node->character = '\0';
                    finished = true;
                } else {
                    BinaryNode* child = node;
                    node = node->parent;
                    if (node->left == child) {
                        node->left = NULL;
                    } else if (node->right == child) {
                        node->right = NULL;
                    } else {
                        node->equals = NULL;
                    }
                    child->parent = NULL;
                    binarynode_free(child);
                }
            } else {
                rearrange_trie(node);
            }
        } else if (node->equals != NULL && node->equals->string == NULL) {
            finished = true;
        } else if (node->equals != NULL && node->equals->string != NULL && node->left == NULL && node->right == NULL) {
            // de top heeft enkel een vertakking via equals naar een blad
            // dus het pad van de wortel naar deze top kan eventueel verkort worden
            while (node->parent != NULL && node->parent->equals == node && node->left == NULL && node->right == NULL) {
                BinaryNode* parent = node->parent;
                parent->equals = NULL;
                if (node->equals != NULL) {
                    BinaryNode* equals = node->equals;
                    node->equals = NULL;
                    parent->equals = equals;
                    equals->parent = parent;
                }
                if (node->string != NULL) {
                    free(node->string);
                }
                free(node);
                node = parent;
            }
            finished = true;
        } else {
            finished = true;
        }
    }
    trie->size --;
    return true;
}


size_t ternarytrie_size(TernaryTrie* trie) {
    return trie->size;
}