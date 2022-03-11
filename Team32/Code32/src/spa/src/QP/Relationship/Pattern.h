#ifndef SPA_SRC_QP_RELATIONSHIP_PATTERN_H
#define SPA_SRC_QP_RELATIONSHIP_PATTERN_H

#include <string>
#include <unordered_map>

#include "Common/ExpressionProcessor/Expression.h"
#include "PKB/StorageAccessInterface.h"
#include "QP/QueryResult.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relationship.h"

class QP::Relationship::Pattern {
public:
	// Trivial Executors
	static QueryResult executeTrivialNameWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& name);
	static QueryResult executeTrivialSynonymOrWildcardWildcard(PKB::StorageAccessInterface& pkb);
	static QueryResult executeTrivialNameExpression(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& name,
	                                                const Types::ReferenceArgument& expression);
	static QueryResult executeTrivialSynonymOrWildcardExpression(PKB::StorageAccessInterface& pkb,
	                                                             const Types::ReferenceArgument& expression);

	// Executors
	static QueryResult executeNameWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& assign,
	                                       const Types::ReferenceArgument& name);
	static QueryResult executeWildcardWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& assign);
	static QueryResult executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& assign,
	                                          const Types::ReferenceArgument& synonym);
	static QueryResult executeNameExpression(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& assign,
	                                         const Types::ReferenceArgument& name, const Types::ReferenceArgument& expression);
	static QueryResult executeWildcardExpression(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& assign,
	                                             const Types::ReferenceArgument& expression);
	static QueryResult executeSynonymExpression(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& assign,
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
