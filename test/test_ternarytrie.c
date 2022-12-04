#include <stdlib.h>
#include "test.h"
#include "ternarytrie.h"
#include "../src/ternarytrie.c"
#include <sys/time.h>

#define TEST_SIZE(ct, size) \
    TEST_CHECK(ternarytrie_size(ct) == size); \
    TEST_MSG("Size: %zu", ternarytrie_size(ct))
                                \
void test_init() {
    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);
    TEST_SIZE(ct, 0);
    ternarytrie_free(ct);
}

void test_add_one() {
    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    const char* string = "this is a test";
    TEST_CHECK(ternarytrie_add(ct, string));
    TEST_CHECK(ternarytrie_search(ct, string));
    TEST_SIZE(ct, 1);

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(ct->root->equals->character == '\0');
    TEST_CHECK(strcmp(ct->root->equals->string, string) == 0);

    ternarytrie_free(ct);
}

void test_search_not_present() {
    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    TEST_CHECK(ternarytrie_add(ct, "this string exists"));
    TEST_CHECK(!ternarytrie_search(ct, "this string does not exist"));
    ternarytrie_free(ct);
}

void test_add_more() {
    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";

    TEST_CHECK(ternarytrie_add(ct, one));
    TEST_CHECK(ternarytrie_add(ct, two));
    TEST_CHECK(ternarytrie_add(ct, twenty));
    TEST_CHECK(ternarytrie_add(ct, twentytwo));

    TEST_SIZE(ct, 4);

    TEST_CHECK(ternarytrie_search(ct, one));
    TEST_CHECK(ternarytrie_search(ct, two));
    TEST_CHECK(ternarytrie_search(ct, twenty));
    TEST_CHECK(ternarytrie_search(ct, twentytwo));

    TEST_CHECK(!ternarytrie_add(ct, one));
    TEST_CHECK(!ternarytrie_add(ct, two));
    TEST_CHECK(!ternarytrie_add(ct, twenty));
    TEST_CHECK(!ternarytrie_add(ct, twentytwo));

    //structuur nakijken
    TEST_CHECK(ct->root->character == 'o');
    TEST_CHECK(strcmp(ct->root->equals->string, one) == 0);
    TEST_CHECK(ct->root->right->character == 't');
    TEST_CHECK(ct->root->right->equals->equals->character == 'o');
    TEST_CHECK(strcmp(ct->root->right->equals->equals->equals->string, two) == 0);
    TEST_CHECK(ct->root->right->equals->equals->left->character == 'e');
    TEST_CHECK(ct->root->right->equals->equals->left->equals->equals->equals->equals->character == '\0');
    TEST_CHECK(strcmp(ct->root->right->equals->equals->left->equals->equals->equals->equals->equals->string, twenty) == 0);
    TEST_CHECK(ct->root->right->equals->equals->left->equals->equals->equals->equals->right->character == 't');
    TEST_CHECK(strcmp(ct->root->right->equals->equals->left->equals->equals->equals->equals->right->equals->string, twentytwo) == 0);

    ternarytrie_free(ct);
}

// Hierbij worden 2 gelijkaardige strings toegevoegd, waardoor er een lang pad naar de bladeren zou moeten ontstaan
void test_add_far_leafs() {
    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    const char* twentyone = "twentyone";
    const char* twentytwo = "twentytwo";

    TEST_CHECK(ternarytrie_add(ct, twentyone));

    // Structuur nakijken
    TEST_CHECK(ct->root->character == 't' && ct->root->right == NULL && ct->root->left == NULL);
    TEST_CHECK(ct->root->equals->character == '\0');
    TEST_CHECK(strcmp(ct->root->equals->string, twentyone) == 0);

    TEST_CHECK(ternarytrie_add(ct, twentytwo));
    TEST_SIZE(ct, 2);

    // Structuur nakijken
    TEST_CHECK(ct->root->character == 't' && ct->root->right == NULL && ct->root->left == NULL);
    TEST_CHECK(ct->root->equals->character == 'w' && ct->root->equals->equals->character == 'e');
    TEST_CHECK(ct->root->equals->equals->equals->character == 'n' && ct->root->equals->equals->equals->equals->character == 't');
    TEST_CHECK(ct->root->equals->equals->equals->equals->equals->character == 'y');
    TEST_CHECK(ct->root->equals->equals->equals->equals->equals->equals->character == 'o');
    TEST_CHECK(ct->root->equals->equals->equals->equals->equals->equals->right->character == 't');
    TEST_CHECK(strcmp(ct->root->equals->equals->equals->equals->equals->equals->equals->string, twentyone) == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->equals->equals->equals->equals->right->equals->string, twentytwo) == 0);

    ternarytrie_free(ct);
}

