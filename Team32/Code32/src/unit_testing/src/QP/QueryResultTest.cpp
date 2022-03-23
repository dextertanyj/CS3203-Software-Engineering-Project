#include "QP/QueryResult.h"

#include "catch.hpp"

TEST_CASE("QP::QueryResult::joinResult same synonym, should set result to false") {
	QP::QueryResult result_one = QP::QueryResult();
	QP::QueryResult result_two = QP::QueryResult();
	result_one.addColumn("a", {"5", "6"});
	result_two.addColumn("a", {"1", "2", "2", "3"});
	result_two.addColumn("b", {"5", "5", "6", "7"});

	result_one.joinResult(result_two);

	REQUIRE(!result_one.getResult());
}

TEST_CASE("QP::QueryResult::joinResult same synonym, should set result to true") {
	QP::QueryResult result_one = QP::QueryResult();
	QP::QueryResult result_two = QP::QueryResult();
	result_one.addColumn("a", {"1", "2"});
	result_two.addColumn("a", {"1", "2", "2", "3"});
	result_two.addColumn("b", {"5", "5", "6", "7"});

	result_one.joinResult(result_two);

	REQUIRE(result_one.getResult());
	REQUIRE(result_one.getNumberOfRows() == 3);
}

TEST_CASE("QP::QueryResult::joinResult Exact same synonyms, should set result to false") {
	QP::QueryResult result_one = QP::QueryResult();
	QP::QueryResult result_two = QP::QueryResult();
	result_one.addColumn("a", {"3", "1"});
	result_one.addColumn("b", {"8", "1"});
	result_two.addColumn("a", {"1", "2", "2", "3"});
	result_two.addColumn("b", {"5", "5", "6", "7"});

	result_one.joinResult(result_two);

	REQUIRE(!result_one.getResult());
}

TEST_CASE("QP::QueryResult::joinResult Exact same synonyms, should set result to true") {
	QP::QueryResult result_one = QP::QueryResult();
	QP::QueryResult result_two = QP::QueryResult();
	result_one.addColumn("a", {"3", "1"});
	result_one.addColumn("b", {"7", "1"});
	result_two.addColumn("a", {"1", "2", "2", "3"});
	result_two.addColumn("b", {"5", "5", "6", "7"});

	result_one.joinResult(result_two);

	REQUIRE(result_one.getResult());
	REQUIRE(result_one.getNumberOfRows() == 1);
}

TEST_CASE("QP::QueryResult::joinResult Different synonyms, should set result to false") {
	QP::QueryResult result_one = QP::QueryResult();
	QP::QueryResult result_two = QP::QueryResult();
	result_one.addColumn("a", {"3", "1", "2", "4"});
	result_one.addColumn("b", {"7", "8", "9", "4"});
	result_two.addColumn("b", {"1", "2", "2", "3", "3", "1"});
	result_two.addColumn("c", {"5", "5", "6", "7", "4", "9"});

	result_one.joinResult(result_two);

	REQUIRE(!result_one.getResult());
}

TEST_CASE("QP::QueryResult::filterBySelect Should filter result") {
	QP::Types::DeclarationList select_list = {
		{QP::Types::DesignEntity::Stmt, "a"},
		{QP::Types::DesignEntity::Stmt, "c"},
	};
	QP::QueryResult result = QP::QueryResult();
	result.addColumn("a", {"1", "3", "5", "1", "7"});
	result.addColumn("b", {"a", "b", "c", "d", "e"});
	result.addColumn("c", {"2", "4", "6", "2", "8"});

	result.filterBySelect(select_list);

	REQUIRE(result.getNumberOfRows() == 4);
}
