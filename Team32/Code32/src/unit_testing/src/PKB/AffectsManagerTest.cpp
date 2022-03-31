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

		CHECK(affects_manager.getAffects(1) == StmtInfoPtrSet{assign_stmt_2, assign_stmt_3});
		CHECK(affects_manager.getAffects(2) == StmtInfoPtrSet{assign_stmt_4});
		CHECK(affects_manager.getAffects(3) == StmtInfoPtrSet{assign_stmt_4});
		CHECK(affects_manager.getAffects(4) == StmtInfoPtrSet{assign_stmt_5});
		// Duplicate to test cache
		CHECK(affects_manager.getAffects(1) == StmtInfoPtrSet{assign_stmt_2, assign_stmt_3});
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

		CHECK(affects_manager.getAffects(1) == StmtInfoPtrSet{assign_stmt_4, assign_stmt_8});
		CHECK_THROWS(affects_manager.getAffects(2));
		CHECK(affects_manager.getAffects(3) == StmtInfoPtrSet{assign_stmt_9});
		CHECK(affects_manager.getAffects(4).empty());
		CHECK(affects_manager.getAffects(5) == StmtInfoPtrSet{assign_stmt_6});
		CHECK(affects_manager.getAffects(6) == StmtInfoPtrSet{assign_stmt_9});
		CHECK(affects_manager.getAffects(7) == StmtInfoPtrSet{assign_stmt_8});
		CHECK(affects_manager.getAffects(8).empty());
		CHECK(affects_manager.getAffects(9).empty());
	}

	SECTION("PKB::AffectsManager::getAffects While Loop Test") {
		/* SIMPLE Code:
		 * 1. x = 5;
		 * 2. while (a > 0) {
		 * 3.   x = x + 1;
		 *    }
		 * 4. while (y < 2);
		 * 5.   b = x - 1;
		 * 6.   x = b + 1;
		 * 7.   while (a == 1) {
		 * 8.     read x;
		 * 9.     x = x + 1;
		 *      }
		 *    }
		 * 10.z = x;
		 */
		shared_ptr<StmtInfo> assign_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::Assign);
		shared_ptr<StmtInfo> while_stmt_2 = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
		shared_ptr<StmtInfo> assign_stmt_3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
		shared_ptr<StmtInfo> while_stmt_4 = TestUtilities::createStmtInfo(4, StmtType::WhileStmt);
		shared_ptr<StmtInfo> assign_stmt_5 = TestUtilities::createStmtInfo(5, StmtType::Assign);
		shared_ptr<StmtInfo> assign_stmt_6 = TestUtilities::createStmtInfo(6, StmtType::Assign);
		shared_ptr<StmtInfo> while_stmt_7 = TestUtilities::createStmtInfo(7, StmtType::WhileStmt);
		shared_ptr<StmtInfo> read_stmt_8 = TestUtilities::createStmtInfo(8, StmtType::Read);
		shared_ptr<StmtInfo> assign_stmt_9 = TestUtilities::createStmtInfo(9, StmtType::Assign);
		shared_ptr<StmtInfo> assign_stmt_10 = TestUtilities::createStmtInfo(10, StmtType::Assign);

		CHECK_NOTHROW(cfg.createNode(assign_stmt_1));
		CHECK_NOTHROW(cfg.createNode(while_stmt_2));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_3));
		CHECK_NOTHROW(cfg.createNode(while_stmt_4));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_5));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_6));
		CHECK_NOTHROW(cfg.createNode(while_stmt_7));
		CHECK_NOTHROW(cfg.createNode(read_stmt_8));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_9));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_10));

		CHECK_NOTHROW(modifies_store.set(assign_stmt_1, "x"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_3, "x"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_5, "b"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_6, "x"));
		CHECK_NOTHROW(modifies_store.set(read_stmt_8, "x"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_9, "x"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_10, "z"));

		CHECK_NOTHROW(uses_store.set(while_stmt_2, "a"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_3, "x"));
		CHECK_NOTHROW(uses_store.set(while_stmt_4, "y"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_5, "x"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_6, "b"));
		CHECK_NOTHROW(uses_store.set(while_stmt_7, "a"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_9, "x"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_10, "x"));

		CHECK_NOTHROW(next_manager.setNext(1, 2));
		CHECK_NOTHROW(next_manager.setNext(2, 3));
		CHECK_NOTHROW(next_manager.setNext(3, 2));
		CHECK_NOTHROW(next_manager.setNext(2, 4));
		CHECK_NOTHROW(next_manager.setNext(4, 5));
		CHECK_NOTHROW(next_manager.setNext(5, 6));
		CHECK_NOTHROW(next_manager.setNext(6, 7));
		CHECK_NOTHROW(next_manager.setNext(7, 8));
		CHECK_NOTHROW(next_manager.setNext(7, 4));
		CHECK_NOTHROW(next_manager.setNext(8, 9));
		CHECK_NOTHROW(next_manager.setNext(9, 7));
		CHECK_NOTHROW(next_manager.setNext(4, 10));

		CHECK(affects_manager.getAffects(1) == StmtInfoPtrSet{assign_stmt_3, assign_stmt_5, assign_stmt_10});
		CHECK_THROWS(affects_manager.getAffects(2));
		CHECK(affects_manager.getAffects(3) == StmtInfoPtrSet{assign_stmt_3, assign_stmt_5, assign_stmt_10});
		CHECK_THROWS(affects_manager.getAffects(4));
		CHECK(affects_manager.getAffects(5) == StmtInfoPtrSet{assign_stmt_6});
		CHECK(affects_manager.getAffects(6) == StmtInfoPtrSet{assign_stmt_5, assign_stmt_10});
		CHECK_THROWS(affects_manager.getAffects(7));
		CHECK_THROWS(affects_manager.getAffects(8));
		CHECK(affects_manager.getAffects(9) == StmtInfoPtrSet{assign_stmt_5, assign_stmt_10});
		CHECK(affects_manager.getAffects(10).empty());
	}
}

