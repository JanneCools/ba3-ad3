#include <malloc.h>
#include "../include/customtrie.h"
#include "binarynode.c"
#include "string.h"

typedef struct ctrie {
    BinaryNode* root;
    size_t size;
} CustomTrie;

CustomTrie* customtrie_init() {
    CustomTrie* trie = calloc(1, sizeof(CustomTrie));
    trie->root = binarynode_init(true);
    return trie;
}

void customtrie_free(CustomTrie* trie) {
    binarynode_free(trie->root);
    trie->root = NULL;
    free(trie);
}

bool customtrie_search(CustomTrie* trie, const char* string) {
    return binarynode_search(trie->root, string);
}

bool customtrie_add(CustomTrie* trie, const char* string) {
    if (customtrie_search(trie, string)) {
        // de string zit al in de boom
        return false;
    }
    if (trie->size == 0) {
        // de boom is nog volledig leeg
        trie->root->character = string[0];
        BinaryNode* leaf = binarynode_init(true);
        leaf->string = calloc(strlen(string)+1,sizeof(char));
        strcpy(leaf->string, string);
        leaf->parent = trie->root;
        trie->root->equals = leaf;
        trie->size ++;
        return true;
    }
    BinaryNode* node = trie->root;
    bool searching = true;
    int index = 0;
    // zoeken tot de skips niet gelijk zijn of tot een NULL-pointer bereikt wordt (door left of right te volgen naar NULL)
    BinaryNode* parent = node;
    while (searching) {
        if (node == NULL || node->string != NULL) {
            // we zitten in een NULL-pointer of een blad (met een andere string want we hebben al nagekeken of de string in de boom zit)
            searching = false;
        } else if (string[index] < node->character) {
            parent = node;
            node = node->left;
        } else if (string[index] > node->character) {
            parent = node;
            node = node->right;
        } else {
            size_t skip_length = strlen(node->skip);
            // het deel van de string dat geskipt wordt, bepalen
            char* skip_string = calloc(skip_length+1, sizeof(char));
            strncpy(skip_string, &string[index+1], skip_length);
            if (strlen(string)-index-1 < skip_length || strcmp(skip_string, node->skip) != 0) {
                // de skips zijn niet gelijk dus hier gaan we moeten splitsen
                parent = node->parent;
                index ++;
                searching = false;
            } else {
                index += (int)skip_length + 1;
                parent = node;
                node = node->equals;
            }
            free(skip_string);
        }
    }
    if (node == NULL) {
        // we zitten in een NULL-pointer dus we moeten een nieuwe top en een nieuw blad maken
        node = parent;
        BinaryNode* binaryNode = binarynode_init(true);
        binaryNode->character = string[index];
        BinaryNode* leaf = binarynode_init(true);
        leaf->string = calloc(strlen(string)+1, sizeof(char));
        strcpy(leaf->string, string);
        if (binaryNode->character < node->character) {
            node->left = binaryNode;
        } else {
            node->right = binaryNode;
        }
        binaryNode->parent = node;
        binaryNode->equals = leaf;
        leaf->parent = binaryNode;
    } else if (node->string != NULL) {
        // we zitten in een blad met een andere string dus een extra top toevoegen
        int mutual_skip = 0;
        while (index+mutual_skip < (int)strlen(string) && index+mutual_skip < (int)strlen(node->string)
               && string[index+mutual_skip] == node->string[index+mutual_skip]) {
            mutual_skip ++;
        }
        parent->skip = realloc(parent->skip, (mutual_skip+1)* sizeof(char));
        strncpy(parent->skip, &string[index], mutual_skip);
        parent->skip[mutual_skip] = '\0';
        BinaryNode* node1 = binarynode_init(true);  // ouder van string dat al in boom zat
        node1->character = node->string[index+mutual_skip];
        node1->equals = node;
        node1->parent = parent;
        parent->equals = node1;
        node->parent = node1;
        BinaryNode* node2 = binarynode_init(true);  // ouder van nieuwe string
        node2->character = string[index+mutual_skip];
        if (string[index+mutual_skip] < node->string[index+mutual_skip]) {
            node1->left = node2;
        } else {
            node1->right = node2;
        }
        node2->parent = node1;
        BinaryNode* leaf = binarynode_init(true);         // blad met nieuwe string
        leaf->string = calloc(strlen(string)+1, sizeof(char));
        strcpy(leaf->string, string);
        leaf->parent = node2;
        node2->equals = leaf;
    } else {
        // we zitten in een interne top waarbij de skips niet gelijk zijn
        // we moeten de skip splitsen
        size_t skip_length = strlen(node->skip);
        int mutual_skip = 0;
        while (index+mutual_skip < (int)strlen(string) && mutual_skip < (int)strlen(node->skip)
               && string[index+mutual_skip] == node->skip[mutual_skip]) {
            mutual_skip ++;
        }
        // karakter en skip van trie aanpassen
        char* new_skip = calloc(skip_length-mutual_skip, sizeof(char));
        strncpy(new_skip, &node->skip[mutual_skip+1], skip_length-mutual_skip-1);
        node->character = node->skip[mutual_skip];
        free(node->skip);
        node->skip = new_skip;
        // nieuwe ouder maken voor trie die het kind wordt van de ouder van trie
        BinaryNode* new_parent = binarynode_init(false);
        new_parent->character = string[index-1];
        new_parent->skip = calloc(mutual_skip+1, sizeof(char));
        strncpy(new_parent->skip, &string[index], mutual_skip);
        // blad met nieuwe string maken
        BinaryNode* leaf = binarynode_init(true);
        leaf->string = calloc(strlen(string)+1, sizeof(char));
        strcpy(leaf->string, string);
        // ouder van nieuw blad maken
        BinaryNode* binaryNode = binarynode_init(true);
        binaryNode->character = string[index + mutual_skip];
        // alle toppen verbinden
        if (new_parent->character < parent->character) {
            parent->left = new_parent;
        } else if (new_parent->character > parent->character) {
            parent->right = new_parent;
        } else {
            parent->equals = new_parent;
        }
        new_parent->parent = parent;
        new_parent->equals = node;
        node->parent = new_parent;
        if (binaryNode->character < node->character) {
            node->left = binaryNode;
        } else {
            node->right = binaryNode;
        }
        binaryNode->parent = node;
        binaryNode->equals = leaf;
        leaf->parent = binaryNode;
    }
    trie->size ++;
    return true;
}

