//
// Created by Andrew Russell on 10/20/16.
//

#define CATCH_CONFIG_MAIN

#include <stdlib.h>

#include "catch.h"
#include "storage.h"

using namespace std;

TEST_CASE("Check initialization") {
    SafeString key(
            (unsigned char *) "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f",
            16);
    Storage *s = new Storage(&key);
    REQUIRE(s != NULL);
}

TEST_CASE("Try save and load operations on an empty safe") {
    SafeString key(
            (unsigned char *) "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f",
            16);
    Storage *s = new Storage(&key);
    SafeString *ss = s->save();

    Storage *s2 = new Storage(&key);
    s2->load(ss);

    free(ss);
}

TEST_CASE("Test create, get, set, delete, and list") {
    vector<entry> entries;

    SafeString key(
            (unsigned char *) "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f",
            16);
    Storage *s = new Storage(&key);

    entries = s->list();
    REQUIRE(entries.size() == 0);

    entry e;
    e.id = 0;
    e.title = new SafeString("EntryTitle");
    e.username = new SafeString("EntryUsername");
    e.password = new SafeString("EntryPassword");
    REQUIRE(s->create(&e));
    REQUIRE(e.id != 0); // Will fail 1 in 2^32 tests

    entries = s->list();
    REQUIRE(entries.size() == 1);

    entry e2;
    e2.id = e.id;
    REQUIRE(s->get(&e2));
    REQUIRE(e.title->get_data() == e2.title->get_data());
    REQUIRE(e.username->get_data() == e2.username->get_data());
    REQUIRE(e.password->get_data() == e2.password->get_data());

    entries = s->list();
    REQUIRE(entries.size() == 1);

    free(e2.title);
    e2.title = new SafeString("UpdatedEntryTitle");
    REQUIRE(s->set(&e2));
    REQUIRE(s->get(&e2));
    REQUIRE(e.id == e2.id);
    REQUIRE(e2.title->get_data().compare("UpdatedEntryTitle") == 0);

    entries = s->list();
    REQUIRE(entries.size() == 1);

    REQUIRE(s->remove(&e2));

    entries = s->list();
    REQUIRE(entries.size() == 0);
}

TEST_CASE("Check entry serialization and deserialization") {
    entry e = {};
    e.id = 63;
    e.title = new SafeString("Title");
    e.username = new SafeString("Username");
    e.password = new SafeString("Password");
    string serialized = e.serialize();

    entry e2 = {};
    e2.deserialize(serialized);

    REQUIRE(e.id == e2.id);
    REQUIRE(e.title->get_data() == e2.title->get_data());
    REQUIRE(e.username->get_data() == e2.username->get_data());
    REQUIRE(e.password->get_data() == e2.password->get_data());
}

TEST_CASE("Try save and load operations on a non-empty safe") {
    SafeString key(
            (unsigned char *) "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f",
            16);
    Storage *s = new Storage(&key);

    entry e;
    e.id = 0;
    e.title = new SafeString("ExampleTitle");
    e.username = new SafeString("ExampleUsername");
    e.password = new SafeString("ExamplePassword");
    REQUIRE(s->create(&e));
    REQUIRE(e.id != 0);

    SafeString *ss = s->save();

    Storage *s2 = new Storage(&key);
    s2->load(ss);

    entry e2;
    e2.id = e.id;
    REQUIRE(s2->get(&e2));

    REQUIRE(e.id == e2.id);
    REQUIRE(e.title->get_data() == e2.title->get_data());
    REQUIRE(e.username->get_data() == e2.username->get_data());
    REQUIRE(e.password->get_data() == e2.password->get_data());

    free(ss);
}
