#ifndef SPA_SRC_RELATIONSHIP_CALLDISPATCHERTEMPLATE_TPP
#define SPA_SRC_RELATIONSHIP_CALLDISPATCHERTEMPLATE_TPP

#include "QP/Relationship/CallDispatcherTemplate.h"

#include <utility>

#include "QP/DispatchProcessors.h"

template <class T>
QP::Types::ExecutorSetBundle QP::CallDispatcherTemplate<T>::argumentDispatcher(ClauseType type, vector<Types::ReferenceArgument> args) {
	return DispatchProcessors::processDoubleArgument(type, QP::CallDispatcherTemplate<T>::argument_dispatch_map, std::move(args));
};

template <class T>
const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::CallDispatcherTemplate<T>::name_map = {
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

template <class T>
const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::CallDispatcherTemplate<T>::wildcard_map = {
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

template <class T>
const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::CallDispatcherTemplate<T>::synonym_map = {
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

template <class T>
const unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>
	QP::CallDispatcherTemplate<T>::argument_dispatch_map = {{Types::ReferenceType::Name, name_map},
                                                            {Types::ReferenceType::Wildcard, wildcard_map},
                                                            {Types::DesignEntity::Procedure, synonym_map}};

#endif