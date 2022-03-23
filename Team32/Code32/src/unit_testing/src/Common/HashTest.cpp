#include "Common/Hash.h"

#include <unordered_map>
#include <utility>

#include "catch_tools.h"

TEST_CASE("Pair Hash Test") {
	SECTION("Value") {
		REQUIRE_EQUALS(Common::Hash::PairHash{}(std::pair<int, int>({1, 2})), Common::Hash::PairHash{}(std::pair<int, int>({1, 2})));
		REQUIRE(Common::Hash::PairHash{}(std::pair<int, int>({1, 2})) != Common::Hash::PairHash{}(std::pair<int, int>({1, 3})));
		REQUIRE(Common::Hash::PairHash{}(std::pair<int, int>({1, 2})) != Common::Hash::PairHash{}(std::pair<int, int>({2, 1})));
	}

	SECTION("Map") {
		std::unordered_map<std::pair<std::string, std::string>, int, Common::Hash::PairHash> map;
		map.insert({{"A", "B"}, 1});
		REQUIRE_EQUALS(map.at({"A", "B"}), 1);
		(map.insert({{"A", "B"}, 2}));
		REQUIRE_EQUALS(map.at({"A", "B"}), 1);
	}
}

TEST_CASE("Vector Hash Test") {
	SECTION("Value") {
		REQUIRE_EQUALS(Common::Hash::VectorHash{}(std::vector<int>({1, 2, 3})), Common::Hash::VectorHash{}(std::vector<int>({1, 2, 3})));
		REQUIRE(Common::Hash::VectorHash{}(std::vector<int>({1, 2, 3})) != Common::Hash::VectorHash{}(std::vector<int>({4, 2, 3})));
		REQUIRE(Common::Hash::VectorHash{}(std::vector<int>({1, 2, 3})) != Common::Hash::VectorHash{}(std::vector<int>({2, 1, 3})));
	}

	SECTION("Map") {
		std::unordered_map<std::vector<std::string>, int, Common::Hash::VectorHash> map;
		map.insert({{"A", "B"}, 1});
		map.insert({{"A", "B", "C"}, 2});
		REQUIRE_EQUALS(map.at({"A", "B"}), 1);
		REQUIRE_EQUALS(map.at({"A", "B", "C"}), 2);
	}
}