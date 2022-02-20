#include "MockUtilities.h"

shared_ptr<StmtInfo> MockUtilities::createStmtInfo(StmtRef stmtNo, StmtType type) {
    StmtInfo s = {stmtNo, type};
    shared_ptr<StmtInfo> stmt_info = make_shared<StmtInfo>(s);
    return stmt_info;
}

PKB MockUtilities::generateFollowsTestPKB() {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setStmtType(4, StmtType::Print);
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setStmtType(6, StmtType::Read);
	return pkb;
}

PKB MockUtilities::generateParentTestPKB() {
	PKB pkb = PKB();
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

PKB MockUtilities::generateUsesTestPKB() {
    PKB pkb = PKB();
    pkb.setStmtType(1, StmtType::Assign);
    pkb.setStmtType(2, StmtType::IfStmt);
    pkb.setStmtType(3, StmtType::Print);
    pkb.setStmtType(SIZE_MAX, StmtType::WhileStmt);
    return pkb;
}

PKB MockUtilities::generateModifyTestPKB() {
    PKB pkb = PKB();
    pkb.setStmtType(1, StmtType::Assign);
    pkb.setStmtType(2, StmtType::Read);
    pkb.setStmtType(3, StmtType::Call);
    pkb.setStmtType(SIZE_MAX, StmtType::Assign);
    return pkb;
}

StatementStore MockUtilities::generateStatementStore() {
    StatementStore statement_store = StatementStore();
    statement_store.insert(1, StmtType::WhileStmt);
    statement_store.insert(2, StmtType::IfStmt);
    statement_store.insert(3, StmtType::Assign);
    statement_store.insert(4, StmtType::Call);
    statement_store.insert(5, StmtType::Print);
    statement_store.insert(6, StmtType::Read);
    return statement_store;
}
