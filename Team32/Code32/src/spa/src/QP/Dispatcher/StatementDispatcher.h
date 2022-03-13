#ifndef SPA_STATEMENTDISPATCHER_H
#define SPA_STATEMENTDISPATCHER_H

#include "QP/Executor/Executor.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"

template <QP::Types::ClauseType T>
static QP::Types::ExecutorSetFactory lambda_index_synonym() {
	static const QP::Types::ExecutorSetFactory lambda = [](vector<QP::Types::ReferenceArgument> args) {
		return pair{[lhs = args.at(0), rhs = args.at(1)](QP::StorageAdapter& storage) {
						return QP::Executor::StatementExecutor<T>::executeTrivialIndexSynonym(storage, lhs, rhs);
					},
		            [lhs = args.at(0), rhs = args.at(1)](QP::StorageAdapter& storage) {
						return QP::Executor::StatementExecutor<T>::executeIndexSynonym(storage, lhs, rhs);
					}};
	};
	return lambda;
}

template <QP::Types::ClauseType T>
static QP::Types::ExecutorSetFactory lambda_wildcard_synonym() {
	static const QP::Types::ExecutorSetFactory lambda = [](vector<QP::Types::ReferenceArgument> args) {
		return pair{[rhs = args.at(1)](QP::StorageAdapter& storage) {
						return QP::Executor::StatementExecutor<T>::executeTrivialWildcardSynonym(storage, rhs);
					},
		            [rhs = args.at(1)](QP::StorageAdapter& storage) {
						return QP::Executor::StatementExecutor<T>::executeWildcardSynonym(storage, rhs);
					}};
	};
	return lambda;
}

template <QP::Types::ClauseType T>
static QP::Types::ExecutorSetFactory lambda_synonym_synonym() {
	static const QP::Types::ExecutorSetFactory lambda = [](vector<QP::Types::ReferenceArgument> args) {
		return pair{[lhs = args.at(0), rhs = args.at(1)](QP::StorageAdapter& storage) {
						return QP::Executor::StatementExecutor<T>::executeTrivialSynonymSynonym(storage, lhs, rhs);
					},
		            [lhs = args.at(0), rhs = args.at(1)](QP::StorageAdapter& storage) {
						return QP::Executor::StatementExecutor<T>::executeSynonymSynonym(storage, lhs, rhs);
					}};
	};
	return lambda;
}

#endif  // SPA_STATEMENTDISPATCHER_H
