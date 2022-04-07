#ifndef SPA_SRC_PKB_ASSIGNSTORE_H
#define SPA_SRC_PKB_ASSIGNSTORE_H

#include <cassert>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/Hash.h"
#include "Common/Hash.h"
#include "Common/TypeDefs.h"
#include "PKB/PKB.h"

class PKB::AssignStore {
public:
	struct AssignRelation {
		shared_ptr<StmtInfo> node;          // NOLINT(misc-non-private-member-variables-in-classes)
		VarRef variable;                    // NOLINT(misc-non-private-member-variables-in-classes)
		Common::EP::Expression expression;  // NOLINT(misc-non-private-member-variables-in-classes)
		inline bool operator==(const AssignRelation& other) const {
			return node == other.node && variable == other.variable && expression == other.expression;
		}

		friend hash<AssignRelation>;
	};

	AssignStore();
	void setAssign(const shared_ptr<StmtInfo>& statement, const VarRef& variable, const Common::EP::Expression& expression);
	bool patternExists(const VarRef& variable, const Common::EP::Expression& expression, bool is_exact_match);
	StmtInfoPtrSet getStmtsWithPattern(const VarRef& variable, const Common::EP::Expression& expression, bool is_exact_match);
	StmtInfoPtrSet getStmtsWithPatternLHS(const VarRef& var_name);
	StmtInfoPtrVarRefSet getStmtsWithPatternRHS(const Common::EP::Expression& expression, bool is_exact_match);
	unordered_map<VarRef, unordered_set<AssignRelation>> getAssignMap();
	void clear();

private:
	static bool compareExpressions(const Common::EP::Expression& expression, const Common::EP::Expression& op_tree, bool is_exact_match);
	unordered_map<VarRef, unordered_set<AssignRelation>> var_to_relation_store;
	unordered_map<Common::EP::Expression, unordered_set<AssignRelation>> exp_to_relation_store;
};

namespace std {
template <>
struct hash<PKB::AssignStore::AssignRelation> {
	inline size_t operator()(const PKB::AssignStore::AssignRelation& assign_relation) const {
		std::size_t seed = 0;
		combineHash(seed, assign_relation.node);
		combineHash(seed, assign_relation.variable);
		combineHash(seed, assign_relation.expression);
		return seed;
	}
};
}

#endif
