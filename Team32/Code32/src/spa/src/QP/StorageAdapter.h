#ifndef SPA_SRC_QP_STORAGEADAPTER_H
#define SPA_SRC_QP_STORAGEADAPTER_H

#include "PKB/StorageAccessInterface.h"
#include "QP/QP.h"
#include "QP/Types.h"

class QP::StorageAdapter {
public:
	explicit StorageAdapter(PKB::StorageAccessInterface& pkb) : pkb(pkb){};

	[[nodiscard]] StmtInfoPtrSet getStatements() const { return pkb.getStatements(); };
	[[nodiscard]] VarRefSet getVariables() const { return pkb.getVariables(); };
	[[nodiscard]] ConstValSet getConstants() const { return pkb.getConstants(); };
	[[nodiscard]] ProcRefSet getProcedures() const { return pkb.getProcedures(); };
	void resetCache() { pkb.resetCFGCache(); };

	// Follows, Parent, Next, Affects
	template <Types::ClauseType T>
	[[nodiscard]] inline bool checkStatementRelation(StmtRef lhs, StmtRef rhs) const = delete;
	template <>
	[[nodiscard]] inline bool checkStatementRelation<Types::ClauseType::Parent>(StmtRef lhs, StmtRef rhs) const {
		return pkb.checkParent(lhs, rhs);
	};
	template <>
	[[nodiscard]] inline bool checkStatementRelation<Types::ClauseType::ParentT>(StmtRef lhs, StmtRef rhs) const {
		return pkb.checkParentStar(lhs, rhs);
	};
	template <>
	[[nodiscard]] inline bool checkStatementRelation<Types::ClauseType::Follows>(StmtRef lhs, StmtRef rhs) const {
		return pkb.checkFollows(lhs, rhs);
	};
	template <>
	[[nodiscard]] inline bool checkStatementRelation<Types::ClauseType::FollowsT>(StmtRef lhs, StmtRef rhs) const {
		return pkb.checkFollowsStar(lhs, rhs);
	};
	template <>
	[[nodiscard]] inline bool checkStatementRelation<Types::ClauseType::Next>(StmtRef lhs, StmtRef rhs) const {
		return pkb.checkNext(lhs, rhs);
	};
	template <>
	[[nodiscard]] inline bool checkStatementRelation<Types::ClauseType::NextT>(StmtRef lhs, StmtRef rhs) const {
		return pkb.checkNextStar(lhs, rhs);
	};
	template <>
	[[nodiscard]] inline bool checkStatementRelation<Types::ClauseType::Affects>(StmtRef lhs, StmtRef rhs) const {
		return pkb.checkAffects(lhs, rhs);
	};
	template <>
	[[nodiscard]] inline bool checkStatementRelation<Types::ClauseType::AffectsT>(StmtRef lhs, StmtRef rhs) const {
		return pkb.checkAffectsStar(lhs, rhs);
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
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getForwardStatements<Types::ClauseType::Next>(StmtRef index) const {
		return pkb.getPrevious(index);
	};
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getForwardStatements<Types::ClauseType::NextT>(StmtRef index) const {
		return pkb.getPreviousStar(index);
	};
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getForwardStatements<Types::ClauseType::Affects>(StmtRef index) const {
		return pkb.getAffected(index);
	};
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getForwardStatements<Types::ClauseType::AffectsT>(StmtRef index) const {
		return pkb.getAffectedStar(index);
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
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getReverseStatements<Types::ClauseType::Next>(StmtRef index) const {
		return pkb.getNext(index);
	};
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getReverseStatements<Types::ClauseType::NextT>(StmtRef index) const {
		return pkb.getNextStar(index);
	};
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getReverseStatements<Types::ClauseType::Affects>(StmtRef index) const {
		return pkb.getAffects(index);
	};
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getReverseStatements<Types::ClauseType::AffectsT>(StmtRef index) const {
		return pkb.getAffectsStar(index);
	};

	// Calls
	template <Types::ClauseType T>
	[[nodiscard]] inline bool checkProcedureRelation(const ProcRef& lhs, const ProcRef& rhs) const = delete;
	template <>
	[[nodiscard]] inline bool checkProcedureRelation<Types::ClauseType::Calls>(const ProcRef& lhs, const ProcRef& rhs) const {
		return pkb.checkCalls(lhs, rhs);
	}
	template <>
	[[nodiscard]] inline bool checkProcedureRelation<Types::ClauseType::CallsT>(const ProcRef& lhs, const ProcRef& rhs) const {
		return pkb.checkCallsStar(lhs, rhs);
	}

	template <Types::ClauseType T>
	[[nodiscard]] inline ProcRefSet getForwardProcedures(const ProcRef& procedure) const = delete;
	template <>
	[[nodiscard]] inline ProcRefSet getForwardProcedures<Types::ClauseType::Calls>(const ProcRef& procedure) const {
		return pkb.getCaller(procedure);
	}
	template <>
	[[nodiscard]] inline ProcRefSet getForwardProcedures<Types::ClauseType::CallsT>(const ProcRef& procedure) const {
		return pkb.getCallerStar(procedure);
	}

	template <Types::ClauseType T>
	[[nodiscard]] inline ProcRefSet getReverseProcedures(const ProcRef& procedure) const = delete;
	template <>
	[[nodiscard]] inline ProcRefSet getReverseProcedures<Types::ClauseType::Calls>(const ProcRef& procedure) const {
		return pkb.getCallee(procedure);
	}
	template <>
	[[nodiscard]] inline ProcRefSet getReverseProcedures<Types::ClauseType::CallsT>(const ProcRef& procedure) const {
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

	// Pattern Assign
	[[nodiscard]] bool patternExists(const VarRef& name, const Common::EP::Expression& expression, bool is_exact_match) const {
		return pkb.patternExists(name, expression, is_exact_match);
	};
	[[nodiscard]] StmtInfoPtrSet getStmtsWithPattern(const VarRef& name, const Common::EP::Expression& expression,
	                                                 bool is_exact_match) const {
		return pkb.getStmtsWithPattern(name, expression, is_exact_match);
	};
	[[nodiscard]] StmtInfoPtrSet getStmtsWithPatternLHS(const VarRef& name) const { return pkb.getStmtsWithPatternLHS(name); };
	[[nodiscard]] StmtInfoPtrVarRefSet getStmtsWithPatternRHS(const Common::EP::Expression& expression, bool is_exact_match) const {
		return pkb.getStmtsWithPatternRHS(expression, is_exact_match);
	}

	// Pattern If and While
	template <Types::ClauseType T>
	[[nodiscard]] inline bool checkControl(StmtRef stmt, const VarRef& var) const = delete;
	template <>
	[[nodiscard]] inline bool checkControl<Types::ClauseType::PatternIf>(StmtRef stmt, const VarRef& var) const {
		return pkb.checkIfControl(stmt, var);
	}
	template <>
	[[nodiscard]] inline bool checkControl<Types::ClauseType::PatternWhile>(StmtRef stmt, const VarRef& var) const {
		return pkb.checkWhileControl(stmt, var);
	}
	template <Types::ClauseType T>
	[[nodiscard]] inline VarRefSet getControlVar(StmtRef stmt) const = delete;
	template <>
	[[nodiscard]] inline VarRefSet getControlVar<Types::ClauseType::PatternIf>(StmtRef stmt) const {
		return pkb.getIfControlVar(stmt);
	}
	template <>
	[[nodiscard]] inline VarRefSet getControlVar<Types::ClauseType::PatternWhile>(StmtRef stmt) const {
		return pkb.getWhileControlVar(stmt);
	}
	template <Types::ClauseType T>
	[[nodiscard]] inline StmtInfoPtrSet getControlStmt(const VarRef& var) const = delete;
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getControlStmt<Types::ClauseType::PatternIf>(const VarRef& var) const {
		return pkb.getIfControlStmt(var);
	}
	template <>
	[[nodiscard]] inline StmtInfoPtrSet getControlStmt<Types::ClauseType::PatternWhile>(const VarRef& var) const {
		return pkb.getWhileControlStmt(var);
	}

	// With
	[[nodiscard]] inline ProcRef getCalledProcedure(StmtRef stmt) const { return pkb.getCalledProcedure(stmt); };

private:
	PKB::StorageAccessInterface& pkb;
};

#endif  // SPA_SRC_QP_STORAGEADAPTER_H
