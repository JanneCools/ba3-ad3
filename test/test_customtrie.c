#include "test.h"
#include "customtrie.h"
#include "../src/customtrie.c"

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
    //char* string = NULL;
    char string[200];
    //size_t len = 0;

    size_t size = 0;

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    while (fgets(string, sizeof(string), fp)) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(customtrie_add(ct, string));
        TEST_CHECK(customtrie_search(ct, string));
        size ++;
        //printf("size: %zu\n", size);
    }

    fclose(fp);

    TEST_SIZE(ct, size);
    printf("size: %zu\n", size);

    fp = fopen("../data/geschud_piepklein.g6", "r");
    while (fgets(string, sizeof(string), fp)) {
        //printf("string %s", string);
        TEST_CHECK(customtrie_search(ct, string));
    }
    fclose(fp);

    fp = fopen("../data/geschud_piepklein.g6", "r");
    while (fgets(string, sizeof(string), fp)) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_search(ct, string));
        TEST_CHECK(customtrie_remove(ct, string));
    }

    fclose(fp);


    TEST_SIZE(ct, 0);

    customtrie_free(ct);
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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    while (fgets(string, sizeof(string), fp)) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(customtrie_add(ct, string));
        size ++;
    }

    fclose(fp);

    TEST_SIZE(ct, size);
    printf("size: %zu", size);

    fp = fopen("../data/geschud_klein.g6", "r");
    while (fgets(string, sizeof(string), fp)) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(customtrie_search(ct, string));
    }
    fclose(fp);

    fp = fopen("../data/geschud_klein.g6", "r");
    while (fgets(string, sizeof(string), fp)) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_search(ct, string));
        TEST_CHECK(customtrie_remove(ct, string));
    }

    fclose(fp);


    TEST_SIZE(ct, 0);

    customtrie_free(ct);
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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    while (fgets(string, sizeof(string), fp)) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(customtrie_add(ct, string));
        size ++;
    }

    fclose(fp);

    TEST_SIZE(ct, size);
    printf("size: %zu", size);

    fp = fopen("../data/geschud_middelmaat.g6", "r");
    while (fgets(string, sizeof(string), fp)) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(customtrie_search(ct, string));
    }
    fclose(fp);

    fp = fopen("../data/geschud_middelmaat.g6", "r");
    while (fgets(string, sizeof(string), fp)) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_search(ct, string));
        TEST_CHECK(customtrie_remove(ct, string));
    }

    fclose(fp);


    TEST_SIZE(ct, 0);

    customtrie_free(ct);
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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    while (fgets(string, sizeof(string), fp)) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(customtrie_add(ct, string));
        size ++;
    }

    fclose(fp);

    TEST_SIZE(ct, size);
    printf("size: %zu", size);

    fp = fopen("../data/geschud_groot.g6", "r");
    while (fgets(string, sizeof(string), fp)) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(customtrie_search(ct, string));
    }
    fclose(fp);

    fp = fopen("../data/geschud_groot.g6", "r");
    while (fgets(string, sizeof(string), fp)) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_search(ct, string));
        TEST_CHECK(customtrie_remove(ct, string));
    }

    fclose(fp);


    TEST_SIZE(ct, 0);

    customtrie_free(ct);
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

    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    while (fgets(string, sizeof(string), fp)) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(customtrie_add(ct, string));
        size ++;
    }

    fclose(fp);

    TEST_SIZE(ct, size);
    printf("size: %zu", size);

    fp = fopen("../data/geschud.g6", "r");
    while (fgets(string, sizeof(string), fp)) {
        //printf("string %s, size %d", string, (int)size);
        TEST_CHECK(customtrie_search(ct, string));
    }
    fclose(fp);

    fp = fopen("../data/geschud.g6", "r");
    while (fgets(string, sizeof(string), fp)) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(customtrie_search(ct, string));
        TEST_CHECK(customtrie_remove(ct, string));
    }

    fclose(fp);


    TEST_SIZE(ct, 0);

    customtrie_free(ct);
}

TEST_LIST = {
        {"customtrie init",test_init },
        { "customtrie add one",test_add_one },
        { "customtrie add more",test_add_more },
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
        { "customtrie dataset piepklein",dataset_geschud_piepklein },
        { "customtrie dataset klein",dataset_geschud_klein },
        { "customtrie dataset middelmaat",dataset_geschud_middelmaat },
        { "customtrie dataset groot",dataset_geschud_groot },
        { "customtrie dataset volledig",dataset_volledig },
        { NULL, NULL}
};

// aangepaste TEST_LIST voor de datasets
/*TEST_LIST = {
        { "customtrie dataset piepklein",dataset_geschud_piepklein },
        { "customtrie dataset klein",dataset_geschud_klein },
        { "customtrie dataset middelmaat",dataset_geschud_middelmaat },
        { "customtrie dataset groot",dataset_geschud_groot },
        { "customtrie dataset volledig",dataset_volledig },
        { NULL, NULL}
};*/
