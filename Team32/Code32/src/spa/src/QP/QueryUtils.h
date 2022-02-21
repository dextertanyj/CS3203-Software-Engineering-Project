#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYUTILS_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYUTILS_H_

#include <memory>
#include <unordered_map>

#include "Common/TypeDefs.h"
#include "QP/QP.h"
#include "QP/QueryTypes.h"

using namespace QP::Types;

namespace QP::Utilities {
static unordered_map<DesignEntity, StmtType> design_ent_to_stmt_type = {
	{DesignEntity::Read, StmtType::Read},       {DesignEntity::Print, StmtType::Print}, {DesignEntity::Call, StmtType::Call},
	{DesignEntity::While, StmtType::WhileStmt}, {DesignEntity::If, StmtType::IfStmt},   {DesignEntity::Assign, StmtType::Assign},
};

inline bool checkStmtTypeMatch(const shared_ptr<StmtInfo>& stmt, DesignEntity design_entity) {
	if (stmt == nullptr) {
		return false;
	}

	return design_entity == DesignEntity::Stmt || stmt->type == design_ent_to_stmt_type[design_entity];
}
};  // namespace QueryUtils

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYUTILS_H_