TEST_CASE("PKB::AffectsManager::getAffected Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	PKB::SVRelationStore<PKB::ModifiesSRelation> modifies_store;
	PKB::SVRelationStore<PKB::UsesSRelation> uses_store;
	PKB::AffectsManager affects_manager = PKB::AffectsManager(cfg, modifies_store, uses_store);

	SECTION("PKB::AffectsManager::getAffected Simple Consecutive Assign Statement Test") {
		/* SIMPLE Code:
		 * 1. a = 5;
		 * 2. b = a * 2;
		 * 3. c = a + b;
		 * 4. d = c + b * a;
		 * 5. e = a + b + c + d;
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

		CHECK_NOTHROW(modifies_store.set(assign_stmt_1, "a"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_2, "b"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_3, "c"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_4, "d"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_5, "e"));

		CHECK_NOTHROW(uses_store.set(assign_stmt_2, "a"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_3, "a"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_3, "b"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_4, "a"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_4, "b"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_4, "c"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_5, "a"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_5, "b"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_5, "c"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_5, "d"));

		CHECK_NOTHROW(next_manager.setNext(1, 2));
		CHECK_NOTHROW(next_manager.setNext(2, 3));
		CHECK_NOTHROW(next_manager.setNext(3, 4));
		CHECK_NOTHROW(next_manager.setNext(4, 5));

		CHECK(affects_manager.getAffected(1).empty());
		CHECK(affects_manager.getAffected(2) == StmtInfoPtrSet{assign_stmt_1});
		CHECK(affects_manager.getAffected(3) == StmtInfoPtrSet{assign_stmt_1, assign_stmt_2});
		CHECK(affects_manager.getAffected(4) == StmtInfoPtrSet{assign_stmt_1, assign_stmt_2, assign_stmt_3});
		CHECK(affects_manager.getAffected(5) == StmtInfoPtrSet{assign_stmt_1, assign_stmt_2, assign_stmt_3, assign_stmt_4});
		// Duplicate to test cache
		CHECK(affects_manager.getAffected(2) == StmtInfoPtrSet{assign_stmt_1});
	}

	SECTION("PKB::AffectsManager::getAffected If Branching Test") {
		/* SIMPLE Code:
		 * 1. x = 5;
		 * 2. call Monk; // Assume Monk modifies 'x'.
		 * 3. x = 3;
		 * 4. a = 1;
		 * 5. if (a > 0) {
		 * 6.   b = a + x;
		 * 7.   c = x * b;
		 *    } else {
		 * 8.   b = x - 1;
		 * 9.   x = y + x;
		 *    }
		 * 10.z = x / c * a + b;
		 */
		shared_ptr<StmtInfo> assign_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::Assign);
		shared_ptr<StmtInfo> call_stmt_2 = TestUtilities::createStmtInfo(2, StmtType::Call);
		shared_ptr<StmtInfo> assign_stmt_3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
		shared_ptr<StmtInfo> assign_stmt_4 = TestUtilities::createStmtInfo(4, StmtType::Assign);
		shared_ptr<StmtInfo> if_stmt_5 = TestUtilities::createStmtInfo(5, StmtType::IfStmt);
		shared_ptr<StmtInfo> assign_stmt_6 = TestUtilities::createStmtInfo(6, StmtType::Assign);
		shared_ptr<StmtInfo> assign_stmt_7 = TestUtilities::createStmtInfo(7, StmtType::Assign);
		shared_ptr<StmtInfo> assign_stmt_8 = TestUtilities::createStmtInfo(8, StmtType::Assign);
		shared_ptr<StmtInfo> assign_stmt_9 = TestUtilities::createStmtInfo(9, StmtType::Assign);
		shared_ptr<StmtInfo> assign_stmt_10 = TestUtilities::createStmtInfo(10, StmtType::Assign);

		CHECK_NOTHROW(cfg.createNode(assign_stmt_1));
		CHECK_NOTHROW(cfg.createNode(call_stmt_2));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_3));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_4));
		CHECK_NOTHROW(cfg.createNode(if_stmt_5));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_6));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_7));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_8));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_9));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_10));

		CHECK_NOTHROW(modifies_store.set(assign_stmt_1, "x"));
		CHECK_NOTHROW(modifies_store.set(call_stmt_2, "x"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_3, "x"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_4, "a"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_6, "b"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_7, "c"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_8, "b"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_9, "x"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_10, "z"));

		CHECK_NOTHROW(uses_store.set(if_stmt_5, "a"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_6, "a"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_6, "x"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_7, "x"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_7, "b"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_8, "x"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_9, "y"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_9, "x"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_10, "a"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_10, "b"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_10, "c"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_10, "x"));

		CHECK_NOTHROW(next_manager.setIfNext(5, 6, 8));
		CHECK_NOTHROW(next_manager.setIfExit(7, 9, 5));

		CHECK_NOTHROW(next_manager.setNext(1, 2));
		CHECK_NOTHROW(next_manager.setNext(2, 3));
		CHECK_NOTHROW(next_manager.setNext(3, 4));
		CHECK_NOTHROW(next_manager.setNext(4, 5));
		CHECK_NOTHROW(next_manager.setNext(5, 10));
		CHECK_NOTHROW(next_manager.setNext(6, 7));
		CHECK_NOTHROW(next_manager.setNext(8, 9));

		CHECK(affects_manager.getAffected(1).empty());
		CHECK_THROWS(affects_manager.getAffected(2));
		CHECK(affects_manager.getAffected(3).empty());
		CHECK(affects_manager.getAffected(4).empty());
		CHECK_THROWS(affects_manager.getAffected(5));
		CHECK(affects_manager.getAffected(6) == StmtInfoPtrSet{assign_stmt_3, assign_stmt_4});
		CHECK(affects_manager.getAffected(7) == StmtInfoPtrSet{assign_stmt_3, assign_stmt_6});
		CHECK(affects_manager.getAffected(8) == StmtInfoPtrSet{assign_stmt_3});
		CHECK(affects_manager.getAffected(9) == StmtInfoPtrSet{assign_stmt_3});
		CHECK(affects_manager.getAffected(10) ==
		      StmtInfoPtrSet{assign_stmt_3, assign_stmt_4, assign_stmt_6, assign_stmt_7, assign_stmt_8, assign_stmt_9});
	}

	SECTION("PKB::AffectsManager::getAffected While Loop Test") {
		/* SIMPLE Code:
		 * 1. x = 5;
		 * 2. while (a > 0) {
		 * 3.   x = x + 1;
		 *    }
		 * 4. while (y < 2);
		 * 5.   b = x - 1;
		 * 6.   x = b + 1;
		 * 7.   while (a == 1) {
		 * 8.     read x;
		 * 9.     x = x + 1;
		 *      }
		 *    }
		 * 10.z = x;
		 */
		shared_ptr<StmtInfo> assign_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::Assign);
		shared_ptr<StmtInfo> while_stmt_2 = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
		shared_ptr<StmtInfo> assign_stmt_3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
		shared_ptr<StmtInfo> while_stmt_4 = TestUtilities::createStmtInfo(4, StmtType::WhileStmt);
		shared_ptr<StmtInfo> assign_stmt_5 = TestUtilities::createStmtInfo(5, StmtType::Assign);
		shared_ptr<StmtInfo> assign_stmt_6 = TestUtilities::createStmtInfo(6, StmtType::Assign);
		shared_ptr<StmtInfo> while_stmt_7 = TestUtilities::createStmtInfo(7, StmtType::WhileStmt);
		shared_ptr<StmtInfo> read_stmt_8 = TestUtilities::createStmtInfo(8, StmtType::Read);
		shared_ptr<StmtInfo> assign_stmt_9 = TestUtilities::createStmtInfo(9, StmtType::Assign);
		shared_ptr<StmtInfo> assign_stmt_10 = TestUtilities::createStmtInfo(10, StmtType::Assign);

		CHECK_NOTHROW(cfg.createNode(assign_stmt_1));
		CHECK_NOTHROW(cfg.createNode(while_stmt_2));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_3));
		CHECK_NOTHROW(cfg.createNode(while_stmt_4));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_5));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_6));
		CHECK_NOTHROW(cfg.createNode(while_stmt_7));
		CHECK_NOTHROW(cfg.createNode(read_stmt_8));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_9));
		CHECK_NOTHROW(cfg.createNode(assign_stmt_10));

		CHECK_NOTHROW(modifies_store.set(assign_stmt_1, "x"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_3, "x"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_5, "b"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_6, "x"));
		CHECK_NOTHROW(modifies_store.set(read_stmt_8, "x"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_9, "x"));
		CHECK_NOTHROW(modifies_store.set(assign_stmt_10, "z"));

		CHECK_NOTHROW(uses_store.set(while_stmt_2, "a"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_3, "x"));
		CHECK_NOTHROW(uses_store.set(while_stmt_4, "y"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_5, "x"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_6, "b"));
		CHECK_NOTHROW(uses_store.set(while_stmt_7, "a"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_9, "x"));
		CHECK_NOTHROW(uses_store.set(assign_stmt_10, "x"));

		CHECK_NOTHROW(next_manager.setNext(1, 2));
		CHECK_NOTHROW(next_manager.setNext(2, 3));
		CHECK_NOTHROW(next_manager.setNext(3, 2));
		CHECK_NOTHROW(next_manager.setNext(2, 4));
		CHECK_NOTHROW(next_manager.setNext(4, 5));
		CHECK_NOTHROW(next_manager.setNext(5, 6));
		CHECK_NOTHROW(next_manager.setNext(6, 7));
		CHECK_NOTHROW(next_manager.setNext(7, 8));
		CHECK_NOTHROW(next_manager.setNext(7, 4));
		CHECK_NOTHROW(next_manager.setNext(8, 9));
		CHECK_NOTHROW(next_manager.setNext(9, 7));
		CHECK_NOTHROW(next_manager.setNext(4, 10));

		CHECK(affects_manager.getAffected(1).empty());
		CHECK_THROWS(affects_manager.getAffected(2));
		CHECK(affects_manager.getAffected(3) == StmtInfoPtrSet{assign_stmt_1, assign_stmt_3});
		CHECK_THROWS(affects_manager.getAffected(4));
		CHECK(affects_manager.getAffected(5) == StmtInfoPtrSet{assign_stmt_1, assign_stmt_3, assign_stmt_6, assign_stmt_9});
		CHECK(affects_manager.getAffected(6) == StmtInfoPtrSet{assign_stmt_5});
		CHECK_THROWS(affects_manager.getAffected(7));
		CHECK_THROWS(affects_manager.getAffected(8));
		CHECK(affects_manager.getAffected(9).empty());
		CHECK(affects_manager.getAffected(10) == StmtInfoPtrSet{assign_stmt_1, assign_stmt_3, assign_stmt_6, assign_stmt_9});
	}
}

