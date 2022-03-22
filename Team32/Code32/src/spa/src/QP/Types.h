#ifndef SPA_SRC_QP_QUERYTYPES_H
#define SPA_SRC_QP_QUERYTYPES_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

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
	Affects,
	AffectsT,
	Calls,
	CallsT,
	Follows,
	FollowsT,
	UnknownModifies,
	ModifiesP,
	ModifiesS,
	Next,
	NextT,
	Parent,
	ParentT,
	UnknownUses,
	UsesS,
	UsesP,
	PatternAssign,
	PatternIf,
	PatternWhile,
};

enum class ReferenceType { StatementIndex, Synonym, Wildcard, Name, ExactExpression, SubExpression, Attribute };
enum class AttributeType { Name, Index, Value };

class ReferenceArgument;
class ConnectedSynonyms;

typedef struct Declaration {
	DesignEntity type;  // NOLINT(misc-non-private-member-variables-in-classes)
	string symbol;      // NOLINT(misc-non-private-member-variables-in-classes)
	bool operator==(const Declaration& other) const { return type == other.type && symbol == other.symbol; }
} Declaration;

typedef struct SynonymAttribute {
	Declaration synonym;      // NOLINT(misc-non-private-member-variables-in-classes)
	AttributeType attribute;  // NOLINT(misc-non-private-member-variables-in-classes)
	bool operator==(const SynonymAttribute& other) const { return synonym == other.synonym && attribute == other.attribute; }
} SynonymAttribute;

typedef struct Clause {
	shared_ptr<Relationship::Relation> relation;
} Clause;

typedef struct Node {
	string declaration_symbol;
	vector<string> adjacent_symbols;
} Node;

typedef function<QP::QueryResult(const QP::StorageAdapter&)> Executor;
typedef variant<Executor, pair<Executor, Executor>> ExecutorSet;
typedef function<ExecutorSet(const vector<ReferenceArgument>&)> ExecutorSetFactory;
typedef pair<ClauseType, ExecutorSetFactory> ExecutorSetFactoryBundle;
typedef variant<ReferenceType, DesignEntity> ArgumentDispatchKey;
typedef pair<ClauseType, ExecutorSet> ExecutorSetBundle;
typedef function<ExecutorSetBundle(const vector<ReferenceArgument>&)> ArgumentDispatcher;
typedef unordered_map<ClauseType, ArgumentDispatcher> ArgumentDispatchMap;

typedef vector<Declaration> DeclarationList;
typedef vector<Clause> ClauseList;

typedef unordered_map<string, vector<string>> ResultTable;
typedef unordered_map<string, string> ResultRow;
}

#endif  // SPA_SRC_QP_QUERYTYPES_H
