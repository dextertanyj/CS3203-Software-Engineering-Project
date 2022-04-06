#include "QP/Dispatcher/PatternWhileDispatcher.h"

#include <unordered_map>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/PatternContainerStatementExecutor.tpp"

using namespace std;
using namespace QP::Executor;
using namespace QP::Types;

using inner_map = unordered_map<ArgumentDispatchKey, ExecutorSetFactory>;
using outer_map = unordered_map<ArgumentDispatchKey, inner_map>;

static const outer_map map = {
	{ArgumentType::Name, {{ArgumentType::Wildcard, PatternContainerStatementExecutor::executorFactoryName<ClauseType::PatternWhile>}}},
	{ArgumentType::Wildcard,
     {{ArgumentType::Wildcard, PatternContainerStatementExecutor::executorFactoryWildcard<ClauseType::PatternWhile>}}},
	{DesignEntity::Variable,
     {{ArgumentType::Wildcard, PatternContainerStatementExecutor::executorFactorySynonym<ClauseType::PatternWhile>}}}};

static const unordered_map<QP::Types::ArgumentDispatchKey, outer_map> argument_dispatch_map = {{DesignEntity::While, map}};

ExecutorSetBundle QP::Dispatcher::PatternWhileDispatcher::dispatcher(const vector<ClauseArgument>& args) {
	return DispatchProcessors::processArgument(ClauseType::PatternWhile, argument_dispatch_map, args);
}
