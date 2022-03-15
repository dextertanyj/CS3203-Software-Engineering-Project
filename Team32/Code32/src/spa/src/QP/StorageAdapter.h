#ifndef SPA_STORAGEADAPTER_H
#define SPA_STORAGEADAPTER_H

#include "PKB/StorageAccessInterface.h"
#include "QP/QP.h"
#include "QP/QueryTypes.h"

class QP::StorageAdapter {
public:
	explicit StorageAdapter(PKB::StorageAccessInterface& pkb) : pkb(pkb){};

	[[nodiscard]] StmtInfoPtrSet getStatements() const { return pkb.getStatements(); };
	[[nodiscard]] VarRefSet getVariables() const { return pkb.getVariables(); };
	[[nodiscard]] unordered_set<ConstVal> getConstants() const { return pkb.getConstants(); };
	[[nodiscard]] unordered_set<ProcRef> getProcedures() const { return pkb.getProcedures(); };

	// Follows & Parent
	template <Types::ClauseType T>
	[[nodiscard]] inline bool checkStatementRelation(StmtRef lhs, StmtRef rhs) const = delete;
	template <>
	[[nodiscard]] inline bool checkStatementRelation<Types::ClauseType::Parent>(StmtRef lhs, StmtRef rhs) const {
		return pkb.checkParents(lhs, rhs);
	};
	template <>
	[[nodiscard]] inline bool checkStatementRelation<Types::ClauseType::Follows>(StmtRef lhs, StmtRef rhs) const {
		return pkb.checkFollows(lhs, rhs);
	};
	template <Types::ClauseType T>
	[[nodiscard]] inline StmtInfoPtrSet getForwardStatements(StmtRef index) const = delete;
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getForwardStatements<Types::ClauseType::Parent>(StmtRef index) const {
		auto result = pkb.getParent(index);
		if (result == nullptr) {
			return {};
		}
		return {result};
	};
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getForwardStatements<Types::ClauseType::ParentT>(StmtRef index) const {
		return pkb.getParentStar(index);
	};
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getForwardStatements<Types::ClauseType::Follows>(StmtRef index) const {
		auto result = pkb.getPreceding(index);
		if (result == nullptr) {
			return {};
		}
		return {result};
	};
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getForwardStatements<Types::ClauseType::FollowsT>(StmtRef index) const {
		return pkb.getPrecedingStar(index);
	};
	template <Types::ClauseType T>
	[[nodiscard]] inline StmtInfoPtrSet getReverseStatements(StmtRef index) const = delete;
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getReverseStatements<Types::ClauseType::Parent>(StmtRef index) const {
		return pkb.getChildren(index);
	};
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getReverseStatements<Types::ClauseType::ParentT>(StmtRef index) const {
		return pkb.getChildStar(index);
	};
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getReverseStatements<Types::ClauseType::Follows>(StmtRef index) const {
		auto result = pkb.getFollower(index);
		if (result == nullptr) {
			return {};
		}
		return {result};
	};
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getReverseStatements<Types::ClauseType::FollowsT>(StmtRef index) const {
		return pkb.getFollowerStar(index);
	};

	// Calls
	template <Types::ClauseType T>
	[[nodiscard]] inline bool checkProcedureRelation(const ProcRef& lhs, const ProcRef& rhs) const = delete;
	template <>
	[[nodiscard]] inline bool checkProcedureRelation<Types::ClauseType::Call>(const ProcRef& lhs, const ProcRef& rhs) const {
		return pkb.checkCall(lhs, rhs);
	}
	template <Types::ClauseType T>
	[[nodiscard]] inline ProcRefSet getForwardProcedures(const ProcRef& procedure) const = delete;
	template <>
	[[nodiscard]] inline ProcRefSet getForwardProcedures<Types::ClauseType::Call>(const ProcRef& procedure) const {
		return pkb.getCaller(procedure);
	}
	template <>
	[[nodiscard]] inline ProcRefSet getForwardProcedures<Types::ClauseType::CallT>(const ProcRef& procedure) const {
		return pkb.getCallerStar(procedure);
	}
	template <Types::ClauseType T>
	[[nodiscard]] inline ProcRefSet getReverseProcedures(const ProcRef& procedure) const = delete;
	template <>
	[[nodiscard]] inline ProcRefSet getReverseProcedures<Types::ClauseType::Call>(const ProcRef& procedure) const {
		return pkb.getCallee(procedure);
	}
	template <>
	[[nodiscard]] inline ProcRefSet getReverseProcedures<Types::ClauseType::CallT>(const ProcRef& procedure) const {
		return pkb.getCalleeStar(procedure);
	}

