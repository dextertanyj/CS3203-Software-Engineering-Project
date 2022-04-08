#include "QP/Dispatcher/PatternIfDispatcher.h"

#include <unordered_map>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/PatternContainerStatementExecutor.tpp"

using namespace std;
using namespace QP::Executor::PatternContainerStatementExecutor;
using namespace QP::Types;

using innermost_map = unordered_map<ArgumentDispatchKey, ExecutorSetFactory>;
using lower_map = unordered_map<ArgumentDispatchKey, innermost_map>;
using upper_map = unordered_map<ArgumentDispatchKey, lower_map>;

static const upper_map map = {
	{ArgumentType::Name, {{ArgumentType::Wildcard, {{ArgumentType::Wildcard, executorFactoryName<ClauseType::PatternIf>}}}}},
	{ArgumentType::Wildcard, {{ArgumentType::Wildcard, {{ArgumentType::Wildcard, executorFactoryWildcard<ClauseType::PatternIf>}}}}},
	{DesignEntity::Variable, {{ArgumentType::Wildcard, {{ArgumentType::Wildcard, executorFactorySynonym<ClauseType::PatternIf>}}}}}};

static const unordered_map<ArgumentDispatchKey, upper_map> argument_dispatch_map = {{DesignEntity::If, map}};

ExecutorSetBundle QP::Dispatcher::PatternIfDispatcher::dispatcher(const vector<ClauseArgument>& args) {
	return DispatchProcessors::processArgument(ClauseType::PatternIf, argument_dispatch_map, args);
}
