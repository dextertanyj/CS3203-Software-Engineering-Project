#include "QP/Dispatcher/PatternIfDispatcher.h"

#include <unordered_map>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/PatternContainerStatementExecutor.tpp"

using namespace QP::Executor;
using namespace QP::Types;

using innermost_map = unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>;
using lower_map = unordered_map<QP::Types::ArgumentDispatchKey, innermost_map>;
using upper_map = unordered_map<QP::Types::ArgumentDispatchKey, lower_map>;

static const upper_map map = {
	{ReferenceType::Name,
     {{ReferenceType::Wildcard,
       {{ReferenceType::Wildcard, PatternContainerStatementExecutor::executorFactoryName<QP::Types::ClauseType::PatternIf>}}}}},
	{ReferenceType::Wildcard,
     {{ReferenceType::Wildcard,
       {{ReferenceType::Wildcard, PatternContainerStatementExecutor::executorFactoryWildcard<QP::Types::ClauseType::PatternIf>}}}}},
	{DesignEntity::Variable,
     {{ReferenceType::Wildcard,
       {{ReferenceType::Wildcard, PatternContainerStatementExecutor::executorFactorySynonym<QP::Types::ClauseType::PatternIf>}}}}}};

static const unordered_map<QP::Types::ArgumentDispatchKey, upper_map> argument_dispatch_map = {{DesignEntity::If, map}};

ExecutorSetBundle QP::Dispatcher::PatternIfDispatcher::dispatcher(const vector<ReferenceArgument>& args) {
	return DispatchProcessors::processArgument(ClauseType::PatternIf, argument_dispatch_map, args);
}
