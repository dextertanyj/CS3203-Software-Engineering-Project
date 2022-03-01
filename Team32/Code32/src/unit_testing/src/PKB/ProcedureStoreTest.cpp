#include "PKB/ProcedureStore.h"

#include <algorithm>
#include <stdexcept>

#include "MockUtilities.h"
#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::ProcedureStore Test") {
	PKB::ProcedureStore store;
	ProcRef name_1 = "procedure";
	ProcRef name_2 = "main";
	vector<shared_ptr<StmtInfo>> single_statement = {MockUtilities::createStmtInfo(1, StmtType::Read)};
	vector<shared_ptr<StmtInfo>> statements = {MockUtilities::createStmtInfo(2, StmtType::Read),
	                                           MockUtilities::createStmtInfo(3, StmtType::Print),
	                                           MockUtilities::createStmtInfo(4, StmtType::Call)};

	SECTION("Insert Single Statement") {
		store.insert(name_1, single_statement);

		REQUIRE_EQUALS(store.getAll(), unordered_set<ProcRef>({name_1}));
		REQUIRE_EQUALS(store.get(name_1)->getIdentifier(), name_1);
		REQUIRE_EQUALS(store.get(name_1)->getStatements(), single_statement);
	}

	SECTION("Insert Multiple Statements") {
		store.insert(name_1, statements);

		REQUIRE_EQUALS(store.getAll(), unordered_set<ProcRef>({name_1}));
		REQUIRE_EQUALS(store.get(name_1)->getIdentifier(), name_1);
		REQUIRE_EQUALS(store.get(name_1)->getStatements(), statements);
	}

	SECTION("Insert Multiple Procedures") {
		store.insert(name_1, single_statement);
		store.insert(name_2, statements);

		unordered_set<ProcRef> expected_names = {name_1, name_2};
		vector<vector<shared_ptr<StmtInfo>>> expected_statements = {single_statement, statements};

		unordered_set<shared_ptr<PKB::ProcedureInfo>> infos = store.getAllInfo();

		REQUIRE_EQUALS(store.getAll(), unordered_set<ProcRef>({name_1, name_2}));
		REQUIRE_EQUALS(infos.size(), 2);
		for (auto info : infos) {
			REQUIRE(any_of(expected_names.begin(), expected_names.end(),
			               [info](const ProcRef& name) { return info->getIdentifier() == name; }));
			REQUIRE(any_of(expected_statements.begin(), expected_statements.end(),
			               [info](const vector<shared_ptr<StmtInfo>>& statements) { return info->getStatements() == statements; }));
		}
	}

	SECTION("Insert Repeated Procedure Name") {
		store.insert(name_1, single_statement);
		REQUIRE_THROWS_AS(store.insert(name_1, statements), logic_error);
	}

	SECTION("Insert Invalid Name") { REQUIRE_THROWS_AS(store.insert("", single_statement), invalid_argument); }

	SECTION("Get... No Record") {
		REQUIRE_EQUALS(store.get(name_1), nullptr);
		REQUIRE_EQUALS(store.getAll(), unordered_set<ProcRef>());
		REQUIRE_EQUALS(store.getAllInfo(), unordered_set<shared_ptr<PKB::ProcedureInfo>>());
	}

	SECTION("Clear") {
		store.insert(name_1, single_statement);
		REQUIRE_EQUALS(store.get(name_1)->getIdentifier(), name_1);
		store.clear();
		REQUIRE_EQUALS(store.get(name_1), nullptr);
	}
}
