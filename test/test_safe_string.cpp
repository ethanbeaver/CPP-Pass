//
// Created by Andrew Russell on 10/20/16.
//

#define CATCH_CONFIG_MAIN

#include <stdlib.h>

#include "catch.h"
#include "safe_string.h"

using namespace std;

TEST_CASE("Check initialization") {
    char *buf_in = "example";
    SafeString *ss = new SafeString((unsigned char*) buf_in, 8);
    REQUIRE(ss != NULL);
}

TEST_CASE("Ensure 'get' returns the correct value") {
    char *buf_in = "example";
    SafeString *ss = new SafeString((unsigned char*) buf_in, 8);
    char *buf = (char*) malloc(ss->get_length());
    ss->get_data((unsigned char *)buf);
}
