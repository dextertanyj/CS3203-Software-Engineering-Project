#ifndef SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_TPP
#define SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_TPP

#include "QP/Dispatcher/DispatchProcessors.h"

#include <cassert>

template <typename T>
QP::Types::ExecutorSetFactory processArgumentRecurse(T map, const std::vector<QP::ReferenceArgument>& args) {
	QP::Types::ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == QP::Types::ReferenceType::Synonym) {
		key = args.at(0).getSynonym().type;
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QP::QueryDispatchException("Incorrect argument type.");
	}
	auto inner_map = iter->second;
	return processArgumentRecurse(inner_map, {++args.begin(), args.end()});
}

template <>
inline QP::Types::ExecutorSetFactory processArgumentRecurse(
	std::unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map, const std::vector<QP::ReferenceArgument>& args) {
	assert(args.size() == 1);
	QP::Types::ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == QP::Types::ReferenceType::Synonym) {
		key = args.at(0).getSynonym().type;
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QP::QueryDispatchException("Incorrect argument type.");
	}
	return iter->second;
}

template <typename T>
QP::Types::ExecutorSetFactoryBundle processArgumentBundleRecurse(T map, const std::vector<QP::ReferenceArgument>& args) {
	QP::Types::ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == QP::Types::ReferenceType::Synonym) {
		key = args.at(0).getSynonym().type;
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QP::QueryDispatchException("Incorrect argument type.");
	}
	auto inner_map = iter->second;
	return processArgumentBundleRecurse(inner_map, {++args.begin(), args.end()});
}

template <>
inline QP::Types::ExecutorSetFactoryBundle processArgumentBundleRecurse(
	std::unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> map,
	const std::vector<QP::ReferenceArgument>& args) {
	assert(args.size() == 1);
	QP::Types::ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == QP::Types::ReferenceType::Synonym) {
		key = args.at(0).getSynonym().type;
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QP::QueryDispatchException("Incorrect argument type.");
	}
	return iter->second;
}

template <typename T>
QP::Types::ExecutorSetBundle QP::Dispatcher::DispatchProcessors::processArgument(QP::Types::ClauseType type, T map,
                                                                                 const std::vector<ReferenceArgument>& args) {
	return {type, processArgumentRecurse(map, args)(args)};
}

template <typename T>
QP::Types::ExecutorSetBundle QP::Dispatcher::DispatchProcessors::processArgument(T map, const std::vector<ReferenceArgument>& args) {
	auto result = processArgumentBundleRecurse(map, args);
	return {result.first, result.second(args)};
}
#endif  // SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_TPP
