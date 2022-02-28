#include "PKB/ProcedureStore.h"

#include <stdexcept>

#include "MockUtilities.h"
#include "catch.hpp"
#include "catch_tools.h"

TEST_CASE("PKB::ProcedureStore Test") {
	PKB::ProcedureStore store;
	ProcRef name_1 = "procedure";
	ProcRef name_2 = "main";
	vector<shared_ptr<StmtInfo>> single_statement = {MockUtilities::createStmtInfo(1, StmtType::Read)};
	vector<shared_ptr<StmtInfo>> statements = {
		MockUtilities::createStmtInfo(2, StmtType::Read),
		MockUtilities::createStmtInfo(3, StmtType::Print),
		MockUtilities::createStmtInfo(4, StmtType::Call)
	};

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

		REQUIRE_EQUALS(store.getAll(), unordered_set<ProcRef>({name_1, name_2}));
		unordered_set<shared_ptr<PKB::ProcedureInfo>> infos = store.getAllInfo();
		REQUIRE_EQUALS(infos.size(), 2);
	}

	SECTION("Insert invalid name") {
		REQUIRE_THROWS_AS(store.insert("", single_statement), invalid_argument);
	}

	SECTION("Get No Record") {
		REQUIRE_EQUALS(store.get(name_1), nullptr);
	}
}