void test_add_similar_strings() {
    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    const char* five = "five";
    const char* six = "six";
    const char* seven = "seven";
    const char* eight = "eight";
    const char* nine = "nine";
    const char* ten = "ten";
    const char* eleven = "eleven";
    const char* sixteen = "sixteen";
    const char* seventeen = "seventeen";
    const char* nineteen = "nineteen";
    const char* twenty = "twenty";
    const char* sevens = "sevens";


    TEST_CHECK(ternarytrie_add(ct, five));
    TEST_CHECK(ternarytrie_add(ct, six));
    TEST_CHECK(ternarytrie_add(ct, seven));
    TEST_CHECK(ternarytrie_add(ct, eight));
    TEST_CHECK(ternarytrie_add(ct, nine));
    TEST_CHECK(ternarytrie_add(ct, ten));
    TEST_CHECK(ternarytrie_add(ct, eleven));
    TEST_CHECK(ternarytrie_add(ct, sixteen));
    TEST_CHECK(ternarytrie_add(ct, seventeen));
    TEST_CHECK(ternarytrie_add(ct, nineteen));
    TEST_CHECK(ternarytrie_add(ct, twenty));
    TEST_CHECK(ternarytrie_add(ct, sevens));

    TEST_SIZE(ct, 12);

    TEST_CHECK(ternarytrie_search(ct, five));
    TEST_CHECK(ternarytrie_search(ct, six));
    TEST_CHECK(ternarytrie_search(ct, seven));
    TEST_CHECK(ternarytrie_search(ct, eight));
    TEST_CHECK(ternarytrie_search(ct, nine));
    TEST_CHECK(ternarytrie_search(ct, ten));
    TEST_CHECK(ternarytrie_search(ct, eleven));
    TEST_CHECK(ternarytrie_search(ct, sixteen));
    TEST_CHECK(ternarytrie_search(ct, seventeen));
    TEST_CHECK(ternarytrie_search(ct, nineteen));
    TEST_CHECK(ternarytrie_search(ct, twenty));
    TEST_CHECK(ternarytrie_search(ct, sevens));

    TEST_CHECK(!ternarytrie_add(ct, five));
    TEST_CHECK(!ternarytrie_add(ct, six));
    TEST_CHECK(!ternarytrie_add(ct, seven));
    TEST_CHECK(!ternarytrie_add(ct, eight));
    TEST_CHECK(!ternarytrie_add(ct, nine));
    TEST_CHECK(!ternarytrie_add(ct, ten));
    TEST_CHECK(!ternarytrie_add(ct, eleven));
    TEST_CHECK(!ternarytrie_add(ct, sixteen));
    TEST_CHECK(!ternarytrie_add(ct, seventeen));
    TEST_CHECK(!ternarytrie_add(ct, nineteen));
    TEST_CHECK(!ternarytrie_add(ct, twenty));
    TEST_CHECK(!ternarytrie_add(ct, sevens));

    // structuur nakijken
    TEST_CHECK(ct->root->character == 'f');
    TEST_CHECK(ct->root->left->character == 'e');
    TEST_CHECK(strcmp(ct->root->left->equals->equals->string, eight) == 0);
    TEST_CHECK(strcmp(ct->root->left->equals->right->equals->string, eleven) == 0);
    TEST_CHECK(strcmp(ct->root->equals->string, five) == 0);
    TEST_CHECK(ct->root->right->character == 's');
    TEST_CHECK(ct->root->right->right->character == 't');
    TEST_CHECK(strcmp(ct->root->right->right->equals->equals->string, ten) == 0);
    TEST_CHECK(strcmp(ct->root->right->right->equals->right->equals->string, twenty) == 0);
    TEST_CHECK(ct->root->right->equals->character == 'i');
    TEST_CHECK(strcmp(ct->root->right->equals->equals->equals->equals->string, six) == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->equals->equals->right->equals->string, sixteen) == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->equals->equals->equals->equals->equals->string, seven) == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->equals->equals->equals->equals->right->equals->string, seventeen) == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->equals->equals->equals->equals->right->left->equals->string, sevens) == 0);
    TEST_CHECK(ct->root->right->left->character = 'n');
    TEST_CHECK(strcmp(ct->root->right->left->equals->equals->equals->equals->equals->string, nine) == 0);
    TEST_CHECK(strcmp(ct->root->right->left->equals->equals->equals->equals->right->equals->string, nineteen) == 0);

    ternarytrie_free(ct);
}

