#include "test.h"
#include "ternarytrie.h"

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

/*TEST_LIST = {
        {"ternarytrie init",test_init },
        { "ternarytrie add one",test_add_one },
        { "ternarytrie add more",test_add_more },
        { "ternarytrie add similar strings",test_add_similar_strings },
        { "ternarytrie search not present",test_search_not_present},

        { "ternarytrie remove one",test_remove_one },
        { "ternarytrie remove more",test_remove_more },
        { "ternarytrie remove not present",test_remove_not_present},
        { NULL, NULL}
};*/

TEST_LIST = {
        {"ternarytrie init",test_init },
        { "ternarytrie add one",test_add_one },
        { "ternarytrie add more",test_add_more },
        { "ternarytrie add similar strings",test_add_similar_strings },
        { "ternarytrie search not present",test_search_not_present},
        {NULL, NULL}
};
