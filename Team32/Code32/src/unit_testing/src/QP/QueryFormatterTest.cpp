#include "QP/QueryFormatter.h"

#include "catch.hpp"

TEST_CASE("QP::QueryFormatter::formatResult No results") {
	QP::QueryProperties properties = QP::QueryProperties({}, {DesignEntity::Stmt, "s"}, {}, {});
	QP::QueryResult result = QP::QueryResult();
	REQUIRE(QP::QueryFormatter::formatResult(properties, result).empty());
}

TEST_CASE("QP::QueryFormatter::formatQuery List of statement number") {
	QP::QueryProperties properties = QP::QueryProperties({}, {DesignEntity::Stmt, "s"}, {}, {});
	QP::QueryResult result = QP::QueryResult();
	result.addColumn("s", {"1", "2", "3", "3"});
	result.addColumn("sss", {"1", "2", "3", "4"});

	vector<string> result_string = QP::QueryFormatter::formatResult(properties, result);

	sort(result_string.begin(), result_string.end());
	REQUIRE(result_string == vector<string>({"1", "2", "3"}));
}
