#include "PKB/TransitiveRelationStore.tpp"

#include "MockUtilities.h"
#include "catch_tools.h"

TEST_CASE("PKB::TransitiveRelationStore Test") {
	PKB::TransitiveRelationStore<string, MockInfo, MockRelation> store;

	SECTION("Bidirectional Test") {
		shared_ptr<MockInfo> info_1 = make_shared<MockInfo>("i1");
		shared_ptr<MockInfo> info_2 = make_shared<MockInfo>("i2");
		store.set(info_1, info_2);

		REQUIRE(store.isRelated("i1", "i2"));
		REQUIRE_EQUALS(store.getForward("i2"), unordered_set<shared_ptr<MockInfo>>({info_1}));
		REQUIRE_EQUALS(store.getReverse("i1"), unordered_set<shared_ptr<MockInfo>>({info_2}));
		REQUIRE(!store.isRelated("i2", "i1"));
	}

	SECTION("Existing Forward Test") {
		shared_ptr<MockInfo> info_1 = make_shared<MockInfo>("i1");
		shared_ptr<MockInfo> info_2 = make_shared<MockInfo>("i2");
		shared_ptr<MockInfo> info_3 = make_shared<MockInfo>("i3");
		store.set(info_1, info_2);
		REQUIRE_NOTHROW(store.set(info_1, info_3));

		REQUIRE(store.isRelated("i1", "i3"));
		REQUIRE(!store.isRelated("i3", "i1"));
		REQUIRE_EQUALS(store.getReverse("i1"),
		               unordered_set<shared_ptr<MockInfo>>({info_2, info_3}));  // Old object should not be overwritten.
	}

	SECTION("Existing Reverse Test") {
		shared_ptr<MockInfo> info_1 = make_shared<MockInfo>("i1");
		shared_ptr<MockInfo> info_2 = make_shared<MockInfo>("i2");
		shared_ptr<MockInfo> info_3 = make_shared<MockInfo>("i3");
		store.set(info_2, info_3);
		REQUIRE_NOTHROW(store.set(info_1, info_3));

		REQUIRE(store.isRelated("i1", "i3"));
		REQUIRE(!store.isRelated("i3", "i1"));
		REQUIRE_EQUALS(store.getForward("i3"),
		               unordered_set<shared_ptr<MockInfo>>({info_1, info_2}));  // Old object should not be overwritten.
	}
}