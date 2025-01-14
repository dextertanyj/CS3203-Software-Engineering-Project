#ifndef UNIT_TESTING_SRC_PKB_TESTUTILITIES_H
#define UNIT_TESTING_SRC_PKB_TESTUTILITIES_H

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "PKB/Types.h"

using namespace std;

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
	pkb.setStmtType(2, StmtType::If);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setStmtType(4, StmtType::Print);
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setStmtType(6, StmtType::Read);
	return pkb;
};

inline PKB::Storage generateParentTestPKB() {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::While);
	pkb.setStmtType(2, StmtType::If);
	pkb.setStmtType(3, StmtType::Call);
	pkb.setStmtType(4, StmtType::Print);
	pkb.setStmtType(5, StmtType::If);
	pkb.setStmtType(6, StmtType::Assign);
	pkb.setStmtType(7, StmtType::If);
	pkb.setStmtType(8, StmtType::If);
	pkb.setStmtType(9, StmtType::Read);
	return pkb;
};

inline PKB::Storage generateUsesTestPKB() {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::If);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setStmtType(4, StmtType::Call);
	pkb.setStmtType(5, StmtType::Call);
	pkb.setStmtType(6, StmtType::While);
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

inline PKB::Storage generateCallsTestPKB() {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Call);
	pkb.setStmtType(2, StmtType::Call);
	pkb.setStmtType(3, StmtType::Call);
	pkb.setStmtType(4, StmtType::Call);
	pkb.setStmtType(5, StmtType::Read);
	pkb.setStmtType(6, StmtType::Print);
	pkb.setProc("procedure_1", 1, 1);
	pkb.setProc("procedure_2", 2, 2);
	pkb.setProc("procedure_3", 3, 4);
	pkb.setProc("procedure_4", 5, 5);
	pkb.setProc("procedure_5", 6, 6);
	pkb.setCall(1, "procedure_3");
	pkb.setCall(2, "procedure_3");
	pkb.setCall(3, "procedure_4");
	pkb.setCall(4, "procedure_5");
	pkb.populateComplexRelations();
	return pkb;
};

inline PKB::Storage generateNextTestPKB() {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::If);
	pkb.setStmtType(3, StmtType::Assign);
	pkb.setStmtType(4, StmtType::If);
	pkb.setStmtType(5, StmtType::Read);
	pkb.setStmtType(6, StmtType::If);
	pkb.setStmtType(7, StmtType::Read);
	pkb.setStmtType(8, StmtType::Assign);
	pkb.setStmtType(9, StmtType::Print);
	pkb.setStmtType(10, StmtType::If);
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
	statement_store.insert(1, StmtType::While);
	statement_store.insert(2, StmtType::If);
	statement_store.insert(3, StmtType::Assign);
	statement_store.insert(4, StmtType::Call);
	statement_store.insert(5, StmtType::Print);
	statement_store.insert(6, StmtType::Read);
	return statement_store;
};
};  // namespace TestUtilities

#endif  // UNIT_TESTING_SRC_PKB_TESTUTILITIES_H
