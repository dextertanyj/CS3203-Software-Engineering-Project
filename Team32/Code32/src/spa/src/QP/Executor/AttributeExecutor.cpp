#include "QP/Executor/AttributeExecutor.h"

#include "Common/TypeDefs.h"
#include "QP/QueryUtils.h"

using namespace std;

unordered_set<QP::Types::Name> QP::Executor::AttributeExecutor::extractName(const StorageAdapter& /*store*/,
                                                                            const ClauseArgument& argument) {
	return {argument.getName()};
}

unordered_set<QP::Types::Number> QP::Executor::AttributeExecutor::extractNumber(const StorageAdapter& /*store*/,
                                                                                const ClauseArgument& argument) {
	return {argument.getStatementIndex()};
}

unordered_set<QP::Types::Number> QP::Executor::AttributeExecutor::selectStatements(const StorageAdapter& store,
                                                                                   const ClauseArgument& argument) {
	Types::DesignEntity synonym = argument.getSynonymType();
	StmtInfoPtrSet statements = store.getStatements();
	unordered_set<Types::Number> result;
	for (const auto& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, synonym)) {
			continue;
		}
		result.emplace(statement->getIdentifier());
	}
	return result;
}

unordered_set<QP::Types::Number> QP::Executor::AttributeExecutor::selectConstants(const StorageAdapter& store,
                                                                                  const ClauseArgument& /*unused*/) {
	return store.getConstants();
}

unordered_set<QP::Types::Name> QP::Executor::AttributeExecutor::selectProcedures(const StorageAdapter& store,
                                                                                 const ClauseArgument& /*unused*/) {
	return store.getProcedures();
}

unordered_set<QP::Types::Name> QP::Executor::AttributeExecutor::selectVariables(const StorageAdapter& store,
                                                                                const ClauseArgument& /*unused*/) {
	return store.getVariables();
}

QP::Types::Name QP::Executor::AttributeExecutor::callToProcedure(const StorageAdapter& store, const Types::Number& value) {
	return store.getCalledProcedure(value);
}
