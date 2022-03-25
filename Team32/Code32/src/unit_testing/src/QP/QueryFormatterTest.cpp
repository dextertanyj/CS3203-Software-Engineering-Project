#include "QP/QueryFormatter.h"

#include "PKB/Storage.h"
#include "QP/ReferenceArgument.h"
#include "catch.hpp"

TEST_CASE("QP::QueryFormatter::formatResult No results") {
	PKB::Storage pkb;
	QP::StorageAdapter store(pkb);
	QP::QueryFormatter post_processor(store);

	QP::QueryProperties properties = QP::QueryProperties({}, {QP::Types::ReferenceArgument{{QP::Types::DesignEntity::Stmt, "s"}}}, {});
	QP::QueryResult result = QP::QueryResult();
	REQUIRE(post_processor.formatResult(properties, result).empty());
}

TEST_CASE("QP::QueryFormatter::formatResult List of statement number") {
	PKB::Storage pkb;
	QP::StorageAdapter store(pkb);
	QP::QueryFormatter post_processor(store);

	QP::QueryProperties properties = QP::QueryProperties({}, {QP::Types::ReferenceArgument{{QP::Types::DesignEntity::Stmt, "s"}}}, {});
	QP::QueryResult result = QP::QueryResult();
	result.addRow({"1"});
	result.addRow({"2"});
	result.addRow({"3"});

	vector<string> result_string = post_processor.formatResult(properties, result);

	sort(result_string.begin(), result_string.end());
	REQUIRE(result_string == vector<string>({"1", "2", "3"}));
}

TEST_CASE("QP::QueryFormatter::formatResult Boolean") {
	PKB::Storage pkb;
	QP::StorageAdapter store(pkb);
	QP::QueryFormatter post_processor(store);

	QP::QueryProperties properties = QP::QueryProperties({}, {}, {});
	QP::QueryResult result = QP::QueryResult(true);

	vector<string> result_string = post_processor.formatResult(properties, result);

	sort(result_string.begin(), result_string.end());
	REQUIRE(result_string == vector<string>({"TRUE"}));
}

TEST_CASE("QP::QueryFormatter::formatResult Tuple") {
	PKB::Storage pkb;
	QP::StorageAdapter store(pkb);
	QP::QueryFormatter post_processor(store);

	QP::Types::SelectList select_list = {
		QP::Types::ReferenceArgument{{QP::Types::DesignEntity::Stmt, "s"}},
		QP::Types::ReferenceArgument{{QP::Types::DesignEntity::Stmt, "s1"}},
	};
	QP::QueryProperties properties = QP::QueryProperties({}, select_list, {});
	QP::QueryResult result = QP::QueryResult(vector<string>{"s", "s1"});
	result.addRow({"1", "5"});
	result.addRow({"2", "6"});
	result.addRow({"3", "7"});

	vector<string> result_string = post_processor.formatResult(properties, result);

	sort(result_string.begin(), result_string.end());
	REQUIRE(result_string == vector<string>({"1 5", "2 6", "3 7"}));
}

TEST_CASE("QP::QueryFormatter::formatResult Tuple repeated") {
	PKB::Storage pkb;
	QP::StorageAdapter store(pkb);
	QP::QueryFormatter post_processor(store);

	QP::Types::SelectList select_list = {
		QP::Types::ReferenceArgument{{QP::Types::DesignEntity::Stmt, "s"}},
		QP::Types::ReferenceArgument{{QP::Types::DesignEntity::Stmt, "s"}},
	};
	QP::QueryProperties properties = QP::QueryProperties({}, select_list, {});
	QP::QueryResult result = QP::QueryResult(vector<string>{"s", "s1"});
	result.addRow({"1", "5"});
	result.addRow({"2", "6"});
	result.addRow({"3", "7"});

	vector<string> result_string = post_processor.formatResult(properties, result);

	sort(result_string.begin(), result_string.end());
	REQUIRE(result_string == vector<string>({"1 1", "2 2", "3 3"}));
}
