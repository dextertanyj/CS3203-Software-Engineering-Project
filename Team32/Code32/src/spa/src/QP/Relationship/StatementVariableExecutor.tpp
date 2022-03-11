#ifndef SPA_STATEMENTVARIABLEEXECUTOR_TPP
#define SPA_STATEMENTVARIABLEEXECUTOR_TPP

#include "StatementVariableExecutor.h"

#include "QP/QueryUtils.h"

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementVariableExecutor<T>::executeTrivialIndexName(QP::StorageAdapter& pkb,
                                                                                        const Types::ReferenceArgument& index,
                                                                                        const Types::ReferenceArgument& variable) {
	return QueryResult(pkb.checkStatementVariableRelation<T>(index.getStatementIndex(), variable.getName()));
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementVariableExecutor<T>::executeTrivialIndexWildcardOrSynonym(
	QP::StorageAdapter& pkb, const Types::ReferenceArgument& index) {
	return QueryResult(!pkb.getVariableByStatement<T>(index.getStatementIndex()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementVariableExecutor<T>::executeTrivialSynonymName(QP::StorageAdapter& pkb,
                                                                                          const Types::ReferenceArgument& index,
                                                                                          const Types::ReferenceArgument& variable) {
	StmtInfoPtrSet index_set = pkb.getStatementByVariable<T>(variable.getName());
	for (auto const& res_index : index_set) {
		if (Utilities::checkStmtTypeMatch(res_index, index.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementVariableExecutor<T>::executeTrivialSynonymWildcardOrSynonym(
	QP::StorageAdapter& pkb, const Types::ReferenceArgument& index) {
	StmtInfoPtrSet index_set = pkb.getStatements();
	for (auto const& res_index : index_set) {
		if (!Utilities::checkStmtTypeMatch(res_index, index.getSynonym().type)) {
			continue;
		}

		VarRefSet var_set = pkb.getVariableByStatement<T>(res_index->getIdentifier());
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementVariableExecutor<T>::executeSynonymName(QP::StorageAdapter& pkb,
                                                                                   const Types::ReferenceArgument& index,
                                                                                   const Types::ReferenceArgument& variable) {
	StmtInfoPtrSet index_set = pkb.getStatementByVariable<T>(variable.getName());
	vector<string> column;
	for (auto const& res_index : index_set) {
		if (Utilities::checkStmtTypeMatch(res_index, index.getSynonym().type)) {
			column.push_back(to_string(res_index->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(index.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementVariableExecutor<T>::executeSynonymWildcard(QP::StorageAdapter& pkb,
                                                                                       const Types::ReferenceArgument& index) {
	StmtInfoPtrSet index_set = pkb.getStatements();
	vector<string> column;
	for (auto const& res_index : index_set) {
		if (!Utilities::checkStmtTypeMatch(res_index, index.getSynonym().type)) {
			continue;
		}

		VarRefSet var_set = pkb.getVariableByStatement<T>(res_index->getIdentifier());
		if (!var_set.empty()) {
			column.push_back(to_string(res_index->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(index.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementVariableExecutor<T>::executeSynonymSynonym(QP::StorageAdapter& pkb,
                                                                                      const Types::ReferenceArgument& index,
                                                                                      const Types::ReferenceArgument& variable) {
	StmtInfoPtrSet index_set = pkb.getStatements();
	Types::DesignEntity design_variableity = index.getSynonym().type;
	vector<string> index_column;
	vector<string> var_column;
	for (auto const& res_index : index_set) {
		if (!Utilities::checkStmtTypeMatch(res_index, design_variableity)) {
			continue;
		}

		VarRefSet var_set = pkb.getVariableByStatement<T>(res_index->getIdentifier());
		for (auto const& var : var_set) {
			index_column.push_back(to_string(res_index->getIdentifier()));
			var_column.push_back(var);
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(index.getSynonym().symbol, index_column);
	result.addColumn(variable.getSynonym().symbol, var_column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementVariableExecutor<T>::executeIndexSynonym(QP::StorageAdapter& pkb,
                                                                                    const Types::ReferenceArgument& index,
                                                                                    const Types::ReferenceArgument& variable) {
	VarRefSet var_set = pkb.getVariableByStatement<T>(index.getStatementIndex());
	vector<string> column;

	for (auto const& var : var_set) {
		column.push_back(var);
	}

	QueryResult result = QueryResult();
	result.addColumn(variable.getSynonym().symbol, column);
	return result;
}

#endif