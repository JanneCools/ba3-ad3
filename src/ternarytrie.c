#include <malloc.h>
#include "../include/ternarytrie.h"
#include "string.h"

typedef struct ttrie {
    char character;
    char* string;
    TernaryTrie* left;
    TernaryTrie* right;
    TernaryTrie* equals;
    TernaryTrie* parent;
} TernaryTrie;

TernaryTrie* ternarytrie_init() {
    TernaryTrie* trie = calloc(1, sizeof(TernaryTrie));
    return trie;
}

void ternarytrie_free(TernaryTrie* trie) {
    if (trie->string != NULL) {
        free(trie->string);
    }
    TernaryTrie* left = trie->left;
    TernaryTrie* right = trie->right;
    TernaryTrie* equals = trie->equals;
    if (trie->left != NULL) {
        trie->left = NULL;
        left->parent = NULL;
        ternarytrie_free(left);
    }
    if (trie->right != NULL) {
        trie->right = NULL;
        right->parent = NULL;
        ternarytrie_free(right);
    }
    if (trie->equals != NULL) {
        trie->equals = NULL;
        equals->parent = NULL;
        ternarytrie_free(equals);
    }
    free(trie);
}

bool ternarytrie_search(TernaryTrie* trie, const char* string) {
    int index = 0;
    while (trie != NULL && trie->string == NULL) {
        if (string[index] < trie->character) {
            trie = trie->left;
        } else if (string[index] > trie->character) {
            trie = trie->right;
        } else {
            trie = trie->equals;
            index ++;
        }
    }
    return trie != NULL && strcmp(trie->string, string) == 0;
}

bool ternarytrie_add(TernaryTrie* trie, const char* string) {
    if (trie->character == '\0') {
        // de boom is nog volledig leeg
        trie->character = string[0];
        TernaryTrie* leaf = calloc(1, sizeof(TernaryTrie));
        leaf->string = calloc(strlen(string),sizeof(char));
        strcpy(leaf->string, string);
        leaf->parent =trie;
        trie->equals = leaf;
        return true;
    }
    if (ternarytrie_search(trie, string)) {
        return false;
    }
    bool trie_changed = false;
    size_t index = 0;
    TernaryTrie* parent = trie;
    // de while-lus wordt uitgevoerd uit zolang we geen NULL-pointer of blad tegenkomen
    while (trie != NULL && trie->string == NULL) {
        if (string[index] < trie->character) {
            parent = trie;
            trie = trie->left;
        } else if (string[index] > trie->character) {
            parent = trie;
            trie = trie->right;
        } else {
            parent = trie;
            trie = trie->equals;
            index ++;
        }
    }
    if (trie == NULL) {
        // hier nieuwe top aanmaken
        trie = (TernaryTrie*) calloc(1, sizeof(TernaryTrie));
        trie->character = string[index];
        if (trie->character > parent->character) {
            parent->right = trie;
        } else {
            parent->left = trie;
        }
        trie->parent = parent;
        TernaryTrie* leaf = calloc(1, sizeof(TernaryTrie));
        leaf->string = malloc((strlen(string)+1) * sizeof(char));
        strcpy(leaf->string, string);
        leaf->parent = trie;
        trie->equals = leaf;
        trie_changed = true;
    } else if (strcmp(trie->string, string) == 0) {
        trie_changed = false;
    } else {
        TernaryTrie* node;
        while (string[index] == trie->string[index]) {
            node = calloc(1, sizeof(TernaryTrie));
            node->character = string[index];
            parent->equals = node;
            node->parent = parent;
            parent = node;
            index ++;
        }
        node = calloc(1, sizeof(TernaryTrie));
        TernaryTrie* child = calloc(1, sizeof(TernaryTrie));
        TernaryTrie* newleaf = calloc(1, sizeof(TernaryTrie));
        node->character = trie->string[index];
        child->character = string[index];
        newleaf->string = malloc((strlen(string)+1)*sizeof(char));
        strcpy(newleaf->string, string);
        if (child->character < node->character) {
            node->left = child;
        } else {
            node->right = child;
        }
        node->equals = trie;
        child->equals = newleaf;
        parent->equals = node;
        child->parent = node;
        trie->parent = node;
        newleaf->parent = child;
        node->parent = parent;
        trie_changed = true;
    }
    return trie_changed;
}

