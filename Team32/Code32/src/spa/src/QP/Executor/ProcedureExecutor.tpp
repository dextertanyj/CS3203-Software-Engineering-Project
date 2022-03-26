#ifndef SPA_SRC_QP_EXECUTOR_CALLEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_CALLEXECUTOR_TPP

#include "QP/Executor/ProcedureExecutor.h"

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor<T>::executeTrivialNameName(const QP::StorageAdapter& storage,
                                                                           const QP::Types::ReferenceArgument& lhs,
                                                                           const QP::Types::ReferenceArgument& rhs) {
	return QP::QueryResult(storage.checkProcedureRelation<T>(lhs.getName(), rhs.getName()));
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor<T>::executeTrivialNameWildcardOrSynonym(const QP::StorageAdapter& storage,
                                                                                        const Types::ReferenceArgument& lhs) {
	return QueryResult(!storage.getReverseProcedures<T>(lhs.getName()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor<T>::executeTrivialWildcardOrSynonymName(const QP::StorageAdapter& storage,
                                                                                        const Types::ReferenceArgument& rhs) {
	return QueryResult(!storage.template getForwardProcedures<T>(rhs.getName()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor<T>::executeTrivialWildcardOrSynonymWildcardOrSynonym(const QP::StorageAdapter& storage) {
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
QP::QueryResult QP::Executor::ProcedureExecutor<T>::executeTrivialSynonymSynonym(const QP::StorageAdapter& storage,
                                                                                 const Types::ReferenceArgument& lhs,
                                                                                 const Types::ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return {};
	}
	return executeTrivialWildcardOrSynonymWildcardOrSynonym(storage);
}

// Executors
template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor<T>::executeNameSynonym(const QP::StorageAdapter& storage,
                                                                       const Types::ReferenceArgument& lhs,
                                                                       const Types::ReferenceArgument& rhs) {
	QueryResult result = QueryResult({rhs.getSynonym().symbol});
	ProcRefSet rhs_set = storage.getReverseProcedures<T>(lhs.getName());
	for (auto const& rhs_reference : rhs_set) {
		result.addRow({rhs_reference});
	}

	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor<T>::executeWildcardSynonym(const QP::StorageAdapter& storage,
                                                                           const Types::ReferenceArgument& rhs) {
	QueryResult result = QueryResult({rhs.getSynonym().symbol});
	ProcRefSet procedures = storage.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet lhss = storage.getForwardProcedures<T>(procedure);
		if (!lhss.empty()) {
			result.addRow({procedure});
		}
	}

	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor<T>::executeSynonymName(const QP::StorageAdapter& storage,
                                                                       const Types::ReferenceArgument& lhs,
                                                                       const Types::ReferenceArgument& rhs) {
	QueryResult result = QueryResult({lhs.getSynonym().symbol});
	ProcRefSet lhs_set = storage.getForwardProcedures<T>(rhs.getName());
	for (auto const& lhs_reference : lhs_set) {
		result.addRow({lhs_reference});
	}
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor<T>::executeSynonymWildcard(const QP::StorageAdapter& storage,
                                                                           const Types::ReferenceArgument& lhs) {
	QueryResult result = QueryResult({lhs.getSynonym().symbol});
	ProcRefSet procedures = storage.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet rhs_set = storage.getReverseProcedures<T>(procedure);
		if (!rhs_set.empty()) {
			result.addRow({procedure});
		}
	}
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureExecutor<T>::executeSynonymSynonym(const QP::StorageAdapter& storage,
                                                                          const Types::ReferenceArgument& lhs,
                                                                          const Types::ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return {};
	}

	QueryResult result = QueryResult({lhs.getSynonym().symbol, rhs.getSynonym().symbol});

	ProcRefSet procedures = storage.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet rhs_set = storage.getReverseProcedures<T>(procedure);
		for (auto const& rhs_reference : rhs_set) {
			result.addRow({procedure, rhs_reference});
		}
	}
	return result;
}

#endif  // SPA_SRC_QP_EXECUTOR_CALLEXECUTOR_TPP