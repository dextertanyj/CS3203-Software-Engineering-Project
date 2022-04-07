#ifndef SPA_SRC_QP_TYPES_H
#define SPA_SRC_QP_TYPES_H

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/Hash.h"
#include "Common/TypeDefs.h"
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

enum class ArgumentType { Number, Synonym, Wildcard, Name, ExactExpression, SubExpression, Attribute };
enum class AttributeToken { StatementIndex, ProcedureName, VariableName, Value };
enum class AttributeType { NumberIdentifier, NameIdentifier, ProcedureName, VariableName };

struct Declaration {
	DesignEntity type;   // NOLINT(misc-non-private-member-variables-in-classes)
	std::string symbol;  // NOLINT(misc-non-private-member-variables-in-classes)
	bool operator==(const Declaration& other) const { return type == other.type && symbol == other.symbol; }
};

struct Attribute {
	// We cast 0 rather than use a concrete enum value to prevent favouring a particular value if the enum is updated.
	AttributeType attribute = static_cast<AttributeType>(0);  // NOLINT(misc-non-private-member-variables-in-classes)
	Declaration synonym;                                      // NOLINT(misc-non-private-member-variables-in-classes)
	bool operator==(const Attribute& other) const { return attribute == other.attribute && synonym == other.synonym; }
};

using ArgumentValue = std::variant<std::monostate, Declaration, Attribute, std::string, StmtRef, std::pair<Common::EP::Expression, bool>>;

// Types for attribute selection
using Name = std::string;
using Number = unsigned long long;
template <typename TSynonym>
using SelectExecutor = std::function<std::unordered_set<TSynonym>(const StorageAdapter&, const ClauseArgument&)>;
template <typename TAttribute, typename TSynonym>
using AttributeMapper = std::function<TAttribute(const StorageAdapter&, const TSynonym&)>;

// Types for such-that and pattern clause execution
using Executor = std::function<QueryResult(const StorageAdapter&)>;

using Types::Executor;

using OptimizedExecutor = std::function<QueryResult(const StorageAdapter&, const QueryResult&)>;
using ExecutorSet =
	std::variant<Types::Executor, std::pair<Types::Executor, Types::Executor>, std::pair<Types::Executor, OptimizedExecutor>>;
using ExecutorSetFactory = std::function<ExecutorSet(const std::vector<ClauseArgument>&)>;
using ExecutorSetFactoryBundle = std::pair<ClauseType, ExecutorSetFactory>;
using ArgumentDispatchKey = std::variant<ArgumentType, DesignEntity>;
using ExecutorSetBundle = std::pair<ClauseType, ExecutorSet>;
using ArgumentDispatcher = std::function<ExecutorSetBundle(std::vector<ClauseArgument>&)>;
using ArgumentDispatchMap = std::unordered_map<ClauseType, ArgumentDispatcher>;

// Types for with clause execution
template <typename TAttribute, typename TSynonym>
using WithInternalExecutors = std::pair<SelectExecutor<TSynonym>, AttributeMapper<TAttribute, TSynonym>>;
using DispatchAttributeKey = std::pair<DesignEntity, AttributeType>;
using WithClauseArgumentDispatchKey = std::variant<ArgumentType, DispatchAttributeKey>;
using WithClauseBasicDispatchKey = std::variant<ArgumentType, AttributeType>;
template <typename TAttribute, typename TLeft, typename TRight>
using WithExecutorFunction =
	std::function<QueryResult(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs,
                              Types::WithInternalExecutors<TAttribute, TLeft>, Types::WithInternalExecutors<TAttribute, TRight>)>;
template <typename TAttribute, typename TLeft, typename TRight>
using WithExecutorFunctionSet =
	std::variant<WithExecutorFunction<TAttribute, TLeft, TRight>,
                 std::pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>>;

using DeclarationList = std::vector<Declaration>;
using SelectList = std::vector<ClauseArgument>;
using ClauseList = std::vector<std::shared_ptr<Evaluator::Clause>>;

using ResultRow = std::vector<std::string>;
using ResultColumn = std::vector<std::string>;
}

#endif  // SPA_SRC_QP_TYPES_H
