#include "PKB/NextManager.h"

#include "PKB/CFG/ControlFlowGraph.h"
#include "TestUtilities.h"
#include "catch.hpp"

TEST_CASE("PKB::NextManager::getNext Test") {
	/* SIMPLE Code:
	 * 1. if (x==0) {
	 * 2.   while (y<2) {
	 * 3.     print(y);
	 *      }
	 *    } then else {
	 * 4.   read x;
	 *    }
	 * 5. z = 5;
	 * */
	PKB::CFG::ControlFlowGraph cfg = PKB::CFG::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt = TestUtilities::createStmtInfo(5, StmtType::Assign);

	cfg.createNode(if_stmt);
	cfg.createNode(while_stmt);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);
	cfg.createNode(assign_stmt);

	cfg.setIfNext(if_stmt->getIdentifier(), while_stmt->getIdentifier(), read_stmt->getIdentifier());
	cfg.setIfExit(while_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt->getIdentifier());
	cfg.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier());
	cfg.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier());
	cfg.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier());

	CHECK(next_manager.getNext(if_stmt->getIdentifier()) == unordered_set{while_stmt, read_stmt});
	CHECK(next_manager.getNext(while_stmt->getIdentifier()) == unordered_set{assign_stmt, print_stmt});
	CHECK(next_manager.getNext(print_stmt->getIdentifier()) == unordered_set{while_stmt});
	CHECK(next_manager.getNext(read_stmt->getIdentifier()) == unordered_set{assign_stmt});
	CHECK(next_manager.getNext(assign_stmt->getIdentifier()).empty());
}

TEST_CASE("PKB::NextManager::getPrevious Test") {
	/* Example SIMPLE Code:
	 * 1. if (x==0) then {
	 * 2.   while (y<2) {
	 * 3.     print(y);
	 *      }
	 *    } else {
	 * 4.   read x;
	 *    }
	 * 5. z = 5;
	 * */
	PKB::CFG::ControlFlowGraph cfg = PKB::CFG::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt = TestUtilities::createStmtInfo(5, StmtType::Assign);

	cfg.createNode(if_stmt);
	cfg.createNode(while_stmt);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);
	cfg.createNode(assign_stmt);

	cfg.setIfNext(if_stmt->getIdentifier(), while_stmt->getIdentifier(), read_stmt->getIdentifier());
	cfg.setIfExit(while_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt->getIdentifier());
	cfg.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier());
	cfg.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier());
	cfg.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier());

	CHECK(next_manager.getPrevious(if_stmt->getIdentifier()).empty());
	CHECK(next_manager.getPrevious(while_stmt->getIdentifier()) == unordered_set{if_stmt, print_stmt});
	CHECK(next_manager.getPrevious(print_stmt->getIdentifier()) == unordered_set{while_stmt});
	CHECK(next_manager.getPrevious(read_stmt->getIdentifier()) == unordered_set{if_stmt});
	CHECK(next_manager.getPrevious(assign_stmt->getIdentifier()) == unordered_set{while_stmt, read_stmt});
}

TEST_CASE("PKB::NextManager::checkNext Test") {
	/* SIMPLE Code:
	 * 1. if (x==0) then {
	 * 2.   while (y<2) {
	 * 3.     print(y);
	 *      }
	 *    } else {
	 * 4.   read x;
	 *    }
	 * 5. z = 5;
	 * */
	PKB::CFG::ControlFlowGraph cfg = PKB::CFG::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt = TestUtilities::createStmtInfo(5, StmtType::Assign);

	cfg.createNode(if_stmt);
	cfg.createNode(while_stmt);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);
	cfg.createNode(assign_stmt);

	cfg.setIfNext(if_stmt->getIdentifier(), while_stmt->getIdentifier(), read_stmt->getIdentifier());
	cfg.setIfExit(while_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt->getIdentifier());
	cfg.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier());
	cfg.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier());
	cfg.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier());

	CHECK(next_manager.checkNext(1, 2));
	CHECK(next_manager.checkNext(1, 4));
	CHECK(next_manager.checkNext(2, 3));
	CHECK(next_manager.checkNext(3, 2));
	CHECK(next_manager.checkNext(2, 5));
	CHECK(next_manager.checkNext(4, 5));
}

