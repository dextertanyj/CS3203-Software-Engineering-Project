#include "QP/Relationship/Pattern.h"

#include <utility>

#include "QP/DispatchProcessors.h"
#include "QP/QueryUtils.h"

// Trivial Executors
QP::QueryResult QP::Relationship::Pattern::executeTrivialNameWildcard(PKB::StorageAccessInterface& pkb,
                                                                      const Types::ReferenceArgument& name) {
	return QP::QueryResult(!pkb.getStmtsWithPatternLHS(name.getName()).empty());
}

QP::QueryResult QP::Relationship::Pattern::executeTrivialSynonymOrWildcardWildcard(PKB::StorageAccessInterface& pkb) {
	auto statements = pkb.getStatements();
	for (auto const& statement : statements) {
		if (QP::Utilities::checkStmtTypeMatch(statement, Types::DesignEntity::Assign)) {
			return QP::QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::Pattern::executeTrivialNameExpression(PKB::StorageAccessInterface& pkb,
                                                                        const Types::ReferenceArgument& name,
                                                                        const Types::ReferenceArgument& expression) {
	return QP::QueryResult(
		pkb.patternExists(name.getName(), expression.getExpression(), expression.getType() == QP::Types::ReferenceType::ExactExpression));
}

QP::QueryResult QP::Relationship::Pattern::executeTrivialSynonymOrWildcardExpression(PKB::StorageAccessInterface& pkb,
                                                                                     const Types::ReferenceArgument& expression) {
	return QP::QueryResult(
		!pkb.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == QP::Types::ReferenceType::ExactExpression).empty());
}

// Executors
QP::QueryResult QP::Relationship::Pattern::executeNameWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& assign,
                                                               const Types::ReferenceArgument& name) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	auto results = pkb.getStmtsWithPatternLHS(name.getName());
	for (auto const& entry : results) {
		statement_result.push_back(to_string(entry->getIdentifier()));
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);

	return result;
}

QP::QueryResult QP::Relationship::Pattern::executeWildcardWildcard(PKB::StorageAccessInterface& pkb,
                                                                   const Types::ReferenceArgument& assign) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	auto results = pkb.getStatements();
	for (auto const& entry : results) {
		if (QP::Utilities::checkStmtTypeMatch(entry, Types::DesignEntity::Assign)) {
			statement_result.push_back(to_string(entry->getIdentifier()));
		}
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);

	return result;
}

QP::QueryResult QP::Relationship::Pattern::executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& assign,
                                                                  const Types::ReferenceArgument& synonym) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	vector<string> variable_result;
	for (auto const& var_ref : pkb.getVariables()) {
		auto statements = pkb.getStmtsWithPatternLHS(var_ref);
		for (auto const& statement : statements) {
			statement_result.push_back(to_string(statement->getIdentifier()));
			variable_result.push_back(var_ref);
		}
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);
	result.addColumn(synonym.getSynonym().symbol, variable_result);

	return result;
}

QP::QueryResult QP::Relationship::Pattern::executeNameExpression(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& assign,
                                                                 const Types::ReferenceArgument& name,
                                                                 const Types::ReferenceArgument& expression) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	auto results = pkb.getStmtsWithPattern(name.getName(), expression.getExpression(),
	                                       expression.getType() == QP::Types::ReferenceType::ExactExpression);
	for (auto const& entry : results) {
		statement_result.push_back(to_string(entry->getIdentifier()));
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);
	return result;
}

QP::QueryResult QP::Relationship::Pattern::executeWildcardExpression(PKB::StorageAccessInterface& pkb,
                                                                     const Types::ReferenceArgument& assign,
                                                                     const Types::ReferenceArgument& expression) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	auto result_pairs =
		pkb.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == QP::Types::ReferenceType::ExactExpression);
	for (auto const& pair : result_pairs) {
		statement_result.push_back(to_string(pair.first->getIdentifier()));
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);

	return result;
}

QP::QueryResult QP::Relationship::Pattern::executeSynonymExpression(PKB::StorageAccessInterface& pkb,
                                                                    const Types::ReferenceArgument& assign,
                                                                    const Types::ReferenceArgument& synonym,
                                                                    const Types::ReferenceArgument& expression) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	vector<string> variable_result;
	auto result_pairs =
		pkb.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == QP::Types::ReferenceType::ExactExpression);
	for (auto const& pair : result_pairs) {
		statement_result.push_back(to_string(pair.first->getIdentifier()));
		variable_result.push_back(pair.second);
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);
	result.addColumn(synonym.getSynonym().symbol, variable_result);

	return result;
}

