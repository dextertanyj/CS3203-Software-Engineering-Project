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
	result.addColumn("sss", { "1", "2", "3", "4"});

	vector<string>resultString = QueryFormatter::formatResult(properties, result);
	
	sort(resultString.begin(), resultString.end());
	REQUIRE(resultString == vector<string>({"1", "2", "3"}));
}
