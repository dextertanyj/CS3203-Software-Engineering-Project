#include "CallDispatcherTemplate.h"

template <class T>
QP::Types::ExecutorSetBundle QP::CallDispatcherTemplate<T>::argumentDispatcher(vector<Types::ReferenceArgument> args) {
	if (args.size() != 2) {
		throw QP::QueryException("Incorrect number of arguments.");
	}
	Types::ArgumentDispatchKey outer_key = args.at(0).getType();
	if (args.at(0).getType() == Types::ReferenceType::Synonym) {
		outer_key = args.at(0).getSynonym().type;
	}
	Types::ArgumentDispatchKey inner_key = args.at(0).getType();
	if (args.at(0).getType() == Types::ReferenceType::Synonym) {
		inner_key = args.at(0).getSynonym().type;
	}
	auto outer_map_iter = argument_dispatch_map.find(outer_key);
	if (outer_map_iter == argument_dispatch_map.end()) {
		throw "Incorrect first argument.";
	}
	auto inner_map = outer_map_iter->second;
	auto inner_map_iter = inner_map.find(inner_key);
	if (inner_map_iter == inner_map.end()) {
		throw "Incorrect second argument.";
	}
	return {Types::ClauseType::Call, inner_map_iter->second(args)};
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
