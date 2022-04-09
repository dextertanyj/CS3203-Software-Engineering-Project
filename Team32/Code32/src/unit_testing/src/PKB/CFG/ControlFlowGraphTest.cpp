#include "PKB/CFG/ControlFlowGraph.h"

#include "../TestUtilities.h"
#include "PKB/CFG/NodeInterface.h"
#include "catch.hpp"

TEST_CASE("PKB::CFG::ControlFlowGraph::createNode Test") {
	PKB::CFG::ControlFlowGraph cfg = PKB::CFG::ControlFlowGraph();
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	CHECK_NOTHROW(cfg.createNode(if_stmt));
	CHECK_NOTHROW(cfg.createNode(while_stmt));
	CHECK_NOTHROW(cfg.createNode(print_stmt));
}

TEST_CASE("PKB::CFG::ControlFlowGraph::setNext Test") {
	PKB::CFG::ControlFlowGraph cfg = PKB::CFG::ControlFlowGraph();
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

	CHECK_NOTHROW(cfg.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier()));
	CHECK_NOTHROW(cfg.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier()));
	CHECK_NOTHROW(cfg.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier()));
}

TEST_CASE("PKB::CFG::ControlFlowGraph::setIfNext Test") {
	PKB::CFG::ControlFlowGraph cfg = PKB::CFG::ControlFlowGraph();
	shared_ptr<StmtInfo> if_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(2, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(3, StmtType::Read);
	shared_ptr<StmtInfo> if_stmt_2 = TestUtilities::createStmtInfo(4, StmtType::If);
	cfg.createNode(if_stmt_1);
	cfg.createNode(if_stmt_2);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);

	CHECK_NOTHROW(cfg.setIfNext(if_stmt_1->getIdentifier(), print_stmt->getIdentifier(), read_stmt->getIdentifier()));
}

TEST_CASE("PKB::CFG::ControlFlowGraph::setIfExit Test") {
	PKB::CFG::ControlFlowGraph cfg = PKB::CFG::ControlFlowGraph();
	shared_ptr<StmtInfo> if_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(2, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(3, StmtType::Read);
	shared_ptr<StmtInfo> if_stmt_2 = TestUtilities::createStmtInfo(4, StmtType::If);
	cfg.createNode(if_stmt_1);
	cfg.createNode(if_stmt_2);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);

	CHECK_NOTHROW(cfg.setIfExit(print_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt_1->getIdentifier()));
}

TEST_CASE("PKB""ControlFlowGrpah::getNextNodes Test") {
	PKB::CFG::ControlFlowGraph cfg = PKB::CFG::ControlFlowGraph();
	shared_ptr<StmtInfo> if_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(2, StmtType::Print);
	shared_ptr<StmtInfo> if_stmt_2 = TestUtilities::createStmtInfo(3, StmtType::If);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt = TestUtilities::createStmtInfo(5, StmtType::Assign);
	shared_ptr<StmtInfo> call_stmt = TestUtilities::createStmtInfo(6, StmtType::Call);
	cfg.createNode(if_stmt_1);
	cfg.createNode(print_stmt);
	cfg.createNode(if_stmt_2);
	cfg.createNode(read_stmt);
	cfg.createNode(assign_stmt);
	cfg.createNode(call_stmt);
	cfg.setIfNext(if_stmt_1->getIdentifier(), print_stmt->getIdentifier(), if_stmt_2->getIdentifier());
	cfg.setIfNext(if_stmt_2->getIdentifier(), read_stmt->getIdentifier(), assign_stmt->getIdentifier());
	cfg.setIfExit(read_stmt->getIdentifier(), assign_stmt->getIdentifier(), if_stmt_2->getIdentifier());
	cfg.setIfExit(print_stmt->getIdentifier(), if_stmt_2->getIdentifier(), if_stmt_1->getIdentifier());
	cfg.setNext(if_stmt_1->getIdentifier(), call_stmt->getIdentifier());

	CHECK(cfg.getNextNodes(if_stmt_1->getIdentifier()) == unordered_set{print_stmt, if_stmt_2});
	CHECK(cfg.getNextNodes(if_stmt_2->getIdentifier()) == unordered_set{read_stmt, assign_stmt});
	CHECK(cfg.getNextNodes(assign_stmt->getIdentifier()) == unordered_set{call_stmt});
	CHECK(cfg.getNextNodes(print_stmt->getIdentifier()) == unordered_set{call_stmt});
	CHECK(cfg.getNextNodes(read_stmt->getIdentifier()) == unordered_set{call_stmt});
	CHECK(cfg.getNextNodes(call_stmt->getIdentifier()).empty());
}

