#ifndef SPA_SRC_QP_RELATIONSHIP_PARENTDISPATCHERTEMPLATE_TPP
#define SPA_SRC_QP_RELATIONSHIP_PARENTDISPATCHERTEMPLATE_TPP

#include "ParentDispatcher.h"

#include <unordered_map>
#include <utility>

#include "DispatchProcessors.h"
#include "QP/Dispatcher/StatementDispatcher.h"
#include "QP/Executor/StatementRelationExecutor.tpp"

template <QP::Types::ClauseType T>
QP::Types::ArgumentDispatcher QP::Dispatcher::ParentDispatcher<T>::dispatcher =
	[](vector<Types::ReferenceArgument> args) { return argumentDispatcher(T, move(args)); };

template <QP::Types::ClauseType T>
QP::Types::ExecutorSetBundle QP::Dispatcher::ParentDispatcher<T>::argumentDispatcher(Types::ClauseType type,
                                                                                     vector<Types::ReferenceArgument> args) {
	return DispatchProcessors::processDoubleArgument(type, ParentDispatcher<T>::argument_dispatch_map, std::move(args));
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
	     [](vector<Types::ReferenceArgument> args) {
			 return [lhs = args.at(0), rhs = args.at(1)](QP::StorageAdapter& storage) {
				 return Executor::StatementRelationExecutor<T>::executeTrivialIndexIndex(storage, lhs, rhs);
			 };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return [lhs = args.at(0)](QP::StorageAdapter& storage) {
				 return Executor::StatementRelationExecutor<T>::executeTrivialIndexWildcard(storage, lhs);
			 };
		 }},
		{Types::DesignEntity::Stmt, lambda_index_synonym<T>()},
		{Types::DesignEntity::Call, lambda_index_synonym<T>()},
		{Types::DesignEntity::Assign, lambda_index_synonym<T>()},
		{Types::DesignEntity::Print, lambda_index_synonym<T>()},
		{Types::DesignEntity::Read, lambda_index_synonym<T>()},
		{Types::DesignEntity::While, lambda_index_synonym<T>()},
		{Types::DesignEntity::If, lambda_index_synonym<T>()},
	};
	return map;
}

template <QP::Types::ClauseType T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Dispatcher::ParentDispatcher<T>::getWildcardMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::StatementIndex,
	     [](vector<Types::ReferenceArgument> args) {
			 return [rhs = args.at(1)](QP::StorageAdapter& storage) {
				 return Executor::StatementRelationExecutor<T>::executeTrivialWildcardIndex(storage, rhs);
			 };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> /*args*/) {
			 return [](QP::StorageAdapter& storage) {
				 return Executor::StatementRelationExecutor<T>::executeTrivialWildcardWildcard(storage);
			 };
		 }},
		{Types::DesignEntity::Stmt, lambda_wildcard_synonym<T>()},
		{Types::DesignEntity::Call, lambda_wildcard_synonym<T>()},
		{Types::DesignEntity::Assign, lambda_wildcard_synonym<T>()},
		{Types::DesignEntity::Print, lambda_wildcard_synonym<T>()},
		{Types::DesignEntity::Read, lambda_wildcard_synonym<T>()},
		{Types::DesignEntity::While, lambda_wildcard_synonym<T>()},
		{Types::DesignEntity::If, lambda_wildcard_synonym<T>()},
	};
	return map;
}

template <QP::Types::ClauseType T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Dispatcher::ParentDispatcher<T>::getSynonymMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::StatementIndex,
	     [](vector<QP::Types::ReferenceArgument> args) {
			 return pair{[lhs = args.at(0), rhs = args.at(1)](QP::StorageAdapter& storage) {
							 return Executor::StatementRelationExecutor<T>::executeTrivialSynonymIndex(storage, lhs, rhs);
						 },
		                 [lhs = args.at(0), rhs = args.at(1)](QP::StorageAdapter& storage) {
							 return Executor::StatementRelationExecutor<T>::executeSynonymIndex(storage, lhs, rhs);
						 }};
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<QP::Types::ReferenceArgument> args) {
			 return pair{[lhs = args.at(0)](QP::StorageAdapter& storage) {
							 return Executor::StatementRelationExecutor<T>::executeTrivialSynonymWildcard(storage, lhs);
						 },
		                 [lhs = args.at(0)](QP::StorageAdapter& storage) {
							 return Executor::StatementRelationExecutor<T>::executeSynonymWildcard(storage, lhs);
						 }};
		 }},
		{Types::DesignEntity::Stmt, lambda_synonym_synonym<T>()},
		{Types::DesignEntity::Call, lambda_synonym_synonym<T>()},
		{Types::DesignEntity::Assign, lambda_synonym_synonym<T>()},
		{Types::DesignEntity::Print, lambda_synonym_synonym<T>()},
		{Types::DesignEntity::Read, lambda_synonym_synonym<T>()},
		{Types::DesignEntity::While, lambda_synonym_synonym<T>()},
		{Types::DesignEntity::If, lambda_synonym_synonym<T>()},
	};
	return map;
}

#endif  // SPA_SRC_RELATIONSHIP_PARENTDISPATCHERTEMPLATE_TPP