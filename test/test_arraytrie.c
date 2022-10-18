#include "test.h"
#include "arraytrie.h"

#define TEST_SIZE(ct, size) \
    TEST_CHECK(arraytrie_size(ct) == size); \
    TEST_MSG("Size: %zu", arraytrie_size(ct))
                                \
void test_init() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);
    TEST_SIZE(ct, 0);
    arraytrie_free(ct);
}

void test_add_one() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* string = "this is a test";
    TEST_CHECK(arraytrie_add(ct, string));
    TEST_CHECK(arraytrie_search(ct, string));
    TEST_SIZE(ct, 1);
    arraytrie_free(ct);
}

void test_search_not_present() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    TEST_CHECK(arraytrie_add(ct, "this string exists"));
    TEST_CHECK(!arraytrie_search(ct, "this string does not exist"));
    arraytrie_free(ct);
}

void test_add_more() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";

    TEST_CHECK(arraytrie_add(ct, one));
    TEST_CHECK(arraytrie_add(ct, two));
    TEST_CHECK(arraytrie_add(ct, twenty));
    TEST_CHECK(arraytrie_add(ct, twentytwo));

    TEST_SIZE(ct, 4);

    TEST_CHECK(arraytrie_search(ct, one));
    TEST_CHECK(arraytrie_search(ct, two));
    TEST_CHECK(arraytrie_search(ct, twenty));
    TEST_CHECK(arraytrie_search(ct, twentytwo));

    TEST_CHECK(!arraytrie_add(ct, one));
    TEST_CHECK(!arraytrie_add(ct, two));
    TEST_CHECK(!arraytrie_add(ct, twenty));
    TEST_CHECK(!arraytrie_add(ct, twentytwo));

    arraytrie_free(ct);
}

// Hierbij wordt een extra woord "twenz" toegevoegd.
// De skip ("nty") van de top die naar "twenty" en "twentytwo" gaat, moet hierdoor aangepast worden
// naar een kortere skip ("n"). In deze test wordt nagekeken of dit correct verlopen is.
void test_add_string_smaller_than_skip_length() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    const char* twenz = "twenz";

    TEST_CHECK(arraytrie_add(ct, one));
    TEST_CHECK(arraytrie_add(ct, two));
    TEST_CHECK(arraytrie_add(ct, twenty));
    TEST_CHECK(arraytrie_add(ct, twentytwo));
    TEST_CHECK(arraytrie_add(ct, twenz));

    TEST_SIZE(ct, 5);

    TEST_CHECK(arraytrie_search(ct, one));
    TEST_CHECK(arraytrie_search(ct, two));
    TEST_CHECK(arraytrie_search(ct, twenty));
    TEST_CHECK(arraytrie_search(ct, twentytwo));
    TEST_CHECK(arraytrie_search(ct, twenz));
}

// Hier wordt een string toegevoegd waardoor de skip van een top aangepast moet worden.
// De string "twentwo" wordt toegevoegd. Deze heeft de string "nt" gelijk met de string "twenty" en "twentytwo".
// De skip moet dus aangepast worden.
void test_add_string_with_different_skip_length() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    const char* twentwo = "twentwo";

    TEST_CHECK(arraytrie_add(ct, one));
    TEST_CHECK(arraytrie_add(ct, two));
    TEST_CHECK(arraytrie_add(ct, twenty));
    TEST_CHECK(arraytrie_add(ct, twentytwo));
    TEST_CHECK(arraytrie_add(ct, twentwo));

    TEST_SIZE(ct, 5);

    TEST_CHECK(arraytrie_search(ct, one));
    TEST_CHECK(arraytrie_search(ct, two));
    TEST_CHECK(arraytrie_search(ct, twenty));
    TEST_CHECK(arraytrie_search(ct, twentytwo));
    TEST_CHECK(arraytrie_search(ct, twentwo));
}

void test_remove_one() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* string = "this is a test";
    TEST_CHECK(arraytrie_add(ct, string));
    TEST_SIZE(ct, 1);

    TEST_CHECK(arraytrie_remove(ct, string));
    TEST_SIZE(ct, 0);

    arraytrie_free(ct);
}

