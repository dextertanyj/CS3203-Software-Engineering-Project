#include "QP/QueryFormatter.h"

#include "catch.hpp"

TEST_CASE("QP::QueryFormatter::formatResult No results") {
	QueryProperties properties = QueryProperties({}, { DesignEntity::stmt, "s" }, {}, {});
	QueryResult result = QueryResult();
	REQUIRE(QueryFormatter::formatResult(properties, result) == "none");
}

TEST_CASE("QP::QueryFormatter::formatQuery List of statement number") {
	QueryProperties properties = QueryProperties({}, { DesignEntity::stmt, "s" }, {}, {});
	QueryResult result = QueryResult({ 1, 2, 3 }, {});
	REQUIRE(QueryFormatter::formatResult(properties, result) == "1, 2, 3");
}
