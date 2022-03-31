#ifndef SPA_SRC_PKB_STORAGEUPDATEINTERFACE_H
#define SPA_SRC_PKB_STORAGEUPDATEINTERFACE_H

#include "PKB/PKB.h"

class PKB::StorageUpdateInterface {
public:
	virtual void setProc(ProcRef procedure, StmtRef start, StmtRef end) = 0;
	virtual void setCall(StmtRef index, ProcRef name) = 0;
	virtual void setFollows(StmtRef front, StmtRef back) = 0;
	virtual void setParent(StmtRef parent, StmtRef child) = 0;
	virtual void setStmtType(StmtRef index, StmtType type) = 0;
	virtual void setConstant(ConstVal value) = 0;
	virtual void setConstant(const unordered_set<ConstVal>& values) = 0;
	virtual void setUses(StmtRef index, VarRef name) = 0;
	virtual void setModifies(StmtRef index, VarRef name) = 0;
	virtual void setUses(StmtRef index, VarRefSet names) = 0;
	virtual void setModifies(StmtRef index, VarRefSet names) = 0;
	virtual void setAssign(StmtRef index, VarRef variable, Common::ExpressionProcessor::Expression expression) = 0;
	virtual void setIfControl(StmtRef index, VarRefSet names) = 0;
	virtual void setWhileControl(StmtRef index, VarRefSet names) = 0;
	virtual void setIfControl(StmtRef index, VarRef name) = 0;
	virtual void setWhileControl(StmtRef index, VarRef name) = 0;
	virtual void setNext(StmtRef previous, StmtRef next) = 0;
	virtual void setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next) = 0;
	virtual void setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref) = 0;
	virtual void populateComplexRelations() = 0;
	virtual void resetNextCache() = 0;
	virtual void resetAffectsCache() = 0;

	virtual ~StorageUpdateInterface() = default;
};

#endif  // SPA_SRC_PKB_STORAGEUPDATEINTERFACE_H
