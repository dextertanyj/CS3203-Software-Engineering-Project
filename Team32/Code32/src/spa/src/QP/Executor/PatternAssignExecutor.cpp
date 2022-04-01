#include "PatternAssignExecutor.h"

#include "QP/QueryUtils.h"

namespace QP::Executor::PatternAssignExecutor {

// Trivial Executors
QueryResult executeTrivialNameWildcard(const StorageAdapter& storage, const ReferenceArgument& name) {
	return QueryResult(!storage.getStmtsWithPatternLHS(name.getName()).empty());
}

QueryResult executeTrivialSynonymOrWildcardWildcard(const StorageAdapter& storage) {
	auto statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (Utilities::checkStmtTypeMatch(statement, DesignEntity::Assign)) {
			return QueryResult(true);
		}
	}
	return {};
}

QueryResult executeTrivialNameExpression(const StorageAdapter& storage, const ReferenceArgument& name,
                                         const ReferenceArgument& expression) {
	return QueryResult(
		storage.patternExists(name.getName(), expression.getExpression(), expression.getType() == ReferenceType::ExactExpression));
}

QueryResult executeTrivialSynonymOrWildcardExpression(const StorageAdapter& storage, const ReferenceArgument& expression) {
	return QueryResult(
		!storage.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == ReferenceType::ExactExpression).empty());
}

// Executors
QueryResult executeNameWildcard(const StorageAdapter& storage, const ReferenceArgument& assign, const ReferenceArgument& name) {
	QueryResult result = QueryResult({assign.getSynonym().symbol});
	auto results = storage.getStmtsWithPatternLHS(name.getName());
	for (auto const& entry : results) {
		result.addRow({to_string(entry->getIdentifier())});
	}

	return result;
}

QueryResult executeWildcardWildcard(const StorageAdapter& storage, const ReferenceArgument& assign) {
	QueryResult result = QueryResult({assign.getSynonym().symbol});
	auto results = storage.getStatements();
	for (auto const& entry : results) {
		if (Utilities::checkStmtTypeMatch(entry, DesignEntity::Assign)) {
			result.addRow({to_string(entry->getIdentifier())});
		}
	}

	return result;
}

QueryResult executeSynonymWildcard(const StorageAdapter& storage, const ReferenceArgument& assign, const ReferenceArgument& synonym) {
	QueryResult result = QueryResult({assign.getSynonym().symbol, synonym.getSynonym().symbol});
	for (auto const& var_ref : storage.getVariables()) {
		auto statements = storage.getStmtsWithPatternLHS(var_ref);
		for (auto const& statement : statements) {
			result.addRow({to_string(statement->getIdentifier()), var_ref});
		}
	}

	return result;
}

QueryResult executeNameExpression(const StorageAdapter& storage, const ReferenceArgument& assign, const ReferenceArgument& name,
                                  const ReferenceArgument& expression) {
	QueryResult result = QueryResult({assign.getSynonym().symbol});
	auto results =
		storage.getStmtsWithPattern(name.getName(), expression.getExpression(), expression.getType() == ReferenceType::ExactExpression);
	for (auto const& entry : results) {
		result.addRow({to_string(entry->getIdentifier())});
	}
	return result;
}

QueryResult executeWildcardExpression(const StorageAdapter& storage, const ReferenceArgument& assign, const ReferenceArgument& expression) {
	QueryResult result = QueryResult({assign.getSynonym().symbol});
	vector<string> statement_result;
	auto result_pairs = storage.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == ReferenceType::ExactExpression);
	for (auto const& pair : result_pairs) {
		result.addRow({to_string(pair.first->getIdentifier())});
	}

	return result;
}

QueryResult executeSynonymExpression(const StorageAdapter& storage, const ReferenceArgument& assign, const ReferenceArgument& synonym,
                                     const ReferenceArgument& expression) {
	QueryResult result = QueryResult({assign.getSynonym().symbol, synonym.getSynonym().symbol});
	auto result_pairs = storage.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == ReferenceType::ExactExpression);
	for (auto const& pair : result_pairs) {
		result.addRow({to_string(pair.first->getIdentifier()), pair.second});
	}

	return result;
}

ExecutorSet executorFactoryNameWildcard(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [variable = args.at(1)](const StorageAdapter& storage) {
		return executeTrivialNameWildcard(storage, variable);
	};
	Types::Executor executor = [assign = args.at(0), variable = args.at(1)](const StorageAdapter& storage) {
		return executeNameWildcard(storage, assign, variable);
	};
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactoryNameExpression(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [variable = args.at(1), expression = args.at(2)](const StorageAdapter& storage) {
		return executeTrivialNameExpression(storage, variable, expression);
	};
	Types::Executor executor = [assign = args.at(0), variable = args.at(1), expression = args.at(2)](const StorageAdapter& storage) {
		return executeNameExpression(storage, assign, variable, expression);
	};
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactoryWildcardWildcard(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& storage) { return executeTrivialSynonymOrWildcardWildcard(storage); };
	Types::Executor executor = [assign = args.at(0)](const StorageAdapter& storage) { return executeWildcardWildcard(storage, assign); };
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactoryWildcardExpression(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [expression = args.at(2)](const StorageAdapter& storage) {
		return executeTrivialSynonymOrWildcardExpression(storage, expression);
	};
	Types::Executor executor = [assign = args.at(0), expression = args.at(2)](const StorageAdapter& storage) {
		return executeWildcardExpression(storage, assign, expression);
	};
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactorySynonymWildcard(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& storage) { return executeTrivialSynonymOrWildcardWildcard(storage); };
	Types::Executor executor = [assign = args.at(0), variable = args.at(1)](const StorageAdapter& storage) {
		return executeSynonymWildcard(storage, assign, variable);
	};
	return pair{trivial_executor, executor};
}

ExecutorSet executorFactorySynonymExpression(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [expression = args.at(2)](const StorageAdapter& storage) {
		return executeTrivialSynonymOrWildcardExpression(storage, expression);
	};
	Types::Executor executor = [assign = args.at(0), variable = args.at(1), expression = args.at(2)](const StorageAdapter& storage) {
		return executeSynonymExpression(storage, assign, variable, expression);
	};
	return pair{trivial_executor, executor};
}
}