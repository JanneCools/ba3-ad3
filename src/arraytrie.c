#include <malloc.h>
#include "../include/arraytrie.h"
#include "string.h"

typedef struct atrie {
    char character;                 // het karakter van de vertakking vanuit de oudertop
    char* string;                   // de string wanneer het een blad is, anders NULL
    ArrayTrie** children;           // de vertakkingen vanuit deze top
    int children_size;              // het aantal kinderen van de top
    char* skip;                     // de karakters die geskipt zijn
} ArrayTrie;


ArrayTrie* arraytrie_init() {
    ArrayTrie* trie = calloc(1, sizeof(ArrayTrie));
    trie->skip = calloc(1, sizeof(char));
    return trie;
}

void arraytrie_free(ArrayTrie* trie) {
    if (trie->string != NULL) {
        free(trie->string);
    }
    free(trie->skip);
    ArrayTrie** children = (ArrayTrie **) trie->children;
    trie->children = NULL;
    int size = trie->children_size;
    free(trie);
    for (int i = 0; i < size; i++) {
        ArrayTrie* child = children[i];
        children[i] = NULL;
        arraytrie_free(child);
    }
    free(children);
}

bool arraytrie_search(ArrayTrie* trie, const char* string) {
    int index = 0;
    while (trie->string == NULL) {
        size_t skip_length = strlen(trie->skip);
        if (index + skip_length > strlen(string)) {
            return false;
        }
        index += (int) skip_length + 1;
        int i = 0;
        while (i < trie->children_size && trie->children[i]->character != string[index - 1]) {
            i ++;
        }
        if (i == trie->children_size) {
            return false;
        }
        trie = trie->children[i];
    }
    if (strcmp(trie->string, string) == 0) {
        return true;
    } else {
        return false;
    }
}

// extra methode om een nieuwe ArrayTrie aan te maken
ArrayTrie* new_arraytrie(char character, const char* string, int skip_size, char* skip_string) {
        ArrayTrie* newtrie = calloc(1, sizeof(ArrayTrie));
        newtrie->character = character;
        if (string != NULL) {
            newtrie->string = malloc((strlen(string)+1) * sizeof(char));
            strcpy(newtrie->string, string);
        }
        char* skip = malloc(skip_size * sizeof(char));
        strcpy(skip, skip_string);
        newtrie->skip = skip;
        return newtrie;
}