void test_remove_more() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    TEST_CHECK(arraytrie_add(ct, one));
    TEST_CHECK(arraytrie_add(ct, two));
    TEST_CHECK(arraytrie_add(ct, twenty));
    TEST_CHECK(arraytrie_add(ct, twentytwo));

    TEST_SIZE(ct, 4);

    TEST_CHECK(arraytrie_remove(ct, one));
    TEST_CHECK(arraytrie_remove(ct, two));
    TEST_CHECK(arraytrie_remove(ct, twenty));
    TEST_CHECK(arraytrie_remove(ct, twentytwo));

    TEST_SIZE(ct, 0);

    arraytrie_free(ct);
}

void test_remove_not_present() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    TEST_CHECK(arraytrie_add(ct, "this string exists"));
    TEST_CHECK(!arraytrie_remove(ct, "this string does not exist"));

    arraytrie_free(ct);
}

// Het kind van de wortel verwijderen. Hierbij heeft de wortel 1 kind.
void test_remove_only_child_of_root() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    TEST_CHECK(arraytrie_add(ct, one));
    TEST_SIZE(ct, 1);

    TEST_CHECK(arraytrie_remove(ct, one));
    TEST_SIZE(ct, 0);
    TEST_CHECK(!arraytrie_search(ct, one));
}

// Het kind van de wortel verwijderen. Hierbij heeft de wortel 2 kinderen, waarvan de 2de een interne top is.
void test_remove_child_of_root_with_2_children() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    TEST_CHECK(arraytrie_add(ct, one));
    TEST_CHECK(arraytrie_add(ct, two));
    TEST_CHECK(arraytrie_add(ct, twenty));
    TEST_CHECK(arraytrie_add(ct, twentytwo));

    TEST_SIZE(ct, 4);

    TEST_CHECK(arraytrie_remove(ct, one));

    TEST_SIZE(ct, 3);
    TEST_CHECK(!arraytrie_search(ct, one));
    TEST_CHECK(arraytrie_search(ct, two));
    TEST_CHECK(arraytrie_search(ct, twenty));
    TEST_CHECK(arraytrie_search(ct, twentytwo));

    arraytrie_free(ct);
}

// Het kind van de wortel verwijderen. Hierbij heeft de wortel 2 kinderen, waarvan de 2de een interne top is.
void test_remove_child_of_root_with_2_leafs() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    TEST_CHECK(arraytrie_add(ct, one));
    TEST_CHECK(arraytrie_add(ct, two));

    TEST_SIZE(ct, 2);

    TEST_CHECK(arraytrie_remove(ct, one));

    TEST_SIZE(ct, 1);
    TEST_CHECK(!arraytrie_search(ct, one));
    TEST_CHECK(arraytrie_search(ct, two));

    arraytrie_free(ct);
}

// Het kind van de wortel verwijderen. Hierbij heeft de wortel meer dan 2 kinderen.
void test_remove_child_of_root_with_more_children() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    const char* four = "four";
    TEST_CHECK(arraytrie_add(ct, one));
    TEST_CHECK(arraytrie_add(ct, two));
    TEST_CHECK(arraytrie_add(ct, twenty));
    TEST_CHECK(arraytrie_add(ct, twentytwo));
    TEST_CHECK(arraytrie_add(ct, four));

    TEST_SIZE(ct, 5);

    TEST_CHECK(arraytrie_search(ct, one));
    TEST_CHECK(arraytrie_search(ct, two));
    TEST_CHECK(arraytrie_search(ct, twenty));
    TEST_CHECK(arraytrie_search(ct, twentytwo));
    TEST_CHECK(arraytrie_search(ct, four));

    TEST_CHECK(arraytrie_remove(ct, four));

    TEST_SIZE(ct, 4);
    TEST_CHECK(!arraytrie_search(ct, four));
    TEST_CHECK(arraytrie_search(ct, one));
    TEST_CHECK(arraytrie_search(ct, two));
    TEST_CHECK(arraytrie_search(ct, twenty));
    TEST_CHECK(arraytrie_search(ct, twentytwo));

    arraytrie_free(ct);
}

