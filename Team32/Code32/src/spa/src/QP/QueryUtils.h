#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYUTILS_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYUTILS_H_

#include <unordered_map>

#include "QP/QueryTypeDefs.h"
#include "Common/TypeDefs.h"

namespace QueryUtils {
  static unordered_map<DesignEntity, StmtType> designEntToStmtType = {
		{ DesignEntity::read, StmtType::Read },
		{ DesignEntity::print, StmtType::Print },
		{ DesignEntity::call, StmtType::Call },
		{ DesignEntity::while_, StmtType::WhileStmt },
		{ DesignEntity::if_, StmtType::IfStmt },
		{ DesignEntity::assign, StmtType::Assign },
	};
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYUTILS_H_
