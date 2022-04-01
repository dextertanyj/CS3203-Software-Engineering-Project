#include "QP/Relationship/Relation.h"

#include <cassert>

#include "QP/QueryUtils.h"

QP::Relationship::Relation::Relation(Types::ClauseType type, vector<Types::ReferenceArgument> arguments, Types::ExecutorSet executor)
	: type(type), arguments(move(arguments)), executor(move(executor)) {}

vector<string> QP::Relationship::Relation::getDeclarationSymbols() const {
	vector<string> symbols;
	for (const Types::ReferenceArgument& arg : arguments) {
		if (arg.getType() == Types::ReferenceType::Synonym) {
			symbols.push_back(arg.getSynonym().symbol);
		}
		if (arg.getType() == Types::ReferenceType::Attribute) {
			symbols.push_back(arg.getAttribute().synonym.symbol);
		}
	}
	return symbols;
}

QP::Types::ClauseType QP::Relationship::Relation::getType() const { return type; }

QP::QueryResult QP::Relationship::Relation::executeTrivial(const QP::StorageAdapter& pkb) const {
	QueryResult result;
	visit(Visitor{[&](const Types::Executor& exec) { result = exec(pkb); },
	              [&](const pair<Types::Executor, Types::Executor>& execs) { result = execs.first(pkb); },
	              [&](const pair<Types::Executor, Types::OptimizedExecutor>& execs) { result = execs.first(pkb); }},
	      executor);
	return result;
}

QP::QueryResult QP::Relationship::Relation::execute(const QP::StorageAdapter& pkb, vector<QueryResult>& existing_results) const {
	QueryResult result;
	auto invalid_visitor = [&](const Types::Executor&) { assert(false); };
	auto standard_visitor = [&](const pair<Types::Executor, Types::Executor>& execs) { result = execs.second(pkb); };
	auto optimized_visitor = [&](const pair<Types::Executor, Types::OptimizedExecutor>& execs) {
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

size_t QP::Relationship::Relation::getCost() const {
	size_t number_of_declarations = getDeclarationSymbols().size();
	return QP::Utilities::cost_map[type] * number_of_declarations;
}