TEST_CASE("PKB""ControlFlowGrpah::getPreviousNodes Test") {
	PKB::CFG::ControlFlowGraph cfg = PKB::CFG::ControlFlowGraph();
	shared_ptr<StmtInfo> if_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(2, StmtType::Print);
	shared_ptr<StmtInfo> if_stmt_2 = TestUtilities::createStmtInfo(3, StmtType::If);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt = TestUtilities::createStmtInfo(5, StmtType::Assign);
	shared_ptr<StmtInfo> call_stmt = TestUtilities::createStmtInfo(6, StmtType::Call);
	cfg.createNode(if_stmt_1);
	cfg.createNode(print_stmt);
	cfg.createNode(if_stmt_2);
	cfg.createNode(read_stmt);
	cfg.createNode(assign_stmt);
	cfg.createNode(call_stmt);
	cfg.setIfNext(if_stmt_1->getIdentifier(), print_stmt->getIdentifier(), if_stmt_2->getIdentifier());
	cfg.setIfNext(if_stmt_2->getIdentifier(), read_stmt->getIdentifier(), assign_stmt->getIdentifier());
	cfg.setIfExit(read_stmt->getIdentifier(), assign_stmt->getIdentifier(), if_stmt_2->getIdentifier());
	cfg.setIfExit(print_stmt->getIdentifier(), if_stmt_2->getIdentifier(), if_stmt_1->getIdentifier());
	cfg.setNext(if_stmt_1->getIdentifier(), call_stmt->getIdentifier());

	CHECK(cfg.getPreviousNodes(if_stmt_1->getIdentifier()).empty());
	CHECK(cfg.getPreviousNodes(if_stmt_2->getIdentifier()) == unordered_set{if_stmt_1});
	CHECK(cfg.getPreviousNodes(print_stmt->getIdentifier()) == unordered_set{if_stmt_1});
	CHECK(cfg.getPreviousNodes(assign_stmt->getIdentifier()) == unordered_set{if_stmt_2});
	CHECK(cfg.getPreviousNodes(read_stmt->getIdentifier()) == unordered_set{if_stmt_2});
	CHECK(cfg.getPreviousNodes(call_stmt->getIdentifier()) == unordered_set{assign_stmt, read_stmt, print_stmt});
}

TEST_CASE("PKB""ControlFlowGrpah::getLoopNode Test") {
	/**
	 * if (...) then {
	 *   if (...) then {
	 *     ...
	 *   } else {
	 *     ...
	 *   }
	 *   while (...) {
	 *     while (...) {
	 *       ...
	 *     }
	 *   }
	 * } else {
	 *   ...
	 * }
	 * ...
	 */
	PKB::CFG::ControlFlowGraph cfg = PKB::CFG::ControlFlowGraph();
	shared_ptr<StmtInfo> if_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> read_stmt_1 = TestUtilities::createStmtInfo(2, StmtType::Read);
	shared_ptr<StmtInfo> if_stmt_2 = TestUtilities::createStmtInfo(3, StmtType::If);
	shared_ptr<StmtInfo> read_stmt_2 = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> read_stmt_3 = TestUtilities::createStmtInfo(5, StmtType::Read);
	shared_ptr<StmtInfo> while_stmt_1 = TestUtilities::createStmtInfo(6, StmtType::While);
	shared_ptr<StmtInfo> while_stmt_2 = TestUtilities::createStmtInfo(7, StmtType::While);
	shared_ptr<StmtInfo> read_stmt_4 = TestUtilities::createStmtInfo(8, StmtType::Read);
	shared_ptr<StmtInfo> read_stmt_5 = TestUtilities::createStmtInfo(9, StmtType::Read);
	cfg.createNode(if_stmt_1);
	cfg.createNode(if_stmt_2);
	cfg.createNode(while_stmt_1);
	cfg.createNode(while_stmt_2);
	cfg.createNode(read_stmt_1);
	cfg.createNode(read_stmt_2);
	cfg.createNode(read_stmt_3);
	cfg.createNode(read_stmt_4);
	cfg.createNode(read_stmt_5);
	cfg.setIfNext(if_stmt_1->getIdentifier(), read_stmt_1->getIdentifier(), if_stmt_2->getIdentifier());
	cfg.setIfNext(if_stmt_2->getIdentifier(), read_stmt_2->getIdentifier(), read_stmt_3->getIdentifier());
	cfg.setIfExit(read_stmt_2->getIdentifier(), read_stmt_3->getIdentifier(), if_stmt_2->getIdentifier());
	cfg.setNext(if_stmt_2->getIdentifier(), while_stmt_1->getIdentifier());
	cfg.setNext(while_stmt_1->getIdentifier(), while_stmt_2->getIdentifier());
	cfg.setNext(while_stmt_2->getIdentifier(), read_stmt_4->getIdentifier());
	cfg.setNext(read_stmt_4->getIdentifier(), while_stmt_2->getIdentifier());
	cfg.setNext(while_stmt_2->getIdentifier(), while_stmt_1->getIdentifier());
	cfg.setIfExit(read_stmt_1->getIdentifier(), while_stmt_1->getIdentifier(), if_stmt_1->getIdentifier());
	cfg.setNext(if_stmt_1->getIdentifier(), read_stmt_5->getIdentifier());

	SECTION("getLoopExternalNextNodes") {
		CHECK(cfg.getLoopExternalNextNodes(while_stmt_1->getIdentifier()) == unordered_set{read_stmt_5});
		CHECK(cfg.getLoopExternalNextNodes(while_stmt_2->getIdentifier()) == unordered_set{while_stmt_1});
	}

	SECTION("getLoopExternalPreviousNodes") {
		CHECK(cfg.getLoopExternalPreviousNodes(while_stmt_1->getIdentifier()) == unordered_set{read_stmt_2, read_stmt_3});
		CHECK(cfg.getLoopExternalPreviousNodes(while_stmt_2->getIdentifier()) == unordered_set{while_stmt_1});
	}

	SECTION("getLoopInternalNextNodes") {
		CHECK(cfg.getLoopInternalNextNodes(while_stmt_1->getIdentifier()) == unordered_set{while_stmt_2});
		CHECK(cfg.getLoopInternalNextNodes(while_stmt_2->getIdentifier()) == unordered_set{read_stmt_4});
	}

	SECTION("getLoopInternalPreviousNodes") {
		CHECK(cfg.getLoopInternalPreviousNodes(while_stmt_1->getIdentifier()) == unordered_set{while_stmt_2});
		CHECK(cfg.getLoopInternalPreviousNodes(while_stmt_2->getIdentifier()) == unordered_set{read_stmt_4});
	}
}