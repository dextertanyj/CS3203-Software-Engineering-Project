#include "QP/Evaluator/Clause.h"

#include <cassert>

#include "QP/QueryUtils.h"

using namespace std;
using namespace QP::Evaluator;
using namespace QP::Types;

QP::Evaluator::Clause::Clause(ClauseType type, vector<ClauseArgument> arguments, ExecutorSet executor)
	: type(type), arguments(move(arguments)), executor(move(executor)) {}

vector<string> QP::Evaluator::Clause::getDeclarationSymbols() const {
	vector<string> symbols;
	for (const ClauseArgument& arg : arguments) {
		if (arg.getType() == ArgumentType::Synonym || arg.getType() == ArgumentType::Attribute) {
			symbols.push_back(arg.getSynonymSymbol());
		}
	}
	return symbols;
}

ClauseType QP::Evaluator::Clause::getType() const { return type; }

QP::QueryResult QP::Evaluator::Clause::executeTrivial(const QP::StorageAdapter& pkb) const {
	QueryResult result;
	visit(Visitor{[&](const Types::Executor& exec) { result = exec(pkb); },
	              [&](const pair<Types::Executor, Types::Executor>& execs) { result = execs.first(pkb); },
	              [&](const pair<Types::Executor, OptimizedExecutor>& execs) { result = execs.first(pkb); }},
	      executor);
	return result;
}

QP::QueryResult QP::Evaluator::Clause::execute(const QP::StorageAdapter& pkb, vector<QueryResult>& existing_results) const {
	QueryResult result;
	auto invalid_visitor = [&](const Types::Executor&) { assert(false); };  // NOLINT(bugprone-lambda-function-name)
	auto standard_visitor = [&](const pair<Types::Executor, Types::Executor>& execs) { result = execs.second(pkb); };
	auto optimized_visitor = [&](const pair<Types::Executor, OptimizedExecutor>& execs) {
		if (existing_results.empty()) {
			result = execs.second(pkb, {});
			return;
		}
		QueryResult intermediate_result = QueryResult::joinIntraGroupResults(existing_results);
		existing_results.clear();
		if (!intermediate_result.getResult()) {
			return;
		}
		existing_results.push_back(intermediate_result);
		result = execs.second(pkb, intermediate_result);
	};
	visit(Visitor{invalid_visitor, standard_visitor, optimized_visitor}, executor);
	return result;
}

size_t QP::Evaluator::Clause::getCost() const {
	size_t number_of_declarations = getDeclarationSymbols().size();
	return QP::Utilities::cost_map[type] * number_of_declarations;
}

bool QP::Evaluator::Clause::operator==(const Clause& other) const {
	if (type != other.getType()) {
		return false;
	}

	vector<ClauseArgument> other_arguments = other.arguments;
	for (size_t i = 0; i < arguments.size(); i++) {
		if (arguments[i] == other_arguments[i]) {
			continue;
		}
		return false;
	}

	return true;
}