void test_remove_one() {
    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    const char* string = "this is a test";
    TEST_CHECK(ternarytrie_add(ct, string));
    TEST_SIZE(ct, 1);

    TEST_CHECK(ternarytrie_remove(ct, string));
    TEST_SIZE(ct, 0);

    // Structuur nakijken
    TEST_CHECK(ct->root->character == '\0');
    TEST_CHECK(ct->root->equals == NULL && ct->root->right == NULL && ct->root->left == NULL);

    ternarytrie_free(ct);
}

void test_remove_more() {
    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    TEST_CHECK(ternarytrie_add(ct, one));
    TEST_CHECK(ternarytrie_add(ct, two));
    TEST_CHECK(ternarytrie_add(ct, twenty));
    TEST_CHECK(ternarytrie_add(ct, twentytwo));

    TEST_SIZE(ct, 4);

    TEST_CHECK(ternarytrie_remove(ct, one));

    // Structuur nakijken
    TEST_CHECK(ct->root->character == 't' && ct->root->right == NULL && ct->root->left == NULL);
    TEST_CHECK(ct->root->equals->character == 'w' && ct->root->equals->equals->character == 'o');
    TEST_CHECK(ct->root->equals->equals->equals->character == '\0');
    TEST_CHECK(strcmp(ct->root->equals->equals->equals->string, two) == 0);
    TEST_CHECK(ct->root->equals->equals->left->character == 'e');
    TEST_CHECK(ct->root->equals->equals->left->equals->character == 'n');
    TEST_CHECK(ct->root->equals->equals->left->equals->equals->character == 't');
    TEST_CHECK(ct->root->equals->equals->left->equals->equals->equals->character == 'y');
    TEST_CHECK(ct->root->equals->equals->left->equals->equals->equals->equals->character == '\0');
    TEST_CHECK(ct->root->equals->equals->left->equals->equals->equals->equals->equals->character == '\0');
    TEST_CHECK(strcmp(ct->root->equals->equals->left->equals->equals->equals->equals->equals->string, twenty) == 0);
    TEST_CHECK(ct->root->equals->equals->left->equals->equals->equals->equals->right->character == 't');
    TEST_CHECK(ct->root->equals->equals->left->equals->equals->equals->equals->right->equals->character == '\0');
    TEST_CHECK(strcmp(ct->root->equals->equals->left->equals->equals->equals->equals->right->equals->string, twentytwo) == 0);

    TEST_CHECK(ternarytrie_remove(ct, two));

    // Structuur nakijken
    TEST_CHECK(ct->root->character == 't' && ct->root->left == NULL && ct->root->right == NULL);
    TEST_CHECK(ct->root->equals->character == 'w' && ct->root->equals->equals->character == 'e');
    TEST_CHECK(ct->root->equals->equals->equals->character == 'n' && ct->root->equals->equals->equals->equals->character == 't');
    TEST_CHECK(ct->root->equals->equals->equals->equals->equals->character == 'y');
    TEST_CHECK(ct->root->equals->equals->equals->equals->equals->equals->character == '\0');
    TEST_CHECK(strcmp(ct->root->equals->equals->equals->equals->equals->equals->equals->string, twenty) == 0);
    TEST_CHECK(ct->root->equals->equals->equals->equals->equals->equals->right->character == 't');
    TEST_CHECK(strcmp(ct->root->equals->equals->equals->equals->equals->equals->right->equals->string, twentytwo) == 0);

    TEST_CHECK(ternarytrie_remove(ct, twenty));

    // Structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(ct->root->equals->character == '\0');
    TEST_CHECK(strcmp(ct->root->equals->string, twentytwo) == 0);

    TEST_CHECK(ternarytrie_remove(ct, twentytwo));
    TEST_CHECK(ct->root->character == '\0');

    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);
}

void test_remove_not_present() {
    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    TEST_CHECK(ternarytrie_add(ct, "this string exists"));
    TEST_CHECK(!ternarytrie_remove(ct, "this string does not exist"));

    ternarytrie_free(ct);
}