bool arraytrie_add(ArrayTrie* trie, const char* string) {
    if (arraytrie_search(trie, string)) {
        return false;
    }
    bool finished = false;
    bool trie_changed = false;
    if (trie->children_size == 0) {
        // enkel de wortel kan eventueel geen kinderen hebben, dat betekent dat de boom nog leeg is
        ArrayTrie* newtrie = new_arraytrie(string[0], string, 1, "\0");
        trie->children_size = 1;
        trie->children = malloc(sizeof(ArrayTrie*));
        trie->children[0] = newtrie;
        finished = true;
        trie_changed = true;
    }
    int index = 0;
    while (! finished) {
        size_t skip_length = strlen(trie->skip);

        if (strlen(string) - index - 1 < skip_length) {
            // string is korter dan het aantal skips, dus de top moet gesplitst worden
            int mutual_skip = 0;
            while (string[index+mutual_skip] == trie->skip[mutual_skip]) {
                mutual_skip ++;
            }
            // nieuwe top maken die de al bestaande kinderen als kinderen heeft
            ArrayTrie* newnode = new_arraytrie(trie->skip[mutual_skip], NULL, (int)skip_length-mutual_skip, &trie->skip[mutual_skip+1]);
            newnode->children_size = trie->children_size;
            newnode->children = trie->children;
            // nieuw blad maken met de toe te voegen string
            ArrayTrie* leaf = new_arraytrie(string[index+mutual_skip], string, 1, "\0");
            // de huidige top aanpassen zodat de skip-string en de kinderen kloppen
            trie->children_size = 2;
            trie->children = malloc(2*sizeof(ArrayTrie*));
            trie->children[0] = newnode;
            trie->children[1] = leaf;
            trie->skip = realloc(trie->skip, (mutual_skip+1) * sizeof(char));
            strcpy(trie->skip, &string[index]);
            trie->skip[mutual_skip] = '\0';
            finished = true;
            trie_changed = true;
        } else {
            // het deel van de string dat geskipt wordt, bepalen
            char* skip_string = malloc((skip_length+1)*sizeof(char));
            memcpy(skip_string, &string[index], skip_length);
            skip_string[skip_length] = '\0';

            if (skip_length == 0 || strcmp(skip_string, trie->skip) == 0) {
                // de skip-strings zijn gelijk dus we kunnen naar de volgende top
                index += (int) skip_length;
                int i = 0;
                while (i < trie->children_size && trie->children[i]->character != string[index]) {
                    i ++;
                }
                if (i == trie->children_size) {
                    // er is nog geen kind met het karakter van de string dus een blad kan aangemaakt worden
                    ArrayTrie* newleaf = new_arraytrie(string[index], string, 1, "\0");
                    trie->children_size ++;
                    trie->children = realloc(trie->children, trie->children_size * sizeof(ArrayTrie*));
                    trie->children[trie->children_size-1] = newleaf;
                    finished = true;
                    trie_changed = true;
                } else if (trie->children[i]->string != NULL) {   // de boog die we nemen leidt naar een blad
                    index ++;
                    if (strcmp(trie->children[i]->string, string) == 0) {
                        // de string in het blad komt overeen met de string die we willen toevoegen
                        finished = true;
                        trie_changed = false;
                    } else {
                        // de string in het blad komt niet overeen met de string die we willen toevoegen
                        // Dus we moeten een nieuwe top maken met als kinderen het huidig blad en het nieuw blad
                        // en met als ouder de top waar we nu zijn (trie)
                        ArrayTrie* leaf = trie->children[i];
                        int skip = 0;
                        while (leaf->string[index+skip] == string[index+skip]) {
                            skip ++;
                        }
                        leaf->character = leaf->string[index+skip];     // het karakter bij het huidig blad aanpassen
                        // blad maken met de nieuwe string
                        ArrayTrie* newleaf = new_arraytrie(string[index+skip], string, 1, "\0");
                        // een nieuwe top maken met als ouder "trie"
                        ArrayTrie* node = new_arraytrie(string[index-1], NULL, 1, "\0");
                        node->skip = realloc(node->skip, (skip+1)*sizeof(char));
                        strcpy(node->skip, &string[index]);
                        node->skip[skip] = '\0';
                        node->children_size = 2;
                        node->children = malloc(2 * sizeof(ArrayTrie*));
                        node->children[0] = leaf;
                        node->children[1] = newleaf;
                        //memcpy(node->skip, &string[index+1], skip);
                        trie->children[i] = node;
                        finished = true;
                        trie_changed = true;
                    }
                } else {
                    // De boog die we nemen, leidt naar een interne top dus we kijken alles opnieuw na voor deze top
                    trie = trie->children[i];
                    index ++;
                }
            } else {
                // de skip-strings zijn niet gelijk dus we moeten deze top splitsen in een nieuwe top voor de nieuwe string en
                // een nieuwe top voor de huidige kinderen van deze top
                int mutual_skip = 0;
                while (skip_string[mutual_skip] == trie->skip[mutual_skip]) {
                    mutual_skip ++;
                }
                // nieuwe top maken die de al bestaande kinderen als kinderen heeft
                ArrayTrie* newnode = new_arraytrie(trie->skip[mutual_skip], NULL, (int)skip_length-mutual_skip+1, &trie->skip[mutual_skip+1]);
                newnode->children_size = trie->children_size;
                newnode->children = trie->children;
                // nieuw blad maken met de toe te voegen string
                ArrayTrie* leaf = new_arraytrie(string[index+mutual_skip], string, 1, "\0");
                // de huidige top aanpassen zodat de skip-string en de kinderen kloppen
                trie->children_size = 2;
                trie->children = malloc(2*sizeof(ArrayTrie*));
                trie->children[0] = newnode;
                trie->children[1] = leaf;
                trie->skip = realloc(trie->skip, (mutual_skip+1) * sizeof(char));
                strcpy(trie->skip, &string[index]);
                trie->skip[mutual_skip] = '\0';
                finished = true;
                trie_changed = true;
            }
            free(skip_string);
        }
    }
    return trie_changed;
}

