#include "test.h"
#include "customtrie.h"

#define TEST_SIZE(ct, size) \
    TEST_CHECK(customtrie_size(ct) == size); \
    TEST_MSG("Size: %llu", customtrie_size(ct))
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
    TEST_CHECK(customtrie_remove(ct, two));
    TEST_CHECK(customtrie_remove(ct, twenty));
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
    TEST_CHECK(customtrie_remove(ct, one));
    TEST_CHECK(customtrie_remove(ct, two));
    TEST_CHECK(customtrie_remove(ct, twenty));

    TEST_SIZE(ct, 0);

    customtrie_free(ct);
}

void test_remove_not_present() {
    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    TEST_CHECK(customtrie_add(ct, "this string exists"));
    TEST_CHECK(!customtrie_remove(ct, "this string does not exist"));

    customtrie_free(ct);
}

/*TEST_LIST = {
        {"customtrie init",test_init },
        { "customtrie add one",test_add_one },
        { "customtrie add more",test_add_more },
        { "customtrie add string smaller than skip length",test_add_string_smaller_than_skip_length},
        { "customtrie add string with different skip length",test_add_string_with_different_skip_length},
        { "customtrie search not present",test_search_not_present},

        { "customtrie remove one",test_remove_one },
        { "customtrie remove more",test_remove_more },
        { "customtrie remove different order",test_remove_different_order },
        { "customtrie remove not present",test_remove_not_present},
        { NULL, NULL}
};*/

TEST_LIST = {
        { "customtrie remove different order",test_remove_different_order },
        { NULL, NULL}
};
