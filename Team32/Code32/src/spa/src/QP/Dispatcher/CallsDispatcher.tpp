#ifndef SPA_SRC_QP_RELATIONSHIP_CALLSHANDLER_TPP
#define SPA_SRC_QP_RELATIONSHIP_CALLSHANDLER_TPP

#include "QP/Dispatcher/CallsDispatcher.h"

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/ProcedureExecutor.tpp"

template <QP::Types::ClauseType T>
QP::Types::ArgumentDispatcher QP::Dispatcher::CallsDispatcher<T>::dispatcher =
	[](const vector<Types::ReferenceArgument>& args) { return argumentDispatcher(T, args); };

template <QP::Types::ClauseType T>
QP::Types::ExecutorSetBundle QP::Dispatcher::CallsDispatcher<T>::argumentDispatcher(Types::ClauseType type,
                                                                                    const vector<Types::ReferenceArgument>& args) {
	return DispatchProcessors::processArgument(type, argument_dispatch_map, args);
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
	     [](const vector<Types::ReferenceArgument>& args) {
			 return [caller = args.at(0), callee = args.at(1)](const QP::StorageAdapter& pkb) {
				 return Executor::ProcedureExecutor<T>::executeTrivialNameName(pkb, caller, callee);
			 };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](const vector<Types::ReferenceArgument>& args) {
			 return [caller = args.at(0)](const QP::StorageAdapter& pkb) {
				 return Executor::ProcedureExecutor<T>::executeTrivialNameWildcardOrSynonym(pkb, caller);
			 };
		 }},
		{Types::DesignEntity::Procedure,
	     [](const vector<Types::ReferenceArgument>& args) {
			 return pair{[caller = args.at(0)](const QP::StorageAdapter& pkb) {
							 return Executor::ProcedureExecutor<T>::executeTrivialNameWildcardOrSynonym(pkb, caller);
						 },
		                 [caller = args.at(0), callee = args.at(1)](const QP::StorageAdapter& pkb) {
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
	     [](const vector<Types::ReferenceArgument>& args) {
			 return [callee = args.at(1)](const QP::StorageAdapter& pkb) {
				 return Executor::ProcedureExecutor<T>::executeTrivialWildcardOrSynonymName(pkb, callee);
			 };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](const vector<Types::ReferenceArgument>& /*args*/) {
			 return [](const QP::StorageAdapter& pkb) {
				 return Executor::ProcedureExecutor<T>::executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb);
			 };
		 }},
		{Types::DesignEntity::Procedure,
	     [](const vector<Types::ReferenceArgument>& args) {
			 return pair{[](const QP::StorageAdapter& pkb) {
							 return Executor::ProcedureExecutor<T>::executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb);
						 },
		                 [callee = args.at(1)](const QP::StorageAdapter& pkb) {
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
	     [](const vector<Types::ReferenceArgument>& args) {
			 return pair{[callee = args.at(1)](const QP::StorageAdapter& pkb) {
							 return Executor::ProcedureExecutor<T>::executeTrivialWildcardOrSynonymName(pkb, callee);
						 },
		                 [caller = args.at(0), callee = args.at(1)](const QP::StorageAdapter& pkb) {
							 return Executor::ProcedureExecutor<T>::executeSynonymName(pkb, caller, callee);
						 }};
		 }},
		{Types::ReferenceType::Wildcard,
	     [](const vector<Types::ReferenceArgument>& args) {
			 return pair{[](const QP::StorageAdapter& pkb) {
							 return Executor::ProcedureExecutor<T>::executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb);
						 },
		                 [caller = args.at(0)](const QP::StorageAdapter& pkb) {
							 return Executor::ProcedureExecutor<T>::executeSynonymWildcard(pkb, caller);
						 }};
		 }},
		{Types::DesignEntity::Procedure,
	     [](const vector<Types::ReferenceArgument>& args) {
			 return pair{[caller = args.at(0), callee = args.at(1)](const QP::StorageAdapter& pkb) {
							 return Executor::ProcedureExecutor<T>::executeTrivialSynonymSynonym(pkb, caller, callee);
						 },
		                 [caller = args.at(0), callee = args.at(1)](const QP::StorageAdapter& pkb) {
							 return Executor::ProcedureExecutor<T>::executeSynonymSynonym(pkb, caller, callee);
						 }};
		 }},
	};
	return map;
}

#endif  // SPA_SRC_QP_RELATIONSHIP_CALLSHANDLER_TPP