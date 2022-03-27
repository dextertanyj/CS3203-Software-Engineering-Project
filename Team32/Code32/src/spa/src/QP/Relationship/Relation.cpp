#include "QP/Relationship/Relation.h"

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
	              [&](const pair<Types::Executor, Types::Executor>& execs) { result = execs.first(pkb); }},
	      executor);
	return result;
}

QP::QueryResult QP::Relationship::Relation::execute(const QP::StorageAdapter& pkb) const {
	QueryResult result;
	visit(Visitor{[&](const Types::Executor&) { throw QP::QueryException("Invalid executor."); },
	              [&](const pair<Types::Executor, Types::Executor>& execs) { result = execs.second(pkb); }},
	      executor);
	return result;
}