TEST_CASE("PKB::AffectsManager::checkAffects Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	PKB::SVRelationStore<PKB::ModifiesSRelation> modifies_store;
	PKB::SVRelationStore<PKB::UsesSRelation> uses_store;
	PKB::AffectsManager affects_manager = PKB::AffectsManager(cfg, modifies_store, uses_store);
	/* SIMPLE Code:
	 * 1. x = 5;
	 * 2. while (a > 0) {
	 * 3.   x = x + 1;
	 *    }
	 * 4. while (y < 2) {
	 * 5.   b = x - 1;
	 * 6.   x = b + 1;
	 * 7.   while (a == 1) {
	 * 8.     read x;
	 * 9.     x = x + 1;
	 *      }
	 *    }
	 * 10.z = x;
	 */
	shared_ptr<StmtInfo> assign_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::Assign);
	shared_ptr<StmtInfo> while_stmt_2 = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> assign_stmt_3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
	shared_ptr<StmtInfo> while_stmt_4 = TestUtilities::createStmtInfo(4, StmtType::WhileStmt);
	shared_ptr<StmtInfo> assign_stmt_5 = TestUtilities::createStmtInfo(5, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_6 = TestUtilities::createStmtInfo(6, StmtType::Assign);
	shared_ptr<StmtInfo> while_stmt_7 = TestUtilities::createStmtInfo(7, StmtType::WhileStmt);
	shared_ptr<StmtInfo> read_stmt_8 = TestUtilities::createStmtInfo(8, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt_9 = TestUtilities::createStmtInfo(9, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_10 = TestUtilities::createStmtInfo(10, StmtType::Assign);

	CHECK_NOTHROW(cfg.createNode(assign_stmt_1));
	CHECK_NOTHROW(cfg.createNode(while_stmt_2));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_3));
	CHECK_NOTHROW(cfg.createNode(while_stmt_4));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_5));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_6));
	CHECK_NOTHROW(cfg.createNode(while_stmt_7));
	CHECK_NOTHROW(cfg.createNode(read_stmt_8));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_9));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_10));

	CHECK_NOTHROW(modifies_store.set(assign_stmt_1, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_3, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_5, "b"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_6, "x"));
	CHECK_NOTHROW(modifies_store.set(read_stmt_8, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_9, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_10, "z"));

	CHECK_NOTHROW(uses_store.set(while_stmt_2, "a"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_3, "x"));
	CHECK_NOTHROW(uses_store.set(while_stmt_4, "y"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_5, "x"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_6, "b"));
	CHECK_NOTHROW(uses_store.set(while_stmt_7, "a"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_9, "x"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_10, "x"));

	CHECK_NOTHROW(next_manager.setNext(1, 2));
	CHECK_NOTHROW(next_manager.setNext(2, 3));
	CHECK_NOTHROW(next_manager.setNext(3, 2));
	CHECK_NOTHROW(next_manager.setNext(2, 4));
	CHECK_NOTHROW(next_manager.setNext(4, 5));
	CHECK_NOTHROW(next_manager.setNext(5, 6));
	CHECK_NOTHROW(next_manager.setNext(6, 7));
	CHECK_NOTHROW(next_manager.setNext(7, 8));
	CHECK_NOTHROW(next_manager.setNext(7, 4));
	CHECK_NOTHROW(next_manager.setNext(8, 9));
	CHECK_NOTHROW(next_manager.setNext(9, 7));
	CHECK_NOTHROW(next_manager.setNext(4, 10));

	CHECK(affects_manager.checkAffects(1, 3));
	CHECK(affects_manager.checkAffects(1, 5));
	CHECK(affects_manager.checkAffects(1, 10));
	CHECK(affects_manager.checkAffects(3, 3));
	CHECK(affects_manager.checkAffects(3, 5));
	CHECK(affects_manager.checkAffects(3, 10));
	CHECK(affects_manager.checkAffects(5, 6));
	CHECK(affects_manager.checkAffects(6, 5));
	CHECK(affects_manager.checkAffects(6, 10));
	CHECK(affects_manager.checkAffects(9, 5));
	CHECK(affects_manager.checkAffects(9, 10));
	CHECK_THROWS(affects_manager.checkAffects(2, 3));
	CHECK_THROWS(affects_manager.checkAffects(4, 5));
	CHECK_THROWS(affects_manager.checkAffects(7, 8));
	CHECK_THROWS(affects_manager.checkAffects(8, 9));
	CHECK_FALSE(affects_manager.checkAffects(1, 9));
	CHECK_FALSE(affects_manager.checkAffects(3, 1));
	CHECK_FALSE(affects_manager.checkAffects(6, 8));
	CHECK_FALSE(affects_manager.checkAffects(6, 9));
	CHECK_FALSE(affects_manager.checkAffects(9, 9));
	CHECK_FALSE(affects_manager.checkAffects(5, 10));
}

