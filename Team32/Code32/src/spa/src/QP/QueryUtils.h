#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYUTILS_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYUTILS_H_

#include <unordered_map>

#include "QP/QueryTypeDefs.h"
#include "Common/TypeDefs.h"

class QueryUtils {
public:
	static unordered_map<DesignEntity, StmtType> designEntToStmtType;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYUTILS_H_