// Hier wordt de string "twenty" verwijderd. Hiermee wordt gecontroleerd dat het pad tussen "two"
// en "twentytwo" zonder problemen terug verkort wordt. Dit pad was immers enkel nodig als "twenty" ook nog in de boom zat.
void test_remove_twenty() {
    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    TEST_CHECK(ternarytrie_add(ct, one));
    TEST_CHECK(ternarytrie_add(ct, two));
    TEST_CHECK(ternarytrie_add(ct, twenty));
    TEST_CHECK(ternarytrie_add(ct, twentytwo));

    TEST_SIZE(ct, 4);

    TEST_CHECK(ternarytrie_remove(ct, one));
    TEST_CHECK(ternarytrie_remove(ct, twenty));

    TEST_CHECK(!ternarytrie_search(ct, twenty));
    TEST_CHECK(ternarytrie_search(ct,twentytwo));

    TEST_SIZE(ct, 2);

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->equals->equals->equals->string, two) == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->left->equals->string, twentytwo) == 0);


    ternarytrie_free(ct);
}

// Hier wordt de string "twentytwo" verwijderd. Hiermee wordt gecontroleerd dat het pad tussen "two"
// en "twenty" zonder problemen terug verkort wordt. Dit pad was immers enkel nodig als "twentytwo" ook nog in de boom zat.
void test_remove_twentytwo() {
    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    TEST_CHECK(ternarytrie_add(ct, one));
    TEST_CHECK(ternarytrie_add(ct, two));
    TEST_CHECK(ternarytrie_add(ct, twenty));
    TEST_CHECK(ternarytrie_add(ct, twentytwo));

    TEST_SIZE(ct, 4);

    TEST_CHECK(ternarytrie_remove(ct, one));
    TEST_CHECK(ternarytrie_remove(ct, twentytwo));

    TEST_CHECK(ternarytrie_search(ct, twenty));
    TEST_CHECK(!ternarytrie_search(ct,twentytwo));

    TEST_SIZE(ct, 2);

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->equals->equals->equals->string, two) == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->left->equals->string, twenty) == 0);

    ternarytrie_free(ct);
}

// Hier worden de strings "two", "twenty", "twentyfour" en "twentyseven" toegevoegd. Dan wordt "twentyfour" verwijderd.
// Hierbij wordt gecontroleerd of de functie "rearrange_trie" de trie correct hervormt.
void test_remove_twentyfour() {
    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentyfour = "twentyfour";
    const char* twentyseven = "twentyseven";
    TEST_CHECK(ternarytrie_add(ct, two));
    TEST_CHECK(ternarytrie_add(ct, twenty));
    TEST_CHECK(ternarytrie_add(ct, twentyfour));
    TEST_CHECK(ternarytrie_add(ct, twentyseven));

    TEST_SIZE(ct, 4);

    TEST_CHECK(ternarytrie_remove(ct, twentyfour));

    TEST_CHECK(ternarytrie_search(ct, twenty));
    TEST_CHECK(ternarytrie_search(ct,twentyseven));
    TEST_CHECK(!ternarytrie_search(ct, twentyfour));

    TEST_SIZE(ct, 3);

    // Structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->equals->equals->equals->string, two) == 0);
    TEST_CHECK(ct->root->equals->equals->left->character == 'e');
    TEST_CHECK(strcmp(ct->root->equals->equals->left->equals->equals->equals->equals->equals->string, twenty) == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->left->equals->equals->equals->equals->right->equals->string, twentyseven) == 0);

    ternarytrie_free(ct);
}

