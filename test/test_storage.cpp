//
// Created by Andrew Russell on 10/20/16.
//

#define CATCH_CONFIG_MAIN

#include <stdlib.h>

#include "catch.h"
#include "storage.h"

using namespace std;

TEST_CASE("Description goes here!") {
    Storage *s = new Storage();
    REQUIRE(s != NULL);
}
