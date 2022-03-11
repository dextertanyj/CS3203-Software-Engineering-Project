#ifndef SPA_SRC_QP_RELATIONSHIP_PROCEDUREVARIABLEEXECUTOR_TPP
#define SPA_SRC_QP_RELATIONSHIP_PROCEDUREVARIABLEEXECUTOR_TPP

#include "ProcedureVariableExecutor.h"

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::ProcedureVariableExecutor<T>::executeTrivialNameName(QP::StorageAdapter &pkb,
                                                                                       const Types::ReferenceArgument &left_ent,
                                                                                       const Types::ReferenceArgument &right_ent) {
	return QueryResult(pkb.checkProcedureVariableRelation<T>(left_ent.getName(), right_ent.getName()));
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::ProcedureVariableExecutor<T>::executeTrivialNameWildcardOrSynonym(
	QP::StorageAdapter &pkb, const Types::ReferenceArgument &left_ent) {
	return QueryResult(!pkb.getVariableByProcedure<T>(left_ent.getName()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::ProcedureVariableExecutor<T>::executeTrivialSynonymName(QP::StorageAdapter &pkb,
                                                                                          const Types::ReferenceArgument &right_ent) {
	return QueryResult(!pkb.getProcedureByVariable<T>(right_ent.getName()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::ProcedureVariableExecutor<T>::executeTrivialSynonymWildcardOrSynonym(QP::StorageAdapter &pkb) {
	unordered_set<ProcRef> proc_set = pkb.getProcedures();
	for (auto const &proc : proc_set) {
		VarRefSet var_set = pkb.getProcedureByVariable<T>(proc);
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::ProcedureVariableExecutor<T>::executeNameSynonym(QP::StorageAdapter &pkb,
                                                                                   const Types::ReferenceArgument &left_ent,
                                                                                   const Types::ReferenceArgument &right_ent) {
	VarRefSet var_set = pkb.getVariableByProcedure<T>(left_ent.getName());
	vector<string> column;

	for (auto const &var : var_set) {
		column.push_back(var);
	}

	QueryResult result = QueryResult();
	result.addColumn(right_ent.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::ProcedureVariableExecutor<T>::executeSynonymName(QP::StorageAdapter &pkb,
                                                                                   const Types::ReferenceArgument &left_ent,
                                                                                   const Types::ReferenceArgument &right_ent) {
	ProcRefSet proc_set = pkb.getProcedureByVariable<T>(right_ent.getName());
	vector<string> column;
	for (auto const &proc : proc_set) {
		column.push_back(proc);
	}
	QueryResult result = QueryResult();
	result.addColumn(left_ent.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::ProcedureVariableExecutor<T>::executeSynonymWildcard(QP::StorageAdapter &pkb,
                                                                                       const Types::ReferenceArgument &left_ent) {
	unordered_set<ProcRef> proc_set = pkb.getProcedures();
	vector<string> column;
	for (auto const &proc : proc_set) {
		VarRefSet var_set = pkb.getVariableByProcedure<T>(proc);
		if (!var_set.empty()) {
			column.push_back(proc);
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(left_ent.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::ProcedureVariableExecutor<T>::executeSynonymSynonym(QP::StorageAdapter &pkb,
                                                                                      const Types::ReferenceArgument &left_ent,
                                                                                      const Types::ReferenceArgument &right_ent) {
	unordered_set<ProcRef> proc_set = pkb.getProcedures();
	vector<string> proc_column;
	vector<string> var_column;
	for (auto const &proc : proc_set) {
		VarRefSet var_set = pkb.getVariableByProcedure<T>(proc);
		for (auto const &var : var_set) {
			proc_column.push_back(proc);
			var_column.push_back(var);
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(left_ent.getSynonym().symbol, proc_column);
	result.addColumn(right_ent.getSynonym().symbol, var_column);
	return result;
}

#endif