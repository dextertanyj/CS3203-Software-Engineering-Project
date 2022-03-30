#include "QP/Dispatcher/PatternWhileDispatcher.h"

#include <unordered_map>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/PatternContainerStatementExecutor.tpp"

using namespace QP::Executor;
using namespace QP::Types;

using inner_map = unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>;
using outer_map = unordered_map<QP::Types::ArgumentDispatchKey, inner_map>;

static const outer_map map = {
	{ReferenceType::Name,
     {{ReferenceType::Wildcard, PatternContainerStatementExecutor::executorFactoryName<QP::Types::ClauseType::PatternWhile>}}},
	{ReferenceType::Wildcard,
     {{ReferenceType::Wildcard, PatternContainerStatementExecutor::executorFactoryWildcard<QP::Types::ClauseType::PatternWhile>}}},
	{DesignEntity::Variable,
     {{ReferenceType::Wildcard, PatternContainerStatementExecutor::executorFactorySynonym<QP::Types::ClauseType::PatternWhile>}}}};

static const unordered_map<QP::Types::ArgumentDispatchKey, outer_map> argument_dispatch_map = {{DesignEntity::While, map}};

ExecutorSetBundle QP::Dispatcher::PatternWhileDispatcher::dispatcher(const vector<ReferenceArgument>& args) {
	return DispatchProcessors::processArgument(ClauseType::PatternWhile, argument_dispatch_map, args);
}
