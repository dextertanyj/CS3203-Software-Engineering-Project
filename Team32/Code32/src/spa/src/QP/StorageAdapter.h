#ifndef SPA_STORAGEADAPTER_H
#define SPA_STORAGEADAPTER_H

#include "PKB/StorageAccessInterface.h"
#include "QP/QP.h"
#include "QP/QueryTypes.h"

class QP::StorageAdapter {
public:
	explicit StorageAdapter(PKB::StorageAccessInterface& pkb) : pkb(pkb){};

	StmtInfoPtrSet getStatements() const { return pkb.getStatements(); };
	VarRefSet getVariables() const { return pkb.getVariables(); };
	unordered_set<ConstVal> getConstants() const { return pkb.getConstants(); };
	unordered_set<ProcRef> getProcedures() const { return pkb.getProcedures(); };

	// Follows & Parent
	template <Types::ClauseType T>
	inline bool checkStatementRelation(StmtRef lhs, StmtRef rhs) const = delete;
	template <>
	inline bool checkStatementRelation<Types::ClauseType::Parent>(StmtRef lhs, StmtRef rhs) const {
		return pkb.checkParents(lhs, rhs);
	};
	template <>
	inline bool checkStatementRelation<Types::ClauseType::Follows>(StmtRef lhs, StmtRef rhs) const {
		return pkb.checkFollows(lhs, rhs);
	};
	template <Types::ClauseType T>
	inline StmtInfoPtrSet getForwardStatements(StmtRef index) const = delete;
	template <>
	inline StmtInfoPtrSet getForwardStatements<Types::ClauseType::Parent>(StmtRef index) const {
		auto result = pkb.getParent(index);
		if (result == nullptr) {
			return {};
		}
		return {result};
	};
	template <>
	inline StmtInfoPtrSet getForwardStatements<Types::ClauseType::ParentT>(StmtRef index) const {
		return pkb.getParentStar(index);
	};
	template <>
	inline StmtInfoPtrSet getForwardStatements<Types::ClauseType::Follows>(StmtRef index) const {
		auto result = pkb.getPreceding(index);
		if (result == nullptr) {
			return {};
		}
		return {result};
	};
	template <>
	inline StmtInfoPtrSet getForwardStatements<Types::ClauseType::FollowsT>(StmtRef index) const {
		return pkb.getPrecedingStar(index);
	};
	template <Types::ClauseType T>
	inline StmtInfoPtrSet getReverseStatements(StmtRef index) const = delete;
	template <>
	inline StmtInfoPtrSet getReverseStatements<Types::ClauseType::Parent>(StmtRef index) const {
		return pkb.getChildren(index);
	};
	template <>
	inline StmtInfoPtrSet getReverseStatements<Types::ClauseType::ParentT>(StmtRef index) const {
		return pkb.getChildStar(index);
	};
	template <>
	inline StmtInfoPtrSet getReverseStatements<Types::ClauseType::Follows>(StmtRef index) const {
		auto result = pkb.getFollower(index);
		if (result == nullptr) {
			return {};
		}
		return {result};
	};
	template <>
	inline StmtInfoPtrSet getReverseStatements<Types::ClauseType::FollowsT>(StmtRef index) const {
		return pkb.getFollowerStar(index);
	};

	// Calls
	template <Types::ClauseType T>
	inline bool checkProcedureRelation(const ProcRef& lhs, const ProcRef& rhs) const = delete;
	template <>
	inline bool checkProcedureRelation<Types::ClauseType::Call>(const ProcRef& lhs, const ProcRef& rhs) const {
		return pkb.checkCall(lhs, rhs);
	}
	template <Types::ClauseType T>
	inline ProcRefSet getForwardProcedures(const ProcRef& procedure) const = delete;
	template <>
	inline ProcRefSet getForwardProcedures<Types::ClauseType::Call>(const ProcRef& procedure) const {
		return pkb.getCaller(procedure);
	}
	template <>
	inline ProcRefSet getForwardProcedures<Types::ClauseType::CallT>(const ProcRef& procedure) const {
		return pkb.getCallerStar(procedure);
	}
	template <Types::ClauseType T>
	inline ProcRefSet getReverseProcedures(const ProcRef& procedure) const = delete;
	template <>
	inline ProcRefSet getReverseProcedures<Types::ClauseType::Call>(const ProcRef& procedure) const {
		return pkb.getCallee(procedure);
	}
	template <>
	inline ProcRefSet getReverseProcedures<Types::ClauseType::CallT>(const ProcRef& procedure) const {
		return pkb.getCalleeStar(procedure);
	}

