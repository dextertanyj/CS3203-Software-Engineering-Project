#ifndef SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_TPP

#include "QP/Executor/PatternContainerStatementExecutor.h"

#include "QP/QueryUtils.h"

namespace QP::Executor::PatternContainerStatementExecutor {

// Trivial Executors
template <ClauseType T>
QueryResult executeTrivialName(const StorageAdapter& store, const ReferenceArgument& var) {
	return QueryResult(!store.getControlStmt<T>(var.getName()).empty());
}

template <ClauseType T>
QueryResult executeTrivialWildcardOrSynonym(const StorageAdapter& store) {
	VarRefSet var_set = store.getVariables();
	for (auto const& var : var_set) {
		StmtInfoPtrSet stmt_set = store.getControlStmt<T>(var);
		if (!stmt_set.empty()) {
			return QueryResult(true);
		}
	}

	return {};
}

// Executors
template <ClauseType T>
QueryResult executeName(const StorageAdapter& store, const ReferenceArgument& stmt, const ReferenceArgument& var) {
	QueryResult result = QueryResult({stmt.getSynonym().symbol});
	StmtInfoPtrSet stmt_set = store.getControlStmt<T>(var.getName());
	for (auto const& stmt_ref : stmt_set) {
		result.addRow({to_string(stmt_ref->getIdentifier())});
	}

	return result;
}

template <ClauseType T>
QueryResult executeWildcard(const StorageAdapter& store, const ReferenceArgument& stmt) {
	QueryResult result = QueryResult({stmt.getSynonym().symbol});
	StmtInfoPtrSet stmt_set = store.getStatements();
	for (auto const& stmt_ref : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(stmt_ref, stmt.getSynonym().type)) {
			continue;
		}

		VarRefSet var_set = store.getControlVar<T>(stmt_ref->getIdentifier());
		if (!var_set.empty()) {
			result.addRow({to_string(stmt_ref->getIdentifier())});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonym(const StorageAdapter& store, const ReferenceArgument& stmt, const ReferenceArgument& var) {
	QueryResult result = QueryResult({stmt.getSynonym().symbol, var.getSynonym().symbol});
	VarRefSet var_set = store.getVariables();
	for (auto const& var_ref : var_set) {
		StmtInfoPtrSet stmt_set = store.getControlStmt<T>(var_ref);
		for (auto const& stmt_ref : stmt_set) {
			result.addRow({to_string(stmt_ref->getIdentifier()), var_ref});
		}
	}
	return result;
}

// Executor Set Factories

template <ClauseType T>
ExecutorSet executorFactoryName(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [variable = args.at(1)](const StorageAdapter& store) {
		return executeTrivialName<T>(store, variable);
	};
	Types::Executor executor = [statement = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeName<T>(store, statement, variable);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactoryWildcard(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& store) { return executeTrivialWildcardOrSynonym<T>(store); };
	Types::Executor executor = [statement = args.at(0)](const StorageAdapter& store) { return executeWildcard<T>(store, statement); };
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& store) { return executeTrivialWildcardOrSynonym<T>(store); };
	Types::Executor executor = [statement = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeSynonym<T>(store, statement, variable);
	};
	return pair{trivial_executor, executor};
}

}

#endif  // SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_TPP
