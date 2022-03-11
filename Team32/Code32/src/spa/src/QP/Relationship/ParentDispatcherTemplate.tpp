#ifndef SPA_SRC_QP_RELATIONSHIP_PARENTDISPATCHERTEMPLATE_TPP
#define SPA_SRC_QP_RELATIONSHIP_PARENTDISPATCHERTEMPLATE_TPP

#include "QP/Relationship/ParentDispatcherTemplate.h"

#include <unordered_map>
#include <utility>

#include "QP/DispatchProcessors.h"

template <class T>
static QP::Types::ExecutorSetFactory lambda_index_synonym() {
	static const QP::Types::ExecutorSetFactory lambda = [](vector<QP::Types::ReferenceArgument> args) {
		return pair{
			[parent = args.at(0), child = args.at(1)](PKB::StorageAccessInterface& pkb) {
				return T::executeTrivialIndexSynonym(pkb, parent, child);
			},
			[parent = args.at(0), child = args.at(1)](PKB::StorageAccessInterface& pkb) { return T::executeIndexSynonym(pkb, parent, child); }};
	};
	return lambda;
}

template <class T>
static QP::Types::ExecutorSetFactory lambda_wildcard_synonym() {
	static const QP::Types::ExecutorSetFactory lambda = [](vector<QP::Types::ReferenceArgument> args) {
		return pair{[child = args.at(1)](PKB::StorageAccessInterface& pkb) { return T::executeTrivialWildcardSynonym(pkb, child); },
		            [child = args.at(1)](PKB::StorageAccessInterface& pkb) { return T::executeWildcardSynonym(pkb, child); }};
	};
	return lambda;
}

template <class T>
static QP::Types::ExecutorSetFactory lambda_synonym_synonym() {
	static const QP::Types::ExecutorSetFactory lambda = [](vector<QP::Types::ReferenceArgument> args) {
		return pair{[parent = args.at(0), child = args.at(1)](PKB::StorageAccessInterface& pkb) {
						return T::executeTrivialSynonymSynonym(pkb, parent, child);
					},
		            [parent = args.at(0), child = args.at(1)](PKB::StorageAccessInterface& pkb) {
						return T::executeSynonymSynonym(pkb, parent, child);
					}};
	};
	return lambda;
}

template <class T>
QP::Types::ExecutorSetBundle QP::Relationship::ParentDispatcherTemplate<T>::argumentDispatcher(Types::ClauseType type,
                                                                                               vector<Types::ReferenceArgument> args) {
	return DispatchProcessors::processDoubleArgument(type, ParentDispatcherTemplate<T>::argument_dispatch_map, std::move(args));
};

template <class T>
const unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>
	QP::Relationship::ParentDispatcherTemplate<T>::argument_dispatch_map = {{Types::ReferenceType::StatementIndex, getIndexMap()},
                                                                            {Types::ReferenceType::Wildcard, getWildcardMap()},
                                                                            {Types::DesignEntity::Stmt, getSynonymMap()},
                                                                            {Types::DesignEntity::While, getSynonymMap()},
                                                                            {Types::DesignEntity::If, getSynonymMap()}};

template <class T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Relationship::ParentDispatcherTemplate<T>::getIndexMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::StatementIndex,
	     [](vector<Types::ReferenceArgument> args) {
			 return [parent = args.at(0), child = args.at(1)](PKB::StorageAccessInterface& pkb) {
				 return T::executeTrivialIndexIndex(pkb, parent, child);
			 };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return [parent = args.at(0)](PKB::StorageAccessInterface& pkb) { return T::executeTrivialIndexWildcard(pkb, parent); };
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

template <class T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>
QP::Relationship::ParentDispatcherTemplate<T>::getWildcardMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::StatementIndex,
	     [](vector<Types::ReferenceArgument> args) {
			 return [child = args.at(1)](PKB::StorageAccessInterface& pkb) { return T::executeTrivialWildcardIndex(pkb, child); };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> /*args*/) {
			 return [](PKB::StorageAccessInterface& pkb) { return T::executeTrivialWildcardWildcard(pkb); };
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

template <class T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>
QP::Relationship::ParentDispatcherTemplate<T>::getSynonymMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::StatementIndex,
	     [](vector<QP::Types::ReferenceArgument> args) {
			 return pair{[parent = args.at(0), child = args.at(1)](PKB::StorageAccessInterface& pkb) {
							 return T::executeTrivialSynonymIndex(pkb, parent, child);
						 },
		                 [parent = args.at(0), child = args.at(1)](PKB::StorageAccessInterface& pkb) {
							 return T::executeSynonymIndex(pkb, parent, child);
						 }};
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<QP::Types::ReferenceArgument> args) {
			 return pair{[parent = args.at(0)](PKB::StorageAccessInterface& pkb) { return T::executeTrivialSynonymWildcard(pkb, parent); },
		                 [parent = args.at(0)](PKB::StorageAccessInterface& pkb) { return T::executeSynonymWildcard(pkb, parent); }};
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