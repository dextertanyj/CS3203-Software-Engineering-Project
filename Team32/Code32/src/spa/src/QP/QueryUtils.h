#ifndef SPA_SRC_QP_QUERYUTILS_H
#define SPA_SRC_QP_QUERYUTILS_H

#include <memory>
#include <unordered_map>

#include "Common/TypeDefs.h"
#include "QP/QP.h"
#include "QP/QueryTypes.h"

namespace QP::Utilities {
static unordered_map<Types::DesignEntity, StmtType> design_ent_to_stmt_type = {
	{Types::DesignEntity::Read, StmtType::Read}, {Types::DesignEntity::Print, StmtType::Print},
	{Types::DesignEntity::Call, StmtType::Call}, {Types::DesignEntity::While, StmtType::WhileStmt},
	{Types::DesignEntity::If, StmtType::IfStmt}, {Types::DesignEntity::Assign, StmtType::Assign},
};

inline bool checkStmtTypeMatch(const shared_ptr<StmtInfo>& stmt, Types::DesignEntity design_entity) {
	if (stmt == nullptr) {
		return false;
	}

	return design_entity == Types::DesignEntity::Stmt || stmt->getType() == design_ent_to_stmt_type[design_entity];
}
};

#endif  // SPA_SRC_QP_QUERYUTILS_H
