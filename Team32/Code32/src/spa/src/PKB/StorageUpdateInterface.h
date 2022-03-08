#ifndef SPA_UPDATESTORAGEINTERFACE_H
#define SPA_UPDATESTORAGEINTERFACE_H

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
	virtual void setNext(StmtRef previous, StmtRef next) = 0;
	virtual void setLastNode(StmtRef last) = 0;
	virtual void populateComplexRelations() = 0;

	virtual ~StorageUpdateInterface() = default;
};

#endif  // SPA_UPDATESTORAGEINTERFACE_H
