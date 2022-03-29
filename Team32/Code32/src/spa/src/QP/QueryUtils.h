#ifndef SPA_SRC_QP_QUERYUTILS_H
#define SPA_SRC_QP_QUERYUTILS_H

#include <memory>
#include <unordered_map>

#include "Common/TypeDefs.h"
#include "QP/QP.h"
#include "QP/Types.h"

namespace QP::Utilities {
static unordered_map<Types::DesignEntity, StmtType> design_ent_to_stmt_type = {
	{Types::DesignEntity::Read, StmtType::Read}, {Types::DesignEntity::Print, StmtType::Print},
	{Types::DesignEntity::Call, StmtType::Call}, {Types::DesignEntity::While, StmtType::WhileStmt},
	{Types::DesignEntity::If, StmtType::IfStmt}, {Types::DesignEntity::Assign, StmtType::Assign},
};

static unordered_map<Types::ClauseType, size_t> cost_map = {
	{Types::ClauseType::Affects, 30},  {Types::ClauseType::AffectsT, 30},    {Types::ClauseType::Calls, 1},
	{Types::ClauseType::CallsT, 1},    {Types::ClauseType::Follows, 1},      {Types::ClauseType::FollowsT, 1},
	{Types::ClauseType::ModifiesP, 1}, {Types::ClauseType::ModifiesS, 1},    {Types::ClauseType::Next, 10},
	{Types::ClauseType::NextT, 10},    {Types::ClauseType::Parent, 1},       {Types::ClauseType::ParentT, 1},
	{Types::ClauseType::UsesP, 1},     {Types::ClauseType::UsesS, 1},        {Types::ClauseType::PatternAssign, 5},
	{Types::ClauseType::PatternIf, 1}, {Types::ClauseType::PatternWhile, 1}, {Types::ClauseType::With, 3},
};

inline bool checkStmtTypeMatch(const shared_ptr<StmtInfo>& stmt, Types::DesignEntity design_entity) {
	if (stmt == nullptr) {
		return false;
	}

	return design_entity == Types::DesignEntity::Stmt || stmt->getType() == design_ent_to_stmt_type[design_entity];
}
};

#endif  // SPA_SRC_QP_QUERYUTILS_H
