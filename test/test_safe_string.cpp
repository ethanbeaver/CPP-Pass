//
// Created by Andrew Russell on 10/20/16.
//

#define CATCH_CONFIG_MAIN

#include <stdlib.h>

#include "catch.h"
#include "safe_string.h"

using namespace std;

TEST_CASE("Check initialization") {
    char buf_in[] = "example";
    SafeString *ss = new SafeString((unsigned char *) buf_in, 7);
    REQUIRE(ss != NULL);
}

TEST_CASE("Verify reported length upper bound") {
    char buf_in[] = "example";
    SafeString *ss = new SafeString((unsigned char *) buf_in, 7);

    unsigned len = ss->get_max_length();
    REQUIRE(len >= 7);
}

TEST_CASE("Char array storage and retrieval") {
    char buf_in[] = "example";
    SafeString *ss = new SafeString((unsigned char *) buf_in, 7);

    unsigned len_max = ss->get_max_length();
    char *buf = (char *) malloc(len_max);
    unsigned len_actual;
    ss->get_data((unsigned char *) buf, &len_actual);

    REQUIRE(len_actual == 7);
    REQUIRE(memcmp(&buf_in[0], buf, 7) == 0);

    free(buf);
}

TEST_CASE("String storage and retrieval") {
    string str_orig("example");
    SafeString *ss = new SafeString(str_orig);

    string str_new = ss->get_data();
    REQUIRE(str_orig.compare(str_new) == 0);
}

TEST_CASE("Store char array, retrieve string") {
    char buf_in[] = "example";
    SafeString *ss = new SafeString((unsigned char *) buf_in,
                                    (unsigned) strlen(buf_in));

    string buf_out = ss->get_data();

    REQUIRE(buf_out.length() == strlen(buf_in));
    REQUIRE(memcmp(&buf_in[0], buf_out.data(), strlen(buf_in)) == 0);
}

TEST_CASE("Store string, retrieve char array") {
    string str_orig("example");
    SafeString *ss = new SafeString(str_orig);

    char *buf_out = (char *) malloc(ss->get_max_length());
    unsigned buf_len;
    ss->get_data((unsigned char *) buf_out, &buf_len);
    string str_new(buf_out, buf_len);

    REQUIRE(str_orig.compare(str_new) == 0);
}
