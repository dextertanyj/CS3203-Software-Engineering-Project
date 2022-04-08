#ifndef SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_TPP
#define SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_TPP

#include "QP/Dispatcher/DispatchProcessors.h"

#include <cassert>

namespace QP::Dispatcher::DispatchProcessors {

using namespace std;
using namespace QP::Types;

template <typename T>
ExecutorSetFactory processArgumentRecurse(T map, const vector<ClauseArgument>& args) {
	ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == ArgumentType::Synonym) {
		key = args.at(0).getSynonymType();
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QueryDispatchException("Incorrect argument type.");
	}
	auto inner_map = iter->second;
	return processArgumentRecurse(inner_map, {++args.begin(), args.end()});
}

template <>
inline ExecutorSetFactory processArgumentRecurse(unordered_map<ArgumentDispatchKey, ExecutorSetFactory> map,
                                                 const vector<ClauseArgument>& args) {
	if (args.size() != 1) {
		throw QueryDispatchException("Incorrect argument count.");
	}
	ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == ArgumentType::Synonym) {
		key = args.at(0).getSynonymType();
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QueryDispatchException("Incorrect argument type.");
	}
	return iter->second;
}

template <typename T>
ExecutorSetFactoryBundle processArgumentBundleRecurse(T map, const vector<ClauseArgument>& args) {
	ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == ArgumentType::Synonym) {
		key = args.at(0).getSynonymType();
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QueryDispatchException("Incorrect argument type.");
	}
	auto inner_map = iter->second;
	return processArgumentBundleRecurse(inner_map, {++args.begin(), args.end()});
}

template <>
inline ExecutorSetFactoryBundle processArgumentBundleRecurse(unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> map,
                                                             const vector<ClauseArgument>& args) {
	if (args.size() != 1) {
		throw QueryDispatchException("Incorrect argument count.");
	}
	ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == ArgumentType::Synonym) {
		key = args.at(0).getSynonymType();
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QueryDispatchException("Incorrect argument type.");
	}
	return iter->second;
}

template <typename T>
ExecutorSetBundle processArgument(ClauseType type, T map, const vector<ClauseArgument>& args) {
	return {type, processArgumentRecurse(map, args)(args)};
}

template <typename T>
ExecutorSetBundle processArgument(T map, const vector<ClauseArgument>& args) {
	auto result = processArgumentBundleRecurse(map, args);
	return {result.first, result.second(args)};
}
}

#endif  // SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_TPP
