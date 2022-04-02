#ifndef SPA_SRC_QP_TYPES_H
#define SPA_SRC_QP_TYPES_H

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include "Common/Hash.h"
#include "QP/QP.h"
#include "QP/Evaluator/Evaluator.h"

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

typedef struct Declaration {
	DesignEntity type;  // NOLINT(misc-non-private-member-variables-in-classes)
	std::string symbol;      // NOLINT(misc-non-private-member-variables-in-classes)
	bool operator==(const Declaration& other) const { return type == other.type && symbol == other.symbol; }
} Declaration;

struct DeclarationHash {
	std::size_t operator()(const Declaration& key) const {
		std::size_t seed = 0;
		combineHash(seed, key.symbol);
		combineHash(seed, key.type);
		return seed;
	}
};

typedef struct Attribute {
	// We cast 0 rather than use a concrete enum value to prevent favouring a particular value if the enum is updated.
	AttributeType attribute = static_cast<AttributeType>(0);  // NOLINT(misc-non-private-member-variables-in-classes)
	Declaration synonym;                                      // NOLINT(misc-non-private-member-variables-in-classes)
	bool operator==(const Attribute& other) const { return attribute == other.attribute && synonym == other.synonym; }
} Attribute;

// Types for attribute selection
typedef std::string Name;
typedef unsigned long long Number;
template <typename TSynonym>
using SelectExecutor = std::function<std::unordered_set<TSynonym>(const StorageAdapter&, const ReferenceArgument&)>;
template <typename TAttribute, typename TSynonym>
using AttributeMapper = std::function<TAttribute(const StorageAdapter&, const TSynonym&)>;

// Types for such-that and pattern clause execution
typedef std::function<QueryResult(const StorageAdapter&)> Executor;
typedef std::function<QueryResult(const StorageAdapter&, const QueryResult&)> OptimizedExecutor;
typedef std::variant<Executor, std::pair<Executor, Executor>, std::pair<Executor, OptimizedExecutor>> ExecutorSet;
typedef std::function<ExecutorSet(const std::vector<ReferenceArgument>&)> ExecutorSetFactory;
typedef std::pair<ClauseType, ExecutorSetFactory> ExecutorSetFactoryBundle;
typedef std::variant<ReferenceType, DesignEntity> ArgumentDispatchKey;
typedef std::pair<ClauseType, ExecutorSet> ExecutorSetBundle;
typedef std::function<ExecutorSetBundle(const std::vector<ReferenceArgument>&)> ArgumentDispatcher;
typedef std::unordered_map<ClauseType, ArgumentDispatcher> ArgumentDispatchMap;

// Types for with clause execution
template <typename TAttribute, typename TSynonym>
using WithInternalExecutors = std::pair<SelectExecutor<TSynonym>, AttributeMapper<TAttribute, TSynonym>>;
typedef std::pair<DesignEntity, AttributeType> DispatchAttributeKey;
typedef std::variant<ReferenceType, DispatchAttributeKey> WithClauseArgumentDispatchKey;
typedef std::variant<ReferenceType, AttributeType> WithClauseBasicDispatchKey;
template <typename TAttribute, typename TLeft, typename TRight>
using WithExecutorFunction = std::function<QueryResult(
	const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs,
	Types::WithInternalExecutors<TAttribute, TLeft>, Types::WithInternalExecutors<TAttribute, TRight>)>;
template <typename TAttribute, typename TLeft, typename TRight>
using WithExecutorFunctionSet =
	std::variant<WithExecutorFunction<TAttribute, TLeft, TRight>,
                 std::pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>>;

typedef std::vector<Declaration> DeclarationList;
typedef std::vector<ReferenceArgument> SelectList;
typedef std::vector<std::shared_ptr<Evaluator::Clause>> ClauseList;

typedef std::vector<std::string> ResultRow;
typedef std::vector<std::string> ResultColumn;
}

#endif  // SPA_SRC_QP_TYPES_H
