#ifndef SPA_SRC_QP_RELATIONSHIP_PARENTDISPATCHERTEMPLATE_TPP
#define SPA_SRC_QP_RELATIONSHIP_PARENTDISPATCHERTEMPLATE_TPP

#include "QP/Relationship/ParentDispatcher.h"

#include <unordered_map>
#include <utility>

#include "QP/DispatchProcessors.h"
#include "QP/Relationship/StatementRelationExecutor.tpp"

template <QP::Types::ClauseType T>
QP::Types::ArgumentDispatcher QP::Relationship::ParentDispatcher<T>::dispatcher =
	[](vector<Types::ReferenceArgument> args) { return argumentDispatcher(T, move(args)); };

template <QP::Types::ClauseType T>
QP::Types::ExecutorSetBundle QP::Relationship::ParentDispatcher<T>::argumentDispatcher(Types::ClauseType type,
                                                                                               vector<Types::ReferenceArgument> args) {
	return DispatchProcessors::processDoubleArgument(type, ParentDispatcher<T>::argument_dispatch_map, std::move(args));
};

template <QP::Types::ClauseType T>
const unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>
	QP::Relationship::ParentDispatcher<T>::argument_dispatch_map = {{Types::ReferenceType::StatementIndex, getIndexMap()},
                                                                            {Types::ReferenceType::Wildcard, getWildcardMap()},
                                                                            {Types::DesignEntity::Stmt, getSynonymMap()},
                                                                            {Types::DesignEntity::While, getSynonymMap()},
                                                                            {Types::DesignEntity::If, getSynonymMap()}};

template <QP::Types::ClauseType T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Relationship::ParentDispatcher<T>::getIndexMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::StatementIndex,
	     [](vector<Types::ReferenceArgument> args) {
			 return [parent = args.at(0), child = args.at(1)](QP::StorageAdapter& pkb) {
				 return StatementRelationExecutor<T>::executeTrivialIndexIndex(pkb, parent, child);
			 };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return [parent = args.at(0)](QP::StorageAdapter& pkb) {
				 return StatementRelationExecutor<T>::executeTrivialIndexWildcard(pkb, parent);
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
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>
QP::Relationship::ParentDispatcher<T>::getWildcardMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::StatementIndex,
	     [](vector<Types::ReferenceArgument> args) {
			 return [child = args.at(1)](QP::StorageAdapter& pkb) {
				 return StatementRelationExecutor<T>::executeTrivialWildcardIndex(pkb, child);
			 };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> /*args*/) {
			 return [](QP::StorageAdapter& pkb) { return StatementRelationExecutor<T>::executeTrivialWildcardWildcard(pkb); };
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
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>
QP::Relationship::ParentDispatcher<T>::getSynonymMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::StatementIndex,
	     [](vector<QP::Types::ReferenceArgument> args) {
			 return pair{[parent = args.at(0), child = args.at(1)](QP::StorageAdapter& pkb) {
							 return StatementRelationExecutor<T>::executeTrivialSynonymIndex(pkb, parent, child);
						 },
		                 [parent = args.at(0), child = args.at(1)](QP::StorageAdapter& pkb) {
							 return StatementRelationExecutor<T>::executeSynonymIndex(pkb, parent, child);
						 }};
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<QP::Types::ReferenceArgument> args) {
			 return pair{[parent = args.at(0)](QP::StorageAdapter& pkb) {
							 return StatementRelationExecutor<T>::executeTrivialSynonymWildcard(pkb, parent);
						 },
		                 [parent = args.at(0)](QP::StorageAdapter& pkb) {
							 return StatementRelationExecutor<T>::executeSynonymWildcard(pkb, parent);
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