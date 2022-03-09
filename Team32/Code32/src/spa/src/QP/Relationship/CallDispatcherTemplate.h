#ifndef SPA_CALLDISPATCHER_H
#define SPA_CALLDISPATCHER_H

#include "QP/QP.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"

template <class T>
class QP::CallDispatcherTemplate {
public:
	static Types::ExecutorSetBundle argumentDispatcher(vector<Types::ReferenceArgument> arguments);
private:
	static const unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory>>
		argument_dispatch_map;
	static const unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> name_map;
	static const unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> wildcard_map;
	static const unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> synonym_map;
};

#endif  // SPA_CALLDISPATCHER_H