// Hierbij heeft de boom een lang pad naar 2 bladeren, waarbij het pad geen extra vertakkingen heeft.
// Deze bladeren worden verwijderd.
void test_remove_far_leafs() {
    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    const char* twentyone = "twentyone";
    const char* twentytwo = "twentytwo";

    TEST_CHECK(ternarytrie_add(ct, twentyone));
    TEST_CHECK(ternarytrie_add(ct, twentytwo));

    TEST_SIZE(ct, 2);

    TEST_CHECK(ternarytrie_search(ct, twentyone));
    TEST_CHECK(ternarytrie_search(ct, twentytwo));

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->equals->equals->equals->equals->equals->equals->equals->string, twentyone) == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->equals->equals->equals->equals->right->equals->string, twentytwo) == 0);

    TEST_CHECK(ternarytrie_remove(ct, twentyone));
    TEST_CHECK(!ternarytrie_search(ct, twentyone));
    TEST_CHECK(ternarytrie_search(ct, twentytwo));
    TEST_SIZE(ct, 1);

    // structuur nakijken
    TEST_CHECK(strcmp(ct->root->equals->string, twentytwo) == 0);

    TEST_CHECK(ternarytrie_remove(ct, twentytwo));
    TEST_CHECK(!ternarytrie_search(ct, twentytwo));
    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hier worden de datasets toegevoegd en terug verwijderd aan de trie om te controleren dat alles foutloos verkoopt
void dataset_geschud_piepklein() {
    FILE* fp = fopen("../data/geschud_piepklein.g6", "r");
    size_t size = 1000;
    char string[200];
    char** strings = calloc(size, sizeof(char*));

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    int index = 0;
    while (fgets(string, sizeof(string), fp)) {
        char* s = calloc(strlen(string)+1, sizeof(char));
        strcpy(s, string);
        strings[index] = s;
        index ++;
    }
    fclose(fp);

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);

    TEST_SIZE(ct, size);

    gettimeofday(&begin, 0);
    for(size_t i = 0; i < size; i++) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}

void dataset_geschud_klein() {
    FILE* fp = fopen("../data/geschud_klein.g6", "r");
    size_t size = 10000;
    char string[200];
    char** strings = calloc(size, sizeof(char*));

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    int index = 0;
    while (fgets(string, sizeof(string), fp)) {
        char* s = calloc(strlen(string)+1, sizeof(char));
        strcpy(s, string);
        strings[index] = s;
        index ++;
    }
    fclose(fp);

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);

    TEST_SIZE(ct, size);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}

void dataset_geschud_middelmaat() {
    FILE* fp = fopen("../data/geschud_middelmaat.g6", "r");
    size_t size = 100000;
    char string[200];
    char** strings = calloc(size, sizeof(char*));

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    int index = 0;
    while (fgets(string, sizeof(string), fp)) {
        char* s = calloc(strlen(string)+1, sizeof(char));
        strcpy(s, string);
        strings[index] = s;
        index ++;
    }
    fclose(fp);

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);

    TEST_SIZE(ct, size);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}

void dataset_geschud_groot() {
    FILE* fp = fopen("../data/geschud_groot.g6", "r");
    size_t size = 500000;
    char string[200];
    char** strings = calloc(size, sizeof(char*));

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    int index = 0;
    while (fgets(string, sizeof(string), fp)) {
        char* s = calloc(strlen(string)+1, sizeof(char));
        strcpy(s, string);
        strings[index] = s;
        index ++;
    }
    fclose(fp);

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);

    TEST_SIZE(ct, size);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}

void dataset_geschud_zeer_groot() {
    FILE* fp = fopen("../data/geschud_zeer_groot.g6", "r");
    size_t size = 1000000;
    char string[200];
    char** strings = calloc(size, sizeof(char*));

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    int index = 0;
    while (fgets(string, sizeof(string), fp)) {
        char* s = calloc(strlen(string)+1, sizeof(char));
        strcpy(s, string);
        strings[index] = s;
        index ++;
    }
    fclose(fp);

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);

    TEST_SIZE(ct, size);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}

void dataset_geschud_volledig() {
    FILE* fp = fopen("../data/geschud.g6", "r");
    size_t size = 1743797;
    char string[200];
    char** strings = calloc(size, sizeof(char*));

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    int index = 0;
    while (fgets(string, sizeof(string), fp)) {
        char* s = calloc(strlen(string)+1, sizeof(char));
        strcpy(s, string);
        strings[index] = s;
        index ++;
    }
    fclose(fp);

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);

    TEST_SIZE(ct, size);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}
void dataset_piepklein() {
    FILE* fp = fopen("../data/blokgrafen_piepklein.g6", "r");
    size_t size = 1000;
    char string[200];
    char** strings = calloc(size, sizeof(char*));

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    int index = 0;
    while (fgets(string, sizeof(string), fp)) {
        char* s = calloc(strlen(string)+1, sizeof(char));
        strcpy(s, string);
        strings[index] = s;
        index ++;
    }
    fclose(fp);

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);

    TEST_SIZE(ct, size);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}

