#include "QP/Executor/AttributeExecutor.h"

#include "QP/QueryUtils.h"

QP::Types::Name QP::Executor::AttributeExecutor::extractName(const Types::ReferenceArgument& argument) { return argument.getName(); };

QP::Types::Number QP::Executor::AttributeExecutor::extractNumber(const Types::ReferenceArgument& argument) {
	return argument.getStatementIndex();
};

std::unordered_set<QP::Types::Number> QP::Executor::AttributeExecutor::selectStatements(const QP::StorageAdapter& store,
                                                                                        const Types::ReferenceArgument& argument) {
	Types::Declaration synonym = argument.getAttribute().synonym;
	StmtInfoPtrSet statements = store.getStatements();
	std::unordered_set<Types::Number> result;
	for (const auto& statement : statements) {
		if (!QP::Utilities::checkStmtTypeMatch(statement, synonym.type)) {
			continue;
		}
		result.insert(statement->getIdentifier());
	}
	return result;
}

std::unordered_set<QP::Types::Number> QP::Executor::AttributeExecutor::selectConstants(const QP::StorageAdapter& store,
                                                                                       const Types::ReferenceArgument& /*unused*/) {
	return store.getConstants();
}

std::unordered_set<QP::Types::Name> QP::Executor::AttributeExecutor::selectProcedures(const QP::StorageAdapter& store,
                                                                                      const Types::ReferenceArgument& /*unused*/) {
	return store.getProcedures();
}

std::unordered_set<QP::Types::Name> QP::Executor::AttributeExecutor::selectVariables(const QP::StorageAdapter& store,
                                                                                     const Types::ReferenceArgument& /*unused*/) {
	return store.getVariables();
}

template <typename T>
T QP::Executor::AttributeExecutor::identity(const QP::StorageAdapter& /*store*/, const T& value) {
	return value;
}

QP::Types::Name QP::Executor::AttributeExecutor::callToProcedure(const QP::StorageAdapter& store, const StmtRef& value) {
	return store.getCalledProcedure(value);
}

template <QP::Types::ClauseType T>
QP::Types::Name QP::Executor::AttributeExecutor::statementToVariable(const QP::StorageAdapter& store, const StmtRef& value) {
	auto variable_set = store.getVariableByStatement<T>(value);
	if (variable_set.empty()) {
		throw QP::QueryException("Corresponding variable not found.");
	}
	return *variable_set.begin();
}