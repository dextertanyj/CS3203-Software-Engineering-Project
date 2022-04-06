#include <memory>
#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/StatementInfo.h"
#include "PKB/Types.h"
#include "catch_tools.h"

TEST_CASE("PKB::Types::StatementStore Test") {
	PKB::Types::StatementStore store;
	StmtRef s_0 = 0, s_1 = 1, s_2 = 2, s_3 = 3, s_4 = 4, s_5 = 5, s_6 = 6;
	StmtType t_1 = StmtType::If, t_2 = StmtType::Assign, t_3 = StmtType::Call, t_4 = StmtType::Read, t_5 = StmtType::Print,
			 t_6 = StmtType::While;

	SECTION("Insert") {
		store.insert(s_1, t_1);
		store.insert(s_2, t_2);
		store.insert(s_3, t_3);
		store.insert(s_4, t_4);
		store.insert(s_5, t_5);
		store.insert(s_6, t_6);

		REQUIRE_EQUALS(store.getAll().size(), 6);
		REQUIRE_EQUALS(store.get(s_1)->getType(), t_1);
		REQUIRE_EQUALS(store.get(s_2)->getType(), t_2);
		REQUIRE_EQUALS(store.get(s_3)->getType(), t_3);
		REQUIRE_EQUALS(store.get(s_4)->getType(), t_4);
		REQUIRE_EQUALS(store.get(s_5)->getType(), t_5);
		REQUIRE_EQUALS(store.get(s_6)->getType(), t_6);
	}

	SECTION("Insert Repeated Statement Number") {
		store.insert(s_1, t_1);
		REQUIRE_THROWS_AS(store.insert(s_1, t_2), logic_error);
	}

	SECTION("Insert Invalid Name") { REQUIRE_THROWS_AS(store.insert(s_0, StmtType::Assign), logic_error); }

	SECTION("Get... No Record") {
		REQUIRE_EQUALS(store.get(s_1), nullptr);
		REQUIRE_EQUALS(store.getAll(), unordered_set<shared_ptr<PKB::StatementInfo>>());
	}

	SECTION("Get Invalid Record") { REQUIRE_THROWS_AS(store.get(s_0), logic_error); }

	SECTION("Clear") {
		store.insert(s_1, StmtType::If);
		REQUIRE_EQUALS(store.get(s_1)->getIdentifier(), s_1);
		store.clear();
		REQUIRE_EQUALS(store.get(s_1), nullptr);
	}
}
