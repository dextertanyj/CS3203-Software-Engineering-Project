#ifndef SPA_SRC_QP_QUERYUTILS_H
#define SPA_SRC_QP_QUERYUTILS_H

#include <memory>
#include <unordered_map>

#include "Common/TypeDefs.h"
#include "QP/QP.h"
#include "QP/Types.h"

#define UNIT_COST 1
#define AFFECTS_COST 30
#define NEXT_COST 10
#define ASSIGN_COST 5
#define WITH_COST 3

namespace QP::Utilities {
static std::unordered_map<Types::DesignEntity, StmtType> design_ent_to_stmt_type = {
	{Types::DesignEntity::Read, StmtType::Read}, {Types::DesignEntity::Print, StmtType::Print},
	{Types::DesignEntity::Call, StmtType::Call}, {Types::DesignEntity::While, StmtType::While},
	{Types::DesignEntity::If, StmtType::If}, {Types::DesignEntity::Assign, StmtType::Assign},
};

static std::unordered_map<Types::ClauseType, size_t> cost_map = {
	{Types::ClauseType::Affects, AFFECTS_COST},
	{Types::ClauseType::AffectsT, AFFECTS_COST},
	{Types::ClauseType::Calls, UNIT_COST},
	{Types::ClauseType::CallsT, UNIT_COST},
	{Types::ClauseType::Follows, UNIT_COST},
	{Types::ClauseType::FollowsT, UNIT_COST},
	{Types::ClauseType::ModifiesP, UNIT_COST},
	{Types::ClauseType::ModifiesS, UNIT_COST},
	{Types::ClauseType::Next, NEXT_COST},
	{Types::ClauseType::NextT, NEXT_COST},
	{Types::ClauseType::Parent, UNIT_COST},
	{Types::ClauseType::ParentT, UNIT_COST},
	{Types::ClauseType::UsesP, UNIT_COST},
	{Types::ClauseType::UsesS, UNIT_COST},
	{Types::ClauseType::PatternAssign, ASSIGN_COST},
	{Types::ClauseType::PatternIf, UNIT_COST},
	{Types::ClauseType::PatternWhile, UNIT_COST},
	{Types::ClauseType::With, WITH_COST},
};

inline bool checkStmtTypeMatch(const std::shared_ptr<StmtInfo>& stmt, Types::DesignEntity design_entity) {
	if (stmt == nullptr) {
		return false;
	}

	return design_entity == Types::DesignEntity::Stmt || stmt->getType() == design_ent_to_stmt_type.at(design_entity);
}
};

#endif  // SPA_SRC_QP_QUERYUTILS_H
