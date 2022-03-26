#include "PatternAssignExecutor.h"

#include "QP/QueryUtils.h"

// Trivial Executors
QP::QueryResult QP::Executor::PatternAssignExecutor::executeTrivialNameWildcard(const QP::StorageAdapter& storage,
                                                                                const Types::ReferenceArgument& name) {
	return QP::QueryResult(!storage.getStmtsWithPatternLHS(name.getName()).empty());
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeTrivialSynonymOrWildcardWildcard(const QP::StorageAdapter& storage) {
	auto statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (QP::Utilities::checkStmtTypeMatch(statement, Types::DesignEntity::Assign)) {
			return QP::QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeTrivialNameExpression(const QP::StorageAdapter& storage,
                                                                                  const Types::ReferenceArgument& name,
                                                                                  const Types::ReferenceArgument& expression) {
	return QP::QueryResult(storage.patternExists(name.getName(), expression.getExpression(),
	                                             expression.getType() == QP::Types::ReferenceType::ExactExpression));
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeTrivialSynonymOrWildcardExpression(const QP::StorageAdapter& storage,
                                                                                               const Types::ReferenceArgument& expression) {
	return QP::QueryResult(
		!storage.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == QP::Types::ReferenceType::ExactExpression)
			 .empty());
}

// Executors
QP::QueryResult QP::Executor::PatternAssignExecutor::executeNameWildcard(const QP::StorageAdapter& storage,
                                                                         const Types::ReferenceArgument& assign,
                                                                         const Types::ReferenceArgument& name) {
	QP::QueryResult result = QP::QueryResult({assign.getSynonym().symbol});
	auto results = storage.getStmtsWithPatternLHS(name.getName());
	for (auto const& entry : results) {
		result.addRow({to_string(entry->getIdentifier())});
	}

	return result;
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeWildcardWildcard(const QP::StorageAdapter& storage,
                                                                             const Types::ReferenceArgument& assign) {
	QP::QueryResult result = QP::QueryResult({assign.getSynonym().symbol});
	auto results = storage.getStatements();
	for (auto const& entry : results) {
		if (QP::Utilities::checkStmtTypeMatch(entry, Types::DesignEntity::Assign)) {
			result.addRow({to_string(entry->getIdentifier())});
		}
	}

	return result;
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeSynonymWildcard(const QP::StorageAdapter& storage,
                                                                            const Types::ReferenceArgument& assign,
                                                                            const Types::ReferenceArgument& synonym) {
	QP::QueryResult result = QP::QueryResult({assign.getSynonym().symbol, synonym.getSynonym().symbol});
	for (auto const& var_ref : storage.getVariables()) {
		auto statements = storage.getStmtsWithPatternLHS(var_ref);
		for (auto const& statement : statements) {
			result.addRow({to_string(statement->getIdentifier()), var_ref});
		}
	}

	return result;
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeNameExpression(const QP::StorageAdapter& storage,
                                                                           const Types::ReferenceArgument& assign,
                                                                           const Types::ReferenceArgument& name,
                                                                           const Types::ReferenceArgument& expression) {
	QP::QueryResult result = QP::QueryResult({assign.getSynonym().symbol});
	auto results = storage.getStmtsWithPattern(name.getName(), expression.getExpression(),
	                                           expression.getType() == QP::Types::ReferenceType::ExactExpression);
	for (auto const& entry : results) {
		result.addRow({to_string(entry->getIdentifier())});
	}
	return result;
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeWildcardExpression(const QP::StorageAdapter& storage,
                                                                               const Types::ReferenceArgument& assign,
                                                                               const Types::ReferenceArgument& expression) {
	QP::QueryResult result = QP::QueryResult({assign.getSynonym().symbol});
	vector<string> statement_result;
	auto result_pairs =
		storage.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == QP::Types::ReferenceType::ExactExpression);
	for (auto const& pair : result_pairs) {
		result.addRow({to_string(pair.first->getIdentifier())});
	}

	return result;
}

QP::QueryResult QP::Executor::PatternAssignExecutor::executeSynonymExpression(const QP::StorageAdapter& storage,
                                                                              const Types::ReferenceArgument& assign,
                                                                              const Types::ReferenceArgument& synonym,
                                                                              const Types::ReferenceArgument& expression) {
	QP::QueryResult result = QP::QueryResult({assign.getSynonym().symbol, synonym.getSynonym().symbol});
	auto result_pairs =
		storage.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == QP::Types::ReferenceType::ExactExpression);
	for (auto const& pair : result_pairs) {
		result.addRow({to_string(pair.first->getIdentifier()), pair.second});
	}

	return result;
}
