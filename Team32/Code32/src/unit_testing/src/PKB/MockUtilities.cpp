#include "MockUtilities.h"

shared_ptr<StmtInfo> MockUtilities::createStmtInfo(StmtRef stmt_no, StmtType type) {
	StmtInfo s = {stmt_no, type};
	shared_ptr<StmtInfo> stmt_info = make_shared<StmtInfo>(s);
	return stmt_info;
}

PKB::Storage MockUtilities::generateFollowsTestPKB() {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setStmtType(4, StmtType::Print);
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setStmtType(6, StmtType::Read);
	return pkb;
}

PKB::Storage MockUtilities::generateParentTestPKB() {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::WhileStmt);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Call);
	pkb.setStmtType(4, StmtType::Print);
	pkb.setStmtType(5, StmtType::IfStmt);
	pkb.setStmtType(6, StmtType::Assign);
	pkb.setStmtType(7, StmtType::IfStmt);
	pkb.setStmtType(8, StmtType::IfStmt);
	pkb.setStmtType(9, StmtType::Read);
	return pkb;
}

PKB::Storage MockUtilities::generateUsesTestPKB() {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setStmtType(SIZE_MAX, StmtType::WhileStmt);
	return pkb;
}

PKB::Storage MockUtilities::generateModifyTestPKB() {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::Call);
	pkb.setStmtType(SIZE_MAX, StmtType::Assign);
	return pkb;
}

PKB::StatementStore MockUtilities::generateStatementStore() {
	PKB::StatementStore statement_store = PKB::StatementStore();
	statement_store.insert(1, StmtType::WhileStmt);
	statement_store.insert(2, StmtType::IfStmt);
	statement_store.insert(3, StmtType::Assign);
	statement_store.insert(4, StmtType::Call);
	statement_store.insert(5, StmtType::Print);
	statement_store.insert(6, StmtType::Read);
	return statement_store;
}
