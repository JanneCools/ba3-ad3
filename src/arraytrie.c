#include <malloc.h>
#include "arraytrie.h"
#include "string.h"

typedef struct atrie {
    char character;                 // het karakter van de vertakking vanuit de oudertop
    char* string;                   // de string wanneer het een blad is, anders NULL
    ArrayTrie** children;           // de vertakkingen vanuit deze top
    int children_size;              // het aantal kinderen van de top
    char* skip;                     // de karakters die geskipt zijn
} ArrayTrie;


ArrayTrie* arrayTrie_init() {
    ArrayTrie* trie = (ArrayTrie*) malloc(sizeof(ArrayTrie));
    trie->skip = "";
    trie->children = NULL;
    trie->string = NULL;
    trie->character = '\0';
    trie->children_size = 0;
    return trie;
}

void arraytrie_free(ArrayTrie* trie) {
    char* string = trie->string;
    trie->string = NULL;
    free(string);
    ArrayTrie** children = (ArrayTrie **) trie->children;
    trie->children = NULL;
    int size = trie->children_size;
    free(trie);
    for (int i = 0; i < size; i++) {
        arraytrie_free(children[i]);
    }
}

bool arraytrie_search(ArrayTrie* trie, const char* string) {
    size_t skip_length = strlen(trie->skip);
    int index = 0;
    while (trie->string == NULL) {
        if (index + skip_length >= strlen(string)) {
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

bool arraytrie_add(ArrayTrie* trie, const char* string) {
    if (trie->children_size == 0) {
        // enkel de wortel kan eventueel geen kinderen hebben, dat betekent dat de boom nog leeg is
        ArrayTrie* newtrie = calloc(1,sizeof(ArrayTrie));
        newtrie->children_size = 0;
        newtrie->children = NULL;
        newtrie->skip = "";
        newtrie->character = string[0];
        strcpy(newtrie->string, string);
        return true;
    }
    bool finished = false;
    bool trie_changed = false;
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
            ArrayTrie* newnode = calloc(1,sizeof(ArrayTrie));
            newnode->children_size = trie->children_size;
            newnode->children = trie->children;
            memcpy(newnode->skip, &trie->skip[mutual_skip], skip_length-mutual_skip);
            newnode->character = trie->skip[mutual_skip];
            newnode->string = NULL;
            // nieuw blad maken met de toe te voegen string
            ArrayTrie* leaf = malloc(sizeof(ArrayTrie));
            leaf->children= NULL;
            leaf->children_size = 0;
            leaf->string = malloc((strlen(string)+1) * sizeof(char));
            strcpy(leaf->string, string);
            leaf->character = string[index+mutual_skip];
            // de huidige top aanpassen zodat de skip-string en de kinderen kloppen
            trie->children_size = 2;
            trie->children = realloc(trie->children, 2*sizeof(ArrayTrie*));
            trie->children[0] = newnode;
            trie->children[1] = leaf;
            memcpy(trie->skip, &string[index], mutual_skip);
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
                    ArrayTrie* newleaf = malloc(sizeof(ArrayTrie));     // TODO: calloc gebruiken
                    newleaf->children_size = 0;
                    newleaf->children = NULL;
                    newleaf->skip = "";
                    newleaf->character = string[index];
                    newleaf->string = malloc(strlen(string) * sizeof(char));
                    strcpy(newleaf->string, string);
                    trie->children_size ++;
                    trie->children = realloc(trie->children, trie->children_size * sizeof(ArrayTrie*));
                    trie->children[trie->children_size-1] = newleaf;
                    finished = true;
                    trie_changed = true;
                } else if (trie->children[i]->string != NULL) {
                    // de boog die we nemen leidt naar een blad
                    if (strcmp(trie->children[i]->string, string) == 0) {
                        // de string in het blad komt overeen met de string die we willen toevoegen
                        finished = true;
                        trie_changed = false;
                    } else {
                        // de string in het blad komt niet overeen met de string die we willen toevoegen
                        // Dus we moeten een nieuwe top maken
                        ArrayTrie* leaf = trie->children[i];
                        int skip = 0;
                        while (leaf->string[index+1+skip] == string[index+1+skip]) {
                            skip ++;
                        }
                        leaf->character = leaf->string[index+1+skip];
                        ArrayTrie* newleaf = calloc(1,sizeof(ArrayTrie));
                        newleaf->children_size = 0;
                        newleaf->children = NULL;
                        newleaf->string = malloc((strlen(string)+1) * sizeof(char));
                        strcpy(newleaf->string, string);
                        newleaf->skip = "";
                        newleaf->character = string[index+1+skip];
                        ArrayTrie* node = calloc(1,sizeof(ArrayTrie));    // TODO: calloc gebruiken
                        node->children_size = 2;
                        node->children = malloc(2 * sizeof(ArrayTrie*));
                        node->children[0] = leaf;
                        node->children[1] = newleaf;
                        //node->skip = malloc(skip*sizeof(char));
                        memcpy(node->skip, &string[index+1], skip);
                        node->character = string[index];
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
                ArrayTrie* newnode = calloc(1,sizeof(ArrayTrie));
                newnode->children_size = trie->children_size;
                newnode->children = trie->children;
                memcpy(newnode->skip, &trie->skip[mutual_skip], skip_length-mutual_skip);
                newnode->character = trie->skip[mutual_skip];
                newnode->string = NULL;
                // nieuw blad maken met de toe te voegen string
                ArrayTrie* leaf = malloc(sizeof(ArrayTrie));
                leaf->children= NULL;
                leaf->children_size = 0;
                leaf->string = malloc((strlen(string)+1) * sizeof(char));
                strcpy(leaf->string, string);
                leaf->character = string[index+mutual_skip];
                // de huidige top aanpassen zodat de skip-string en de kinderen kloppen
                trie->children_size = 2;
                trie->children = realloc(trie->children, 2*sizeof(ArrayTrie*));
                trie->children[0] = newnode;
                trie->children[1] = leaf;
                memcpy(trie->skip, &string[index], mutual_skip);
                finished = true;
                trie_changed = true;
            }
        }
    }
    return trie_changed;
}

bool arraytrie_remove(ArrayTrie* trie, const char* string) {
    return false;
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