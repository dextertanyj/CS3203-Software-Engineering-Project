#include "PatternAssignExecutor.h"

#include "QP/QueryUtils.h"

// Trivial Executors
QP::QueryResult QP::Executor::PatternAssignExecutor::executeTrivialNameWildcard(QP::StorageAdapter& storage,
                                                                                const Types::ReferenceArgument& name) {
	return QP::QueryResult(!storage.getStmtsWithPatternLHS(name.getName()).empty());
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeTrivialSynonymOrWildcardWildcard(QP::StorageAdapter& storage) {
	auto statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (QP::Utilities::checkStmtTypeMatch(statement, Types::DesignEntity::Assign)) {
			return QP::QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeTrivialNameExpression(QP::StorageAdapter& storage,
                                                                                  const Types::ReferenceArgument& name,
                                                                                  const Types::ReferenceArgument& expression) {
	return QP::QueryResult(storage.patternExists(name.getName(), expression.getExpression(),
	                                             expression.getType() == QP::Types::ReferenceType::ExactExpression));
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeTrivialSynonymOrWildcardExpression(QP::StorageAdapter& storage,
                                                                                               const Types::ReferenceArgument& expression) {
	return QP::QueryResult(
		!storage.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == QP::Types::ReferenceType::ExactExpression)
			 .empty());
}

// Executors
QP::QueryResult QP::Executor::PatternAssignExecutor::executeNameWildcard(QP::StorageAdapter& storage,
                                                                         const Types::ReferenceArgument& assign,
                                                                         const Types::ReferenceArgument& name) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	auto results = storage.getStmtsWithPatternLHS(name.getName());
	statement_result.reserve(results.size());
	for (auto const& entry : results) {
		statement_result.push_back(to_string(entry->getIdentifier()));
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);

	return result;
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeWildcardWildcard(QP::StorageAdapter& storage,
                                                                             const Types::ReferenceArgument& assign) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	auto results = storage.getStatements();
	for (auto const& entry : results) {
		if (QP::Utilities::checkStmtTypeMatch(entry, Types::DesignEntity::Assign)) {
			statement_result.push_back(to_string(entry->getIdentifier()));
		}
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);

	return result;
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeSynonymWildcard(QP::StorageAdapter& storage,
                                                                            const Types::ReferenceArgument& assign,
                                                                            const Types::ReferenceArgument& synonym) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	vector<string> variable_result;
	for (auto const& var_ref : storage.getVariables()) {
		auto statements = storage.getStmtsWithPatternLHS(var_ref);
		for (auto const& statement : statements) {
			statement_result.push_back(to_string(statement->getIdentifier()));
			variable_result.push_back(var_ref);
		}
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);
	result.addColumn(synonym.getSynonym().symbol, variable_result);

	return result;
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeNameExpression(QP::StorageAdapter& storage,
                                                                           const Types::ReferenceArgument& assign,
                                                                           const Types::ReferenceArgument& name,
                                                                           const Types::ReferenceArgument& expression) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	auto results = storage.getStmtsWithPattern(name.getName(), expression.getExpression(),
	                                           expression.getType() == QP::Types::ReferenceType::ExactExpression);
	statement_result.reserve(results.size());
	for (auto const& entry : results) {
		statement_result.push_back(to_string(entry->getIdentifier()));
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);
	return result;
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeWildcardExpression(QP::StorageAdapter& storage,
                                                                               const Types::ReferenceArgument& assign,
                                                                               const Types::ReferenceArgument& expression) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	auto result_pairs =
		storage.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == QP::Types::ReferenceType::ExactExpression);
	statement_result.reserve(result_pairs.size());
	for (auto const& pair : result_pairs) {
		statement_result.push_back(to_string(pair.first->getIdentifier()));
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);

	return result;
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeSynonymExpression(QP::StorageAdapter& storage,
                                                                              const Types::ReferenceArgument& assign,
                                                                              const Types::ReferenceArgument& synonym,
                                                                              const Types::ReferenceArgument& expression) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	vector<string> variable_result;
	auto result_pairs =
		storage.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == QP::Types::ReferenceType::ExactExpression);
	for (auto const& pair : result_pairs) {
		statement_result.push_back(to_string(pair.first->getIdentifier()));
		variable_result.push_back(pair.second);
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);
	result.addColumn(synonym.getSynonym().symbol, variable_result);

	return result;
}
