#ifndef SPA_FOLLOWSDISPATCHERTEMPLATE_H
#define SPA_FOLLOWSDISPATCHERTEMPLATE_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Relationship.h"

template <class T>
class QP::Relationship::FollowsDispatcherTemplate {
public:
	static Types::ExecutorSetBundle argumentDispatcher(Types::ClauseType type, vector<Types::ReferenceArgument> arguments);

private:
	static const unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory>>
		argument_dispatch_map;
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getIndexMap();
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getWildcardMap();
	static unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> getSynonymMap();
};

#endif  // SPA_FOLLOWSDISPATCHERTEMPLATE_H
