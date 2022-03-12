#ifndef SPA_SRC_QP_RELATIONSHIP_CALLSHANDLER_TPP
#define SPA_SRC_QP_RELATIONSHIP_CALLSHANDLER_TPP

#include "QP/Dispatcher/CallsDispatcher.h"

#include "DispatchProcessors.h"
#include "QP/Executor/ProcedureExecutor.tpp"

template <QP::Types::ClauseType T>
QP::Types::ArgumentDispatcher QP::Dispatcher::CallsDispatcher<T>::dispatcher =
	[](vector<Types::ReferenceArgument> args) { return argumentDispatcher(T, move(args)); };

template <QP::Types::ClauseType T>
QP::Types::ExecutorSetBundle QP::Dispatcher::CallsDispatcher<T>::argumentDispatcher(Types::ClauseType type,
                                                                                    vector<Types::ReferenceArgument> args) {
	return DispatchProcessors::processDoubleArgument(type, argument_dispatch_map, move(args));
};

template <QP::Types::ClauseType T>
const unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>
	QP::Dispatcher::CallsDispatcher<T>::argument_dispatch_map = {{Types::ReferenceType::Name, getNameMap()},
                                                                 {Types::ReferenceType::Wildcard, getWildcardMap()},
                                                                 {Types::DesignEntity::Procedure, getSynonymMap()}};

template <QP::Types::ClauseType T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Dispatcher::CallsDispatcher<T>::getNameMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::Name,
	     [](vector<Types::ReferenceArgument> args) {
			 return [caller = args.at(0), callee = args.at(1)](QP::StorageAdapter& pkb) {
				 return Executor::ProcedureExecutor<T>::executeTrivialNameName(pkb, caller, callee);
			 };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return [caller = args.at(0)](QP::StorageAdapter& pkb) {
				 return Executor::ProcedureExecutor<T>::executeTrivialNameWildcardOrSynonym(pkb, caller);
			 };
		 }},
		{Types::DesignEntity::Procedure,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[caller = args.at(0)](QP::StorageAdapter& pkb) {
							 return Executor::ProcedureExecutor<T>::executeTrivialNameWildcardOrSynonym(pkb, caller);
						 },
		                 [caller = args.at(0), callee = args.at(1)](QP::StorageAdapter& pkb) {
							 return Executor::ProcedureExecutor<T>::executeNameSynonym(pkb, caller, callee);
						 }};
		 }},
	};
	return map;
}

template <QP::Types::ClauseType T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Dispatcher::CallsDispatcher<T>::getWildcardMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::Name,
	     [](vector<Types::ReferenceArgument> args) {
			 return [callee = args.at(1)](QP::StorageAdapter& pkb) {
				 return Executor::ProcedureExecutor<T>::executeTrivialWildcardOrSynonymName(pkb, callee);
			 };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return
				 [](QP::StorageAdapter& pkb) { return Executor::ProcedureExecutor<T>::executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb); };
		 }},
		{Types::DesignEntity::Procedure,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{
				 [](QP::StorageAdapter& pkb) { return Executor::ProcedureExecutor<T>::executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb); },
				 [callee = args.at(1)](QP::StorageAdapter& pkb) {
					 return Executor::ProcedureExecutor<T>::executeWildcardSynonym(pkb, callee);
				 }};
		 }},
	};
	return map;
}

template <QP::Types::ClauseType T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Dispatcher::CallsDispatcher<T>::getSynonymMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::Name,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[callee = args.at(1)](QP::StorageAdapter& pkb) {
							 return Executor::ProcedureExecutor<T>::executeTrivialWildcardOrSynonymName(pkb, callee);
						 },
		                 [caller = args.at(0), callee = args.at(1)](QP::StorageAdapter& pkb) {
							 return Executor::ProcedureExecutor<T>::executeSynonymName(pkb, caller, callee);
						 }};
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{
				 [](QP::StorageAdapter& pkb) { return Executor::ProcedureExecutor<T>::executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb); },
				 [caller = args.at(0)](QP::StorageAdapter& pkb) {
					 return Executor::ProcedureExecutor<T>::executeSynonymWildcard(pkb, caller);
				 }};
		 }},
		{Types::DesignEntity::Procedure,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[caller = args.at(0), callee = args.at(1)](QP::StorageAdapter& pkb) {
							 return Executor::ProcedureExecutor<T>::executeTrivialSynonymSynonym(pkb, caller, callee);
						 },
		                 [caller = args.at(0), callee = args.at(1)](QP::StorageAdapter& pkb) {
							 return Executor::ProcedureExecutor<T>::executeSynonymSynonym(pkb, caller, callee);
						 }};
		 }},
	};
	return map;
}

#endif  // SPA_SRC_QP_RELATIONSHIP_CALLSHANDLER_TPP