TEST_CASE("PKB::AffectsManager::checkAffectsStar Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	PKB::SVRelationStore<PKB::ModifiesSRelation> modifies_store;
	PKB::SVRelationStore<PKB::UsesSRelation> uses_store;
	PKB::AffectsManager affects_manager = PKB::AffectsManager(cfg, modifies_store, uses_store);
	/* SIMPLE Code:
	 * 1. x = 5;
	 * 2. while (a > 0) {
	 * 3.   x = x + 1;
	 *    }
	 * 4. while (y < 2) {
	 * 5.   b = x - 1;
	 * 6.   x = b + 1;
	 * 7.   while (a == 1) {
	 * 8.     read x;
	 * 9.     x = x + 1;
	 *      }
	 *    }
	 * 10.z = x;
	 */
	shared_ptr<StmtInfo> assign_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::Assign);
	shared_ptr<StmtInfo> while_stmt_2 = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> assign_stmt_3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
	shared_ptr<StmtInfo> while_stmt_4 = TestUtilities::createStmtInfo(4, StmtType::WhileStmt);
	shared_ptr<StmtInfo> assign_stmt_5 = TestUtilities::createStmtInfo(5, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_6 = TestUtilities::createStmtInfo(6, StmtType::Assign);
	shared_ptr<StmtInfo> while_stmt_7 = TestUtilities::createStmtInfo(7, StmtType::WhileStmt);
	shared_ptr<StmtInfo> read_stmt_8 = TestUtilities::createStmtInfo(8, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt_9 = TestUtilities::createStmtInfo(9, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_10 = TestUtilities::createStmtInfo(10, StmtType::Assign);

	CHECK_NOTHROW(cfg.createNode(assign_stmt_1));
	CHECK_NOTHROW(cfg.createNode(while_stmt_2));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_3));
	CHECK_NOTHROW(cfg.createNode(while_stmt_4));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_5));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_6));
	CHECK_NOTHROW(cfg.createNode(while_stmt_7));
	CHECK_NOTHROW(cfg.createNode(read_stmt_8));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_9));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_10));

	CHECK_NOTHROW(modifies_store.set(assign_stmt_1, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_3, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_5, "b"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_6, "x"));
	CHECK_NOTHROW(modifies_store.set(read_stmt_8, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_9, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_10, "z"));

	CHECK_NOTHROW(uses_store.set(while_stmt_2, "a"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_3, "x"));
	CHECK_NOTHROW(uses_store.set(while_stmt_4, "y"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_5, "x"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_6, "b"));
	CHECK_NOTHROW(uses_store.set(while_stmt_7, "a"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_9, "x"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_10, "x"));

	CHECK_NOTHROW(next_manager.setNext(1, 2));
	CHECK_NOTHROW(next_manager.setNext(2, 3));
	CHECK_NOTHROW(next_manager.setNext(3, 2));
	CHECK_NOTHROW(next_manager.setNext(2, 4));
	CHECK_NOTHROW(next_manager.setNext(4, 5));
	CHECK_NOTHROW(next_manager.setNext(5, 6));
	CHECK_NOTHROW(next_manager.setNext(6, 7));
	CHECK_NOTHROW(next_manager.setNext(7, 8));
	CHECK_NOTHROW(next_manager.setNext(7, 4));
	CHECK_NOTHROW(next_manager.setNext(8, 9));
	CHECK_NOTHROW(next_manager.setNext(9, 7));
	CHECK_NOTHROW(next_manager.setNext(4, 10));

	CHECK(affects_manager.checkAffectsStar(1, 6));
	CHECK(affects_manager.checkAffectsStar(3, 6));
	CHECK(affects_manager.checkAffectsStar(5, 10));
	CHECK(affects_manager.checkAffectsStar(5, 5));
	CHECK(affects_manager.checkAffectsStar(6, 6));
	CHECK(affects_manager.checkAffectsStar(9, 6));
	CHECK_THROWS(affects_manager.checkAffectsStar(2, 3));
	CHECK_THROWS(affects_manager.checkAffectsStar(4, 5));
	CHECK_THROWS(affects_manager.checkAffectsStar(7, 8));
	CHECK_THROWS(affects_manager.checkAffectsStar(8, 9));
	CHECK_FALSE(affects_manager.checkAffectsStar(1, 9));
	CHECK_FALSE(affects_manager.checkAffectsStar(6, 9));
	CHECK_FALSE(affects_manager.checkAffectsStar(9, 9));
}

