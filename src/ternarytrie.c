#include <malloc.h>
#include "../include/ternarytrie.h"
#include "string.h"
#include "stdio.h"

typedef struct ttrie {
    char character;
    char* string;
    TernaryTrie* left;
    TernaryTrie* right;
    TernaryTrie* equals;
} TernaryTrie;

TernaryTrie* ternarytrie_init() {
    TernaryTrie* trie = calloc(1, sizeof(TernaryTrie));
    return trie;
}

void ternarytrie_free(TernaryTrie* trie) {
    free(trie->string);
    TernaryTrie* left = trie->left;
    TernaryTrie* right = trie->right;
    TernaryTrie* equals = trie->equals;
    if (trie->left != NULL) {
        trie->left = NULL;
        ternarytrie_free(left);
    }
    if (trie->right != NULL) {
        trie->right = NULL;
        ternarytrie_free(right);
    }
    if (trie->equals != NULL) {
        trie->equals = NULL;
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
        trie->equals = leaf;
        return true;
    }
    if (ternarytrie_search(trie, string)) {
        return false;
    }
    bool trie_changed = false;
    size_t index = 0;
    TernaryTrie* parent = trie;
    // we voeren de while-lus uit zolang we geen NULL-pointer of blad tegenkomen
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
        TernaryTrie* leaf = calloc(1, sizeof(TernaryTrie));
        leaf->string = malloc((strlen(string)+1) * sizeof(char));
        strcpy(leaf->string, string);
        trie->equals = leaf;
        trie_changed = true;
    } else if (strcmp(trie->string, string) == 0) {
        trie_changed = false;
    } else {
        //TernaryTrie* leaf = trie->equals;
        TernaryTrie* node;
        while (string[index] == trie->string[index]) {
            node = calloc(1, sizeof(TernaryTrie));
            node->character = string[index];
            parent->equals = node;
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
        trie_changed = true;
    }
    return trie_changed;
}

bool ternarytrie_remove(TernaryTrie* trie, const char* string) {
    return strcmp(trie->string, string) == 0;
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