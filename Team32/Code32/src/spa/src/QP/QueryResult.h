#ifndef QUERY_RESULT_H_
#define QUERY_RESULT_H_

#include "Common/TypeDefs.h"

class QueryResult {
public:
	QueryResult();
	QueryResult(StmtInfoList stmtList, VarRefList varList);

	StmtInfoList stmtList;
	VarRefList varList;
};

#endif // QUERY_RESULT_H_