QP::Types::ArgumentDispatcher QP::Relationship::Pattern::dispatcher = [](vector<Types::ReferenceArgument> args) {
	return DispatchProcessors::processTripleArgument(Types::ClauseType::PatternAssign, argument_dispatch_map, std::move(args));
};

unordered_map<QP::Types::ArgumentDispatchKey,
              unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>>
	QP::Relationship::Pattern::argument_dispatch_map = {{Types::DesignEntity::Assign, getSynonymMap()}};

unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>
QP::Relationship::Pattern::getSynonymMap() {
	static unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>> map =
		{{Types::ReferenceType::Name, getNameMap()},
	     {Types::ReferenceType::Wildcard, getWildcardMap()},
	     {Types::DesignEntity::Variable, getVariableSynonymMap()}};
	return map;
}

unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Relationship::Pattern::getNameMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[variable = args.at(1)](PKB::StorageAccessInterface& pkb) { return executeTrivialNameWildcard(pkb, variable); },
		                 [assign = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
							 return executeNameWildcard(pkb, assign, variable);
						 }};
		 }},
		{Types::ReferenceType::ExactExpression,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[variable = args.at(1), expression = args.at(2)](PKB::StorageAccessInterface& pkb) {
							 return executeTrivialNameExpression(pkb, variable, expression);
						 },
		                 [assign = args.at(0), variable = args.at(1), expression = args.at(2)](PKB::StorageAccessInterface& pkb) {
							 return executeNameExpression(pkb, assign, variable, expression);
						 }};
		 }},
		{Types::ReferenceType::SubExpression,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[variable = args.at(1), expression = args.at(2)](PKB::StorageAccessInterface& pkb) {
							 return executeTrivialNameExpression(pkb, variable, expression);
						 },
		                 [assign = args.at(0), variable = args.at(1), expression = args.at(2)](PKB::StorageAccessInterface& pkb) {
							 return executeNameExpression(pkb, assign, variable, expression);
						 }};
		 }},
	};
	return map;
}

unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Relationship::Pattern::getWildcardMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[](PKB::StorageAccessInterface& pkb) { return executeTrivialSynonymOrWildcardWildcard(pkb); },
		                 [assign = args.at(0)](PKB::StorageAccessInterface& pkb) { return executeWildcardWildcard(pkb, assign); }};
		 }},
		{Types::ReferenceType::ExactExpression,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[expression = args.at(2)](PKB::StorageAccessInterface& pkb) {
							 return executeTrivialSynonymOrWildcardExpression(pkb, expression);
						 },
		                 [assign = args.at(0), expression = args.at(2)](PKB::StorageAccessInterface& pkb) {
							 return executeWildcardExpression(pkb, assign, expression);
						 }};
		 }},
		{Types::ReferenceType::SubExpression,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[expression = args.at(2)](PKB::StorageAccessInterface& pkb) {
							 return executeTrivialSynonymOrWildcardExpression(pkb, expression);
						 },
		                 [assign = args.at(0), expression = args.at(2)](PKB::StorageAccessInterface& pkb) {
							 return executeWildcardExpression(pkb, assign, expression);
						 }};
		 }},
	};
	return map;
}

unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Relationship::Pattern::getVariableSynonymMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[](PKB::StorageAccessInterface& pkb) { return executeTrivialSynonymOrWildcardWildcard(pkb); },
		                 [assign = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
							 return executeSynonymWildcard(pkb, assign, variable);
						 }};
		 }},
		{Types::ReferenceType::ExactExpression,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[expression = args.at(2)](PKB::StorageAccessInterface& pkb) {
							 return executeTrivialSynonymOrWildcardExpression(pkb, expression);
						 },
		                 [assign = args.at(0), variable = args.at(1), expression = args.at(2)](PKB::StorageAccessInterface& pkb) {
							 return executeSynonymExpression(pkb, assign, variable, expression);
						 }};
		 }},
		{Types::ReferenceType::SubExpression,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[expression = args.at(2)](PKB::StorageAccessInterface& pkb) {
							 return executeTrivialSynonymOrWildcardExpression(pkb, expression);
						 },
		                 [assign = args.at(0), variable = args.at(1), expression = args.at(2)](PKB::StorageAccessInterface& pkb) {
							 return executeSynonymExpression(pkb, assign, variable, expression);
						 }};
		 }},
	};
	return map;
}
