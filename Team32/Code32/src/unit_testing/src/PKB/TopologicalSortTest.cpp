#include "PKB/TopologicalSort.tpp"

#include "MockUtilities.h"
#include "PKB/TransitiveRelationStore.h"
#include "catch_tools.h"

TEST_CASE("PKB::TopologicalSort::sort Test") {
	MockInfoStore info_store;
	PKB::TransitiveRelationStore<string, MockInfo, MockRelation> store;
	PKB::TopologicalSort<MockInfo> topological_sort;

	SECTION("Empty store") {
		topological_sort.sort(info_store, store);
		REQUIRE_EQUALS(topological_sort.get(), vector<shared_ptr<MockInfo>>());
	}

	SECTION("Empty graph") {
		shared_ptr<MockInfo> info = make_shared<MockInfo>("Ident");
		info_store.insert(info);
		topological_sort.sort<MockInfoStore, string, MockRelation>(info_store, store);
		REQUIRE_EQUALS(topological_sort.get(), vector<shared_ptr<MockInfo>>({info}));
	}

	SECTION("Disconnected graph") {
		shared_ptr<MockInfo> info_1 = make_shared<MockInfo>("i1");
		shared_ptr<MockInfo> info_2 = make_shared<MockInfo>("i2");
		shared_ptr<MockInfo> info_3 = make_shared<MockInfo>("i3");
		shared_ptr<MockInfo> info_4 = make_shared<MockInfo>("i4");
		info_store.insert(info_1);
		info_store.insert(info_2);
		info_store.insert(info_3);
		info_store.insert(info_4);
		store.set(info_1, info_2);
		store.set(info_3, info_4);
		topological_sort.sort<MockInfoStore, string, MockRelation>(info_store, store);
		vector<shared_ptr<MockInfo>> order = topological_sort.get();
		REQUIRE_EQUALS(order.size(), 4);
		REQUIRE(find(order.begin(), order.end(), info_1) < find(order.begin(), order.end(), info_2));
		REQUIRE(find(order.begin(), order.end(), info_3) < find(order.begin(), order.end(), info_4));
	}

	SECTION("Cyclic graph") {
		shared_ptr<MockInfo> info_1 = make_shared<MockInfo>("i1");
		shared_ptr<MockInfo> info_2 = make_shared<MockInfo>("i2");
		shared_ptr<MockInfo> info_3 = make_shared<MockInfo>("i3");
		shared_ptr<MockInfo> info_4 = make_shared<MockInfo>("i4");
		info_store.insert(info_1);
		info_store.insert(info_2);
		info_store.insert(info_3);
		info_store.insert(info_4);
		store.set(info_1, info_2);
		store.set(info_2, info_3);
		store.set(info_3, info_4);
		store.set(info_4, info_2);
		REQUIRE_THROWS_AS(topological_sort.sort(info_store, store), logic_error);
	}

	SECTION("Pure Cyclic graph") {
		shared_ptr<MockInfo> info_1 = make_shared<MockInfo>("i1");
		shared_ptr<MockInfo> info_2 = make_shared<MockInfo>("i2");
		shared_ptr<MockInfo> info_3 = make_shared<MockInfo>("i3");
		shared_ptr<MockInfo> info_4 = make_shared<MockInfo>("i4");
		info_store.insert(info_1);
		info_store.insert(info_2);
		info_store.insert(info_3);
		info_store.insert(info_4);
		store.set(info_1, info_2);
		store.set(info_2, info_3);
		store.set(info_3, info_4);
		store.set(info_4, info_1);
		REQUIRE_THROWS_AS(topological_sort.sort(info_store, store), logic_error);
	}

	SECTION("Diamond graph") {
		shared_ptr<MockInfo> info_1 = make_shared<MockInfo>("i1");
		shared_ptr<MockInfo> info_2 = make_shared<MockInfo>("i2");
		shared_ptr<MockInfo> info_3 = make_shared<MockInfo>("i3");
		shared_ptr<MockInfo> info_4 = make_shared<MockInfo>("i4");
		shared_ptr<MockInfo> info_5 = make_shared<MockInfo>("i5");
		shared_ptr<MockInfo> info_6 = make_shared<MockInfo>("i6");
		info_store.insert(info_1);
		info_store.insert(info_2);
		info_store.insert(info_3);
		info_store.insert(info_4);
		info_store.insert(info_5);
		info_store.insert(info_6);
		store.set(info_1, info_2);
		store.set(info_2, info_3);
		store.set(info_2, info_4);
		store.set(info_3, info_5);
		store.set(info_4, info_5);
		store.set(info_5, info_6);
		topological_sort.sort(info_store, store);
		vector<shared_ptr<MockInfo>> order = topological_sort.get();
		REQUIRE_EQUALS(order.size(), 6);
		REQUIRE(find(order.begin(), order.end(), info_1) < find(order.begin(), order.end(), info_2));
		REQUIRE(find(order.begin(), order.end(), info_1) < find(order.begin(), order.end(), info_3));
		REQUIRE(find(order.begin(), order.end(), info_1) < find(order.begin(), order.end(), info_4));
		REQUIRE(find(order.begin(), order.end(), info_1) < find(order.begin(), order.end(), info_5));

		REQUIRE(find(order.begin(), order.end(), info_2) < find(order.begin(), order.end(), info_3));
		REQUIRE(find(order.begin(), order.end(), info_2) < find(order.begin(), order.end(), info_4));
		REQUIRE(find(order.begin(), order.end(), info_2) < find(order.begin(), order.end(), info_5));
		REQUIRE(find(order.begin(), order.end(), info_2) < find(order.begin(), order.end(), info_6));

		REQUIRE(find(order.begin(), order.end(), info_3) < find(order.begin(), order.end(), info_5));
		REQUIRE(find(order.begin(), order.end(), info_3) < find(order.begin(), order.end(), info_6));

		REQUIRE(find(order.begin(), order.end(), info_4) < find(order.begin(), order.end(), info_5));
		REQUIRE(find(order.begin(), order.end(), info_4) < find(order.begin(), order.end(), info_6));

		REQUIRE(find(order.begin(), order.end(), info_5) < find(order.begin(), order.end(), info_6));
	}
}