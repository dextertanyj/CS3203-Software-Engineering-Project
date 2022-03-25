#ifndef SPA_SRC_QP_EXECUTOR_CALLEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_CALLEXECUTOR_TPP

#include "QP/Executor/ProcedureExecutor.h"

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor::executeTrivialNameName(const QP::StorageAdapter& storage,
                                                                           const QP::Types::ReferenceArgument& lhs,
                                                                           const QP::Types::ReferenceArgument& rhs) {
	return QP::QueryResult(storage.checkProcedureRelation<T>(lhs.getName(), rhs.getName()));
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor::executeTrivialNameWildcardOrSynonym(const QP::StorageAdapter& storage,
                                                                                        const Types::ReferenceArgument& lhs) {
	return QueryResult(!storage.getReverseProcedures<T>(lhs.getName()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor::executeTrivialWildcardOrSynonymName(const QP::StorageAdapter& storage,
                                                                                        const Types::ReferenceArgument& rhs) {
	return QueryResult(!storage.template getForwardProcedures<T>(rhs.getName()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor::executeTrivialWildcardOrSynonymWildcardOrSynonym(const QP::StorageAdapter& storage) {
	ProcRefSet proc_set = storage.getProcedures();
	for (auto const& proc : proc_set) {
		ProcRefSet lhs_set = storage.getForwardProcedures<T>(proc);
		if (!lhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor::executeTrivialSynonymSynonym(const QP::StorageAdapter& storage,
                                                                                 const Types::ReferenceArgument& lhs,
                                                                                 const Types::ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return {};
	}
	return executeTrivialWildcardOrSynonymWildcardOrSynonym<T>(storage);
}

// Executors
template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor::executeNameSynonym(const QP::StorageAdapter& storage,
                                                                       const Types::ReferenceArgument& lhs,
                                                                       const Types::ReferenceArgument& rhs) {
	QueryResult result = QueryResult();
	vector<string> column;
	ProcRefSet rhs_set = storage.getReverseProcedures<T>(lhs.getName());
	for (auto const& rhs_reference : rhs_set) {
		column.push_back(rhs_reference);
	}

	result.addColumn(rhs.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor::executeWildcardSynonym(const QP::StorageAdapter& storage,
                                                                           const Types::ReferenceArgument& rhs) {
	QueryResult result = QueryResult();
	vector<string> column;
	ProcRefSet procedures = storage.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet lhss = storage.getForwardProcedures<T>(procedure);
		if (!lhss.empty()) {
			column.push_back(procedure);
		}
	}

	result.addColumn(rhs.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor::executeSynonymName(const QP::StorageAdapter& storage,
                                                                       const Types::ReferenceArgument& lhs,
                                                                       const Types::ReferenceArgument& rhs) {
	QueryResult result = QueryResult();
	vector<string> lhs_column;
	ProcRefSet lhs_set = storage.getForwardProcedures<T>(rhs.getName());
	for (auto const& lhs_reference : lhs_set) {
		lhs_column.push_back(lhs_reference);
	}
	result.addColumn(lhs.getSynonym().symbol, lhs_column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor::executeSynonymWildcard(const QP::StorageAdapter& storage,
                                                                           const Types::ReferenceArgument& lhs) {
	QueryResult result = QueryResult();
	vector<string> lhs_column;
	ProcRefSet procedures = storage.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet rhs_set = storage.getReverseProcedures<T>(procedure);
		if (!rhs_set.empty()) {
			lhs_column.push_back(procedure);
		}
	}
	result.addColumn(lhs.getSynonym().symbol, lhs_column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor::executeSynonymSynonym(const QP::StorageAdapter& storage,
                                                                          const Types::ReferenceArgument& lhs,
                                                                          const Types::ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return {};
	}

	QueryResult result = QueryResult();
	vector<string> lhs_column;
	vector<string> rhs_column;

	ProcRefSet procedures = storage.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet rhs_set = storage.getReverseProcedures<T>(procedure);
		for (auto const& rhs_reference : rhs_set) {
			lhs_column.push_back(procedure);
			rhs_column.push_back(rhs_reference);
		}
	}

	result.addColumn(lhs.getSynonym().symbol, lhs_column);
	result.addColumn(rhs.getSynonym().symbol, rhs_column);
	return result;
}

#endif  // SPA_SRC_QP_EXECUTOR_CALLEXECUTOR_TPP