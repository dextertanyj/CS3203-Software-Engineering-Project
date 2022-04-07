#ifndef SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_TPP

#include "QP/Executor/StatementVariableExecutor.h"

#include "QP/QueryUtils.h"

namespace QP::Executor::StatementVariableExecutor {

using namespace std;

template <ClauseType T>
QueryResult executeTrivialIndexName(const StorageAdapter& store, const ClauseArgument& index, const ClauseArgument& variable) {
	return QueryResult(store.checkStatementVariableRelation<T>(index.getStatementIndex(), variable.getName()));
}

template <ClauseType T>
QueryResult executeTrivialIndexWildcardOrSynonym(const StorageAdapter& store, const ClauseArgument& index) {
	return QueryResult(!store.getVariableByStatement<T>(index.getStatementIndex()).empty());
}

template <ClauseType T>
QueryResult executeTrivialSynonymName(const StorageAdapter& store, const ClauseArgument& index, const ClauseArgument& variable) {
	StmtInfoPtrSet index_set = store.getStatementByVariable<T>(variable.getName());
	for (const auto& res_index : index_set) {
		if (Utilities::checkStmtTypeMatch(res_index, index.getSynonymType())) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeTrivialSynonymWildcardOrSynonym(const StorageAdapter& store, const ClauseArgument& index) {
	StmtInfoPtrSet index_set = store.getStatements();
	for (const auto& res_index : index_set) {
		if (!Utilities::checkStmtTypeMatch(res_index, index.getSynonymType())) {
			continue;
		}

		VarRefSet var_set = store.getVariableByStatement<T>(res_index->getIdentifier());
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeSynonymName(const StorageAdapter& store, const ClauseArgument& index, const ClauseArgument& variable) {
	QueryResult result = QueryResult({index.getSynonymSymbol()});
	StmtInfoPtrSet index_set = store.getStatementByVariable<T>(variable.getName());
	vector<string> column;
	for (const auto& res_index : index_set) {
		if (Utilities::checkStmtTypeMatch(res_index, index.getSynonymType())) {
			result.addRow({to_string(res_index->getIdentifier())});
		}
	}
	return result;
}

template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& store, const ClauseArgument& index) {
	QueryResult result = QueryResult({index.getSynonymSymbol()});
	StmtInfoPtrSet index_set = store.getStatements();
	for (const auto& res_index : index_set) {
		if (!Utilities::checkStmtTypeMatch(res_index, index.getSynonymType())) {
			continue;
		}

		VarRefSet var_set = store.getVariableByStatement<T>(res_index->getIdentifier());
		if (!var_set.empty()) {
			result.addRow({to_string(res_index->getIdentifier())});
		}
	}
	return result;
}

template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& store, const ClauseArgument& index, const ClauseArgument& variable) {
	QueryResult result = QueryResult({index.getSynonymSymbol(), variable.getSynonymSymbol()});
	StmtInfoPtrSet index_set = store.getStatements();
	DesignEntity design_entity = index.getSynonymType();
	for (const auto& res_index : index_set) {
		if (!Utilities::checkStmtTypeMatch(res_index, design_entity)) {
			continue;
		}

		VarRefSet var_set = store.getVariableByStatement<T>(res_index->getIdentifier());
		for (const auto& var : var_set) {
			result.addRow({to_string(res_index->getIdentifier()), var});
		}
	}
	return result;
}

template <ClauseType T>
QueryResult executeIndexSynonym(const StorageAdapter& store, const ClauseArgument& index, const ClauseArgument& variable) {
	QueryResult result = QueryResult({variable.getSynonymSymbol()});
	VarRefSet var_set = store.getVariableByStatement<T>(index.getStatementIndex());

	for (const auto& var : var_set) {
		result.addRow({var});
	}

	return result;
}

template <ClauseType T>
ExecutorSet executorFactoryIndexName(const vector<ClauseArgument>& args) {
	return [index = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeTrivialIndexName<T>(store, index, variable);
	};
}

template <ClauseType T>
ExecutorSet executorFactoryIndexWildcard(const vector<ClauseArgument>& args) {
	return [index = args.at(0)](const StorageAdapter& store) { return executeTrivialIndexWildcardOrSynonym<T>(store, index); };
}

template <ClauseType T>
ExecutorSet executorFactoryIndexSynonym(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [index = args.at(0)](const StorageAdapter& store) {
		return executeTrivialIndexWildcardOrSynonym<T>(store, index);
	};
	Types::Executor executor = [index = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeIndexSynonym<T>(store, index, variable);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymName(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [statement = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeTrivialSynonymName<T>(store, statement, variable);
	};
	Types::Executor executor = [statement = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeSynonymName<T>(store, statement, variable);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [statement = args.at(0)](const StorageAdapter& store) {
		return executeTrivialSynonymWildcardOrSynonym<T>(store, statement);
	};
	Types::Executor executor = [statement = args.at(0)](const StorageAdapter& store) {
		return executeSynonymWildcard<T>(store, statement);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [statement = args.at(0)](const StorageAdapter& store) {
		return executeTrivialSynonymWildcardOrSynonym<T>(store, statement);
	};
	Types::Executor executor = [statement = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeSynonymSynonym<T>(store, statement, variable);
	};
	return pair{trivial_executor, executor};
}

}

#endif  // SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_TPP