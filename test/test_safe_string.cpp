//
// Created by Andrew Russell on 10/20/16.
//

#define CATCH_CONFIG_MAIN

#include <stdlib.h>

#include "catch.h"
#include "safe_string.h"

using namespace std;

TEST_CASE("Check initialization") {
    string str("example");
    SafeString *ss = new SafeString(&str);
    REQUIRE(ss != NULL);
}

TEST_CASE("Ensure 'get' returns the correct value") {
    string str("example");
    SafeString *ss = new SafeString(&str);
    string str_to_check;
    ss->get(&str_to_check);
    REQUIRE(str_to_check.compare(str) == 0);
}