// Het kind verwijderen van een top die 2 kinderen heeft. Het ander kind is hierbij een interne top.
void test_remove_child_of_node_with_2_children() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    TEST_CHECK(arraytrie_add(ct, one));
    TEST_CHECK(arraytrie_add(ct, two));
    TEST_CHECK(arraytrie_add(ct, twenty));
    TEST_CHECK(arraytrie_add(ct, twentytwo));

    TEST_SIZE(ct, 4);

    TEST_CHECK(arraytrie_remove(ct, two));

    TEST_SIZE(ct, 3);
    TEST_CHECK(!arraytrie_search(ct, two));
    TEST_CHECK(arraytrie_search(ct, one));
    TEST_CHECK(arraytrie_search(ct, twenty));
    TEST_CHECK(arraytrie_search(ct, twentytwo));
}

// Het kind verwijderen van een top met 2 bladeren als kinderen. (Het te verwijderen blad is er 1 van.)
void test_remove_child_of_node_with_2_leafs_as_children() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    TEST_CHECK(arraytrie_add(ct, one));
    TEST_CHECK(arraytrie_add(ct, two));
    TEST_CHECK(arraytrie_add(ct, twenty));
    TEST_CHECK(arraytrie_add(ct, twentytwo));

    TEST_SIZE(ct, 4);

    TEST_CHECK(arraytrie_remove(ct, twentytwo));

    TEST_SIZE(ct, 3);
    TEST_CHECK(!arraytrie_search(ct, twentytwo));
    TEST_CHECK(arraytrie_search(ct, one));
    TEST_CHECK(arraytrie_search(ct, twenty));
    TEST_CHECK(arraytrie_search(ct, two));
}

// Het kind verwijderen van een top met meer dan 2 kinderen.
// Hierbij heeft de top 3 kinderen: het blad dat verwijderd wordt, een ander blad en een interne top.
void test_remove_child_of_node_with_more_than_2_children() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* one = "one";
    const char* two = "two";
    const char* twenty = "twenty";
    const char* twentytwo = "twentytwo";
    const char* twentyone = "twentyone";
    const char* twentyfour = "twentyfour";
    const char* twentyfive = "twentyfive";
    TEST_CHECK(arraytrie_add(ct, one));
    TEST_CHECK(arraytrie_add(ct, two));
    TEST_CHECK(arraytrie_add(ct, twenty));
    TEST_CHECK(arraytrie_add(ct, twentytwo));
    TEST_CHECK(arraytrie_add(ct, twentyone));
    TEST_CHECK(arraytrie_add(ct, twentyfour));
    TEST_CHECK(arraytrie_add(ct, twentyfive));

    TEST_SIZE(ct, 7);

    TEST_CHECK(arraytrie_search(ct, twentyone));
    TEST_CHECK(arraytrie_remove(ct, twentyone));

    TEST_CHECK(!arraytrie_search(ct, twentyone));
    TEST_CHECK(arraytrie_search(ct, one));
    TEST_CHECK(arraytrie_search(ct, twenty));
    TEST_CHECK(arraytrie_search(ct, two));
    TEST_CHECK(arraytrie_search(ct, twentytwo));
    TEST_CHECK(arraytrie_search(ct, twentyfour));
    TEST_CHECK(arraytrie_search(ct, twentyfive));
    TEST_SIZE(ct, 6);
}

TEST_LIST = {
        { "arraytrie init",test_init },
        { "arraytrie add one",test_add_one },
        { "arraytrie add more",test_add_more },
        { "arraytrie add string smaller than skip size",test_add_string_smaller_than_skip_length},
        { "arraytrie add string with different skip size", test_add_string_with_different_skip_length},
        { "arraytrie search not present",test_search_not_present},

        { "arraytrie remove one",test_remove_one },
        { "arraytrie remove more",test_remove_more },
        { "arraytrie remove not present",test_remove_not_present},
        { "arraytrie remove only child of root",test_remove_only_child_of_root},
        { "arraytrie remove child of root with 2 children",test_remove_child_of_root_with_2_children},
        { "arraytrie remove child of root with 2 leafs",test_remove_child_of_root_with_2_leafs},
        { "arraytrie remove child of root with more than 2 children",test_remove_child_of_root_with_more_children},
        { "arraytrie remove child of node with 2 children",test_remove_child_of_node_with_2_children},
        { "arraytrie remove child of node with 2 leafs as children",test_remove_child_of_node_with_2_leafs_as_children},
        { "arraytrie remove child of node with more than 2 children",test_remove_child_of_node_with_more_than_2_children},
        { NULL, NULL}
};
