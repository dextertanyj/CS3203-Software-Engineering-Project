#include "QP/QueryResult.h"

#include "catch.hpp"

TEST_CASE("QP::QueryResult::updateStmtList Update statement list") {
	QueryResult result = QueryResult({ 1, 2, 3, 4 }, {});
	result.updateStmtList({ 2, 4, 5 });
	StmtRefList expectedList = { 2, 4 };
	REQUIRE(result.getStmtRefList() == expectedList);
}

TEST_CASE("QP::QueryResult::updateVarList Update variable list") {
	QueryResult result = QueryResult({}, { "a", "b", "c"});
	result.updateVarList({ "c", "d", "e" });
	VarRefList expectedList = { "c" };
	REQUIRE(result.getVarRefList() == expectedList);
}