void rearrange_trie(TernaryTrie* trie) {
    if (trie->left != NULL) {
        TernaryTrie* right = trie->right;
        TernaryTrie* left = trie->left;
        trie->character = left->character;
        trie->left = left->left;
        trie->right = left->right;
        trie->equals = left->equals;
        if (left->left != NULL) {
            left->left->parent = trie;
        }
        if (left->right != NULL) {
            left->right->parent = trie;
        }
        left->equals->parent = trie;
        left->left = NULL;
        left->right = NULL;
        left->equals = NULL;
        left->parent = NULL;
        free(left);
        if (right != NULL) {
            TernaryTrie* newRight = right;
            while (newRight->right != NULL) {
                newRight = newRight->right;
            }
            newRight->right = right;
            right->parent = newRight;
        }
    } else if (trie->right != NULL) {
        TernaryTrie* right = trie->right;
        trie->character = right->character;
        trie->left = right->left;
        trie->right = right->right;
        trie->equals = right->equals;
        if (right->left != NULL) {
            right->left->parent = trie;
        }
        if (right->right != NULL) {
            right->right->parent = trie;
        }
        right->equals->parent = trie;
        right->left = NULL;
        right->right = NULL;
        right->equals = NULL;
        right->parent = NULL;
        free(right);
    }
}

bool ternarytrie_remove(TernaryTrie* trie, const char* string) {
    // de string zoeken in de boom
    TernaryTrie* leaf = trie;
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
    trie = leaf->parent;
    leaf->parent = NULL;
    trie->equals = NULL;
    free(leaf);
    //TernaryTrie* parent = trie->parent;
    bool finished = false;
    while (! finished) {
        if (trie->equals == NULL) {
            if (trie->left == NULL && trie->right == NULL) {
                if (trie->parent == NULL) {
                    // de top is de wortel dus het karakter wordt omgezet naar '\0'
                    trie->character = '\0';
                    finished = true;
                } else {
                    TernaryTrie* child = trie;
                    trie = trie->parent;
                    if (trie->left == child) {
                        trie->left = NULL;
                    } else if (trie->right == child) {
                        trie->right = NULL;
                    } else {
                        trie->equals = NULL;
                    }
                    child->parent = NULL;
                    ternarytrie_free(child);
                }
            } else {
                rearrange_trie(trie);
            }
        } else if (trie->equals != NULL && trie->equals->string == NULL) {
            finished = true;
        } else if (trie->equals != NULL && trie->equals->string != NULL && trie->left == NULL && trie->right == NULL) {
            // de top heeft enkel een vertakking via equals naar een blad
            // dus het pad van de wortel naar deze top kan eventueel verkort worden
            while (trie->parent != NULL && trie->parent->equals == trie && trie->left == NULL && trie->right == NULL) {
                TernaryTrie* parent = trie->parent;
                if (parent->right == trie) {
                    parent->right = NULL;
                } else if (parent->left == trie) {
                    parent->left = NULL;
                } else {
                    parent->equals = NULL;
                }
                if (trie->equals != NULL) {
                    TernaryTrie* equals = trie->equals;
                    trie->equals = NULL;
                    parent->equals = equals;
                    equals->parent = parent;
                }
                if (trie->string != NULL) {
                    free(trie->string);
                }
                free(trie);
                trie = parent;
            }
            finished = true;    //TODO: nakijken of dit klopt
        } else {
            finished = true;    //TODO: nakijken of dit klopt
        }
    }
    if (trie->parent == NULL) {
        rearrange_trie(trie);
    }
    return true;
}


size_t ternarytrie_size(TernaryTrie* trie) {
    size_t size = 0;
    if (trie->left != NULL) {
        size += ternarytrie_size(trie->left);
    }
    if (trie->right != NULL) {
        size += ternarytrie_size(trie->right);
    }
    if (trie->equals != NULL) {
        size += ternarytrie_size(trie->equals);
    }
    if (trie->string != NULL) {
        size ++;
    }
    return size;
}