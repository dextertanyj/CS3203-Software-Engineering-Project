#include "PKB/PKB.cpp"

// Generate PKB with sample statement type mappings
PKB generateFollowTestPKB() {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setStmtType(INT32_MAX, StmtType::Read);
	return pkb;
}