bool customtrie_remove(CustomTrie* trie, const char* string) {
    if (!customtrie_search(trie, string)) {
        return false;
    }
    bool searching = true;
    int index = 0;
    BinaryNode* leaf = trie->root;
    while (searching) {
        if (leaf == NULL || leaf->string != NULL) {
            // we zitten in een NULL-pointer of een blad
            searching = false;
        } else if (string[index] < leaf->character) {
            leaf = leaf->left;
        } else if (string[index] > leaf->character) {
            leaf = leaf->right;
        } else {
            size_t skip_length = strlen(leaf->skip);
            // het deel van de string dat geskipt wordt bepalen
            char* skip_string = malloc((skip_length+1)*sizeof(char));
            strncpy(skip_string, &string[index+1], skip_length);
            skip_string[skip_length] = '\0';
            if (strlen(string)-index-1 < skip_length || strcmp(skip_string, leaf->skip) != 0) {
                // de skips zijn niet gelijk dus hier gaan we moeten splitsen
                index ++;
                searching = false;
            } else {
                index += (int)skip_length + 1;
                leaf = leaf->equals;
            }
            free(skip_string);
        }
    }
    BinaryNode* node = leaf->parent;
    node->equals = NULL;
    leaf->parent = NULL;
    binarynode_free(leaf);
    bool rearranging = true;
    while (rearranging) {
        if (node->equals == NULL) {
            if (node->left == NULL && node->right == NULL) {
                if (node->parent == NULL) {
                    // de top is de wortel dus het karakter moet omgezet worden naar '\0'
                    node->character = '\0';
                    rearranging = false;
                } else {
                    BinaryNode* child = node;
                    node = child->parent;
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
                if (node->left != NULL) {
                    BinaryNode* right = node->right;
                    BinaryNode* left = node->left;
                    node->character = left->character;
                    node->left = left->left;
                    node->right = left->right;
                    node->equals = left->equals;
                    free(node->skip);
                    node->skip = left->skip;
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
                    free(node->skip);
                    node->skip = right->skip;
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
        } else if (node->parent == NULL) {
            rearranging = false;    // we zitten in de wortel
        } else if (node->parent->equals == node && node->left == NULL && node->right == NULL) {
            // de top heeft enkel een blad als kind en is het equals-kind van zijn ouder
            // dus de skip van de ouder mag verwijderd worden en de huidige top mag ook verwijderd worden
            BinaryNode* parent = node->parent;
            size_t skip_length = strlen(parent->skip) + strlen(node->skip) + 2;
            char* skip = malloc(skip_length * sizeof(char));
            char character[2] = {node->character, '\0'};
            strcpy(skip, parent->skip);
            strcat(skip, character);
            strcat(skip, node->skip);
            skip[skip_length - 1] = '\0';
            free(parent->skip);
            parent->skip = skip;
            BinaryNode* child = node->equals;
            child->parent = parent;
            parent->equals = child;
            node->parent = NULL;
            node->equals = NULL;
            customtrie_free(trie);
            node = parent;
            rearranging = false;
        }
        rearranging = false;
    }
    // als we in de wortel geëindigd zijn en de wortel geen kinderen heeft, dan moet de skip van de wortel '\0' zijn
    if (node->parent == NULL && node->left == NULL && node->right == NULL && node->equals == NULL) {
        node->skip = realloc(node->skip, sizeof(char));
        node->skip[0] = '\0';
    }
    trie->size --;
    return true;
}

size_t customtrie_size(CustomTrie* trie) {
    return trie->size;
}