#include "test.h"
#include "customtrie.h"
#include "../src/customtrie.c"
#include <sys/time.h>

#define TEST_SIZE(ct, size) \
    TEST_CHECK(customtrie_size(ct) == size); \
    TEST_MSG("Size: %zu", customtrie_size(ct))
                                \
void test_init() {
    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);
    TEST_SIZE(ct, 0);
    customtrie_free(ct);
}

void test_add_one() {
    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    const char* string = "this is a test";
    TEST_CHECK(customtrie_add(ct, string));
    TEST_CHECK(customtrie_search(ct, string));
    TEST_SIZE(ct, 1);

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->skip, "") == 0);
    TEST_CHECK(strcmp(ct->root->skip, "") == 0);
    TEST_CHECK(strcmp(ct->root->equals->string, string) == 0);

    customtrie_free(ct);
}

void test_search_not_present() {
    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    TEST_CHECK(customtrie_add(ct, "this string exists"));
    TEST_CHECK(!customtrie_search(ct, "this string does not exist"));
    customtrie_free(ct);
}

void test_add_more() {
    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";

    TEST_CHECK(customtrie_add(ct, one));
    TEST_CHECK(customtrie_add(ct, two));
    TEST_CHECK(customtrie_add(ct, twenty));
    TEST_CHECK(customtrie_add(ct, twentytwo));

    TEST_SIZE(ct, 4);

    TEST_CHECK(customtrie_search(ct, one));
    TEST_CHECK(customtrie_search(ct, two));
    TEST_CHECK(customtrie_search(ct, twenty));
    TEST_CHECK(customtrie_search(ct, twentytwo));

    TEST_CHECK(!customtrie_add(ct, one));
    TEST_CHECK(!customtrie_add(ct, two));
    TEST_CHECK(!customtrie_add(ct, twenty));
    TEST_CHECK(!customtrie_add(ct, twentytwo));

    // structuur nakijken
    TEST_CHECK(ct->root->character == 'o');
    TEST_CHECK(strcmp(ct->root->skip, "") == 0);
    TEST_CHECK(strcmp(ct->root->equals->string, one) == 0);
    TEST_CHECK(ct->root->right->character == 't');
    TEST_CHECK(strcmp(ct->root->right->skip, "w") == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->equals->string, two) == 0);
    TEST_CHECK(ct->root->right->equals->left->character == 'e');
    TEST_CHECK(strcmp(ct->root->right->equals->left->skip, "nty") == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->equals->equals->string, twenty) == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->equals->right->equals->string, twentytwo) == 0);

    customtrie_free(ct);
}

// Wanneer een nieuwe string toegevoegd moet worden, wordt er eerst gezocht naar de plaats
// waar de string toegevoegd zal worden. Dit kan eindigen in een blad
// waarbij een nieuwe interne top toegevoegd moet worden. Deze situatie wordt in 2 testen nagekeken.
// In de eerste test hebben de beide strings geen gemeenschappelijke deelstring, in de 2de test wel.
void test_add_string_without_mutual_skip() {
    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    const char* seven = "seven";
    const char* seventy = "seventy";
    const char* seventeen = "seventeen";

    TEST_CHECK(customtrie_add(ct, seven));
    TEST_CHECK(customtrie_add(ct, seventy));
    TEST_CHECK(customtrie_add(ct, seventeen));

    TEST_SIZE(ct, 3);

    // Structuur nakijken
    TEST_CHECK(ct->root->character == 's' && strcmp(ct->root->skip, "even") == 0);
    TEST_CHECK(ct->root->equals->character == '\0' && strcmp(ct->root->equals->skip, "") == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->string, seven) == 0);
    TEST_CHECK(ct->root->equals->right->character == 't');
    TEST_CHECK(strcmp(ct->root->equals->right->skip, "") == 0);
    TEST_CHECK(ct->root->equals->right->equals->character == 'y');
    TEST_CHECK(strcmp(ct->root->equals->right->equals->equals->string, seventy) == 0);
    TEST_CHECK(ct->root->equals->right->equals->left->character == 'e');
    TEST_CHECK(strcmp(ct->root->equals->right->equals->left->equals->string, seventeen) == 0);

    customtrie_free(ct);
}