void dataset_klein() {
    FILE* fp = fopen("../data/blokgrafen_klein.g6", "r");
    size_t size = 10000;
    char string[200];
    char** strings = calloc(size, sizeof(char*));

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    int index = 0;
    while (fgets(string, sizeof(string), fp)) {
        char* s = calloc(strlen(string)+1, sizeof(char));
        strcpy(s, string);
        strings[index] = s;
        index ++;
    }
    fclose(fp);

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);

    TEST_SIZE(ct, size);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}

void dataset_middelmaat() {
    FILE* fp = fopen("../data/blokgrafen_middelmaat.g6", "r");
    size_t size = 100000;
    char string[200];
    char** strings = calloc(size, sizeof(char*));

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    int index = 0;
    while (fgets(string, sizeof(string), fp)) {
        char* s = calloc(strlen(string)+1, sizeof(char));
        strcpy(s, string);
        strings[index] = s;
        index ++;
    }
    fclose(fp);

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);

    TEST_SIZE(ct, size);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}

void dataset_groot() {
    FILE* fp = fopen("../data/blokgrafen_groot.g6", "r");
    size_t size = 500000;
    char string[200];
    char** strings = calloc(size, sizeof(char*));

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    int index = 0;
    while (fgets(string, sizeof(string), fp)) {
        char* s = calloc(strlen(string)+1, sizeof(char));
        strcpy(s, string);
        strings[index] = s;
        index ++;
    }
    fclose(fp);

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);

    TEST_SIZE(ct, size);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}

void dataset_zeer_groot() {
    FILE* fp = fopen("../data/blokgrafen_zeer_groot.g6", "r");
    size_t size = 1000000;
    char string[200];
    char** strings = calloc(size, sizeof(char*));

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    int index = 0;
    while (fgets(string, sizeof(string), fp)) {
        char* s = calloc(strlen(string)+1, sizeof(char));
        strcpy(s, string);
        strings[index] = s;
        index ++;
    }
    fclose(fp);

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);

    TEST_SIZE(ct, size);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}

void dataset_volledig() {
    FILE* fp = fopen("../data/blokgrafen.g6", "r");
    size_t size = 1743797;
    char string[200];
    char** strings = calloc(size, sizeof(char*));

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    int index = 0;
    while (fgets(string, sizeof(string), fp)) {
        char* s = calloc(strlen(string)+1, sizeof(char));
        strcpy(s, string);
        strings[index] = s;
        index ++;
    }
    fclose(fp);

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);

    TEST_SIZE(ct, size);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);


    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}

TEST_LIST = {
        {"ternarytrie init",test_init },
        { "ternarytrie add one",test_add_one },
        { "ternarytrie add more",test_add_more },
        { "ternarytrie add far leafs",test_add_far_leafs },
        { "ternarytrie add similar strings",test_add_similar_strings },
        { "ternarytrie search not present",test_search_not_present},

        { "ternarytrie remove one",test_remove_one },
        { "ternarytrie remove more",test_remove_more },
        { "ternarytrie remove not present",test_remove_not_present},
        { "ternarytrie remove twenty",test_remove_twenty },
        { "ternarytrie remove twentytwo",test_remove_twentytwo },
        { "ternarytrie remove twentyfour",test_remove_twentyfour },
        { "ternarytrie remove far leafs",test_remove_far_leafs},
        { NULL, NULL}
};

// aangepaste TEST_LIST voor de datasets
/*TEST_LIST = {
        { "ternarytrie dataset geschud piepklein",dataset_geschud_piepklein },
        { "ternarytrie dataset geschud klein",dataset_geschud_klein },
        { "ternarytrie dataset geschud middelmaat",dataset_geschud_middelmaat },
        { "ternarytrie dataset geschud groot",dataset_geschud_groot },
        { "ternarytrie dataset geschud zeer groot",dataset_geschud_zeer_groot },
        { "ternarytrie dataset volledig",dataset_geschud_volledig },
        {"ternarytrie dataset piepklein", dataset_piepklein},
        {"ternarytrie dataset klein", dataset_klein},
        {"ternarytrie dataset middelmaat", dataset_middelmaat},
        {"ternarytrie dataset groot", dataset_groot},
        {"ternarytrie dataset zeer groot", dataset_zeer_groot},
        {"ternarytrie dataset volledig", dataset_volledig},
        { NULL, NULL}
};*/