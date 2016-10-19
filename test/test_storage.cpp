#define CATCH_CONFIG_MAIN

#include "catch.h"
#include "storage.h"

#include <stdlib.h>

using namespace std;

TEST_CASE("Description goes here!") {
    Storage *s = new Storage();
    REQUIRE(s != NULL);
}