TEST_CASE("PKB::NextManager Overall Complicated Stress Test") {
	/* SIMPLE Code 1: Many Nested Ifs.
	 * 1. if (x==0) then {
	 * 2.   if (y>0) then {
	 * 3.     print y;
	 *      } else {
	 * 4.     if (z<5) then {
	 * 5.       read z;
	 *        } else {
	 * 6.       x = y + 1;
	 *        }
	 *      }
	 * } else {
	 * 7. if (a==b) then {
	 * 8.   b = c - 1;
	 *    } else {
	 * 9.   a = b;
	 *    }
	 * }
	 * 10. call Monkey;
	 * */
	PKB::CFG::ControlFlowGraph cfg = PKB::CFG::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);

	shared_ptr<StmtInfo> if_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> if_stmt_2 = TestUtilities::createStmtInfo(2, StmtType::If);
	shared_ptr<StmtInfo> print_stmt_3 = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> if_stmt_4 = TestUtilities::createStmtInfo(4, StmtType::If);
	shared_ptr<StmtInfo> read_stmt_5 = TestUtilities::createStmtInfo(5, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt_6 = TestUtilities::createStmtInfo(6, StmtType::Assign);
	shared_ptr<StmtInfo> if_stmt_7 = TestUtilities::createStmtInfo(7, StmtType::If);
	shared_ptr<StmtInfo> assign_stmt_8 = TestUtilities::createStmtInfo(8, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_9 = TestUtilities::createStmtInfo(9, StmtType::Assign);
	shared_ptr<StmtInfo> call_stmt_10 = TestUtilities::createStmtInfo(10, StmtType::Call);

	cfg.createNode(if_stmt_1);
	cfg.createNode(if_stmt_2);
	cfg.createNode(print_stmt_3);
	cfg.createNode(if_stmt_4);
	cfg.createNode(read_stmt_5);
	cfg.createNode(assign_stmt_6);
	cfg.createNode(if_stmt_7);
	cfg.createNode(assign_stmt_8);
	cfg.createNode(assign_stmt_9);
	cfg.createNode(call_stmt_10);

	cfg.setIfNext(if_stmt_1->getIdentifier(), if_stmt_2->getIdentifier(), if_stmt_7->getIdentifier());
	cfg.setIfNext(if_stmt_2->getIdentifier(), print_stmt_3->getIdentifier(), if_stmt_4->getIdentifier());
	cfg.setIfNext(if_stmt_4->getIdentifier(), read_stmt_5->getIdentifier(), assign_stmt_6->getIdentifier());
	cfg.setIfNext(if_stmt_7->getIdentifier(), assign_stmt_8->getIdentifier(), assign_stmt_9->getIdentifier());
	cfg.setIfExit(assign_stmt_8->getIdentifier(), assign_stmt_9->getIdentifier(), if_stmt_7->getIdentifier());
	cfg.setIfExit(read_stmt_5->getIdentifier(), assign_stmt_6->getIdentifier(), if_stmt_4->getIdentifier());
	cfg.setIfExit(print_stmt_3->getIdentifier(), if_stmt_4->getIdentifier(), if_stmt_2->getIdentifier());
	cfg.setIfExit(if_stmt_2->getIdentifier(), if_stmt_7->getIdentifier(), if_stmt_1->getIdentifier());
	cfg.setNext(if_stmt_1->getIdentifier(), call_stmt_10->getIdentifier());

	CHECK(next_manager.getNext(if_stmt_1->getIdentifier()) == unordered_set{if_stmt_2, if_stmt_7});
	CHECK(next_manager.getNext(if_stmt_2->getIdentifier()) == unordered_set{print_stmt_3, if_stmt_4});
	CHECK(next_manager.getNext(print_stmt_3->getIdentifier()) == unordered_set{call_stmt_10});
	CHECK(next_manager.getNext(if_stmt_4->getIdentifier()) == unordered_set{read_stmt_5, assign_stmt_6});
	CHECK(next_manager.getNext(read_stmt_5->getIdentifier()) == unordered_set{call_stmt_10});
	CHECK(next_manager.getNext(assign_stmt_6->getIdentifier()) == unordered_set{call_stmt_10});
	CHECK(next_manager.getNext(if_stmt_7->getIdentifier()) == unordered_set{assign_stmt_8, assign_stmt_9});
	CHECK(next_manager.getNext(assign_stmt_8->getIdentifier()) == unordered_set{call_stmt_10});
	CHECK(next_manager.getNext(assign_stmt_9->getIdentifier()) == unordered_set{call_stmt_10});
	CHECK(next_manager.getNext(call_stmt_10->getIdentifier()).empty());

	CHECK(next_manager.getPrevious(if_stmt_1->getIdentifier()).empty());
	CHECK(next_manager.getPrevious(if_stmt_2->getIdentifier()) == unordered_set{if_stmt_1});
	CHECK(next_manager.getPrevious(print_stmt_3->getIdentifier()) == unordered_set{if_stmt_2});
	CHECK(next_manager.getPrevious(if_stmt_4->getIdentifier()) == unordered_set{if_stmt_2});
	CHECK(next_manager.getPrevious(read_stmt_5->getIdentifier()) == unordered_set{if_stmt_4});
	CHECK(next_manager.getPrevious(assign_stmt_6->getIdentifier()) == unordered_set{if_stmt_4});
	CHECK(next_manager.getPrevious(if_stmt_7->getIdentifier()) == unordered_set{if_stmt_1});
	CHECK(next_manager.getPrevious(assign_stmt_8->getIdentifier()) == unordered_set{if_stmt_7});
	CHECK(next_manager.getPrevious(assign_stmt_9->getIdentifier()) == unordered_set{if_stmt_7});
	CHECK(next_manager.getPrevious(call_stmt_10->getIdentifier()) ==
	      unordered_set{print_stmt_3, read_stmt_5, assign_stmt_6, assign_stmt_8, assign_stmt_9});
}

TEST_CASE("PKB::NextManager::checkNextStar Test") {
	/* SIMPLE Code:
	 * 1. if (x==0) then {
	 * 2.   while (y<2) {
	 * 3.     print(y);
	 *      }
	 *    } else {
	 * 4.   read x;
	 *    }
	 * 5. z = 5;
	 * 6. call Monk;
	 * */
	PKB::CFG::ControlFlowGraph cfg = PKB::CFG::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt = TestUtilities::createStmtInfo(5, StmtType::Assign);
	shared_ptr<StmtInfo> call_stmt = TestUtilities::createStmtInfo(6, StmtType::Call);

	cfg.createNode(if_stmt);
	cfg.createNode(while_stmt);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);
	cfg.createNode(assign_stmt);
	cfg.createNode(call_stmt);

	cfg.setIfNext(if_stmt->getIdentifier(), while_stmt->getIdentifier(), read_stmt->getIdentifier());
	cfg.setIfExit(while_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt->getIdentifier());
	cfg.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier());
	cfg.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier());
	cfg.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier());
	cfg.setNext(assign_stmt->getIdentifier(), call_stmt->getIdentifier());
	cfg.optimize();

	CHECK(next_manager.getNextStar(1) == unordered_set{print_stmt, while_stmt, read_stmt, assign_stmt, call_stmt});
	CHECK(next_manager.getNextStar(2) == unordered_set{while_stmt, print_stmt, assign_stmt, call_stmt});
	CHECK(next_manager.getNextStar(3) == unordered_set{while_stmt, assign_stmt, call_stmt, print_stmt});
	CHECK(next_manager.getNextStar(4) == unordered_set{assign_stmt, call_stmt});
	CHECK(next_manager.getNextStar(5) == unordered_set{call_stmt});
	CHECK(next_manager.getNextStar(6) == StmtInfoPtrSet{});
}

