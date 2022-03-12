#include "PKB/CFG/Refactor/NodeInterface.h"

#include "../TestUtilities.h"
#include "PKB/CFG/Refactor/DummyNode.h"
#include "PKB/CFG/Refactor/IfNode.h"
#include "PKB/CFG/Refactor/NonConditionalNode.h"
#include "PKB/CFG/Refactor/WhileNode.h"
#include "catch.hpp"

TEST_CASE("PKB::NodeInterface Constructor Test") {
	CHECK_THROWS(PKB::NodeInterface());  // Abstract class cannot be instantiated.
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);

	CHECK_NOTHROW(PKB::IfNode(if_stmt));
	CHECK_THROWS(PKB::IfNode(while_stmt));
	CHECK_THROWS(PKB::IfNode(print_stmt));

	CHECK_NOTHROW(PKB::WhileNode(while_stmt));
	CHECK_THROWS(PKB::WhileNode(if_stmt));
	CHECK_THROWS(PKB::WhileNode(print_stmt));

	CHECK_NOTHROW(PKB::NonConditionalNode(print_stmt));
	CHECK_THROWS(PKB::NonConditionalNode(if_stmt));
	CHECK_THROWS(PKB::NonConditionalNode(while_stmt));

	CHECK_NOTHROW(PKB::DummyNode(1));
	CHECK_NOTHROW(PKB::DummyNode(INT_MAX));
}
/*
TEST_CASE("PKB::NodeInterface::insertNext Test") {

}

TEST_CASE("PKB::NodeInterface::insertPrevious Test") {

}*/