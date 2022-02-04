#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_

#include "Common/TypeDefs.h"

class QueryResult {
public:
	QueryResult();
	QueryResult(StmtRefList stmtList, VarRefList varList);
	StmtRefList getStmtRefList();
	VarRefList getVarRefList();

	/*
	* Updates the list by finding the intersection between existing list and new list.
	* Returns false if the resulting list is empty.
	*/
	bool updateStmtList(StmtRefList newStmtList);
	bool updateVarList(VarRefList newVarList);

private:
	StmtRefList stmtList;
	VarRefList varList;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYRESULT_H_
