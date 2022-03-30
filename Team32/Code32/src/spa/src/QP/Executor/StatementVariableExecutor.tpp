#ifndef SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_TPP

#include "QP/Executor/StatementVariableExecutor.h"

#include "QP/QueryUtils.h"

namespace QP::Executor::StatementVariableExecutor {

template <ClauseType T>
QueryResult executeTrivialIndexName(const StorageAdapter& storage, const ReferenceArgument& index, const ReferenceArgument& variable) {
	return QueryResult(storage.checkStatementVariableRelation<T>(index.getStatementIndex(), variable.getName()));
}

template <ClauseType T>
QueryResult executeTrivialIndexWildcardOrSynonym(const StorageAdapter& storage, const ReferenceArgument& index) {
	return QueryResult(!storage.getVariableByStatement<T>(index.getStatementIndex()).empty());
}

template <ClauseType T>
QueryResult executeTrivialSynonymName(const StorageAdapter& storage, const ReferenceArgument& index, const ReferenceArgument& variable) {
	StmtInfoPtrSet index_set = storage.getStatementByVariable<T>(variable.getName());
	for (auto const& res_index : index_set) {
		if (Utilities::checkStmtTypeMatch(res_index, index.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeTrivialSynonymWildcardOrSynonym(const StorageAdapter& storage, const ReferenceArgument& index) {
	StmtInfoPtrSet index_set = storage.getStatements();
	for (auto const& res_index : index_set) {
		if (!Utilities::checkStmtTypeMatch(res_index, index.getSynonym().type)) {
			continue;
		}

		VarRefSet var_set = storage.getVariableByStatement<T>(res_index->getIdentifier());
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeSynonymName(const StorageAdapter& storage, const ReferenceArgument& index, const ReferenceArgument& variable) {
	QueryResult result = QueryResult({index.getSynonym().symbol});
	StmtInfoPtrSet index_set = storage.getStatementByVariable<T>(variable.getName());
	vector<string> column;
	for (auto const& res_index : index_set) {
		if (Utilities::checkStmtTypeMatch(res_index, index.getSynonym().type)) {
			result.addRow({to_string(res_index->getIdentifier())});
		}
	}
	return result;
}

template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& storage, const ReferenceArgument& index) {
	QueryResult result = QueryResult({index.getSynonym().symbol});
	StmtInfoPtrSet index_set = storage.getStatements();
	for (auto const& res_index : index_set) {
		if (!Utilities::checkStmtTypeMatch(res_index, index.getSynonym().type)) {
			continue;
		}

		VarRefSet var_set = storage.getVariableByStatement<T>(res_index->getIdentifier());
		if (!var_set.empty()) {
			result.addRow({to_string(res_index->getIdentifier())});
		}
	}
	return result;
}

template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& storage, const ReferenceArgument& index, const ReferenceArgument& variable) {
	QueryResult result = QueryResult({index.getSynonym().symbol, variable.getSynonym().symbol});
	StmtInfoPtrSet index_set = storage.getStatements();
	DesignEntity design_entity = index.getSynonym().type;
	for (auto const& res_index : index_set) {
		if (!Utilities::checkStmtTypeMatch(res_index, design_entity)) {
			continue;
		}

		VarRefSet var_set = storage.getVariableByStatement<T>(res_index->getIdentifier());
		for (auto const& var : var_set) {
			result.addRow({to_string(res_index->getIdentifier()), var});
		}
	}
	return result;
}

template <ClauseType T>
QueryResult executeIndexSynonym(const StorageAdapter& storage, const ReferenceArgument& index, const ReferenceArgument& variable) {
	QueryResult result = QueryResult({variable.getSynonym().symbol});
	VarRefSet var_set = storage.getVariableByStatement<T>(index.getStatementIndex());

	for (auto const& var : var_set) {
		result.addRow({var});
	}

	return result;
}

template <ClauseType T>
ExecutorSet executorFactoryIndexName(const vector<ReferenceArgument>& args) {
	return [index = args.at(0), variable = args.at(1)](const StorageAdapter& storage) {
		return executeTrivialIndexName<T>(storage, index, variable);
	};
}

template <ClauseType T>
ExecutorSet executorFactoryIndexWildcard(const vector<ReferenceArgument>& args) {
	return [index = args.at(0)](const StorageAdapter& storage) { return executeTrivialIndexWildcardOrSynonym<T>(storage, index); };
}

template <ClauseType T>
ExecutorSet executorFactoryIndexSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [index = args.at(0)](const StorageAdapter& storage) {
		return executeTrivialIndexWildcardOrSynonym<T>(storage, index);
	};
	Types::Executor executor = [index = args.at(0), variable = args.at(1)](const StorageAdapter& storage) {
		return executeIndexSynonym<T>(storage, index, variable);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymName(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [statement = args.at(0), variable = args.at(1)](const StorageAdapter& storage) {
		return executeTrivialSynonymName<T>(storage, statement, variable);
	};
	Types::Executor executor = [statement = args.at(0), variable = args.at(1)](const StorageAdapter& storage) {
		return executeSynonymName<T>(storage, statement, variable);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [statement = args.at(0)](const StorageAdapter& storage) {
		return executeTrivialSynonymWildcardOrSynonym<T>(storage, statement);
	};
	Types::Executor executor = [statement = args.at(0)](const StorageAdapter& storage) {
		return executeSynonymWildcard<T>(storage, statement);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [statement = args.at(0)](const StorageAdapter& storage) {
		return executeTrivialSynonymWildcardOrSynonym<T>(storage, statement);
	};
	Types::Executor executor = [statement = args.at(0), variable = args.at(1)](const StorageAdapter& storage) {
		return executeSynonymSynonym<T>(storage, statement, variable);
	};
	return pair{trivial_executor, executor};
}

}

#endif  // SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_TPP