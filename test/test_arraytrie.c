#include "test.h"
#include "arraytrie.h"
#include "../src/arraytrie.c"
#include <sys/time.h>

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

    // structuur nakijken
    TEST_CHECK(ct->character == '\0' && ct->children_size == 1 && strcmp(ct->skip, "") == 0);
    TEST_CHECK(ct->children[0]->character == 't' && ct->children[0]->children_size == 0);
    TEST_CHECK(strcmp(ct->children[0]->string, string) == 0);

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

    // structuur nakijken
    TEST_CHECK(ct->children_size == 2 && strcmp(ct->skip, "") == 0);
    TEST_CHECK(ct->children[0]->character == 'o' && ct->children[0]->children_size == 0);
    TEST_CHECK(strcmp(ct->children[0]->string, one) == 0);
    TEST_CHECK(ct->children[1]->character == 't' && strcmp(ct->children[1]->skip, "w") == 0);
    TEST_CHECK(ct->children[1]->children[0]->character == 'o' && strcmp(ct->children[1]->children[0]->string, two) == 0);
    TEST_CHECK(ct->children[1]->children[1]->character == 'e' && strcmp(ct->children[1]->children[1]->skip, "nty") == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[0]->character == '\0');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[0]->string, twenty) == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[1]->character == 't');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[1]->string, twentytwo) == 0);


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

    // structuur nakijken
    TEST_CHECK(ct->children_size == 2 && strcmp(ct->skip, "") == 0);
    TEST_CHECK(ct->children[0]->character == 'o' && strcmp(ct->children[0]->string, one) == 0);
    TEST_CHECK(ct->children[1]->character == 't' && strcmp(ct->children[1]->skip, "w") == 0);
    TEST_CHECK(ct->children[1]->children[0]->character == 'o' && strcmp(ct->children[1]->children[0]->string, two) == 0);
    TEST_CHECK(ct->children[1]->children[1]->character == 'e' && strcmp(ct->children[1]->children[1]->skip, "n") == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[1]->character == 'z');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[1]->string, twenz) == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[0]->character == 't');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[0]->skip, "y") == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[0]->children[0]->character == '\0');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[0]->children[0]->string, twenty) == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[0]->children[1]->character == 't');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[0]->children[1]->string, twentytwo) == 0);

    arraytrie_free(ct);
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

    // structuur nakijken
    TEST_CHECK(ct->children_size == 2 && strcmp(ct->skip, "") == 0);
    TEST_CHECK(ct->children[0]->character == 'o' && strcmp(ct->children[0]->string, one) == 0);
    TEST_CHECK(ct->children[1]->character == 't' && strcmp(ct->children[1]->skip, "w") == 0);
    TEST_CHECK(ct->children[1]->children[0]->character == 'o' && strcmp(ct->children[1]->children[0]->string, two) == 0);
    TEST_CHECK(ct->children[1]->children[1]->character == 'e' && strcmp(ct->children[1]->children[1]->skip, "nt") == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[1]->character == 'w');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[1]->string, twentwo) == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[0]->character == 'y');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[0]->skip, "") == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[0]->children[0]->character == '\0');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[0]->children[0]->string, twenty) == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[0]->children[1]->character == 't');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[0]->children[1]->string, twentytwo) == 0);

    arraytrie_free(ct);
}


// Wanneer een nieuwe string toegevoegd moet worden, wordt er eerst gezocht naar de plaats
// waar de string toegevoegd zal worden. Dit kan eindigen in een blad
// waarbij een nieuwe interne top toegevoegd moet worden. Deze situatie wordt in 2 testen nagekeken.
// In de eerste test hebben de beide strings geen gemeenschappelijke deelstring, in de 2de test wel.
void test_add_string_without_mutual_skip() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* seven = "seven";
    const char* seventy = "seventy";
    const char* seventeen = "seventeen";

    TEST_CHECK(arraytrie_add(ct, seven));
    TEST_CHECK(arraytrie_add(ct, seventy));
    TEST_CHECK(arraytrie_add(ct, seventeen));

    TEST_SIZE(ct, 3);

    // structuur nakijken
    TEST_CHECK(ct->children_size == 2);
    TEST_CHECK(strcmp(ct->skip, "seven") == 0);
    TEST_CHECK(ct->children[0]->character == '\0' && ct->children[0]->children_size == 0);
    TEST_CHECK(strcmp(ct->children[0]->string, seven) == 0);
    TEST_CHECK(ct->children[1]->character == 't' && ct->children[1]->children_size == 2);
    TEST_CHECK(strcmp(ct->children[1]->skip, "") == 0);
    TEST_CHECK(ct->children[1]->children[0]->character == 'y' && ct->children[1]->children[0]->children_size == 0);
    TEST_CHECK(strcmp(ct->children[1]->children[0]->string, seventy) == 0);
    TEST_CHECK(ct->children[1]->children[1]->character == 'e' && ct->children[1]->children[1]->children_size == 0);
    TEST_CHECK(strcmp(ct->children[1]->children[1]->string, seventeen) == 0);

    arraytrie_free(ct);
}