	// Modifies & Uses (Statement)
	template <Types::ClauseType T>
	[[nodiscard]] inline bool checkStatementVariableRelation(StmtRef index, const VarRef& variable) const = delete;
	template <>
	[[nodiscard]] inline bool checkStatementVariableRelation<Types::ClauseType::ModifiesS>(StmtRef index, const VarRef& variable) const {
		return pkb.checkModifies(index, variable);
	};
	template <>
	[[nodiscard]] inline bool checkStatementVariableRelation<Types::ClauseType::UsesS>(StmtRef index, const VarRef& variable) const {
		return pkb.checkUses(index, variable);
	};

	template <Types::ClauseType T>
	[[nodiscard]] inline StmtInfoPtrSet getStatementByVariable(const VarRef& variable) const = delete;
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getStatementByVariable<Types::ClauseType::ModifiesS>(const VarRef& variable) const {
		return pkb.getStmtModifiesByVar(variable);
	};
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getStatementByVariable<Types::ClauseType::UsesS>(const VarRef& variable) const {
		return pkb.getStmtUsesByVar(variable);
	};

	template <Types::ClauseType T>
	[[nodiscard]] inline VarRefSet getVariableByStatement(StmtRef index) const = delete;
	template <>
	[[nodiscard]] inline VarRefSet getVariableByStatement<Types::ClauseType::ModifiesS>(StmtRef index) const {
		return pkb.getModifiesByStmt(index);
	};
	template <>
	[[nodiscard]] inline VarRefSet getVariableByStatement<Types::ClauseType::UsesS>(StmtRef index) const {
		return pkb.getUsesByStmt(index);
	};

	// Modifies & Uses (Procedure)
	template <Types::ClauseType T>
	[[nodiscard]] inline bool checkProcedureVariableRelation(const ProcRef& procedure, const VarRef& variable) const = delete;
	template <>
	[[nodiscard]] inline bool checkProcedureVariableRelation<Types::ClauseType::ModifiesP>(const ProcRef& procedure,
	                                                                                       const VarRef& variable) const {
		return pkb.checkModifies(procedure, variable);
	};
	template <>
	[[nodiscard]] inline bool checkProcedureVariableRelation<Types::ClauseType::UsesP>(const ProcRef& procedure,
	                                                                                   const VarRef& variable) const {
		return pkb.checkUses(procedure, variable);
	};

	template <Types::ClauseType T>
	[[nodiscard]] inline ProcRefSet getProcedureByVariable(const VarRef& variable) const = delete;
	template <>
	[[nodiscard]] inline ProcRefSet getProcedureByVariable<Types::ClauseType::ModifiesP>(const VarRef& variable) const {
		return pkb.getProcModifiesByVar(variable);
	};
	template <>
	[[nodiscard]] inline ProcRefSet getProcedureByVariable<Types::ClauseType::UsesP>(const VarRef& variable) const {
		return pkb.getProcUsesByVar(variable);
	};

	template <Types::ClauseType T>
	[[nodiscard]] inline VarRefSet getVariableByProcedure(const ProcRef& procedure) const = delete;
	template <>
	[[nodiscard]] inline VarRefSet getVariableByProcedure<Types::ClauseType::ModifiesP>(const ProcRef& procedure) const {
		return pkb.getModifiesByProc(procedure);
	};
	template <>
	[[nodiscard]] inline VarRefSet getVariableByProcedure<Types::ClauseType::UsesP>(const ProcRef& procedure) const {
		return pkb.getUsesByProc(procedure);
	};

	[[nodiscard]] bool patternExists(const VarRef& name, const Common::ExpressionProcessor::Expression& expression,
	                                 bool is_exact_match) const {
		return pkb.patternExists(name, expression, is_exact_match);
	};
	[[nodiscard]] StmtInfoPtrSet getStmtsWithPattern(const VarRef& name, const Common::ExpressionProcessor::Expression& expression,
	                                                 bool is_exact_match) const {
		return pkb.getStmtsWithPattern(name, expression, is_exact_match);
	};
	[[nodiscard]] StmtInfoPtrSet getStmtsWithPatternLHS(const VarRef& name) const { return pkb.getStmtsWithPatternLHS(name); };
	[[nodiscard]] vector<pair<shared_ptr<StmtInfo>, VarRef>> getStmtsWithPatternRHS(
		const Common::ExpressionProcessor::Expression& expression, bool is_exact_match) const {
		return pkb.getStmtsWithPatternRHS(expression, is_exact_match);
	}

private:
	PKB::StorageAccessInterface& pkb;
};

#endif  // SPA_STORAGEADAPTER_H