#ifndef SPA_SRC_QP_RELATIONSHIP_CALLSHANDLER_H
#define SPA_SRC_QP_RELATIONSHIP_CALLSHANDLER_H

#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/StorageAdapter.h"
#include "QP/ReferenceArgument.h"

namespace QP::Relationship {
template <QP::Types::ClauseType T>
class CallsHandler;
}

template <QP::Types::ClauseType T>
class QP::Relationship::CallsHandler {
public:
	static Types::ArgumentDispatcher dispatcher;

	// Trivial Executors
	static QueryResult executeTrivialNameName(QP::StorageAdapter& pkb, const Types::ReferenceArgument& caller,
	                                          const Types::ReferenceArgument& callee);
	static QueryResult executeTrivialNameWildcardOrSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& caller);
	static QueryResult executeTrivialWildcardOrSynonymName(QP::StorageAdapter& pkb, const Types::ReferenceArgument& callee);
	static QueryResult executeTrivialWildcardOrSynonymWildcardOrSynonym(QP::StorageAdapter& pkb);
	static QueryResult executeTrivialSynonymSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& caller,
	                                                const Types::ReferenceArgument& callee);

	// Executors
	static QueryResult executeNameSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& caller,
	                                      const Types::ReferenceArgument& callee);
	static QueryResult executeWildcardSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& callee);
	static QueryResult executeSynonymName(QP::StorageAdapter& pkb, const Types::ReferenceArgument& caller,
	                                      const Types::ReferenceArgument& callee);
	static QueryResult executeSynonymWildcard(QP::StorageAdapter& pkb, const Types::ReferenceArgument& caller);
	static QueryResult executeSynonymSynonym(QP::StorageAdapter& pkb, const Types::ReferenceArgument& caller,
	                                         const Types::ReferenceArgument& callee);

private:
	static Types::ExecutorSetBundle argumentDispatcher(Types::ClauseType type, vector<Types::ReferenceArgument> arguments);
	static const unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory>>
		argument_dispatch_map;
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getNameMap();
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getWildcardMap();
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getSynonymMap();
};

#endif  // SPA_SRC_QP_RELATIONSHIP_CALLSHANDLER_H