bool arraytrie_remove(ArrayTrie* trie, const char* string) {
    ArrayTrie* root = trie;
    if (!arraytrie_search(trie, string)) {
        return false;
    }
    bool finished = false;
    bool removed;
    int index = 0;
    while (! finished) {
        size_t skip_length = strlen(trie->skip);
        char* skip_string = malloc((skip_length+1)*sizeof(char));
        memcpy(skip_string, &string[index], skip_length);
        skip_string[skip_length] = '\0';
        if (strlen(string) - index < skip_length || strcmp(skip_string, trie->skip) != 0) {
            // string zit niet in de boom
            finished = true;
            removed = false;
        } else {
            // de skip in de string is gelijk aan de skip van de top
            index += (int) skip_length;
            int i = 0;
            while (i < trie->children_size && trie->children[i]->character != string[index]) {
                i++;
            }
            if (i == trie->children_size) {
                // string is niet aanwezig in de boom
                finished = true;
                removed = false;
            } else if (trie->children[i]->string != NULL) {     // het kind van de top is een blad dus de strings worden vergeleken
                if (strcmp(trie->children[i]->string, string) == 0) {   // de strings zijn gelijk dus het blad kan verwijderd worden
                    if (trie->children_size > 2) {
                        // de top heeft meer dan 2 kinderen (inclusief het blad dat verwijderd zal worden)
                        // dus het blad kan makkelijk verwijderen worden zonder veel aan te passen aan de huidige top
                        ArrayTrie** children = trie->children;
                        ArrayTrie* leaf = children[i];
                        trie->children[i] = trie->children[trie->children_size-1];
                        trie->children[trie->children_size-1] = NULL;
                        trie->children_size --;
                        trie->children = realloc(trie->children, trie->children_size);
                        arraytrie_free(leaf);
                        finished = true;
                        removed = true;
                    } else if (trie->children_size == 1) {
                        // de top heeft maar 1 kind, dus de top is de wortel
                        // Het blad kan dan gewoon verwijderd worden
                        ArrayTrie** children = trie->children;
                        ArrayTrie* leaf = children[0];
                        trie->children_size = 0;
                        trie->children = NULL;
                        children[0] = NULL;
                        arraytrie_free(leaf);
                        free(children);
                        finished = true;
                        removed = true;
                    } else {
                        // de top heeft maar 1 kind na het verwijderen van het blad
                        // dus de boom moet hier wat herschikt worden
                        ArrayTrie* child = trie->children[0];
                        ArrayTrie* leaf = trie->children[1];
                        if (leaf->string == NULL || strcmp(leaf->string, string) != 0) {
                            child = trie->children[1];
                            leaf = trie->children[0];
                        }
                        trie->children[0] = NULL;
                        trie->children[1] = NULL;
                        arraytrie_free(leaf);
                        char* skip = malloc((strlen(trie->skip) + strlen(child->skip) + 2) * sizeof(char));
                        char character[2] = {child->character, '\0'};
                        strcpy(skip, trie->skip);
                        strcat(skip, character);
                        strcat(skip, child->skip);
                        skip[strlen(trie->skip) + strlen(child->skip)+1] = '\0';
                        free(trie->skip);
                        trie->skip = skip;
                        ArrayTrie** children = child->children;
                        child->children = NULL;
                        if (child->string == NULL) {
                            free(trie->children);
                            trie->children = children;
                            trie->children_size = child->children_size;
                            child->children_size = 0;
                            arraytrie_free(child);
                        } else if (trie == root) {
                            free(children);
                            trie->children = realloc(trie->children, sizeof(ArrayTrie*));
                            trie->children[0] = child;
                            trie->children_size = 1;
                            child->character = child->string[strlen(trie->skip)];
                        } else {
                            free(children);
                            trie->children_size = 0;
                            free(trie->children);
                            trie->children = NULL;
                            trie->string = child->string;
                            child->string = NULL;
                            arraytrie_free(child);
                        }
                        finished = true;
                        removed = true;
                    }
                } else {
                    // de strings zijn niet gelijk dus de string zit niet in de boom
                    finished = true;
                    removed = false;
                }
            } else {
                // het kind van de top is een andere top
                index ++;
                trie = trie->children[i];
            }
        }
        free(skip_string);
    }
    return removed;
}

size_t arraytrie_size(ArrayTrie* trie) {
    size_t size = 0;
    for (int i = 0; i < trie->children_size; i++) {
        ArrayTrie* child = trie->children[i];
        if (child->string == NULL) {
            size += arraytrie_size(child);
        } else {
            size ++;
        }
    }
    return size;
}