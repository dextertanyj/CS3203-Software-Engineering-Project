#ifndef SPA_SRC_QP_DISPATCHER_PARENTDISPATCHER_TPP
#define SPA_SRC_QP_DISPATCHER_PARENTDISPATCHER_TPP

#include "QP/Dispatcher/ParentDispatcher.h"

#include <unordered_map>
#include <utility>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Dispatcher/StatementDispatcher.h"
#include "QP/Executor/StatementExecutor.tpp"

template <QP::Types::ClauseType T>
QP::Types::ArgumentDispatcher QP::Dispatcher::ParentDispatcher<T>::dispatcher =
	[](const vector<Types::ReferenceArgument>& args) { return argumentDispatcher(T, move(args)); };

template <QP::Types::ClauseType T>
QP::Types::ExecutorSetBundle QP::Dispatcher::ParentDispatcher<T>::argumentDispatcher(Types::ClauseType type,
                                                                                     const vector<Types::ReferenceArgument>& args) {
	return DispatchProcessors::processArgument(type, ParentDispatcher<T>::argument_dispatch_map, args);
};

template <QP::Types::ClauseType T>
const unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>
	QP::Dispatcher::ParentDispatcher<T>::argument_dispatch_map = {{Types::ReferenceType::StatementIndex, getIndexMap()},
                                                                  {Types::ReferenceType::Wildcard, getWildcardMap()},
                                                                  {Types::DesignEntity::Stmt, getSynonymMap()},
                                                                  {Types::DesignEntity::While, getSynonymMap()},
                                                                  {Types::DesignEntity::If, getSynonymMap()}};

template <QP::Types::ClauseType T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Dispatcher::ParentDispatcher<T>::getIndexMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::StatementIndex,
	     [](const vector<Types::ReferenceArgument>& args) {
			 return [lhs = args.at(0), rhs = args.at(1)](const QP::StorageAdapter& storage) {
				 return Executor::StatementExecutor<T>::executeTrivialIndexIndex(storage, lhs, rhs);
			 };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](const vector<Types::ReferenceArgument>& args) {
			 return [lhs = args.at(0)](const QP::StorageAdapter& storage) {
				 return Executor::StatementExecutor<T>::executeTrivialIndexWildcard(storage, lhs);
			 };
		 }},
		{Types::DesignEntity::Stmt, lambdaIndexSynonym<T>()},
		{Types::DesignEntity::Call, lambdaIndexSynonym<T>()},
		{Types::DesignEntity::Assign, lambdaIndexSynonym<T>()},
		{Types::DesignEntity::Print, lambdaIndexSynonym<T>()},
		{Types::DesignEntity::Read, lambdaIndexSynonym<T>()},
		{Types::DesignEntity::While, lambdaIndexSynonym<T>()},
		{Types::DesignEntity::If, lambdaIndexSynonym<T>()},
	};
	return map;
}

template <QP::Types::ClauseType T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Dispatcher::ParentDispatcher<T>::getWildcardMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::StatementIndex,
	     [](const vector<Types::ReferenceArgument>& args) {
			 return [rhs = args.at(1)](const QP::StorageAdapter& storage) {
				 return Executor::StatementExecutor<T>::executeTrivialWildcardIndex(storage, rhs);
			 };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](const vector<Types::ReferenceArgument>& /*args*/) {
			 return
				 [](const QP::StorageAdapter& storage) { return Executor::StatementExecutor<T>::executeTrivialWildcardWildcard(storage); };
		 }},
		{Types::DesignEntity::Stmt, lambdaWildcardSynonym<T>()},
		{Types::DesignEntity::Call, lambdaWildcardSynonym<T>()},
		{Types::DesignEntity::Assign, lambdaWildcardSynonym<T>()},
		{Types::DesignEntity::Print, lambdaWildcardSynonym<T>()},
		{Types::DesignEntity::Read, lambdaWildcardSynonym<T>()},
		{Types::DesignEntity::While, lambdaWildcardSynonym<T>()},
		{Types::DesignEntity::If, lambdaWildcardSynonym<T>()},
	};
	return map;
}

template <QP::Types::ClauseType T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Dispatcher::ParentDispatcher<T>::getSynonymMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::StatementIndex,
	     [](const vector<QP::Types::ReferenceArgument>& args) {
			 return pair{[lhs = args.at(0), rhs = args.at(1)](const QP::StorageAdapter& storage) {
							 return Executor::StatementExecutor<T>::executeTrivialSynonymIndex(storage, lhs, rhs);
						 },
		                 [lhs = args.at(0), rhs = args.at(1)](const QP::StorageAdapter& storage) {
							 return Executor::StatementExecutor<T>::executeSynonymIndex(storage, lhs, rhs);
						 }};
		 }},
		{Types::ReferenceType::Wildcard,
	     [](const vector<QP::Types::ReferenceArgument>& args) {
			 return pair{[lhs = args.at(0)](const QP::StorageAdapter& storage) {
							 return Executor::StatementExecutor<T>::executeTrivialSynonymWildcard(storage, lhs);
						 },
		                 [lhs = args.at(0)](const QP::StorageAdapter& storage) {
							 return Executor::StatementExecutor<T>::executeSynonymWildcard(storage, lhs);
						 }};
		 }},
		{Types::DesignEntity::Stmt, lambdaSynonymSynonym<T>()},
		{Types::DesignEntity::Call, lambdaSynonymSynonym<T>()},
		{Types::DesignEntity::Assign, lambdaSynonymSynonym<T>()},
		{Types::DesignEntity::Print, lambdaSynonymSynonym<T>()},
		{Types::DesignEntity::Read, lambdaSynonymSynonym<T>()},
		{Types::DesignEntity::While, lambdaSynonymSynonym<T>()},
		{Types::DesignEntity::If, lambdaSynonymSynonym<T>()},
	};
	return map;
}

#endif  // SPA_SRC_QP_DISPATCHER_PARENTDISPATCHER_TPP