TEST_CASE("PKB::AffectsManager::checkAffectsStar While Nested Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	PKB::SVRelationStore<PKB::ModifiesSRelation> modifies_store;
	PKB::SVRelationStore<PKB::UsesSRelation> uses_store;
	PKB::AffectsManager affects_manager = PKB::AffectsManager(cfg, modifies_store, uses_store);
	/* SIMPLE code:
	 * 1. while (a < 1) {
	 * 2. 	x = x + 1;
	 * 3.	while (x < 5) {
	 * 4.   	b = x + y;
	 * 5. 		y = b + 1;
	 * 6. 		x = b + 1;
	 *  	}
	 *   }
	 */
	shared_ptr<StmtInfo> while_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::WhileStmt);
	shared_ptr<StmtInfo> assign_stmt_2 = TestUtilities::createStmtInfo(2, StmtType::Assign);
	shared_ptr<StmtInfo> while_stmt_3 = TestUtilities::createStmtInfo(3, StmtType::WhileStmt);
	shared_ptr<StmtInfo> assign_stmt_4 = TestUtilities::createStmtInfo(4, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_5 = TestUtilities::createStmtInfo(5, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_6 = TestUtilities::createStmtInfo(6, StmtType::Assign);

	CHECK_NOTHROW(cfg.createNode(while_stmt_1));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_2));
	CHECK_NOTHROW(cfg.createNode(while_stmt_3));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_4));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_5));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_6));

	CHECK_NOTHROW(modifies_store.set(assign_stmt_2, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_4, "b"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_5, "y"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_6, "x"));

	CHECK_NOTHROW(uses_store.set(assign_stmt_2, "x"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_4, VarRefSet{"x", "y"}));
	CHECK_NOTHROW(uses_store.set(assign_stmt_5, "b"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_6, "b"));

	CHECK_NOTHROW(next_manager.setNext(1, 2));
	CHECK_NOTHROW(next_manager.setNext(2, 3));
	CHECK_NOTHROW(next_manager.setNext(3, 1));
	CHECK_NOTHROW(next_manager.setNext(3, 4));
	CHECK_NOTHROW(next_manager.setNext(4, 5));
	CHECK_NOTHROW(next_manager.setNext(5, 6));
	CHECK_NOTHROW(next_manager.setNext(6, 3));

	CHECK(affects_manager.checkAffectsStar(6, 2));
	CHECK(affects_manager.checkAffectsStar(6, 4));
	CHECK(affects_manager.checkAffectsStar(6, 5));
	CHECK(affects_manager.checkAffectsStar(6, 6));
	CHECK(affects_manager.checkAffectsStar(2, 2));
	CHECK(affects_manager.checkAffectsStar(2, 4));
	CHECK(affects_manager.checkAffectsStar(2, 5));
	CHECK(affects_manager.checkAffectsStar(2, 6));
	CHECK(affects_manager.checkAffectsStar(4, 5));
	CHECK(affects_manager.checkAffectsStar(4, 6));
	CHECK(affects_manager.checkAffectsStar(4, 4));
	CHECK(affects_manager.checkAffectsStar(4, 2));
}