TEST_CASE("PKB::NextManager::getPreviousStar Test") {
	/* SIMPLE Code:
	 * 1. if (x==0) then {
	 * 2.   while (y<2) {
	 * 3.     print(y);
	 *      }
	 *    } else {
	 * 4.   read x;
	 *    }
	 * 5. z = 5;
	 * 6. call Monk;
	 * */
	PKB::CFG::ControlFlowGraph cfg = PKB::CFG::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt = TestUtilities::createStmtInfo(5, StmtType::Assign);
	shared_ptr<StmtInfo> call_stmt = TestUtilities::createStmtInfo(6, StmtType::Call);

	cfg.createNode(if_stmt);
	cfg.createNode(while_stmt);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);
	cfg.createNode(assign_stmt);
	cfg.createNode(call_stmt);

	cfg.setIfNext(if_stmt->getIdentifier(), while_stmt->getIdentifier(), read_stmt->getIdentifier());
	cfg.setIfExit(while_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt->getIdentifier());
	cfg.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier());
	cfg.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier());
	cfg.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier());
	cfg.setNext(assign_stmt->getIdentifier(), call_stmt->getIdentifier());
	cfg.optimize();

	CHECK(next_manager.getPreviousStar(1) == StmtInfoPtrSet{});
	CHECK(next_manager.getPreviousStar(2) == unordered_set{while_stmt, if_stmt, print_stmt});
	CHECK(next_manager.getPreviousStar(3) == unordered_set{if_stmt, while_stmt, print_stmt});
	CHECK(next_manager.getPreviousStar(4) == unordered_set{if_stmt});
	CHECK(next_manager.getPreviousStar(5) == unordered_set{if_stmt, while_stmt, read_stmt, print_stmt});
	CHECK(next_manager.getPreviousStar(6) == unordered_set{if_stmt, while_stmt, read_stmt, print_stmt, assign_stmt});
}

