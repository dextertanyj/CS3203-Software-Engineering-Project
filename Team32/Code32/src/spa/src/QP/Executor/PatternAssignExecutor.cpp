#include "PatternAssignExecutor.h"

#include "QP/QueryUtils.h"

namespace QP::Executor::PatternAssignExecutor {

using namespace std;

// Trivial Executors
QueryResult executeTrivialNameWildcard(const StorageAdapter& store, const ClauseArgument& name) {
	unordered_set result = store.getStmtsWithPatternLHS(name.getName());
	return QueryResult(!result.empty());
}

QueryResult executeTrivialSynonymOrWildcardWildcard(const StorageAdapter& store) {
	auto statements = store.getStatements();
	for (const auto& statement : statements) {
		if (Utilities::checkStmtTypeMatch(statement, DesignEntity::Assign)) {
			return QueryResult(true);
		}
	}
	return {};
}

QueryResult executeTrivialNameExpression(const StorageAdapter& store, const ClauseArgument& name, const ClauseArgument& expression) {
	bool result = store.patternExists(name.getName(), expression.getExpression(), expression.getType() == ArgumentType::ExactExpression);
	return QueryResult(result);
}

QueryResult executeTrivialSynonymOrWildcardExpression(const StorageAdapter& store, const ClauseArgument& expression) {
	unordered_set result = store.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == ArgumentType::ExactExpression);
	return QueryResult(!result.empty());
}

// Executors
QueryResult executeNameWildcard(const StorageAdapter& store, const ClauseArgument& assign, const ClauseArgument& name) {
	QueryResult result = QueryResult({assign.getSynonymSymbol()});
	auto results = store.getStmtsWithPatternLHS(name.getName());
	for (const auto& entry : results) {
		result.addRow({to_string(entry->getIdentifier())});
	}

	return result;
}

QueryResult executeWildcardWildcard(const StorageAdapter& store, const ClauseArgument& assign) {
	QueryResult result = QueryResult({assign.getSynonymSymbol()});
	auto results = store.getStatements();
	for (const auto& entry : results) {
		if (Utilities::checkStmtTypeMatch(entry, DesignEntity::Assign)) {
			result.addRow({to_string(entry->getIdentifier())});
		}
	}

	return result;
}

QueryResult executeSynonymWildcard(const StorageAdapter& store, const ClauseArgument& assign, const ClauseArgument& synonym) {
	QueryResult result = QueryResult({assign.getSynonymSymbol(), synonym.getSynonymSymbol()});
	for (const auto& var_ref : store.getVariables()) {
		auto statements = store.getStmtsWithPatternLHS(var_ref);
		for (const auto& statement : statements) {
			result.addRow({to_string(statement->getIdentifier()), var_ref});
		}
	}

	return result;
}

QueryResult executeNameExpression(const StorageAdapter& store, const ClauseArgument& assign, const ClauseArgument& name,
                                  const ClauseArgument& expression) {
	QueryResult result = QueryResult({assign.getSynonymSymbol()});
	auto results =
		store.getStmtsWithPattern(name.getName(), expression.getExpression(), expression.getType() == ArgumentType::ExactExpression);
	for (const auto& entry : results) {
		result.addRow({to_string(entry->getIdentifier())});
	}
	return result;
}

QueryResult executeWildcardExpression(const StorageAdapter& store, const ClauseArgument& assign, const ClauseArgument& expression) {
	QueryResult result = QueryResult({assign.getSynonymSymbol()});
	vector<string> statement_result;
	auto result_pairs = store.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == ArgumentType::ExactExpression);
	for (const auto& pair : result_pairs) {
		result.addRow({to_string(pair.first->getIdentifier())});
	}

	return result;
}

QueryResult executeSynonymExpression(const StorageAdapter& store, const ClauseArgument& assign, const ClauseArgument& synonym,
                                     const ClauseArgument& expression) {
	QueryResult result = QueryResult({assign.getSynonymSymbol(), synonym.getSynonymSymbol()});
	auto result_pairs = store.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == ArgumentType::ExactExpression);
	for (const auto& pair : result_pairs) {
		result.addRow({to_string(pair.first->getIdentifier()), pair.second});
	}

	return result;
}

ExecutorSet executorFactoryNameWildcard(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [variable = args.at(1)](const StorageAdapter& store) {
		return executeTrivialNameWildcard(store, variable);
	};
	Types::Executor executor = [assign = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeNameWildcard(store, assign, variable);
	};
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactoryNameExpression(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [variable = args.at(1), expression = args.at(2)](const StorageAdapter& store) {
		return executeTrivialNameExpression(store, variable, expression);
	};
	Types::Executor executor = [assign = args.at(0), variable = args.at(1), expression = args.at(2)](const StorageAdapter& store) {
		return executeNameExpression(store, assign, variable, expression);
	};
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactoryWildcardWildcard(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& store) { return executeTrivialSynonymOrWildcardWildcard(store); };
	Types::Executor executor = [assign = args.at(0)](const StorageAdapter& store) { return executeWildcardWildcard(store, assign); };
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactoryWildcardExpression(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [expression = args.at(2)](const StorageAdapter& store) {
		return executeTrivialSynonymOrWildcardExpression(store, expression);
	};
	Types::Executor executor = [assign = args.at(0), expression = args.at(2)](const StorageAdapter& store) {
		return executeWildcardExpression(store, assign, expression);
	};
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactorySynonymWildcard(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& store) { return executeTrivialSynonymOrWildcardWildcard(store); };
	Types::Executor executor = [assign = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeSynonymWildcard(store, assign, variable);
	};
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactorySynonymExpression(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [expression = args.at(2)](const StorageAdapter& store) {
		return executeTrivialSynonymOrWildcardExpression(store, expression);
	};
	Types::Executor executor = [assign = args.at(0), variable = args.at(1), expression = args.at(2)](const StorageAdapter& store) {
		return executeSynonymExpression(store, assign, variable, expression);
	};
	return pair{trivial_executor, executor};
}
}