TEST_CASE("PKB::AffectsManager::getAffectsStar Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	PKB::SVRelationStore<PKB::ModifiesSRelation> modifies_store;
	PKB::SVRelationStore<PKB::UsesSRelation> uses_store;
	PKB::AffectsManager affects_manager = PKB::AffectsManager(cfg, modifies_store, uses_store);
	/* SIMPLE Code:
	 * 1. x = 5;
	 * 2. while (a > 0) {
	 * 3.   x = x + 1;
	 *    }
	 * 4. if (y < 2) then {
	 * 5.   b = x - 1;
	 * 6.   x = b + 1;
	 * 7.   while (a == 1) {
	 * 8.     read x;
	 * 9.     x = x + 1;
	 *      }
	 *    } else {
	 * 10.   x = x - 1;
	 *    }
	 * 11. z = x;
	 */
	shared_ptr<StmtInfo> assign_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::Assign);
	shared_ptr<StmtInfo> while_stmt_2 = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> assign_stmt_3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
	shared_ptr<StmtInfo> if_stmt_4 = TestUtilities::createStmtInfo(4, StmtType::IfStmt);
	shared_ptr<StmtInfo> assign_stmt_5 = TestUtilities::createStmtInfo(5, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_6 = TestUtilities::createStmtInfo(6, StmtType::Assign);
	shared_ptr<StmtInfo> while_stmt_7 = TestUtilities::createStmtInfo(7, StmtType::WhileStmt);
	shared_ptr<StmtInfo> read_stmt_8 = TestUtilities::createStmtInfo(8, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt_9 = TestUtilities::createStmtInfo(9, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_10 = TestUtilities::createStmtInfo(10, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_11 = TestUtilities::createStmtInfo(11, StmtType::Assign);

	CHECK_NOTHROW(cfg.createNode(assign_stmt_1));
	CHECK_NOTHROW(cfg.createNode(while_stmt_2));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_3));
	CHECK_NOTHROW(cfg.createNode(if_stmt_4));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_5));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_6));
	CHECK_NOTHROW(cfg.createNode(while_stmt_7));
	CHECK_NOTHROW(cfg.createNode(read_stmt_8));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_9));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_10));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_11));

	CHECK_NOTHROW(modifies_store.set(assign_stmt_1, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_3, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_5, "b"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_6, "x"));
	CHECK_NOTHROW(modifies_store.set(read_stmt_8, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_9, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_10, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_11, "z"));

	CHECK_NOTHROW(uses_store.set(while_stmt_2, "a"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_3, "x"));
	CHECK_NOTHROW(uses_store.set(if_stmt_4, "y"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_5, "x"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_6, "b"));
	CHECK_NOTHROW(uses_store.set(while_stmt_7, "a"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_9, "x"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_10, "x"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_11, "x"));

	CHECK_NOTHROW(next_manager.setNext(1, 2));
	CHECK_NOTHROW(next_manager.setNext(2, 3));
	CHECK_NOTHROW(next_manager.setNext(3, 2));
	CHECK_NOTHROW(next_manager.setNext(2, 4));
	CHECK_NOTHROW(next_manager.setIfNext(4, 5, 10));
	CHECK_NOTHROW(next_manager.setNext(5, 6));
	CHECK_NOTHROW(next_manager.setNext(6, 7));
	CHECK_NOTHROW(next_manager.setNext(7, 8));
	CHECK_NOTHROW(next_manager.setNext(8, 9));
	CHECK_NOTHROW(next_manager.setNext(9, 7));
	CHECK_NOTHROW(next_manager.setIfExit(7, 10, 4));
	CHECK_NOTHROW(next_manager.setNext(4, 11));

	CHECK(affects_manager.getAffectsStar(1) == StmtInfoPtrSet{assign_stmt_3, assign_stmt_5, assign_stmt_6, assign_stmt_10, assign_stmt_11});
	CHECK_THROWS(affects_manager.getAffectsStar(2));
	CHECK(affects_manager.getAffectsStar(3) == StmtInfoPtrSet{assign_stmt_3, assign_stmt_5, assign_stmt_6, assign_stmt_10, assign_stmt_11});
	CHECK_THROWS(affects_manager.getAffectsStar(4));
	CHECK(affects_manager.getAffectsStar(5) == StmtInfoPtrSet{assign_stmt_6, assign_stmt_11});
	CHECK(affects_manager.getAffectsStar(6) == StmtInfoPtrSet{assign_stmt_11});
	CHECK_THROWS(affects_manager.getAffectsStar(7));
	CHECK_THROWS(affects_manager.getAffectsStar(8));
	CHECK(affects_manager.getAffectsStar(9) == StmtInfoPtrSet{assign_stmt_11});
	CHECK(affects_manager.getAffectsStar(10) == StmtInfoPtrSet{assign_stmt_11});
	CHECK(affects_manager.getAffectsStar(11).empty());
}