void test_add_string_with_mutual_skip() {
    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    const char* six = "six";
    const char* seventy = "seventy";
    const char* seventeen = "seventeen";

    TEST_CHECK(arraytrie_add(ct, six));
    TEST_CHECK(arraytrie_add(ct, seventy));
    TEST_CHECK(arraytrie_add(ct, seventeen));

    TEST_SIZE(ct, 3);

    // structuur nakijken
    TEST_CHECK(ct->children_size == 2);
    TEST_CHECK(strcmp(ct->skip, "s") == 0);
    TEST_CHECK(ct->children[0]->character == 'i' && ct->children[0]->children_size == 0);
    TEST_CHECK(strcmp(ct->children[0]->string, six) == 0);
    TEST_CHECK(ct->children[1]->character == 'e' && ct->children[1]->children_size == 2);
    TEST_CHECK(strcmp(ct->children[1]->skip, "vent") == 0);
    TEST_CHECK(ct->children[1]->children[0]->character == 'y' && ct->children[1]->children[0]->children_size == 0);
    TEST_CHECK(strcmp(ct->children[1]->children[0]->string, seventy) == 0);
    TEST_CHECK(ct->children[1]->children[1]->character == 'e' && ct->children[1]->children[1]->children_size == 0);
    TEST_CHECK(strcmp(ct->children[1]->children[1]->string, seventeen) == 0);

    arraytrie_free(ct);
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

    // structuur nakijken
    TEST_CHECK(ct->children_size == 2 && strcmp(ct->skip, "tw") == 0);
    TEST_CHECK(ct->children[0]->character == 'o' && strcmp(ct->children[0]->string, two) == 0);
    TEST_CHECK(ct->children[1]->character == 'e' && strcmp(ct->children[1]->skip, "nty") == 0 && ct->children[1]->children_size == 2);
    TEST_CHECK(ct->children[1]->children[0]->character == '\0' && strcmp(ct->children[1]->children[0]->string, twenty) == 0);
    TEST_CHECK(ct->children[1]->children[1]->character == 't' && strcmp(ct->children[1]->children[1]->string, twentytwo) == 0);

    TEST_CHECK(arraytrie_remove(ct, two));

    // structuur nakijken
    TEST_CHECK(ct->children_size == 2 && strcmp(ct->skip, "twenty") == 0);
    TEST_CHECK(ct->children[0]->character == '\0' && strcmp(ct->children[0]->string, twenty) == 0);
    TEST_CHECK(ct->children[1]->character == 't' && strcmp(ct->children[1]->string, twentytwo) == 0);

    TEST_CHECK(arraytrie_remove(ct, twenty));

    // structuur nakijken
    TEST_CHECK(ct->children_size == 1 && strcmp(ct->skip, "") == 0);
    TEST_CHECK(ct->children[0]->character == 't' && strcmp(ct->children[0]->string, twentytwo) == 0);

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

    arraytrie_free(ct);
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

    // Structuur nakijken
    TEST_CHECK(ct->children_size == 2 && strcmp(ct->skip, "tw") == 0);
    TEST_CHECK(ct->children[0]->character == 'o' && strcmp(ct->children[0]->string, two) == 0);
    TEST_CHECK(ct->children[1]->character == 'e' && ct->children[1]->children_size == 2);
    TEST_CHECK(strcmp(ct->children[1]->skip, "nty") == 0);
    TEST_CHECK(ct->children[1]->children[0]->character == '\0');
    TEST_CHECK(strcmp(ct->children[1]->children[0]->string, twenty) == 0);
    TEST_CHECK(ct->children[1]->children[1]->character == 't');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->string, twentytwo) == 0);

    arraytrie_free(ct);
}

