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
#include "QP/Evaluator/Evaluator.h"
#include "QP/QP.h"

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

struct Declaration {
	DesignEntity type;   // NOLINT(misc-non-private-member-variables-in-classes)
	std::string symbol;  // NOLINT(misc-non-private-member-variables-in-classes)
	bool operator==(const Declaration& other) const { return type == other.type && symbol == other.symbol; }
};

struct DeclarationHash {
	std::size_t operator()(const Declaration& key) const {
		std::size_t seed = 0;
		combineHash(seed, key.symbol);
		combineHash(seed, key.type);
		return seed;
	}
};

struct Attribute {
	// We cast 0 rather than use a concrete enum value to prevent favouring a particular value if the enum is updated.
	AttributeType attribute = static_cast<AttributeType>(0);  // NOLINT(misc-non-private-member-variables-in-classes)
	Declaration synonym;                                      // NOLINT(misc-non-private-member-variables-in-classes)
	bool operator==(const Attribute& other) const { return attribute == other.attribute && synonym == other.synonym; }
};

// Types for attribute selection
using Name = std::string;
using Number = unsigned long long;
template <typename TSynonym>
using SelectExecutor = std::function<std::unordered_set<TSynonym>(const StorageAdapter&, const ReferenceArgument&)>;
template <typename TAttribute, typename TSynonym>
using AttributeMapper = std::function<TAttribute(const StorageAdapter&, const TSynonym&)>;

// Types for such-that and pattern clause execution
using Executor = std::function<QueryResult(const StorageAdapter&)>;
using OptimizedExecutor = std::function<QueryResult(const StorageAdapter&, const QueryResult&)>;
using ExecutorSet = std::variant<Executor, std::pair<Executor, Executor>, std::pair<Executor, OptimizedExecutor>>;
using ExecutorSetFactory = std::function<ExecutorSet(const std::vector<ReferenceArgument>&)>;
using ExecutorSetFactoryBundle = std::pair<ClauseType, ExecutorSetFactory>;
using ArgumentDispatchKey = std::variant<ReferenceType, DesignEntity>;
using ExecutorSetBundle = std::pair<ClauseType, ExecutorSet>;
using ArgumentDispatcher = std::function<ExecutorSetBundle(const std::vector<ReferenceArgument>&)>;
using ArgumentDispatchMap = std::unordered_map<ClauseType, ArgumentDispatcher>;

// Types for with clause execution
template <typename TAttribute, typename TSynonym>
using WithInternalExecutors = std::pair<SelectExecutor<TSynonym>, AttributeMapper<TAttribute, TSynonym>>;
using DispatchAttributeKey = std::pair<DesignEntity, AttributeType>;
using WithClauseArgumentDispatchKey = std::variant<ReferenceType, DispatchAttributeKey>;
using WithClauseBasicDispatchKey = std::variant<ReferenceType, AttributeType>;
template <typename TAttribute, typename TLeft, typename TRight>
using WithExecutorFunction =
	std::function<QueryResult(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs,
                              Types::WithInternalExecutors<TAttribute, TLeft>, Types::WithInternalExecutors<TAttribute, TRight>)>;
template <typename TAttribute, typename TLeft, typename TRight>
using WithExecutorFunctionSet =
	std::variant<WithExecutorFunction<TAttribute, TLeft, TRight>,
                 std::pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>>;

using DeclarationList = std::vector<Declaration>;
using SelectList = std::vector<ReferenceArgument>;
using ClauseList = std::vector<std::shared_ptr<Evaluator::Clause>>;

using ResultRow = std::vector<std::string>;
using ResultColumn = std::vector<std::string>;
}

#endif  // SPA_SRC_QP_TYPES_H
