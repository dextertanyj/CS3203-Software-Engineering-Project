#ifndef SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_TPP
#define SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_TPP

#include "QP/Dispatcher/DispatchProcessors.h"

#include <cassert>

namespace QP::Dispatcher::DispatchProcessors {

using namespace std;

template <typename T>
Types::ExecutorSetFactory processArgumentRecurse(T map, const vector<ReferenceArgument>& args) {
	Types::ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == Types::ReferenceType::Synonym) {
		key = args.at(0).getSynonym().type;
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QueryDispatchException("Incorrect argument type.");
	}
	auto inner_map = iter->second;
	return processArgumentRecurse(inner_map, {++args.begin(), args.end()});
}

template <>
inline Types::ExecutorSetFactory processArgumentRecurse(unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactory> map,
                                                        const vector<ReferenceArgument>& args) {
	assert(args.size() == 1);
	Types::ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == Types::ReferenceType::Synonym) {
		key = args.at(0).getSynonym().type;
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QueryDispatchException("Incorrect argument type.");
	}
	return iter->second;
}

template <typename T>
Types::ExecutorSetFactoryBundle processArgumentBundleRecurse(T map, const vector<ReferenceArgument>& args) {
	Types::ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == Types::ReferenceType::Synonym) {
		key = args.at(0).getSynonym().type;
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QueryDispatchException("Incorrect argument type.");
	}
	auto inner_map = iter->second;
	return processArgumentBundleRecurse(inner_map, {++args.begin(), args.end()});
}

template <>
inline Types::ExecutorSetFactoryBundle processArgumentBundleRecurse(
	unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactoryBundle> map, const vector<ReferenceArgument>& args) {
	assert(args.size() == 1);
	Types::ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == Types::ReferenceType::Synonym) {
		key = args.at(0).getSynonym().type;
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QueryDispatchException("Incorrect argument type.");
	}
	return iter->second;
}

template <typename T>
Types::ExecutorSetBundle processArgument(Types::ClauseType type, T map, const vector<ReferenceArgument>& args) {
	return {type, processArgumentRecurse(map, args)(args)};
}

template <typename T>
Types::ExecutorSetBundle processArgument(T map, const vector<ReferenceArgument>& args) {
	auto result = processArgumentBundleRecurse(map, args);
	return {result.first, result.second(args)};
}
}

#endif  // SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_TPP
