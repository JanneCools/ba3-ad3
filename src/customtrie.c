#include <malloc.h>
#include "../include/customtrie.h"
#include "string.h"

typedef struct ctrie {
    char character;
    char* string;
    char* skip;
    CustomTrie* left;
    CustomTrie* right;
    CustomTrie* equals;
    CustomTrie* parent;
} CustomTrie;

CustomTrie* customtrie_init() {
    CustomTrie* trie = calloc(1, sizeof(CustomTrie));
    return trie;
}

void customtrie_free(CustomTrie* trie) {
    free(trie->skip);
    if (trie->string != NULL) {
        free(trie->string);
    }
    CustomTrie* left = trie->left;
    CustomTrie* right = trie->right;
    CustomTrie* equals = trie->equals;
    if (trie->left != NULL) {
        trie->left = NULL;
        left->parent = NULL;
        customtrie_free(left);
    }
    if (trie->right != NULL) {
        trie->right = NULL;
        right->parent = NULL;
        customtrie_free(right);
    }
    if (trie->equals != NULL) {
        trie->equals = NULL;
        equals->parent = NULL;
        customtrie_free(equals);
    }
    free(trie);
}

bool customtrie_search(CustomTrie* trie, const char* string) {
    int index = 0;
    while (trie != NULL && trie->string == NULL) {
        if (string[index] < trie->character) {
            trie = trie->left;
        } else if (string[index] > trie->character) {
            trie = trie->right;
        } else {
            index += (int) strlen(trie->skip) + 1;
            trie = trie->equals;
        }
    }
    return trie != NULL && strcmp(trie->string, string) == 0;
}

