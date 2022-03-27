#include "PKB/NextManager.h"

#include "PKB/CFG/ControlFlowGraph.h"
#include "TestUtilities.h"
#include "catch.hpp"


TEST_CASE("PKB::NextManager::setNext Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt = TestUtilities::createStmtInfo(5, StmtType::Assign);

	cfg.createNode(if_stmt);
	cfg.createNode(while_stmt);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);
	cfg.createNode(assign_stmt);

	CHECK_THROWS(next_manager.setNext(if_stmt->getIdentifier(), if_stmt->getIdentifier()));

	CHECK_NOTHROW(next_manager.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier()));
	CHECK_NOTHROW(next_manager.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier()));
	CHECK_NOTHROW(next_manager.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier()));
}

TEST_CASE("PKB::NextManager::setIfNext Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	shared_ptr<StmtInfo> if_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(2, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(3, StmtType::Read);
	shared_ptr<StmtInfo> if_stmt_2 = TestUtilities::createStmtInfo(4, StmtType::IfStmt);
	cfg.createNode(if_stmt_1);
	cfg.createNode(if_stmt_2);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);

	CHECK_THROWS(next_manager.setIfNext(if_stmt_2->getIdentifier(), print_stmt->getIdentifier(), read_stmt->getIdentifier()));
	CHECK_THROWS(next_manager.setIfNext(if_stmt_1->getIdentifier(), read_stmt->getIdentifier(), print_stmt->getIdentifier()));

	CHECK_NOTHROW(next_manager.setIfNext(if_stmt_1->getIdentifier(), print_stmt->getIdentifier(), read_stmt->getIdentifier()));
}

TEST_CASE("PKB::NextManager::setIfExit Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	shared_ptr<StmtInfo> if_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(2, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(3, StmtType::Read);
	shared_ptr<StmtInfo> if_stmt_2 = TestUtilities::createStmtInfo(4, StmtType::IfStmt);
	cfg.createNode(if_stmt_1);
	cfg.createNode(if_stmt_2);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);

	CHECK_THROWS(next_manager.setIfExit(print_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt_2->getIdentifier()));
	CHECK_THROWS(next_manager.setIfExit(read_stmt->getIdentifier(), print_stmt->getIdentifier(), if_stmt_1->getIdentifier()));

	CHECK_NOTHROW(next_manager.setIfExit(print_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt_1->getIdentifier()));
}

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
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt = TestUtilities::createStmtInfo(5, StmtType::Assign);

	cfg.createNode(if_stmt);
	cfg.createNode(while_stmt);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);
	cfg.createNode(assign_stmt);

	next_manager.setIfNext(if_stmt->getIdentifier(), while_stmt->getIdentifier(), read_stmt->getIdentifier());
	next_manager.setIfExit(while_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt->getIdentifier());
	next_manager.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier());
	next_manager.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier());
	next_manager.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier());

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
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt = TestUtilities::createStmtInfo(5, StmtType::Assign);

	cfg.createNode(if_stmt);
	cfg.createNode(while_stmt);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);
	cfg.createNode(assign_stmt);

	next_manager.setIfNext(if_stmt->getIdentifier(), while_stmt->getIdentifier(), read_stmt->getIdentifier());
	next_manager.setIfExit(while_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt->getIdentifier());
	next_manager.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier());
	next_manager.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier());
	next_manager.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier());

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
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt = TestUtilities::createStmtInfo(5, StmtType::Assign);

	cfg.createNode(if_stmt);
	cfg.createNode(while_stmt);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);
	cfg.createNode(assign_stmt);

	next_manager.setIfNext(if_stmt->getIdentifier(), while_stmt->getIdentifier(), read_stmt->getIdentifier());
	next_manager.setIfExit(while_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt->getIdentifier());
	next_manager.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier());
	next_manager.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier());
	next_manager.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier());

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
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);

	shared_ptr<StmtInfo> if_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> if_stmt_2 = TestUtilities::createStmtInfo(2, StmtType::IfStmt);
	shared_ptr<StmtInfo> print_stmt_3 = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> if_stmt_4 = TestUtilities::createStmtInfo(4, StmtType::IfStmt);
	shared_ptr<StmtInfo> read_stmt_5 = TestUtilities::createStmtInfo(5, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt_6 = TestUtilities::createStmtInfo(6, StmtType::Assign);
	shared_ptr<StmtInfo> if_stmt_7 = TestUtilities::createStmtInfo(7, StmtType::IfStmt);
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

	next_manager.setIfNext(if_stmt_1->getIdentifier(), if_stmt_2->getIdentifier(), if_stmt_7->getIdentifier());
	next_manager.setIfNext(if_stmt_2->getIdentifier(), print_stmt_3->getIdentifier(), if_stmt_4->getIdentifier());
	next_manager.setIfNext(if_stmt_4->getIdentifier(), read_stmt_5->getIdentifier(), assign_stmt_6->getIdentifier());
	next_manager.setIfNext(if_stmt_7->getIdentifier(), assign_stmt_8->getIdentifier(), assign_stmt_9->getIdentifier());
	next_manager.setIfExit(assign_stmt_8->getIdentifier(), assign_stmt_9->getIdentifier(), if_stmt_7->getIdentifier());
	next_manager.setIfExit(read_stmt_5->getIdentifier(), assign_stmt_6->getIdentifier(), if_stmt_4->getIdentifier());
	next_manager.setIfExit(print_stmt_3->getIdentifier(), if_stmt_4->getIdentifier(), if_stmt_2->getIdentifier());
	next_manager.setIfExit(if_stmt_2->getIdentifier(), if_stmt_7->getIdentifier(), if_stmt_1->getIdentifier());
	next_manager.setNext(if_stmt_1->getIdentifier(), call_stmt_10->getIdentifier());

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
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
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

	next_manager.setIfNext(if_stmt->getIdentifier(), while_stmt->getIdentifier(), read_stmt->getIdentifier());
	next_manager.setIfExit(while_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt->getIdentifier());
	next_manager.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier());
	next_manager.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier());
	next_manager.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier());
	next_manager.setNext(assign_stmt->getIdentifier(), call_stmt->getIdentifier());
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
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	PKB::NextManager next_manager = PKB::NextManager(cfg);
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
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

	next_manager.setIfNext(if_stmt->getIdentifier(), while_stmt->getIdentifier(), read_stmt->getIdentifier());
	next_manager.setIfExit(while_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt->getIdentifier());
	next_manager.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier());
	next_manager.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier());
	next_manager.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier());
	next_manager.setNext(assign_stmt->getIdentifier(), call_stmt->getIdentifier());
	cfg.optimize();

	CHECK(next_manager.getPreviousStar(1) == StmtInfoPtrSet{});
	CHECK(next_manager.getPreviousStar(2) == unordered_set{while_stmt, if_stmt, print_stmt});
	CHECK(next_manager.getPreviousStar(3) == unordered_set{if_stmt, while_stmt, print_stmt});
	CHECK(next_manager.getPreviousStar(4) == unordered_set{if_stmt});
	CHECK(next_manager.getPreviousStar(5) == unordered_set{if_stmt, while_stmt, read_stmt, print_stmt});
	CHECK(next_manager.getPreviousStar(6) == unordered_set{if_stmt, while_stmt, read_stmt, print_stmt, assign_stmt});
}