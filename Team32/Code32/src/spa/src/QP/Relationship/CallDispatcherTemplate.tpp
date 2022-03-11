#ifndef SPA_SRC_QP_RELATIONSHIP_CALLDISPATCHERTEMPLATE_TPP
#define SPA_SRC_QP_RELATIONSHIP_CALLDISPATCHERTEMPLATE_TPP

#include "QP/Relationship/CallDispatcherTemplate.h"

#include <unordered_map>
#include <utility>

#include "QP/DispatchProcessors.h"

template <class T>
QP::Types::ExecutorSetBundle QP::Relationship::CallDispatcherTemplate<T>::argumentDispatcher(Types::ClauseType type,
                                                                                             vector<Types::ReferenceArgument> args) {
	return DispatchProcessors::processDoubleArgument(type, CallDispatcherTemplate<T>::argument_dispatch_map, move(args));
};

template <class T>
const unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>
	QP::Relationship::CallDispatcherTemplate<T>::argument_dispatch_map = {{Types::ReferenceType::Name, getNameMap()},
                                                                          {Types::ReferenceType::Wildcard, getWildcardMap()},
                                                                          {Types::DesignEntity::Procedure, getSynonymMap()}};

template <class T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Relationship::CallDispatcherTemplate<T>::getNameMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::Name,
	     [](vector<Types::ReferenceArgument> args) {
			 return [caller = args.at(0), callee = args.at(1)](PKB::StorageAccessInterface& pkb) {
				 return T::executeTrivialNameName(pkb, caller, callee);
			 };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return [caller = args.at(0)](PKB::StorageAccessInterface& pkb) { return T::executeTrivialNameWildcardOrSynonym(pkb, caller); };
		 }},
		{Types::DesignEntity::Procedure,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{
				 [caller = args.at(0)](PKB::StorageAccessInterface& pkb) { return T::executeTrivialNameWildcardOrSynonym(pkb, caller); },
				 [caller = args.at(0), callee = args.at(1)](PKB::StorageAccessInterface& pkb) {
					 return T::executeNameSynonym(pkb, caller, callee);
				 }};
		 }},
	};
	return map;
}

template <class T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Relationship::CallDispatcherTemplate<T>::getWildcardMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::Name,
	     [](vector<Types::ReferenceArgument> args) {
			 return [callee = args.at(1)](PKB::StorageAccessInterface& pkb) { return T::executeTrivialWildcardOrSynonymName(pkb, callee); };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return [](PKB::StorageAccessInterface& pkb) { return T::executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb); };
		 }},
		{Types::DesignEntity::Procedure,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[](PKB::StorageAccessInterface& pkb) { return T::executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb); },
		                 [callee = args.at(1)](PKB::StorageAccessInterface& pkb) { return T::executeWildcardSynonym(pkb, callee); }};
		 }},
	};
	return map;
}

template <class T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Relationship::CallDispatcherTemplate<T>::getSynonymMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::Name,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{
				 [callee = args.at(1)](PKB::StorageAccessInterface& pkb) { return T::executeTrivialWildcardOrSynonymName(pkb, callee); },
				 [caller = args.at(0), callee = args.at(1)](PKB::StorageAccessInterface& pkb) {
					 return T::executeSynonymName(pkb, caller, callee);
				 }};
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[](PKB::StorageAccessInterface& pkb) { return T::executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb); },
		                 [caller = args.at(0)](PKB::StorageAccessInterface& pkb) { return T::executeSynonymWildcard(pkb, caller); }};
		 }},
		{Types::DesignEntity::Procedure,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[caller = args.at(0), callee = args.at(1)](PKB::StorageAccessInterface& pkb) {
							 return T::executeTrivialSynonymSynonym(pkb, caller, callee);
						 },
		                 [caller = args.at(0), callee = args.at(1)](PKB::StorageAccessInterface& pkb) {
							 return T::executeSynonymSynonym(pkb, caller, callee);
						 }};
		 }},
	};
	return map;
}

#endif  // SPA_SRC_QP_RELATIONSHIP_CALLDISPATCHERTEMPLATE_TPP