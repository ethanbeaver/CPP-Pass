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
    SafeString *ss = new SafeString((unsigned char *) buf_in, 8);
    REQUIRE(ss != NULL);
}

TEST_CASE("Verify reported length upper bound") {
    char buf_in[] = "example";
    SafeString *ss = new SafeString((unsigned char *) buf_in, 8);

    unsigned len = ss->get_max_length();
    REQUIRE(len >= 8);
}

TEST_CASE("Ensure 'get' returns correct data and length") {
    char buf_in[] = "example";
    SafeString *ss = new SafeString((unsigned char *) buf_in, 8);

    unsigned len_max = ss->get_max_length();
    char *buf = (char *) malloc(len_max);
    unsigned len_actual;
    ss->get_data((unsigned char *) buf, &len_actual);

    REQUIRE(memcmp(&buf_in[0], buf, 8) == 0);
    REQUIRE(len_actual == 8);

    free(buf);
}
