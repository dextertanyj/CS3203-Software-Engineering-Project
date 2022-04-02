#include "PatternAssignExecutor.h"

#include "QP/QueryUtils.h"


namespace QP::Executor::PatternAssignExecutor {

using namespace std;

// Trivial Executors
QueryResult executeTrivialNameWildcard(const StorageAdapter& store, const ReferenceArgument& name) {
	return QueryResult(!store.getStmtsWithPatternLHS(name.getName()).empty());
}

QueryResult executeTrivialSynonymOrWildcardWildcard(const StorageAdapter& store) {
	auto statements = store.getStatements();
	for (auto const& statement : statements) {
		if (Utilities::checkStmtTypeMatch(statement, DesignEntity::Assign)) {
			return QueryResult(true);
		}
	}
	return {};
}

QueryResult executeTrivialNameExpression(const StorageAdapter& store, const ReferenceArgument& name, const ReferenceArgument& expression) {
	return QueryResult(
		store.patternExists(name.getName(), expression.getExpression(), expression.getType() == ReferenceType::ExactExpression));
}

QueryResult executeTrivialSynonymOrWildcardExpression(const StorageAdapter& store, const ReferenceArgument& expression) {
	return QueryResult(
		!store.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == ReferenceType::ExactExpression).empty());
}

// Executors
QueryResult executeNameWildcard(const StorageAdapter& store, const ReferenceArgument& assign, const ReferenceArgument& name) {
	QueryResult result = QueryResult({assign.getSynonym().symbol});
	auto results = store.getStmtsWithPatternLHS(name.getName());
	for (auto const& entry : results) {
		result.addRow({to_string(entry->getIdentifier())});
	}

	return result;
}

QueryResult executeWildcardWildcard(const StorageAdapter& store, const ReferenceArgument& assign) {
	QueryResult result = QueryResult({assign.getSynonym().symbol});
	auto results = store.getStatements();
	for (auto const& entry : results) {
		if (Utilities::checkStmtTypeMatch(entry, DesignEntity::Assign)) {
			result.addRow({to_string(entry->getIdentifier())});
		}
	}

	return result;
}

QueryResult executeSynonymWildcard(const StorageAdapter& store, const ReferenceArgument& assign, const ReferenceArgument& synonym) {
	QueryResult result = QueryResult({assign.getSynonym().symbol, synonym.getSynonym().symbol});
	for (auto const& var_ref : store.getVariables()) {
		auto statements = store.getStmtsWithPatternLHS(var_ref);
		for (auto const& statement : statements) {
			result.addRow({to_string(statement->getIdentifier()), var_ref});
		}
	}

	return result;
}

QueryResult executeNameExpression(const StorageAdapter& store, const ReferenceArgument& assign, const ReferenceArgument& name,
                                  const ReferenceArgument& expression) {
	QueryResult result = QueryResult({assign.getSynonym().symbol});
	auto results =
		store.getStmtsWithPattern(name.getName(), expression.getExpression(), expression.getType() == ReferenceType::ExactExpression);
	for (auto const& entry : results) {
		result.addRow({to_string(entry->getIdentifier())});
	}
	return result;
}

QueryResult executeWildcardExpression(const StorageAdapter& store, const ReferenceArgument& assign, const ReferenceArgument& expression) {
	QueryResult result = QueryResult({assign.getSynonym().symbol});
	vector<string> statement_result;
	auto result_pairs = store.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == ReferenceType::ExactExpression);
	for (auto const& pair : result_pairs) {
		result.addRow({to_string(pair.first->getIdentifier())});
	}

	return result;
}

QueryResult executeSynonymExpression(const StorageAdapter& store, const ReferenceArgument& assign, const ReferenceArgument& synonym,
                                     const ReferenceArgument& expression) {
	QueryResult result = QueryResult({assign.getSynonym().symbol, synonym.getSynonym().symbol});
	auto result_pairs = store.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == ReferenceType::ExactExpression);
	for (auto const& pair : result_pairs) {
		result.addRow({to_string(pair.first->getIdentifier()), pair.second});
	}

	return result;
}

ExecutorSet executorFactoryNameWildcard(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [variable = args.at(1)](const StorageAdapter& store) {
		return executeTrivialNameWildcard(store, variable);
	};
	Types::Executor executor = [assign = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeNameWildcard(store, assign, variable);
	};
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactoryNameExpression(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [variable = args.at(1), expression = args.at(2)](const StorageAdapter& store) {
		return executeTrivialNameExpression(store, variable, expression);
	};
	Types::Executor executor = [assign = args.at(0), variable = args.at(1), expression = args.at(2)](const StorageAdapter& store) {
		return executeNameExpression(store, assign, variable, expression);
	};
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactoryWildcardWildcard(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& store) { return executeTrivialSynonymOrWildcardWildcard(store); };
	Types::Executor executor = [assign = args.at(0)](const StorageAdapter& store) { return executeWildcardWildcard(store, assign); };
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactoryWildcardExpression(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [expression = args.at(2)](const StorageAdapter& store) {
		return executeTrivialSynonymOrWildcardExpression(store, expression);
	};
	Types::Executor executor = [assign = args.at(0), expression = args.at(2)](const StorageAdapter& store) {
		return executeWildcardExpression(store, assign, expression);
	};
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactorySynonymWildcard(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& store) { return executeTrivialSynonymOrWildcardWildcard(store); };
	Types::Executor executor = [assign = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeSynonymWildcard(store, assign, variable);
	};
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactorySynonymExpression(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [expression = args.at(2)](const StorageAdapter& store) {
		return executeTrivialSynonymOrWildcardExpression(store, expression);
	};
	Types::Executor executor = [assign = args.at(0), variable = args.at(1), expression = args.at(2)](const StorageAdapter& store) {
		return executeSynonymExpression(store, assign, variable, expression);
	};
	return pair{trivial_executor, executor};
}
}