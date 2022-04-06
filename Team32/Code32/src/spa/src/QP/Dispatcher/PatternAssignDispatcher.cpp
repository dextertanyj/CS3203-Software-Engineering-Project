#include "QP/Dispatcher/PatternAssignDispatcher.h"

#include <unordered_map>

#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/PatternAssignExecutor.h"
#include "QP/Preprocessor/QueryExpressionLexer.h"

#define EXPRESSION_ARGUMENT_INDEX (2)
#define PATTERN_ARGUMENT_COUNT (3)
#define EXACT_MATCH true

using namespace std;
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

static Common::ExpressionProcessor::Expression convertNameToExpression(const string& name) {
	auto lex = QP::Preprocessor::QueryExpressionLexer({name});
	auto parser = Common::ExpressionProcessor::ExpressionParser(lex, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	return parser.parse();
}

ExecutorSetBundle QP::Dispatcher::PatternAssignDispatcher::dispatcher(vector<ClauseArgument>& args) {
	// Single token exact match expressions are parsed as a name and require conversion.
	if (args.size() == PATTERN_ARGUMENT_COUNT && args[EXPRESSION_ARGUMENT_INDEX].getType() == Types::ReferenceType::Name) {
		auto name = args[EXPRESSION_ARGUMENT_INDEX].getName();
		auto expression = convertNameToExpression(name);
		args[EXPRESSION_ARGUMENT_INDEX] = ClauseArgument{move(expression), EXACT_MATCH};
	}
	return DispatchProcessors::processArgument(ClauseType::PatternAssign, argument_dispatch_map, args);
}