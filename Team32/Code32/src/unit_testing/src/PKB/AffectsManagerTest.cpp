#include "PKB/AffectsManager.h"

#include "PKB/CFG/ControlFlowGraph.h"
#include "PKB/NextManager.h"
#include "PKB/SVRelationStore.tpp"
#include "TestUtilities.h"
#include "catch.hpp"

TEST_CASE("PKB::AffectsManager::getAffects Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	PKB::SVRelationStore<PKB::ModifiesSRelation> modifies_store;
	PKB::SVRelationStore<PKB::UsesSRelation> uses_store;
	PKB::AffectsManager affects_manager = PKB::AffectsManager(cfg, modifies_store, uses_store);

	SECTION("PKB::AffectsManager::getAffects Simple Consecutive Assign Statement Test") {
		/* SIMPLE Code:
		 * 1. x = 5;
		 * 2. y = x;
		 * 3. z = x;
		 * 4. x = y + z;
		 * 5. z = x;
		 */
		shared_ptr<StmtInfo> assign_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::Assign);
		shared_ptr<StmtInfo> assign_stmt_2 = TestUtilities::createStmtInfo(2, StmtType::Assign);
		shared_ptr<StmtInfo> assign_stmt_3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
		shared_ptr<StmtInfo> assign_stmt_4 = TestUtilities::createStmtInfo(4, StmtType::Assign);
		shared_ptr<StmtInfo> assign_stmt_5 = TestUtilities::createStmtInfo(5, StmtType::Assign);

		CHECK_NOTHROW(cfg.createNode(assign_stmt_1));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_2));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_3));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_4));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_5));

		CHECK_NOTHROW(modifies_store.set(assign_stmt_1, "x"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_2, "y"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_3, "z"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_4, "x"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_5, "z"));

		CHECK_NOTHROW(uses_store.set(assign_stmt_2, "x"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_3, "x"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_4, "y"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_4, "z"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_5, "x"));

		CHECK_NOTHROW(next_manager.setNext(1, 2));
		CHECK_NOTHROW(next_manager.setNext(2, 3));
		CHECK_NOTHROW(next_manager.setNext(3, 4));
		CHECK_NOTHROW(next_manager.setNext(4, 5));

		REQUIRE(affects_manager.getAffects(1) == StmtInfoPtrSet{assign_stmt_2, assign_stmt_3});
		REQUIRE(affects_manager.getAffects(2) == StmtInfoPtrSet{assign_stmt_4});
		REQUIRE(affects_manager.getAffects(3) == StmtInfoPtrSet{assign_stmt_4});
		REQUIRE(affects_manager.getAffects(4) == StmtInfoPtrSet{assign_stmt_5});
		REQUIRE(affects_manager.getAffects(4).empty());
	}

    SECTION("PKB::AffectsManager::getAffects If Branching Test") {
        /* SIMPLE Code:
         * 1. x = 5;
         * 2. if (a > 0) {
         * 3.   b = 2;
         * 4.   c = x * 2;
         *    } else {
         * 5.   x = 2;
         * 6.   b = x - 1;
         * 7.   x = y + z;
         *    }
         * 8. z = x;
         */
        shared_ptr<StmtInfo> assign_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::Assign);
        shared_ptr<StmtInfo> if_stmt_2 = TestUtilities::createStmtInfo(2, StmtType::IfStmt);
        shared_ptr<StmtInfo> assign_stmt_3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
        shared_ptr<StmtInfo> assign_stmt_4 = TestUtilities::createStmtInfo(4, StmtType::Assign);
        shared_ptr<StmtInfo> assign_stmt_5 = TestUtilities::createStmtInfo(5, StmtType::Assign);
        shared_ptr<StmtInfo> assign_stmt_6 = TestUtilities::createStmtInfo(6, StmtType::Assign);
        shared_ptr<StmtInfo> assign_stmt_7 = TestUtilities::createStmtInfo(7, StmtType::Assign);
        shared_ptr<StmtInfo> assign_stmt_8 = TestUtilities::createStmtInfo(8, StmtType::Assign);
        shared_ptr<StmtInfo> assign_stmt_9 = TestUtilities::createStmtInfo(9, StmtType::Assign);

        CHECK_NOTHROW(cfg.createNode(assign_stmt_1));
        CHECK_NOTHROW(cfg.createNode(if_stmt_2));
        CHECK_NOTHROW(cfg.createNode(assign_stmt_3));
        CHECK_NOTHROW(cfg.createNode(assign_stmt_4));
        CHECK_NOTHROW(cfg.createNode(assign_stmt_5));
        CHECK_NOTHROW(cfg.createNode(assign_stmt_6));
        CHECK_NOTHROW(cfg.createNode(assign_stmt_7));
        CHECK_NOTHROW(cfg.createNode(assign_stmt_8));
        CHECK_NOTHROW(cfg.createNode(assign_stmt_9));

        CHECK_NOTHROW(modifies_store.set(assign_stmt_1, "x"));
        CHECK_NOTHROW(modifies_store.set(assign_stmt_3, "b"));
        CHECK_NOTHROW(modifies_store.set(assign_stmt_4, "c"));
        CHECK_NOTHROW(modifies_store.set(assign_stmt_5, "x"));
        CHECK_NOTHROW(modifies_store.set(assign_stmt_6, "b"));
        CHECK_NOTHROW(modifies_store.set(assign_stmt_7, "x"));
        CHECK_NOTHROW(modifies_store.set(assign_stmt_8, "z"));
        CHECK_NOTHROW(modifies_store.set(assign_stmt_9, "a"));

        CHECK_NOTHROW(uses_store.set(if_stmt_2, "a"));
        CHECK_NOTHROW(uses_store.set(assign_stmt_4, "x"));
        CHECK_NOTHROW(uses_store.set(assign_stmt_6, "x"));
        CHECK_NOTHROW(uses_store.set(assign_stmt_7, "y"));
        CHECK_NOTHROW(uses_store.set(assign_stmt_7, "z"));
        CHECK_NOTHROW(uses_store.set(assign_stmt_8, "x"));
        CHECK_NOTHROW(uses_store.set(assign_stmt_9, "b"));

        CHECK_NOTHROW(next_manager.setIfNext(2, 3, 5));
        CHECK_NOTHROW(next_manager.setIfExit(4, 7, 2));

        CHECK_NOTHROW(next_manager.setNext(1, 2));
        CHECK_NOTHROW(next_manager.setNext(2, 8));
        CHECK_NOTHROW(next_manager.setNext(3, 4));
        CHECK_NOTHROW(next_manager.setNext(5, 6));
        CHECK_NOTHROW(next_manager.setNext(6, 7));
        CHECK_NOTHROW(next_manager.setNext(8, 9));

        REQUIRE(affects_manager.getAffects(1) == StmtInfoPtrSet{assign_stmt_4, assign_stmt_8});
        REQUIRE_THROWS(affects_manager.getAffects(2));
        REQUIRE(affects_manager.getAffects(3) == StmtInfoPtrSet{assign_stmt_9});
        REQUIRE(affects_manager.getAffects(4).empty());
        REQUIRE(affects_manager.getAffects(5) == StmtInfoPtrSet{assign_stmt_6});
        REQUIRE(affects_manager.getAffects(6) == StmtInfoPtrSet{assign_stmt_9});
        REQUIRE(affects_manager.getAffects(7) == StmtInfoPtrSet{assign_stmt_8});
        REQUIRE(affects_manager.getAffects(8).empty());
        REQUIRE(affects_manager.getAffects(9).empty());
    }
}