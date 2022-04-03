#ifndef SPA_SRC_QP_TYPES_H
#define SPA_SRC_QP_TYPES_H

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "Common/Hash.h"
#include "PKB/PKB.h"
#include "QP/QP.h"
#include "QP/Relationship/Relationship.h"

using namespace std;

namespace QP::Types {
enum class DesignEntity { Stmt, Read, Print, Call, While, If, Assign, Variable, Constant, Procedure };

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
	With
};

enum class ReferenceType { StatementIndex, Synonym, Wildcard, Name, ExactExpression, SubExpression, Attribute };
enum class AttributeToken { StatementIndex, ProcedureName, VariableName, Value };
enum class AttributeType { NumberIdentifier, NameIdentifier, ProcedureName, VariableName };

class ReferenceArgument;
class ClauseGroups;

typedef struct Declaration {
	DesignEntity type;  // NOLINT(misc-non-private-member-variables-in-classes)
	string symbol;      // NOLINT(misc-non-private-member-variables-in-classes)
	bool operator==(const Declaration& other) const { return type == other.type && symbol == other.symbol; }
} Declaration;

typedef struct Attribute {
	// We cast 0 rather than use a concrete enum value to prevent favouring a particular value if the enum is updated.
	AttributeType attribute = static_cast<AttributeType>(0);  // NOLINT(misc-non-private-member-variables-in-classes)
	Declaration synonym;                                      // NOLINT(misc-non-private-member-variables-in-classes)
	bool operator==(const Attribute& other) const { return attribute == other.attribute && synonym == other.synonym; }
} Attribute;

typedef variant<monostate, Declaration, Attribute, string, StmtRef, pair<Common::ExpressionProcessor::Expression, bool>> ArgumentValue;

typedef struct Clause {
	shared_ptr<Relationship::Relation> relation;
} Clause;

// Types for attribute selection
typedef std::string Name;
typedef unsigned long long Number;
template <typename TSynonym>
using SelectExecutor = std::function<std::unordered_set<TSynonym>(const QP::StorageAdapter&, const ReferenceArgument&)>;
template <typename TAttribute, typename TSynonym>
using AttributeMapper = std::function<TAttribute(const QP::StorageAdapter&, const TSynonym&)>;

// Types for such-that and pattern clause execution
typedef function<QP::QueryResult(const QP::StorageAdapter&)> Executor;
typedef function<QP::QueryResult(const QP::StorageAdapter&, const QP::QueryResult&)> OptimizedExecutor;
typedef variant<Executor, pair<Executor, Executor>, pair<Executor, OptimizedExecutor>> ExecutorSet;
typedef function<ExecutorSet(const vector<ReferenceArgument>&)> ExecutorSetFactory;
typedef pair<ClauseType, ExecutorSetFactory> ExecutorSetFactoryBundle;
typedef variant<ReferenceType, DesignEntity> ArgumentDispatchKey;
typedef pair<ClauseType, ExecutorSet> ExecutorSetBundle;
typedef function<ExecutorSetBundle(const vector<ReferenceArgument>&)> ArgumentDispatcher;
typedef unordered_map<ClauseType, ArgumentDispatcher> ArgumentDispatchMap;

// Types for with clause execution
template <typename TAttribute, typename TSynonym>
using WithInternalExecutors = std::pair<SelectExecutor<TSynonym>, AttributeMapper<TAttribute, TSynonym>>;
typedef pair<DesignEntity, AttributeType> DispatchAttributeKey;
typedef variant<ReferenceType, DispatchAttributeKey> WithClauseArgumentDispatchKey;
typedef variant<ReferenceType, AttributeType> WithClauseBasicDispatchKey;
template <typename TAttribute, typename TLeft, typename TRight>
using WithExecutorFunction = function<QP::QueryResult(
	const QP::StorageAdapter& store, const QP::Types::ReferenceArgument& lhs, const QP::Types::ReferenceArgument& rhs,
	QP::Types::WithInternalExecutors<TAttribute, TLeft>, QP::Types::WithInternalExecutors<TAttribute, TRight>)>;
template <typename TAttribute, typename TLeft, typename TRight>
using WithExecutorFunctionSet =
	variant<WithExecutorFunction<TAttribute, TLeft, TRight>,
            pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>>;

typedef vector<Declaration> DeclarationList;
typedef vector<ReferenceArgument> SelectList;
typedef vector<Clause> ClauseList;

class ResultTable;
typedef vector<string> ResultRow;
typedef vector<string> ResultColumn;
}

#endif  // SPA_SRC_QP_TYPES_H