TEST_CASE("PKB::AffectsManager::getAffectsStar Nested While Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	PKB::SVRelationStore<PKB::ModifiesSRelation> modifies_store;
	PKB::SVRelationStore<PKB::UsesSRelation> uses_store;
	PKB::AffectsManager affects_manager = PKB::AffectsManager(cfg, modifies_store, uses_store);
	/* SIMPLE code:
	 * 1. while (a < 0) {
	 * 2. 	while (b < 0) {
	 * 3. 		while (c < 0) {
	 * 4. 			x = b + 1;
	 * 5. 			b = x + 1;
	 * 			}
	 * 	6.		y = b + x;
	 * 	7.		x = x + 1;
	 * 		 }
	 * 	8.	 x = b + x;
	 * 	   }
	 */
	shared_ptr<StmtInfo> while_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::WhileStmt);
	shared_ptr<StmtInfo> while_stmt_2 = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> while_stmt_3 = TestUtilities::createStmtInfo(3, StmtType::WhileStmt);
	shared_ptr<StmtInfo> assign_stmt_4 = TestUtilities::createStmtInfo(4, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_5 = TestUtilities::createStmtInfo(5, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_6 = TestUtilities::createStmtInfo(6, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_7 = TestUtilities::createStmtInfo(7, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_8 = TestUtilities::createStmtInfo(8, StmtType::Assign);

	CHECK_NOTHROW(cfg.createNode(while_stmt_1));
	CHECK_NOTHROW(cfg.createNode(while_stmt_2));
	CHECK_NOTHROW(cfg.createNode(while_stmt_3));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_4));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_5));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_6));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_7));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_8));

	CHECK_NOTHROW(modifies_store.set(assign_stmt_4, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_5, "b"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_6, "y"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_7, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_8, "x"));

	CHECK_NOTHROW(uses_store.set(assign_stmt_4, "b"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_5, "x"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_6, VarRefSet{"b", "x"}));
	CHECK_NOTHROW(uses_store.set(assign_stmt_7, "x"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_8, VarRefSet{"b", "x"}));

	CHECK_NOTHROW(next_manager.setNext(1, 2));
	CHECK_NOTHROW(next_manager.setNext(2, 3));
	CHECK_NOTHROW(next_manager.setNext(3, 4));
	CHECK_NOTHROW(next_manager.setNext(4, 5));
	CHECK_NOTHROW(next_manager.setNext(5, 3));
	CHECK_NOTHROW(next_manager.setNext(3, 6));
	CHECK_NOTHROW(next_manager.setNext(6, 7));
	CHECK_NOTHROW(next_manager.setNext(7, 2));
	CHECK_NOTHROW(next_manager.setNext(2, 8));
	CHECK_NOTHROW(next_manager.setNext(8, 1));

	CHECK(affects_manager.getAffectsStar(4) == StmtInfoPtrSet{assign_stmt_4, assign_stmt_5, assign_stmt_6, assign_stmt_7, assign_stmt_8});
	CHECK(affects_manager.getAffectsStar(5) == StmtInfoPtrSet{assign_stmt_4, assign_stmt_5, assign_stmt_6, assign_stmt_7, assign_stmt_8});
	CHECK(affects_manager.getAffectsStar(6).empty());
	CHECK(affects_manager.getAffectsStar(7) == StmtInfoPtrSet{assign_stmt_6, assign_stmt_7, assign_stmt_8});
	CHECK(affects_manager.getAffectsStar(8) == StmtInfoPtrSet{assign_stmt_6, assign_stmt_7, assign_stmt_8});
}

