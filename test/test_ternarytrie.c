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
    TEST_CHECK(ternarytrie_remove(ct, two));
    TEST_CHECK(ternarytrie_remove(ct, twenty));
    TEST_CHECK(ternarytrie_remove(ct, twentytwo));

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
// en "twenty" zonder problemen terug verkort wordt. Dit pad was immers enkel nodig als "twenty" ook nog in de boom zat.
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

// Hier worden de string "two", "twenty", "twentyfour" en "twentyseven" toegevoegd. Dan wordt "twentyfour" verwijderd.
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
    //char* string = NULL;
    char string[200];
    //size_t len = 0;

    size_t size = 0;

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, string));
        size ++;
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);
    fclose(fp);

    TEST_SIZE(ct, size);

    fp = fopen("../data/geschud_piepklein.g6", "r");
    gettimeofday(&begin, 0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, string));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);
    fclose(fp);

    fp = fopen("../data/geschud_piepklein.g6", "r");
    gettimeofday(&begin, 0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_search(ct, string));
        TEST_CHECK(ternarytrie_remove(ct, string));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);
    fclose(fp);


    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);
}

void dataset_geschud_klein() {
    FILE* fp = fopen("../data/geschud_klein.g6", "r");
    //char* string = NULL;
    char string[200];
    //size_t len = 0;

    size_t size = 0;

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, string));
        size ++;
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);
    fclose(fp);

    TEST_SIZE(ct, size);

    fp = fopen("../data/geschud_klein.g6", "r");
    gettimeofday(&begin, 0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, string));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);
    fclose(fp);

    fp = fopen("../data/geschud_klein.g6", "r");
    gettimeofday(&begin, 0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_search(ct, string));
        TEST_CHECK(ternarytrie_remove(ct, string));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);
    fclose(fp);


    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);
}

void dataset_geschud_middelmaat() {
    FILE* fp = fopen("../data/geschud_middelmaat.g6", "r");
    //char* string = NULL;
    char string[200];
    //size_t len = 0;

    size_t size = 0;

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, string));
        size ++;
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);
    fclose(fp);

    TEST_SIZE(ct, size);

    fp = fopen("../data/geschud_middelmaat.g6", "r");
    gettimeofday(&begin, 0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, string));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);
    fclose(fp);

    fp = fopen("../data/geschud_middelmaat.g6", "r");
    gettimeofday(&begin, 0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_search(ct, string));
        TEST_CHECK(ternarytrie_remove(ct, string));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);
    fclose(fp);


    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);
}

void dataset_geschud_groot() {
    FILE* fp = fopen("../data/geschud_groot.g6", "r");
    //char* string = NULL;
    char string[200];
    //size_t len = 0;

    size_t size = 0;

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, string));
        size ++;
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);
    fclose(fp);

    TEST_SIZE(ct, size);

    fp = fopen("../data/geschud_groot.g6", "r");
    gettimeofday(&begin, 0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, string));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);
    fclose(fp);

    fp = fopen("../data/geschud_groot.g6", "r");
    gettimeofday(&begin, 0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, string));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);
    fclose(fp);


    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);
}

void dataset_geschud_zeer_groot() {
    FILE* fp = fopen("../data/geschud_zeer_groot.g6", "r");
    //char* string = NULL;
    char string[200];
    //size_t len = 0;

    size_t size = 0;

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, string));
        size ++;
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);
    fclose(fp);

    TEST_SIZE(ct, size);

    fp = fopen("../data/geschud_zeer_groot.g6", "r");
    gettimeofday(&begin, 0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, string));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);
    fclose(fp);

    fp = fopen("../data/geschud_zeer_groot.g6", "r");
    gettimeofday(&begin, 0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, string));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);
    fclose(fp);


    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);
}

void dataset_volledig() {
    FILE* fp = fopen("../data/blokgrafen.g6", "r");
    //char* string = NULL;
    char string[200];
    //size_t len = 0;

    size_t size = 0;

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    TernaryTrie* ct = ternarytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(ternarytrie_add(ct, string));
        size ++;
    }
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("size: %zu\n", size);
    printf("tijd voor toevoegen: %f seconds\n", elapsed);
    fclose(fp);

    TEST_SIZE(ct, size);

    fp = fopen("../data/geschud.g6", "r");
    gettimeofday(&begin, 0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(ternarytrie_search(ct, string));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);
    fclose(fp);

    fp = fopen("../data/geschud.g6", "r");
    gettimeofday(&begin, 0);
    while (fgets(string, sizeof(string), fp)) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(ternarytrie_remove(ct, string));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);
    fclose(fp);


    TEST_SIZE(ct, 0);

    ternarytrie_free(ct);
}

TEST_LIST = {
        {"ternarytrie init",test_init },
        { "ternarytrie add one",test_add_one },
        { "ternarytrie add more",test_add_more },
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
        { "ternarytrie dataset piepklein",dataset_geschud_piepklein },
        { "ternarytrie dataset klein",dataset_geschud_klein },
        { "ternarytrie dataset middelmaat",dataset_geschud_middelmaat },
        { "ternarytrie dataset groot",dataset_geschud_groot },
        { "ternarytrie dataset zeer groot",dataset_geschud_zeer_groot },
        { "ternarytrie dataset volledig",dataset_volledig },
        { NULL, NULL}
};*/