#ifndef SPA_SRC_QP_RELATIONSHIP_CALLSHANDLER_TPP
#define SPA_SRC_QP_RELATIONSHIP_CALLSHANDLER_TPP

#include "QP/Relationship/CallsHandler.h"

#include "QP/DispatchProcessors.h"

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::CallsHandler<T>::executeTrivialNameName(QP::StorageAdapter& pkb,
                                                                          const QP::Types::ReferenceArgument& caller,
                                                                          const QP::Types::ReferenceArgument& callee) {
	return QP::QueryResult(pkb.checkProcedureRelation<T>(caller.getName(), callee.getName()));
}

template <>
inline QP::QueryResult QP::Relationship::CallsHandler<QP::Types::ClauseType::CallT>::executeTrivialNameName(
	QP::StorageAdapter& pkb, const QP::Types::ReferenceArgument& caller, const QP::Types::ReferenceArgument& callee) {
	ProcRefSet callee_set = pkb.getReverseProcedures<QP::Types::ClauseType::CallT>(caller.getName());
	for (auto const& callee_reference : callee_set) {
		if (callee_reference == callee.getName()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::CallsHandler<T>::executeTrivialNameWildcardOrSynonym(QP::StorageAdapter& pkb,
                                                                                       const Types::ReferenceArgument& caller) {
	return QueryResult(!pkb.getReverseProcedures<T>(caller.getName()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::CallsHandler<T>::executeTrivialWildcardOrSynonymName(QP::StorageAdapter& pkb,
                                                                                       const Types::ReferenceArgument& callee) {
	return QueryResult(!pkb.template getForwardProcedures<T>(callee.getName()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::CallsHandler<T>::executeTrivialWildcardOrSynonymWildcardOrSynonym(QP::StorageAdapter& pkb) {
	ProcRefSet proc_set = pkb.getProcedures();
	for (auto const& proc : proc_set) {
		ProcRefSet caller_set = pkb.getForwardProcedures<T>(proc);
		if (!caller_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::CallsHandler<T>::executeTrivialSynonymSynonym(QP::StorageAdapter& pkb,
                                                                                const Types::ReferenceArgument& caller,
                                                                                const Types::ReferenceArgument& callee) {
	if (caller.getSynonym().symbol == callee.getSynonym().symbol) {
		return {};
	}
	return executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb);
}

// Executors
template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::CallsHandler<T>::executeNameSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& caller,
                                                                      const Types::ReferenceArgument& callee) {
	QueryResult result = QueryResult();
	vector<string> column;
	ProcRefSet callees = pkb.getReverseProcedures<T>(caller.getName());
	for (auto const& callee_reference : callees) {
		column.push_back(callee_reference);
	}

	result.addColumn(callee.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::CallsHandler<T>::executeWildcardSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& callee) {
	QueryResult result = QueryResult();
	vector<string> column;
	ProcRefSet procedures = pkb.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet callers = pkb.getForwardProcedures<T>(procedure);
		if (!callers.empty()) {
			column.push_back(procedure);
		}
	}

	result.addColumn(callee.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::CallsHandler<T>::executeSynonymName(QP::StorageAdapter& pkb, const Types::ReferenceArgument& caller,
                                                                      const Types::ReferenceArgument& callee) {
	QueryResult result = QueryResult();
	vector<string> caller_column;
	ProcRefSet callers = pkb.getForwardProcedures<T>(callee.getName());
	for (auto const& caller_reference : callers) {
		caller_column.push_back(caller_reference);
	}
	result.addColumn(caller.getSynonym().symbol, caller_column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::CallsHandler<T>::executeSynonymWildcard(QP::StorageAdapter& pkb, const Types::ReferenceArgument& caller) {
	QueryResult result = QueryResult();
	vector<string> caller_column;
	ProcRefSet procedures = pkb.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet callees = pkb.getReverseProcedures<T>(procedure);
		if (!callees.empty()) {
			caller_column.push_back(procedure);
		}
	}
	result.addColumn(caller.getSynonym().symbol, caller_column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::CallsHandler<T>::executeSynonymSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& caller,
                                                                         const Types::ReferenceArgument& callee) {
	if (caller.getSynonym().symbol == callee.getSynonym().symbol) {
		return {};
	}

	QueryResult result = QueryResult();
	vector<string> caller_column;
	vector<string> callee_column;

	ProcRefSet procedures = pkb.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet callees = pkb.getReverseProcedures<T>(procedure);
		for (auto const& callee_reference : callees) {
			caller_column.push_back(procedure);
			callee_column.push_back(callee_reference);
		}
	}

	result.addColumn(caller.getSynonym().symbol, caller_column);
	result.addColumn(callee.getSynonym().symbol, callee_column);
	return result;
}

// Dispatchers
template <QP::Types::ClauseType T>
QP::Types::ArgumentDispatcher QP::Relationship::CallsHandler<T>::dispatcher =
	[](vector<Types::ReferenceArgument> args) { return argumentDispatcher(T, move(args)); };

template <QP::Types::ClauseType T>
QP::Types::ExecutorSetBundle QP::Relationship::CallsHandler<T>::argumentDispatcher(Types::ClauseType type,
                                                                                   vector<Types::ReferenceArgument> args) {
	return DispatchProcessors::processDoubleArgument(type, argument_dispatch_map, move(args));
};

template <QP::Types::ClauseType T>
const unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>
	QP::Relationship::CallsHandler<T>::argument_dispatch_map = {{Types::ReferenceType::Name, getNameMap()},
                                                                {Types::ReferenceType::Wildcard, getWildcardMap()},
                                                                {Types::DesignEntity::Procedure, getSynonymMap()}};

template <QP::Types::ClauseType T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Relationship::CallsHandler<T>::getNameMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::Name,
	     [](vector<Types::ReferenceArgument> args) {
			 return [caller = args.at(0), callee = args.at(1)](QP::StorageAdapter& pkb) {
				 return executeTrivialNameName(pkb, caller, callee);
			 };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return [caller = args.at(0)](QP::StorageAdapter& pkb) { return executeTrivialNameWildcardOrSynonym(pkb, caller); };
		 }},
		{Types::DesignEntity::Procedure,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{
				 [caller = args.at(0)](QP::StorageAdapter& pkb) { return executeTrivialNameWildcardOrSynonym(pkb, caller); },
				 [caller = args.at(0), callee = args.at(1)](QP::StorageAdapter& pkb) { return executeNameSynonym(pkb, caller, callee); }};
		 }},
	};
	return map;
}

template <QP::Types::ClauseType T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Relationship::CallsHandler<T>::getWildcardMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::Name,
	     [](vector<Types::ReferenceArgument> args) {
			 return [callee = args.at(1)](QP::StorageAdapter& pkb) { return executeTrivialWildcardOrSynonymName(pkb, callee); };
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return [](QP::StorageAdapter& pkb) { return executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb); };
		 }},
		{Types::DesignEntity::Procedure,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[](QP::StorageAdapter& pkb) { return executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb); },
		                 [callee = args.at(1)](QP::StorageAdapter& pkb) { return executeWildcardSynonym(pkb, callee); }};
		 }},
	};
	return map;
}

template <QP::Types::ClauseType T>
unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> QP::Relationship::CallsHandler<T>::getSynonymMap() {
	static const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
		{Types::ReferenceType::Name,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{
				 [callee = args.at(1)](QP::StorageAdapter& pkb) { return executeTrivialWildcardOrSynonymName(pkb, callee); },
				 [caller = args.at(0), callee = args.at(1)](QP::StorageAdapter& pkb) { return executeSynonymName(pkb, caller, callee); }};
		 }},
		{Types::ReferenceType::Wildcard,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[](QP::StorageAdapter& pkb) { return executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb); },
		                 [caller = args.at(0)](QP::StorageAdapter& pkb) { return executeSynonymWildcard(pkb, caller); }};
		 }},
		{Types::DesignEntity::Procedure,
	     [](vector<Types::ReferenceArgument> args) {
			 return pair{[caller = args.at(0), callee = args.at(1)](QP::StorageAdapter& pkb) {
							 return executeTrivialSynonymSynonym(pkb, caller, callee);
						 },
		                 [caller = args.at(0), callee = args.at(1)](QP::StorageAdapter& pkb) {
							 return executeSynonymSynonym(pkb, caller, callee);
						 }};
		 }},
	};
	return map;
}

#endif  // SPA_SRC_QP_RELATIONSHIP_CALLSHANDLER_TPP