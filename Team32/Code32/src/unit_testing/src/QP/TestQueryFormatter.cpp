#include "QP/QueryFormatter.h"

#include "catch.hpp"

TEST_CASE("QP::QueryFormatter::formatResult No results") {
	QueryProperties properties = QueryProperties({}, { DesignEntity::stmt, "s" }, {}, {});
	QueryResult result = QueryResult();
	REQUIRE(QueryFormatter::formatResult(properties, result) == vector<string>());
}

TEST_CASE("QP::QueryFormatter::formatQuery List of statement number") {
	QueryProperties properties = QueryProperties({}, { DesignEntity::stmt, "s" }, {}, {});
	QueryResult result = QueryResult();
	result.addColumn("s", { "1", "2", "3", "3"});
	REQUIRE(QueryFormatter::formatResult(properties, result) == vector<string>({"1", "2", "3"}));
}
