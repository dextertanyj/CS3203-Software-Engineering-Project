#ifndef SPA_SRC_QP_QUERYTYPES_H
#define SPA_SRC_QP_QUERYTYPES_H

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <unordered_map>

#include "PKB/PKB.h"
#include "QP/QP.h"
#include "QP/Relationship/Relationship.h"

using namespace std;

namespace QP::Types {
enum class DesignEntity {
	Stmt,
	Read,
	Print,
	Call,
	While,
	If,
	Assign,
	Variable,
	Constant,
	Procedure,
};

enum class ClauseType {
	Call,
	CallT,
	Follows,
	FollowsT,
	UnknownModifies,
	ModifiesP,
	ModifiesS,
	Parent,
	ParentT,
	UnknownUses,
	UsesS,
	UsesP,
	PatternAssign,
	PatternIf,
	PatternWhile,
};

enum class ReferenceType { StatementIndex, Synonym, Wildcard, Name, ExactExpression, SubExpression };

class ReferenceArgument;

typedef struct Declaration {
	DesignEntity type;
	string symbol;
} Declaration;

typedef struct Clause {
	shared_ptr<Relationship::Relation> relation;
} Clause;

typedef struct Node {
	string declaration_symbol;
	vector<string> adjacent_symbols;
} Node;

typedef function<QP::QueryResult(PKB::StorageAccessInterface&)> Executor;
typedef variant<Executor, pair<Executor, Executor>> ExecutorSet;
typedef function<ExecutorSet(vector<ReferenceArgument>)> ExecutorSetFactory;
typedef pair<ClauseType, ExecutorSetFactory> ExecutorSetFactoryBundle;
typedef variant<ReferenceType, DesignEntity> ArgumentDispatchKey;
typedef pair<ClauseType, ExecutorSet> ExecutorSetBundle;
typedef function<ExecutorSetBundle(vector<ReferenceArgument>)> ArgumentDispatcher;
typedef unordered_map<ClauseType, ArgumentDispatcher> ArgumentDispatchMap;

typedef vector<Declaration> DeclarationList;
typedef vector<Clause> ClauseList;
}  // namespace QP::Types

#endif  // SPA_SRC_QP_QUERYTYPES_H
