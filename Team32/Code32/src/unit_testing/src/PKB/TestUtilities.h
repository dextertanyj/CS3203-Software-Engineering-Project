#ifndef UNIT_TESTING_SRC_PKB_TESTUTILITIES_H
#define UNIT_TESTING_SRC_PKB_TESTUTILITIES_H

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "PKB/Types.h"

namespace TestUtilities {
inline shared_ptr<StmtInfo> createStmtInfo(StmtRef stmt_no, StmtType type) {
	PKB::Types::StatementStore store;
	store.insert(stmt_no, type);
	return store.get(stmt_no);
}

inline shared_ptr<PKB::ProcedureInfo> createProcedureInfo(ProcRef name) {
	PKB::Types::ProcedureStore store;
	store.insert(name, {});
	return store.get(name);
};

inline PKB::Storage generateFollowsTestPKB() {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setStmtType(4, StmtType::Print);
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setStmtType(6, StmtType::Read);
	return pkb;
};

inline PKB::Storage generateParentTestPKB() {
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
};

inline PKB::Storage generateUsesTestPKB() {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setStmtType(4, StmtType::Call);
	pkb.setStmtType(5, StmtType::Call);
	pkb.setStmtType(6, StmtType::WhileStmt);
	return pkb;
};

inline PKB::Storage generateModifyTestPKB() {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::Call);
	pkb.setStmtType(4, StmtType::Call);
	pkb.setStmtType(5, StmtType::Assign);
	return pkb;
};

inline PKB::Storage generateNextTestPKB() {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Assign);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setStmtType(5, StmtType::Read);
	pkb.setStmtType(6, StmtType::IfStmt);
	pkb.setStmtType(7, StmtType::Read);
	pkb.setStmtType(8, StmtType::Assign);
	pkb.setStmtType(9, StmtType::Print);
	pkb.setStmtType(10, StmtType::IfStmt);
	pkb.setStmtType(11, StmtType::Assign);
	pkb.setStmtType(12, StmtType::Print);
	pkb.setStmtType(13, StmtType::Assign);
	pkb.setStmtType(14, StmtType::Print);
	pkb.setStmtType(15, StmtType::Call);
	pkb.setStmtType(16, StmtType::Call);
	return pkb;
}

inline PKB::Types::StatementStore generateStatementStore() {
	PKB::Types::StatementStore statement_store = PKB::Types::StatementStore();
	statement_store.insert(1, StmtType::WhileStmt);
	statement_store.insert(2, StmtType::IfStmt);
	statement_store.insert(3, StmtType::Assign);
	statement_store.insert(4, StmtType::Call);
	statement_store.insert(5, StmtType::Print);
	statement_store.insert(6, StmtType::Read);
	return statement_store;
};
};  // namespace TestUtilities

#endif  // UNIT_TESTING_SRC_PKB_TESTUTILITIES_H
