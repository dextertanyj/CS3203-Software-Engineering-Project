#ifndef SPA_SRC_QP_RELATIONSHIP_CALLDISPATCHERTEMPLATE_H
#define SPA_SRC_QP_RELATIONSHIP_CALLDISPATCHERTEMPLATE_H

#include <unordered_map>

#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relationship.h"

template <class T>
class QP::Relationship::CallDispatcherTemplate {
public:
	static Types::ExecutorSetBundle argumentDispatcher(Types::ClauseType type, vector<Types::ReferenceArgument> arguments);

private:
	static const unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory>>
		argument_dispatch_map;
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getNameMap();
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getWildcardMap();
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getSynonymMap();
};

#endif  // SPA_SRC_QP_RELATIONSHIP_CALLDISPATCHERTEMPLATE_H
