#ifndef SPA_SRC_QP_RELATIONSHIP_PATTERN_H
#define SPA_SRC_QP_RELATIONSHIP_PATTERN_H

#include <string>
#include <unordered_map>

#include "Common/ExpressionProcessor/Expression.h"
#include "QP/StorageAdapter.h"
#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relationship.h"

class QP::Relationship::Pattern {
public:
	// Trivial Executors
	static QueryResult executeTrivialNameWildcard(QP::StorageAdapter& pkb, const Types::ReferenceArgument& name);
	static QueryResult executeTrivialSynonymOrWildcardWildcard(QP::StorageAdapter& pkb);
	static QueryResult executeTrivialNameExpression(QP::StorageAdapter& pkb, const Types::ReferenceArgument& name,
	                                                const Types::ReferenceArgument& expression);
	static QueryResult executeTrivialSynonymOrWildcardExpression(QP::StorageAdapter& pkb,
	                                                             const Types::ReferenceArgument& expression);

	// Executors
	static QueryResult executeNameWildcard(QP::StorageAdapter& pkb, const Types::ReferenceArgument& assign,
	                                       const Types::ReferenceArgument& name);
	static QueryResult executeWildcardWildcard(QP::StorageAdapter& pkb, const Types::ReferenceArgument& assign);
	static QueryResult executeSynonymWildcard(QP::StorageAdapter& pkb, const Types::ReferenceArgument& assign,
	                                          const Types::ReferenceArgument& synonym);
	static QueryResult executeNameExpression(QP::StorageAdapter& pkb, const Types::ReferenceArgument& assign,
	                                         const Types::ReferenceArgument& name, const Types::ReferenceArgument& expression);
	static QueryResult executeWildcardExpression(QP::StorageAdapter& pkb, const Types::ReferenceArgument& assign,
	                                             const Types::ReferenceArgument& expression);
	static QueryResult executeSynonymExpression(QP::StorageAdapter& pkb, const Types::ReferenceArgument& assign,
	                                            const Types::ReferenceArgument& synonym, const Types::ReferenceArgument& expression);

	static Types::ArgumentDispatcher dispatcher;

private:
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getNameMap();
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getWildcardMap();
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getVariableSynonymMap();

	static unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory>> getSynonymMap();

	static unordered_map<Types::ArgumentDispatchKey,
	                     unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory>>>
		argument_dispatch_map;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PATTERN_H