// Het kind van de wortel verwijderen. Hierbij heeft de wortel 2 bladeren als kinderen.
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

    // Structuur nakijken
    TEST_CHECK(ct->children_size == 1 && strcmp(ct->skip, "") == 0);
    TEST_CHECK(ct->children[0]->character == 't' && strcmp(ct->children[0]->string, two) == 0);

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

    // structuur nakijken
    TEST_CHECK(ct->children_size == 3 && strcmp(ct->skip, "") == 0);
    TEST_CHECK(ct->children[0]->character == 'o' && strcmp(ct->children[0]->string, one) == 0);
    TEST_CHECK(ct->children[2]->character == 'f' && strcmp(ct->children[2]->string, four) == 0);
    TEST_CHECK(ct->children[1]->character == 't' && strcmp(ct->children[1]->skip, "w") == 0);
    TEST_CHECK(ct->children[1]->children[0]->character == 'o' && strcmp(ct->children[1]->children[0]->string, two) == 0);
    TEST_CHECK(ct->children[1]->children[1]->character == 'e' && strcmp(ct->children[1]->children[1]->skip, "nty") == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[0]->character == '\0');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[0]->string, twenty) == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[1]->character == 't');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[1]->string, twentytwo) == 0);

    TEST_CHECK(arraytrie_remove(ct, four));

    TEST_SIZE(ct, 4);
    TEST_CHECK(!arraytrie_search(ct, four));
    TEST_CHECK(arraytrie_search(ct, one));
    TEST_CHECK(arraytrie_search(ct, two));
    TEST_CHECK(arraytrie_search(ct, twenty));
    TEST_CHECK(arraytrie_search(ct, twentytwo));

    TEST_CHECK(ct->children_size == 2);

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

    // structuur nakijken
    TEST_CHECK(ct->children_size == 2 && strcmp(ct->skip, "") == 0);
    TEST_CHECK(ct->children[0]->character == 'o' && strcmp(ct->children[0]->string, one) == 0);
    TEST_CHECK(ct->children[1]->character == 't' && strcmp(ct->children[1]->skip, "wenty") == 0);
    TEST_CHECK(ct->children[1]->children[0]->character == '\0' && strcmp(ct->children[1]->children[0]->string, twenty) == 0);
    TEST_CHECK(ct->children[1]->children[1]->character == 't' && strcmp(ct->children[1]->children[1]->string, twentytwo) == 0);

    arraytrie_free(ct);
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

    // structuur nakijken
    TEST_CHECK(ct->children_size == 2 && strcmp(ct->skip, "") == 0);
    TEST_CHECK(ct->children[0]->character == 'o' && strcmp(ct->children[0]->string, one) == 0);
    TEST_CHECK(ct->children[1]->character == 't' && strcmp(ct->children[1]->skip, "w") == 0);
    TEST_CHECK(ct->children[1]->children[0]->character == 'o' && strcmp(ct->children[1]->children[0]->string, two) == 0);
    TEST_CHECK(ct->children[1]->children[1]->character == 'e' && strcmp(ct->children[1]->children[1]->string, twenty) == 0);

    arraytrie_free(ct);
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

    // structuur nakijken
    TEST_CHECK(ct->children_size == 2 && strcmp(ct->skip, "") == 0);
    TEST_CHECK(ct->children[0]->character == 'o' && strcmp(ct->children[0]->string, one) == 0);
    TEST_CHECK(ct->children[1]->character == 't' && strcmp(ct->children[1]->skip, "w") == 0);
    TEST_CHECK(ct->children[1]->children[0]->character == 'o' && strcmp(ct->children[1]->children[0]->string, two) == 0);
    TEST_CHECK(ct->children[1]->children[1]->character == 'e' && strcmp(ct->children[1]->children[1]->skip, "nty") == 0);
    TEST_CHECK(ct->children[1]->children[1]->children_size == 4);
    TEST_CHECK(ct->children[1]->children[1]->children[0]->character == '\0');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[0]->string, twenty) == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[1]->character == 't');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[1]->string, twentytwo) == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[2]->character == 'o');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[2]->string, twentyone) == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[3]->character == 'f');
    TEST_CHECK(ct->children[1]->children[1]->children[3]->children[0]->character == 'o');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[3]->children[0]->string, twentyfour) == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[3]->children[1]->character == 'i');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[3]->children[1]->string, twentyfive) == 0);

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

    // structuur nakijken
    TEST_CHECK(ct->children[1]->children[1]->children_size == 3);
    TEST_CHECK(ct->children[1]->children[1]->children[0]->character == '\0');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[0]->string, twenty) == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[1]->character == 't');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[1]->string, twentytwo) == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[2]->character == 'f');
    TEST_CHECK(ct->children[1]->children[1]->children[2]->children[0]->character == 'o');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[2]->children[0]->string, twentyfour) == 0);
    TEST_CHECK(ct->children[1]->children[1]->children[2]->children[1]->character == 'i');
    TEST_CHECK(strcmp(ct->children[1]->children[1]->children[2]->children[1]->string, twentyfive) == 0);

    arraytrie_free(ct);
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

    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(arraytrie_add(ct, strings[i]));
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
        //printf("string %s", string);
        TEST_CHECK(arraytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(arraytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    arraytrie_free(ct);

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

    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(arraytrie_add(ct, strings[i]));
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
        TEST_CHECK(arraytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(arraytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    arraytrie_free(ct);

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

    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(arraytrie_add(ct, strings[i]));
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
        TEST_CHECK(arraytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(arraytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    arraytrie_free(ct);

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

    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(arraytrie_add(ct, strings[i]));
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
        TEST_CHECK(arraytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(arraytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    arraytrie_free(ct);

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

    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(arraytrie_add(ct, strings[i]));
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
        TEST_CHECK(arraytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(arraytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    arraytrie_free(ct);

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

    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(arraytrie_add(ct, strings[i]));
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
        TEST_CHECK(arraytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(arraytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    arraytrie_free(ct);

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

    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("length: %zu, string %s", strlen(string), string);
        TEST_CHECK(arraytrie_add(ct, strings[i]));
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
        //printf("string %s", string);
        TEST_CHECK(arraytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(arraytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds*1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    arraytrie_free(ct);

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

    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(arraytrie_add(ct, strings[i]));
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
        TEST_CHECK(arraytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(arraytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    arraytrie_free(ct);

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

    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(arraytrie_add(ct, strings[i]));
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
        TEST_CHECK(arraytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(arraytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    arraytrie_free(ct);

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

    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(arraytrie_add(ct, strings[i]));
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
        TEST_CHECK(arraytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(arraytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);

    TEST_SIZE(ct, 0);

    arraytrie_free(ct);

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

    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(arraytrie_add(ct, strings[i]));
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
        TEST_CHECK(arraytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(arraytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);


    TEST_SIZE(ct, 0);

    arraytrie_free(ct);

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

    ArrayTrie* ct = arraytrie_init();
    TEST_CHECK(ct != NULL);

    struct timeval begin, end;
    gettimeofday(&begin,0);
    for (size_t i = 0; i < size; i++) {
        //printf("size %zu, length: %zu, string %s", size, strlen(string), string);
        TEST_CHECK(arraytrie_add(ct, strings[i]));
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
        TEST_CHECK(arraytrie_search(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor zoeken: %f seconds\n", elapsed);

    gettimeofday(&begin, 0);
    for (size_t i = 0; i < size; i++) {
        //printf("size: %llu, string: %s", ct->size, string);
        TEST_CHECK(arraytrie_remove(ct, strings[i]));
    }
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = (double)seconds + (double)microseconds * 1e-6;
    printf("tijd voor verwijderen: %f seconds\n", elapsed);


    TEST_SIZE(ct, 0);

    arraytrie_free(ct);

    for (size_t i = 0; i < size; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}

TEST_LIST = {
        { "arraytrie init",test_init },
        { "arraytrie add one",test_add_one },
        { "arraytrie add more",test_add_more },
        { "arraytrie add string smaller than skip length",test_add_string_smaller_than_skip_length},
        { "arraytrie add string with different skip length", test_add_string_with_different_skip_length},
        { "arraytrie add string without mutual skip",test_add_string_without_mutual_skip },
        { "arraytrie add string with mutual skip",test_add_string_with_mutual_skip },
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

// aangepaste TEST_LIST voor de datasets
/*TEST_LIST = {
        { "arraytrie dataset geschud piepklein",dataset_geschud_piepklein },
        { "arraytrie dataset geschud klein",dataset_geschud_klein },
        { "arraytrie dataset geschud middelmaat",dataset_geschud_middelmaat },
        { "arraytrie dataset geschud groot",dataset_geschud_groot },
        { "arraytrie dataset geschud zeer groot",dataset_geschud_zeer_groot },
        { "arraytrie dataset geschud volledig",dataset_geschud_volledig },
        {"arraytrie dataset piepklein", dataset_piepklein},
        {"arraytrie dataset klein", dataset_klein},
        {"arraytrie dataset middelmaat", dataset_middelmaat},
        {"arraytrie dataset groot", dataset_groot},
        {"arraytrie dataset zeer groot", dataset_zeer_groot},
        {"arraytrie dataset volledig", dataset_volledig},
        { NULL, NULL}
};*/