void test_add_string_with_mutual_skip() {
    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    const char* six = "six";
    const char* seventy = "seventy";
    const char* seventeen = "seventeen";

    TEST_CHECK(customtrie_add(ct, six));
    TEST_CHECK(customtrie_add(ct, seventy));
    TEST_CHECK(customtrie_add(ct, seventeen));

    TEST_SIZE(ct, 3);

    // Structuur nakijken
    TEST_CHECK(ct->root->character == 's' && strcmp(ct->root->skip, "") == 0);
    TEST_CHECK(ct->root->equals->character == 'i' && strcmp(ct->root->equals->skip, "") == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->string, six) == 0);
    TEST_CHECK(ct->root->equals->left->character == 'e');
    TEST_CHECK(strcmp(ct->root->equals->left->skip, "vent") == 0);
    TEST_CHECK(ct->root->equals->left->equals->character == 'y');
    TEST_CHECK(strcmp(ct->root->equals->left->equals->equals->string, seventy) == 0);
    TEST_CHECK(ct->root->equals->left->equals->left->character == 'e');
    TEST_CHECK(strcmp(ct->root->equals->left->equals->left->equals->string, seventeen) == 0);

    customtrie_free(ct);
}

// Hierbij wordt een extra woord "twenz" toegevoegd.
// De skip ("nty") van de top die naar "twenty" en "twentytwo" gaat, moet hierdoor aangepast worden
// naar een kortere skip ("n"). In deze test wordt nagekeken of dit correct verlopen is.
void test_add_string_smaller_than_skip_length() {
    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);
    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    const char* twenz = "twenz";

    TEST_CHECK(customtrie_add(ct, one));
    TEST_CHECK(customtrie_add(ct, two));
    TEST_CHECK(customtrie_add(ct, twenty));
    TEST_CHECK(customtrie_add(ct, twentytwo));
    TEST_CHECK(customtrie_add(ct, twenz));

    TEST_SIZE(ct, 5);

    TEST_CHECK(customtrie_search(ct, one));
    TEST_CHECK(customtrie_search(ct, two));
    TEST_CHECK(customtrie_search(ct, twenty));
    TEST_CHECK(customtrie_search(ct, twentytwo));
    TEST_CHECK(customtrie_search(ct, twenz));

    // structuur nakijken
    TEST_CHECK(ct->root->character == 'o');
    TEST_CHECK(ct->root->right->character == 't');
    TEST_CHECK(strcmp(ct->root->right->skip, "w") == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->skip, "n") == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->equals->skip, "y") == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->equals->right->equals->string, twenz) == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->equals->equals->equals->string, twenty) == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->equals->equals->right->equals->string, twentytwo) == 0);

    customtrie_free(ct);
}

// Hier wordt een string toegevoegd waardoor de skip van een top aangepast moet worden.
// De string "twentwo" wordt toegevoegd. Deze heeft de string "nt" gelijk met de string "twenty" en "twentytwo".
// De skip moet dus aangepast worden.
void test_add_string_with_different_skip_length() {
    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    const char* twentwo = "twentwo";

    TEST_CHECK(customtrie_add(ct, one));
    TEST_CHECK(customtrie_add(ct, two));
    TEST_CHECK(customtrie_add(ct, twenty));
    TEST_CHECK(customtrie_add(ct, twentytwo));
    TEST_CHECK(customtrie_add(ct, twentwo));

    TEST_SIZE(ct, 5);

    TEST_CHECK(customtrie_search(ct, one));
    TEST_CHECK(customtrie_search(ct, two));
    TEST_CHECK(customtrie_search(ct, twenty));
    TEST_CHECK(customtrie_search(ct, twentytwo));
    TEST_CHECK(customtrie_search(ct, twentwo));

    // Structuur nakijken
    TEST_CHECK(ct->root->character == 'o');
    TEST_CHECK(strcmp(ct->root->right->skip, "w") == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->equals->string, two) == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->skip, "nt") == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->equals->left->equals->string, twentwo) == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->equals->equals->equals->string, twenty) == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->equals->equals->right->equals->string, twentytwo) == 0);

    customtrie_free(ct);
}

void test_remove_one() {
    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    const char* string = "this is a test";
    TEST_CHECK(customtrie_add(ct, string));
    TEST_SIZE(ct, 1);

    TEST_CHECK(customtrie_remove(ct, string));
    TEST_SIZE(ct, 0);

    customtrie_free(ct);
}