bool customtrie_add(CustomTrie* trie, const char* string) {
    if (customtrie_search(trie, string)) {
        // de string zit al in de boom
        return false;
    }
    if (trie->character == '\0') {
        // de boom is nog volledig leeg
        trie->character = string[0];
        trie->skip = calloc(1, sizeof(char));
        CustomTrie* leaf = calloc(1, sizeof(CustomTrie));
        leaf->string = calloc(strlen(string),sizeof(char));
        strcpy(leaf->string, string);
        leaf->skip = calloc(1, sizeof(char));
        leaf->parent =trie;
        trie->equals = leaf;
        return true;
    }
    bool searching = true;
    int index = 0;
    // zoeken tot de skips niet gelijk zijn of tot een NULL-pointer bereikt wordt (door left of right te volgen naar NULL)
    CustomTrie* parent = trie;
    while (searching) {
        if (trie == NULL || trie->string != NULL) {
            // we zitten in een NULL-pointer of een blad (met een andere string want we hebben al nagekeken of de string in de boom zit)
            searching = false;
        } else if (string[index] < trie->character) {
            parent = trie;
            trie = trie->left;
        } else if (string[index] > trie->character) {
            parent = trie;
            trie = trie->right;
        } else {
            size_t skip_length = strlen(trie->skip);
            // het deel van de string dat geskipt wordt, bepalen
            char* skip_string = malloc((skip_length+1)*sizeof(char));
            memcpy(skip_string, &string[index+1], skip_length);
            skip_string[skip_length] = '\0';
            if (strlen(string)-index-1 < skip_length || strcmp(skip_string, trie->skip) != 0) {
                // de skips zijn niet gelijk dus hier gaan we moeten splitsen
                parent = trie->parent;
                index ++;
                searching = false;
            } else {
                index += (int)skip_length + 1;
                parent = trie;
                trie = trie->equals;
            }
        }
    }
    if (trie == NULL) {
        // we zitten in een NULL-pointer dus we moeten een nieuwe top en een nieuw blad maken
        trie = parent;
        CustomTrie* node = calloc(1, sizeof(CustomTrie));
        node->skip = calloc(1, sizeof(char));
        node->character = string[index];
        CustomTrie* leaf = calloc(1, sizeof(CustomTrie));
        leaf->skip = calloc(1, sizeof(char));
        leaf->string = malloc((strlen(string)+1) * sizeof(char));
        strcpy(leaf->string, string);
        if (node->character < trie->character) {
            trie->left = node;
        } else {
            trie->right = node;
        }
        node->parent = trie;
        node->equals = leaf;
        leaf->parent = node;
    } else if (trie->string != NULL) {
        // we zitten in een blad met een andere string dus een extra top toevoegen
        int mutual_skip = 0;
        while (index+mutual_skip < (int)strlen(string) && index+mutual_skip < (int)strlen(trie->string)
               && string[index+mutual_skip] == trie->string[index+mutual_skip]) {
            mutual_skip ++;
        }
        parent->skip = malloc((mutual_skip+1) * sizeof(char));
        strcpy(parent->skip, &string[index]);
        parent->skip[mutual_skip] = '\0';
        CustomTrie* node1 = calloc(1, sizeof(CustomTrie));  // ouder van string dat al in boom zat
        node1->character = trie->string[index+mutual_skip];
        node1->skip = calloc(1, sizeof(char));
        node1->equals = trie;
        node1->parent = parent;
        parent->equals = node1;
        trie->parent = node1;
        CustomTrie* node2 = calloc(1, sizeof(CustomTrie));  // ouder van nieuwe string
        node2->character = string[index+mutual_skip];
        node2->skip = calloc(1, sizeof(char));
        if (string[index+mutual_skip] < trie->string[index+mutual_skip]) {
            node1->left = node2;
        } else {
            node1->right = node2;
        }
        node2->parent = node1;
        CustomTrie* leaf = calloc(1, sizeof(CustomTrie));         // blad met nieuwe string
        leaf->string = malloc((strlen(string)+1) * sizeof(char));
        strcpy(leaf->string, string);
        leaf->skip = calloc(1, sizeof(char));
        leaf->parent = node2;
        node2->equals = leaf;
    } else {
        // we zitten in een interne top waarbij de skips niet gelijk zijn
        // we moeten de skip splitsen
        size_t skip_length = strlen(trie->skip);
        int mutual_skip = 0;
        while (index+mutual_skip < (int)strlen(string) && mutual_skip < (int)strlen(trie->skip)
               && string[index+mutual_skip] == trie->skip[mutual_skip]) {
            mutual_skip ++;
        }
        // karakter en skip van trie aanpassen
        char* new_skip = calloc((skip_length-mutual_skip-1), sizeof(char));
        strcpy(new_skip, &trie->skip[mutual_skip+1]);
        trie->character = trie->skip[mutual_skip];
        //trie->skip = realloc(trie->skip, (skip_length-mutual_skip-1) * sizeof(char));
        free(trie->skip);
        trie->skip = new_skip;
        // nieuwe ouder maken voor trie die het kind wordt van de ouder van trie
        CustomTrie* new_parent = calloc(1, sizeof(CustomTrie));
        new_parent->character = string[index-1];
        new_parent->skip = malloc((mutual_skip+1) * sizeof(char));
        strcpy(new_parent->skip, &string[index]);
        new_parent->skip[mutual_skip] = '\0';
        // blad met nieuwe string maken
        CustomTrie* leaf = calloc(1, sizeof(CustomTrie));
        leaf->skip = calloc(1, sizeof(char));
        leaf->string = malloc((strlen(string)+1) * sizeof(char));
        strcpy(leaf->string, string);
        // ouder van nieuw blad maken
        CustomTrie* node = calloc(1, sizeof(CustomTrie));
        node->character = string[index+mutual_skip];
        node->skip = calloc(1, sizeof(char));
        // alle toppen verbinden
        if (new_parent->character < parent->character) {
            parent->left = new_parent;
        } else if (new_parent->character > parent->character) {
            parent->right = new_parent;
        } else {
            parent->equals = new_parent;
        }
        new_parent->parent = parent;
        new_parent->equals = trie;
        trie->parent = new_parent;
        if (node->character < trie->character) {
            trie->left = node;
        } else {
            trie->right = node;
        }
        node->parent = trie;
        node->equals = leaf;
        leaf->parent = node;
    }
    return true;
}

