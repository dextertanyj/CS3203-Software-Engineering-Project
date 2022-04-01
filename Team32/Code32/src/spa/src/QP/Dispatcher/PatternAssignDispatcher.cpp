#include "QP/Dispatcher/PatternAssignDispatcher.h"

#include <unordered_map>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/PatternAssignExecutor.h"

using namespace QP::Executor;
using namespace QP::Types;

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> name_map = {
	{ReferenceType::Wildcard, PatternAssignExecutor::executorFactoryNameWildcard},
	{ReferenceType::ExactExpression, PatternAssignExecutor::executorFactoryNameExpression},
	{ReferenceType::SubExpression, PatternAssignExecutor::executorFactoryNameExpression},
};

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> wildcard_map = {
	{ReferenceType::Wildcard, PatternAssignExecutor::executorFactoryWildcardWildcard},
	{ReferenceType::ExactExpression, PatternAssignExecutor::executorFactoryWildcardExpression},
	{ReferenceType::SubExpression, PatternAssignExecutor::executorFactoryWildcardExpression},
};

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactory> variable_synonym_map = {
	{ReferenceType::Wildcard, PatternAssignExecutor::executorFactorySynonymWildcard},
	{ReferenceType::ExactExpression, PatternAssignExecutor::executorFactorySynonymExpression},
	{ReferenceType::SubExpression, PatternAssignExecutor::executorFactorySynonymExpression},
};

static const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>> synonym_map = {
	{ReferenceType::Name, name_map}, {ReferenceType::Wildcard, wildcard_map}, {DesignEntity::Variable, variable_synonym_map}};

static const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactory>>>
	argument_dispatch_map = {{DesignEntity::Assign, synonym_map}};

ExecutorSetBundle QP::Dispatcher::PatternAssignDispatcher::dispatcher(const vector<ReferenceArgument>& args) {
	return DispatchProcessors::processArgument(ClauseType::PatternAssign, argument_dispatch_map, args);
}