void test_remove_more() {
    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    TEST_CHECK(customtrie_add(ct, one));
    TEST_CHECK(customtrie_add(ct, two));
    TEST_CHECK(customtrie_add(ct, twenty));
    TEST_CHECK(customtrie_add(ct, twentytwo));

    TEST_SIZE(ct, 4);

    TEST_CHECK(customtrie_remove(ct, one));

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->skip, "w") == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->string, two) == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->skip, "nty") == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->equals->equals->string, twenty) == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->equals->right->equals->string, twentytwo) == 0);

    TEST_CHECK(customtrie_remove(ct, two));

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->skip, "wenty") == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->string, twenty) == 0);
    TEST_CHECK(strcmp(ct->root->equals->right->equals->string, twentytwo) == 0);

    TEST_CHECK(customtrie_remove(ct, twenty));

    // structuur nakijken
    TEST_CHECK(ct->root->character = 't');
    TEST_CHECK(strcmp(ct->root->skip, "") == 0);
    TEST_CHECK(strcmp(ct->root->equals->string, twentytwo) == 0);

    TEST_CHECK(customtrie_remove(ct, twentytwo));

    TEST_SIZE(ct, 0);

    customtrie_free(ct);
}

// hierbij wordt het verste blad eerst verwijderd
void test_remove_different_order() {
    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    TEST_CHECK(customtrie_add(ct, one));
    TEST_CHECK(customtrie_add(ct, two));
    TEST_CHECK(customtrie_add(ct, twenty));
    TEST_CHECK(customtrie_add(ct, twentytwo));

    TEST_SIZE(ct, 4);

    TEST_CHECK(customtrie_remove(ct, twentytwo));

    // structuur nakijken
    TEST_CHECK(ct->root->character == 'o');
    TEST_CHECK(strcmp(ct->root->equals->string, one) == 0);
    TEST_CHECK(ct->root->right->character = 't');
    TEST_CHECK(strcmp(ct->root->right->skip, "w") == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->equals->string, two) == 0);
    TEST_CHECK(ct->root->right->equals->left->character == 'e');
    TEST_CHECK(strcmp(ct->root->right->equals->left->skip, "") == 0);
    TEST_CHECK(strcmp(ct->root->right->equals->left->equals->string, twenty) == 0);

    TEST_CHECK(customtrie_remove(ct, one));

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->skip, "w") == 0);

    TEST_CHECK(customtrie_remove(ct, two));

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->skip, "") == 0);
    TEST_CHECK(strcmp(ct->root->equals->string, twenty) == 0);

    TEST_CHECK(customtrie_remove(ct, twenty));

    TEST_SIZE(ct, 0);

    customtrie_free(ct);
}

// Bij de volgende 3 testen hebben 3 strings dezelfde deelstring ("twenty").
// In elke test wordt 1 van die 3 strings verwijderd. Hier wordt de verste vertakking ("twentyseven") verwijderd.
void test_remove_twentyseven() {
    CustomTrie * ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentyfour = "twentyfour";
    const char* twentyseven = "twentyseven";
    TEST_CHECK(customtrie_add(ct, two));
    TEST_CHECK(customtrie_add(ct, twenty));
    TEST_CHECK(customtrie_add(ct, twentyfour));
    TEST_CHECK(customtrie_add(ct, twentyseven));

    TEST_SIZE(ct, 4);

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->skip, "w") == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->string, two) == 0);
    TEST_CHECK(ct->root->equals->left->character == 'e');
    TEST_CHECK(strcmp(ct->root->equals->left->skip, "nty") == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->equals->equals->string, twenty) == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->equals->right->equals->string, twentyfour) == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->equals->right->right->equals->string, twentyseven) == 0);

    TEST_CHECK(customtrie_remove(ct, twentyseven));

    TEST_CHECK(customtrie_search(ct, twenty));
    TEST_CHECK(customtrie_search(ct,twentyfour));
    TEST_CHECK(!customtrie_search(ct, twentyseven));

    TEST_SIZE(ct, 3);

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->skip, "w") == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->string, two) == 0);
    TEST_CHECK(ct->root->equals->left->character == 'e');
    TEST_CHECK(strcmp(ct->root->equals->left->skip, "nty") == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->equals->equals->string, twenty) == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->equals->right->equals->string, twentyfour) == 0);
    TEST_CHECK(ct->root->equals->left->equals->right->right == NULL);

    customtrie_free(ct);
}