bool customtrie_remove(CustomTrie* trie, const char* string) {
    if (!customtrie_search(trie, string)) {
        return false;
    }
    bool searching = true;
    int index = 0;
    CustomTrie* leaf = trie;
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
            memcpy(skip_string, &string[index+1], skip_length);
            skip_string[skip_length] = '\0';
            if (strlen(string)-index-1 < skip_length || strcmp(skip_string, leaf->skip) != 0) {
                // de skips zijn niet gelijk dus hier gaan we moeten splitsen
                index ++;
                searching = false;
            } else {
                index += (int)skip_length + 1;
                leaf = leaf->equals;
            }
        }
    }
    trie = leaf->parent;
    trie->equals = NULL;
    customtrie_free(leaf);
    bool rearranging = true;
    while (rearranging) {
        if (trie->equals == NULL) {
            if (trie->left == NULL && trie->right == NULL) {
                if (trie->parent == NULL) {
                    // de top is de wortel dus het karakter moet omgezet worden naar '\0'
                    trie->character = '\0';
                    rearranging = false;
                } else {
                    CustomTrie* child = trie;
                    trie = trie->parent;
                    if (trie->left == child) {
                        trie->left = NULL;
                    } else if (trie->right == child) {
                        trie->right = NULL;
                    } else {
                        trie->equals = NULL;
                    }
                    child->parent = NULL;
                    customtrie_free(child);
                }
            } else {
                if (trie->left != NULL) {
                    CustomTrie* right = trie->right;
                    CustomTrie* left = trie->left;
                    trie->character = left->character;
                    trie->left = left->left;
                    trie->right = left->right;
                    trie->equals = left->equals;
                    trie->skip = left->skip;
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
                    left->skip = NULL;
                    free(left);
                    if (right != NULL) {
                        CustomTrie* newRight = right;
                        while (newRight->right != NULL) {
                            newRight = newRight->right;
                        }
                        newRight->right = right;
                        right->parent = newRight;
                    }
                } else if (trie->right != NULL) {
                    CustomTrie* right = trie->right;
                    trie->character = right->character;
                    trie->left = right->left;
                    trie->right = right->right;
                    trie->equals = right->equals;
                    trie->skip = right->skip;
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
                    right->skip = NULL;
                    free(right);
                }
            }
        } else if (trie->parent == NULL) {
            rearranging = false;    // we zitten in de wortel
        } else if (trie->parent->equals == trie && trie->left == NULL && trie->right == NULL) {
            // de top heeft enkel een blad als kind en is het equals-kind van zijn ouder
            // dus de skip van de ouder mag verwijderd worden en de huidige top mag ook verwijderd worden
            CustomTrie* parent = trie->parent;
            size_t skip_length = strlen(parent->skip) + strlen(trie->skip) + 2;
            char* skip = malloc(skip_length * sizeof(char));
            char character[2] = {trie->character, '\0'};
            strcpy(skip, parent->skip);
            strcat(skip, character);
            strcat(skip, trie->skip);
            skip[skip_length - 1] = '\0';
            free(parent->skip);
            parent->skip = skip;
            CustomTrie* child = trie->equals;
            child->parent = parent;
            parent->equals = child;
            trie->parent = NULL;
            trie->equals = NULL;
            customtrie_free(trie);
            trie = parent;
            rearranging = false;
        }
        rearranging = false;
    }
    // als we in de wortel geëindigd zijn en de wortel geen kinderen heeft, dan moet de skip van de wortel '\0' zijn
    if (trie->parent == NULL && trie->left == NULL && trie->right == NULL && trie->equals == NULL) {
        trie->skip = realloc(trie->skip, sizeof(char));
        trie->skip[0] = '\0';
    }
    return true;
}

size_t customtrie_size(CustomTrie* trie) {
    size_t size = 0;
    if (trie->left != NULL) {
        size += customtrie_size(trie->left);
    }
    if (trie->right != NULL) {
        size += customtrie_size(trie->right);
    }
    if (trie->equals != NULL) {
        size += customtrie_size(trie->equals);
    }
    if (trie->string != NULL) {
        size ++;
    }
    return size;
}