TEST_CASE("PKB::AffectsManager::getAffectedStar Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	PKB::SVRelationStore<PKB::ModifiesSRelation> modifies_store;
	PKB::SVRelationStore<PKB::UsesSRelation> uses_store;
	PKB::AffectsManager affects_manager = PKB::AffectsManager(cfg, modifies_store, uses_store);
	/* SIMPLE Code:
	 * 1. x = 5;
	 * 2. while (a > 0) {
	 * 3.   x = x + 1;
	 *    }
	 * 4. if (y < 2) then {
	 * 5.   b = x - 1;
	 * 6.   x = b + 1;
	 * 7.   while (a == 1) {
	 * 8.     read x;
	 * 9.     x = x + 1;
	 *      }
	 *    } else {
	 * 10.   x = x - 1;
	 *    }
	 * 11. z = x;
	 */
	shared_ptr<StmtInfo> assign_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::Assign);
	shared_ptr<StmtInfo> while_stmt_2 = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> assign_stmt_3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
	shared_ptr<StmtInfo> if_stmt_4 = TestUtilities::createStmtInfo(4, StmtType::IfStmt);
	shared_ptr<StmtInfo> assign_stmt_5 = TestUtilities::createStmtInfo(5, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_6 = TestUtilities::createStmtInfo(6, StmtType::Assign);
	shared_ptr<StmtInfo> while_stmt_7 = TestUtilities::createStmtInfo(7, StmtType::WhileStmt);
	shared_ptr<StmtInfo> read_stmt_8 = TestUtilities::createStmtInfo(8, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt_9 = TestUtilities::createStmtInfo(9, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_10 = TestUtilities::createStmtInfo(10, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_11 = TestUtilities::createStmtInfo(11, StmtType::Assign);

	CHECK_NOTHROW(cfg.createNode(assign_stmt_1));
	CHECK_NOTHROW(cfg.createNode(while_stmt_2));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_3));
	CHECK_NOTHROW(cfg.createNode(if_stmt_4));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_5));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_6));
	CHECK_NOTHROW(cfg.createNode(while_stmt_7));
	CHECK_NOTHROW(cfg.createNode(read_stmt_8));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_9));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_10));
	CHECK_NOTHROW(cfg.createNode(assign_stmt_11));

	CHECK_NOTHROW(modifies_store.set(assign_stmt_1, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_3, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_5, "b"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_6, "x"));
	CHECK_NOTHROW(modifies_store.set(read_stmt_8, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_9, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_10, "x"));
	CHECK_NOTHROW(modifies_store.set(assign_stmt_11, "z"));

	CHECK_NOTHROW(uses_store.set(while_stmt_2, "a"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_3, "x"));
	CHECK_NOTHROW(uses_store.set(if_stmt_4, "y"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_5, "x"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_6, "b"));
	CHECK_NOTHROW(uses_store.set(while_stmt_7, "a"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_9, "x"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_10, "x"));
	CHECK_NOTHROW(uses_store.set(assign_stmt_11, "x"));

	CHECK_NOTHROW(next_manager.setNext(1, 2));
	CHECK_NOTHROW(next_manager.setNext(2, 3));
	CHECK_NOTHROW(next_manager.setNext(3, 2));
	CHECK_NOTHROW(next_manager.setNext(2, 4));
	CHECK_NOTHROW(next_manager.setIfNext(4, 5, 10));
	CHECK_NOTHROW(next_manager.setNext(5, 6));
	CHECK_NOTHROW(next_manager.setNext(6, 7));
	CHECK_NOTHROW(next_manager.setNext(7, 8));
	CHECK_NOTHROW(next_manager.setNext(8, 9));
	CHECK_NOTHROW(next_manager.setNext(9, 7));
	CHECK_NOTHROW(next_manager.setIfExit(7, 10, 4));
	CHECK_NOTHROW(next_manager.setNext(4, 11));

	CHECK(affects_manager.getAffectedStar(1, {}).empty());
	CHECK_THROWS(affects_manager.getAffectedStar(2, {}));
	CHECK(affects_manager.getAffectedStar(3, {}) == StmtInfoPtrSet{assign_stmt_1, assign_stmt_3});
	CHECK_THROWS(affects_manager.getAffectedStar(4, {}));
	CHECK(affects_manager.getAffectedStar(5, {}) == StmtInfoPtrSet{assign_stmt_1, assign_stmt_3});
	CHECK(affects_manager.getAffectedStar(6, {}) == StmtInfoPtrSet{assign_stmt_1, assign_stmt_3, assign_stmt_5});
	CHECK_THROWS(affects_manager.getAffectedStar(7, {}));
	CHECK_THROWS(affects_manager.getAffectedStar(8, {}));
	CHECK(affects_manager.getAffectedStar(9, {}).empty());
	CHECK(affects_manager.getAffectedStar(10, {}) == StmtInfoPtrSet{assign_stmt_1, assign_stmt_3});
	CHECK(affects_manager.getAffectedStar(11, {}) ==
	      StmtInfoPtrSet{assign_stmt_1, assign_stmt_3, assign_stmt_5, assign_stmt_6, assign_stmt_9, assign_stmt_10});
}