// Hier wordt de middelste vertakking ("twentyfour") verwijderd.
void test_remove_twentyfour() {
    CustomTrie * ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentyfour = "twentyfour";
    const char* twentyseven = "twentyseven";
    TEST_CHECK(customtrie_add(ct, two));
    TEST_CHECK(customtrie_add(ct, twenty));
    TEST_CHECK(customtrie_add(ct, twentyfour));
    TEST_CHECK(customtrie_add(ct, twentyseven));

    TEST_SIZE(ct, 4);

    TEST_CHECK(customtrie_remove(ct, twentyfour));

    TEST_CHECK(customtrie_search(ct, twenty));
    TEST_CHECK(customtrie_search(ct,twentyseven));
    TEST_CHECK(!customtrie_search(ct, twentyfour));

    TEST_SIZE(ct, 3);

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->skip, "w") == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->string, two) == 0);
    TEST_CHECK(ct->root->equals->left->character == 'e');
    TEST_CHECK(strcmp(ct->root->equals->left->skip, "nty") == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->equals->equals->string, twenty) == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->equals->right->equals->string, twentyseven) == 0);

    customtrie_free(ct);
}

// Hier wordt de middelste vertakking ("twenty") verwijderd.
void test_remove_twenty() {
    CustomTrie * ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentyfour = "twentyfour";
    const char* twentyseven = "twentyseven";
    TEST_CHECK(customtrie_add(ct, two));
    TEST_CHECK(customtrie_add(ct, twenty));
    TEST_CHECK(customtrie_add(ct, twentyfour));
    TEST_CHECK(customtrie_add(ct, twentyseven));

    TEST_SIZE(ct, 4);

    TEST_CHECK(customtrie_remove(ct, twenty));

    TEST_CHECK(!customtrie_search(ct, twenty));
    TEST_CHECK(customtrie_search(ct,twentyfour));
    TEST_CHECK(customtrie_search(ct, twentyseven));

    TEST_SIZE(ct, 3);

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->skip, "w") == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->string, two) == 0);
    TEST_CHECK(ct->root->equals->left->character == 'e');
    TEST_CHECK(strcmp(ct->root->equals->left->skip, "nty") == 0);
    TEST_CHECK(ct->root->equals->left->equals->character == 'f');
    TEST_CHECK(strcmp(ct->root->equals->left->equals->equals->string, twentyfour) == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->equals->right->equals->string, twentyseven) == 0);

    customtrie_free(ct);
}

// In deze test worden eerst de strings "two", "twenty", "twentyfour" en "twentyfive" toegevoegd en daarna wordt "twenty"
// verwijderd. Hierbij moet de skip aangepast worden van "nty" naar "ntyf".
void test_remove_and_change_skip() {
    CustomTrie * ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentyfour = "twentyfour";
    const char* twentyfive = "twentyfive";
    TEST_CHECK(customtrie_add(ct, two));
    TEST_CHECK(customtrie_add(ct, twenty));
    TEST_CHECK(customtrie_add(ct, twentyfour));
    TEST_CHECK(customtrie_add(ct, twentyfive));

    TEST_SIZE(ct, 4);

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->skip, "w") == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->string, two) == 0);
    TEST_CHECK(ct->root->equals->left->character == 'e');
    TEST_CHECK(strcmp(ct->root->equals->left->skip, "nty") == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->equals->equals->string, twenty) == 0);
    TEST_CHECK(ct->root->equals->left->equals->right->character == 'f');
    TEST_CHECK(ct->root->equals->left->equals->right->equals->character == 'o');
    TEST_CHECK(ct->root->equals->left->equals->right->equals->left->character == 'i');
    TEST_CHECK(strcmp(ct->root->equals->left->equals->right->equals->equals->string, twentyfour) == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->equals->right->equals->left->equals->string, twentyfive) == 0);

    TEST_CHECK(customtrie_remove(ct, twenty));

    TEST_CHECK(customtrie_search(ct, two));
    TEST_CHECK(customtrie_search(ct, twentyfour));
    TEST_CHECK(customtrie_search(ct, twentyfive));
    TEST_CHECK(!customtrie_search(ct, twenty));

    TEST_SIZE(ct, 3);

    // structuur nakijken
    TEST_CHECK(ct->root->character == 't');
    TEST_CHECK(strcmp(ct->root->skip, "w") == 0);
    TEST_CHECK(strcmp(ct->root->equals->equals->string, two) == 0);
    TEST_CHECK(ct->root->equals->left->character == 'e');
    TEST_CHECK(strcmp(ct->root->equals->left->skip, "ntyf") == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->equals->equals->string, twentyfour) == 0);
    TEST_CHECK(strcmp(ct->root->equals->left->equals->left->equals->string, twentyfive) == 0);

    customtrie_free(ct);
}