	// Modifies & Uses (Statement)
	template <Types::ClauseType T>
	inline bool checkStatementVariableRelation(StmtRef index, const VarRef& variable) const = delete;
	template <>
	inline bool checkStatementVariableRelation<Types::ClauseType::ModifiesS>(StmtRef index, const VarRef& variable) const {
		return pkb.checkModifies(index, variable);
	};
	template <>
	inline bool checkStatementVariableRelation<Types::ClauseType::UsesS>(StmtRef index, const VarRef& variable) const {
		return pkb.checkUses(index, variable);
	};

	template <Types::ClauseType T>
	inline StmtInfoPtrSet getStatementByVariable(const VarRef& variable) const = delete;
	template <>
	inline StmtInfoPtrSet getStatementByVariable<Types::ClauseType::ModifiesS>(const VarRef& variable) const {
		return pkb.getStmtModifiesByVar(variable);
	};
	template <>
	inline StmtInfoPtrSet getStatementByVariable<Types::ClauseType::UsesS>(const VarRef& variable) const {
		return pkb.getStmtUsesByVar(variable);
	};

	template <Types::ClauseType T>
	inline VarRefSet getVariableByStatement(StmtRef index) const = delete;
	template <>
	inline VarRefSet getVariableByStatement<Types::ClauseType::ModifiesS>(StmtRef index) const {
		return pkb.getModifiesByStmt(index);
	};
	template <>
	inline VarRefSet getVariableByStatement<Types::ClauseType::UsesS>(StmtRef index) const {
		return pkb.getUsesByStmt(index);
	};

	// Modifies & Uses (Procedure)
	template <Types::ClauseType T>
	inline bool checkProcedureVariableRelation(const ProcRef& procedure, const VarRef& variable) const = delete;
	template <>
	inline bool checkProcedureVariableRelation<Types::ClauseType::ModifiesP>(const ProcRef& procedure, const VarRef& variable) const {
		return pkb.checkModifies(procedure, variable);
	};
	template <>
	inline bool checkProcedureVariableRelation<Types::ClauseType::UsesP>(const ProcRef& procedure, const VarRef& variable) const {
		return pkb.checkUses(procedure, variable);
	};

	template <Types::ClauseType T>
	inline ProcRefSet getProcedureByVariable(const VarRef& variable) const = delete;
	template <>
	inline ProcRefSet getProcedureByVariable<Types::ClauseType::ModifiesP>(const VarRef& variable) const {
		return pkb.getProcModifiesByVar(variable);
	};
	template <>
	inline ProcRefSet getProcedureByVariable<Types::ClauseType::UsesP>(const VarRef& variable) const {
		return pkb.getProcUsesByVar(variable);
	};

	template <Types::ClauseType T>
	inline VarRefSet getVariableByProcedure(const ProcRef& procedure) const = delete;
	template <>
	inline VarRefSet getVariableByProcedure<Types::ClauseType::ModifiesP>(const ProcRef& procedure) const {
		return pkb.getModifiesByProc(procedure);
	};
	template <>
	inline VarRefSet getVariableByProcedure<Types::ClauseType::UsesP>(const ProcRef& procedure) const {
		return pkb.getUsesByProc(procedure);
	};

	bool patternExists(const VarRef& name, const Common::ExpressionProcessor::Expression& expression, bool is_exact_match) const {
		return pkb.patternExists(name, expression, is_exact_match);
	};
	StmtInfoPtrSet getStmtsWithPattern(const VarRef& name, const Common::ExpressionProcessor::Expression& expression,
	                                   bool is_exact_match) const {
		return pkb.getStmtsWithPattern(name, expression, is_exact_match);
	};
	StmtInfoPtrSet getStmtsWithPatternLHS(const VarRef& name) const { return pkb.getStmtsWithPatternLHS(name); };
	vector<pair<shared_ptr<StmtInfo>, VarRef>> getStmtsWithPatternRHS(const Common::ExpressionProcessor::Expression& expression,
	                                                                  bool is_exact_match) const {
		return pkb.getStmtsWithPatternRHS(expression, is_exact_match);
	}

private:
	PKB::StorageAccessInterface& pkb;
};

#endif  // SPA_STORAGEADAPTER_H