TEST_CASE("PKB::NextManager Nested Container Test") {
	/*
	 * 1. if (...) then {
	 * 2.   while (...) {
	 * 3.     while (...) {
	 * 4.       read ...;
	 * 5.       print ...;
	 *        }
	 *      }
	 *    } else {
	 * 6.   while (...) {
	 * 7.     if (...) then {
	 * 8.       while (...) {
	 * 9.         read ...;
	 * 10.        print ...;
	 *          }
	 *        } else {
	 * 11.      if (...) then {
	 * 12.        read ...;
	 *          } else {
	 * 13.        print ...;
	 *          }
	 *        }
	 *      }
	 *    }
	 */
	PKB::CFG::ControlFlowGraph cfg = PKB::CFG::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);

	shared_ptr<StmtInfo> stmt_1 = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> stmt_2 = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> stmt_3 = TestUtilities::createStmtInfo(3, StmtType::While);
	shared_ptr<StmtInfo> stmt_4 = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> stmt_5 = TestUtilities::createStmtInfo(5, StmtType::Print);
	shared_ptr<StmtInfo> stmt_6 = TestUtilities::createStmtInfo(6, StmtType::While);
	shared_ptr<StmtInfo> stmt_7 = TestUtilities::createStmtInfo(7, StmtType::If);
	shared_ptr<StmtInfo> stmt_8 = TestUtilities::createStmtInfo(8, StmtType::While);
	shared_ptr<StmtInfo> stmt_9 = TestUtilities::createStmtInfo(9, StmtType::Read);
	shared_ptr<StmtInfo> stmt_10 = TestUtilities::createStmtInfo(10, StmtType::Print);
	shared_ptr<StmtInfo> stmt_11 = TestUtilities::createStmtInfo(11, StmtType::If);
	shared_ptr<StmtInfo> stmt_12 = TestUtilities::createStmtInfo(12, StmtType::Read);
	shared_ptr<StmtInfo> stmt_13 = TestUtilities::createStmtInfo(13, StmtType::Print);

	cfg.createNode(stmt_1);
	cfg.createNode(stmt_2);
	cfg.createNode(stmt_3);
	cfg.createNode(stmt_4);
	cfg.createNode(stmt_5);
	cfg.createNode(stmt_6);
	cfg.createNode(stmt_7);
	cfg.createNode(stmt_8);
	cfg.createNode(stmt_9);
	cfg.createNode(stmt_10);
	cfg.createNode(stmt_11);
	cfg.createNode(stmt_12);
	cfg.createNode(stmt_13);

	cfg.setIfNext(1, 2, 6);
	cfg.setIfExit(2, 6, 1);
	cfg.setNext(2, 3);
	cfg.setNext(3, 4);
	cfg.setNext(4, 5);
	cfg.setNext(5, 3);
	cfg.setNext(3, 2);
	cfg.setNext(6, 7);
	cfg.setNext(7, 6);
	cfg.setIfNext(7, 8, 11);
	cfg.setIfExit(8, 11, 7);
	cfg.setNext(8, 9);
	cfg.setNext(9, 10);
	cfg.setNext(10, 8);
	cfg.setIfNext(11, 12, 13);
	cfg.setIfExit(12, 13, 11);

	cfg.optimize();

	auto next_top_loop = unordered_set{stmt_2, stmt_3, stmt_4, stmt_5};
	auto next_bottom_loop = unordered_set{stmt_6, stmt_7, stmt_8, stmt_9, stmt_10, stmt_11, stmt_12, stmt_13};

	auto previous_top_loop = unordered_set{stmt_1, stmt_2, stmt_3, stmt_4, stmt_5};
	auto previous_bottom_loop = unordered_set{stmt_1, stmt_6, stmt_7, stmt_8, stmt_9, stmt_10, stmt_11, stmt_12, stmt_13};

	SECTION("Next Star Independent") {
		SECTION("1") {
			CHECK(next_manager.getNextStar(1) ==
			      unordered_set{stmt_2, stmt_3, stmt_4, stmt_5, stmt_6, stmt_7, stmt_8, stmt_9, stmt_10, stmt_11, stmt_12, stmt_13});
		}
		SECTION("2") { CHECK(next_manager.getNextStar(2) == next_top_loop); }
		SECTION("3") { CHECK(next_manager.getNextStar(3) == next_top_loop); }
		SECTION("4") { CHECK(next_manager.getNextStar(4) == next_top_loop); }
		SECTION("5") { CHECK(next_manager.getNextStar(5) == next_top_loop); }
		SECTION("6") { CHECK(next_manager.getNextStar(6) == next_bottom_loop); }
		SECTION("7") { CHECK(next_manager.getNextStar(7) == next_bottom_loop); }
		SECTION("8") { CHECK(next_manager.getNextStar(8) == next_bottom_loop); }
		SECTION("9") { CHECK(next_manager.getNextStar(9) == next_bottom_loop); }
		SECTION("10") { CHECK(next_manager.getNextStar(10) == next_bottom_loop); }
		SECTION("11") { CHECK(next_manager.getNextStar(11) == next_bottom_loop); }
		SECTION("12") { CHECK(next_manager.getNextStar(12) == next_bottom_loop); }
		SECTION("13") { CHECK(next_manager.getNextStar(13) == next_bottom_loop); }
	}

	SECTION("Previous Star Independent") {
		SECTION("1") { CHECK(next_manager.getPreviousStar(1) == unordered_set<shared_ptr<StmtInfo>>{}); }
		SECTION("2") { CHECK(next_manager.getPreviousStar(2) == previous_top_loop); }
		SECTION("3") { CHECK(next_manager.getPreviousStar(3) == previous_top_loop); }
		SECTION("4") { CHECK(next_manager.getPreviousStar(4) == previous_top_loop); }
		SECTION("5") { CHECK(next_manager.getPreviousStar(5) == previous_top_loop); }
		SECTION("6") { CHECK(next_manager.getPreviousStar(6) == previous_bottom_loop); }
		SECTION("7") { CHECK(next_manager.getPreviousStar(7) == previous_bottom_loop); }
		SECTION("8") { CHECK(next_manager.getPreviousStar(8) == previous_bottom_loop); }
		SECTION("9") { CHECK(next_manager.getPreviousStar(9) == previous_bottom_loop); }
		SECTION("10") { CHECK(next_manager.getPreviousStar(10) == previous_bottom_loop); }
		SECTION("11") { CHECK(next_manager.getPreviousStar(11) == previous_bottom_loop); }
		SECTION("12") { CHECK(next_manager.getPreviousStar(12) == previous_bottom_loop); }
		SECTION("13") { CHECK(next_manager.getPreviousStar(13) == previous_bottom_loop); }
	}

	SECTION("Next Star Cache Test") {
		CHECK(next_manager.getNextStar(8) == next_bottom_loop);
		CHECK(next_manager.getNextStar(3) == next_top_loop);
		CHECK(next_manager.getNextStar(1) ==
		      unordered_set{stmt_2, stmt_3, stmt_4, stmt_5, stmt_6, stmt_7, stmt_8, stmt_9, stmt_10, stmt_11, stmt_12, stmt_13});
	}

	SECTION("Previous Star Cache Test") {
		CHECK(next_manager.getPreviousStar(1) == unordered_set<shared_ptr<StmtInfo>>{});
		CHECK(next_manager.getPreviousStar(4) == previous_top_loop);
		CHECK(next_manager.getPreviousStar(8) == previous_bottom_loop);
	}
}