#include "test.h"
#include "customtrie.h"

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

void test_remove_not_present() {
    CustomTrie* ct = customtrie_init();
    TEST_CHECK(ct != NULL);

    TEST_CHECK(customtrie_add(ct, "this string exists"));
    TEST_CHECK(!customtrie_remove(ct, "this string does not exist"));

    customtrie_free(ct);
}

TEST_LIST = {
        {"customtrie init",test_init },
        { "customtrie add one",test_add_one },
        { "customtrie add more",test_add_more },
        { "customtrie search not present",test_search_not_present},

        { "customtrie remove one",test_remove_one },
        { "customtrie remove more",test_remove_more },
        { "customtrie remove not present",test_remove_not_present},
        { NULL, NULL}
};