void test_remove_not_present() {
    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    TEST_CHECK(customtrie_add(ct, "this string exists"));
    TEST_CHECK(!customtrie_remove(ct, "this string does not exist"));

    customtrie_free(ct);
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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(customtrie_add(ct, strings[i]));
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
        TEST_CHECK(customtrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    customtrie_free(ct);

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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(customtrie_add(ct, strings[i]));
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
        TEST_CHECK(customtrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    customtrie_free(ct);

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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(customtrie_add(ct, strings[i]));
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
        TEST_CHECK(customtrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    customtrie_free(ct);

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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(customtrie_add(ct, strings[i]));
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
        TEST_CHECK(customtrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    customtrie_free(ct);

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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(customtrie_add(ct, strings[i]));
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
        TEST_CHECK(customtrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    customtrie_free(ct);

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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(customtrie_add(ct, strings[i]));
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
        TEST_CHECK(customtrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    customtrie_free(ct);

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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(customtrie_add(ct, strings[i]));
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
        TEST_CHECK(customtrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    customtrie_free(ct);

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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(customtrie_add(ct, strings[i]));
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
        TEST_CHECK(customtrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    customtrie_free(ct);

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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(customtrie_add(ct, strings[i]));
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
        TEST_CHECK(customtrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    customtrie_free(ct);

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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(customtrie_add(ct, strings[i]));
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
        TEST_CHECK(customtrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    customtrie_free(ct);

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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(customtrie_add(ct, strings[i]));
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
        TEST_CHECK(customtrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    customtrie_free(ct);

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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(customtrie_add(ct, strings[i]));
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
        TEST_CHECK(customtrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    customtrie_free(ct);

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}

TEST_LIST = {
        {"customtrie init",test_init },
        { "customtrie add one",test_add_one },
        { "customtrie add more",test_add_more },
        { "customtrie add string without mutual skip",test_add_string_without_mutual_skip },
        { "customtrie add string with mutual skip",test_add_string_with_mutual_skip },
        { "customtrie add string smaller than skip length",test_add_string_smaller_than_skip_length},
        { "customtrie add string with different skip length",test_add_string_with_different_skip_length},
        { "customtrie search not present",test_search_not_present},

        { "customtrie remove one",test_remove_one },
        { "customtrie remove more",test_remove_more },
        { "customtrie remove different order",test_remove_different_order },
        { "customtrie remove twentyseven",test_remove_twentyseven },
        { "customtrie remove twentyfour",test_remove_twentyfour },
        { "customtrie remove twenty",test_remove_twenty },
        { "customtrie remove and change skip",test_remove_and_change_skip},
        { "customtrie remove not present",test_remove_not_present},
        { NULL, NULL}
};

// aangepaste TEST_LIST voor de datasets
/*TEST_LIST = {
        { "customtrie dataset geschud piepklein",dataset_geschud_piepklein },
        { "customtrie dataset geschud klein",dataset_geschud_klein },
        { "customtrie dataset geschud middelmaat",dataset_geschud_middelmaat },
        { "customtrie dataset geschud groot",dataset_geschud_groot },
        { "customtrie dataset geschud zeer groot",dataset_geschud_zeer_groot },
        { "customtrie dataset geschud volledig",dataset_geschud_volledig },
        {"customtrie dataset piepklein", dataset_piepklein},
        {"customtrie dataset klein", dataset_klein},
        {"customtrie dataset middelmaat", dataset_middelmaat},
        {"customtrie dataset groot", dataset_groot},
        {"customtrie dataset zeer groot", dataset_zeer_groot},
        {"customtrie dataset volledig", dataset_volledig},
        { NULL, NULL}
};*/
