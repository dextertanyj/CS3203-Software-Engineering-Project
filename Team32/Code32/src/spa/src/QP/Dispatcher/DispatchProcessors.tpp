#ifndef SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_TPP
#define SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_TPP

#include "QP/Dispatcher/DispatchProcessors.h"

template <typename T>
QP::Types::ExecutorSetFactory processArgumentRecurse(T map, const vector<QP::Types::ReferenceArgument>& args) {
	QP::Types::ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == QP::Types::ReferenceType::Synonym) {
		key = args.at(0).getSynonym().type;
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QP::QueryException("Incorrect argument type.");
	}
	auto inner_map = iter->second;
	return processArgumentRecurse(inner_map, {++args.begin(), args.end()});
}

template <>
inline QP::Types::ExecutorSetFactory processArgumentRecurse(
	unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map, const vector<QP::Types::ReferenceArgument>& args) {
	QP::Types::ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == QP::Types::ReferenceType::Synonym) {
		key = args.at(0).getSynonym().type;
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QP::QueryException("Incorrect argument type.");
	}
	return iter->second;
}

template <typename T>
QP::Types::ExecutorSetFactoryBundle processArgumentBundleRecurse(T map, const vector<QP::Types::ReferenceArgument>& args) {
	QP::Types::ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == QP::Types::ReferenceType::Synonym) {
		key = args.at(0).getSynonym().type;
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QP::QueryException("Incorrect argument type.");
	}
	auto inner_map = iter->second;
	return processArgumentBundleRecurse(inner_map, {++args.begin(), args.end()});
}

template <>
inline QP::Types::ExecutorSetFactoryBundle processArgumentBundleRecurse(
	unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> map,
	const vector<QP::Types::ReferenceArgument>& args) {
	QP::Types::ArgumentDispatchKey key = args.at(0).getType();
	if (args.at(0).getType() == QP::Types::ReferenceType::Synonym) {
		key = args.at(0).getSynonym().type;
	}
	auto iter = map.find(key);
	if (iter == map.end()) {
		throw QP::QueryException("Incorrect argument type.");
	}
	return iter->second;
}

template <typename T>
QP::Types::ExecutorSetBundle QP::Dispatcher::DispatchProcessors::processArgument(QP::Types::ClauseType type, T map,
                                                                                 const vector<Types::ReferenceArgument>& args) {
	return {type, processArgumentRecurse(map, args)(args)};
}

template <typename T>
QP::Types::ExecutorSetBundle QP::Dispatcher::DispatchProcessors::processArgument(T map, const vector<Types::ReferenceArgument>& args) {
	auto result = processArgumentBundleRecurse(map, args);
	return {result.first, result.second(args)};
}

inline QP::Types::ExecutorSetBundle QP::Dispatcher::DispatchProcessors::processPatternIfArgument(
	Types::ClauseType type, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map,
	const vector<Types::ReferenceArgument>& args) {
	if (type != Types::ClauseType::PatternIf) {
		throw QP::QueryException("Incorrect dispatch processor called.");
	}

	if (args.size() != 4) {
		throw QP::QueryException("Incorrect number of arguments.");
	}

	if (args.at(0).getType() != Types::ReferenceType::Synonym && args.at(0).getSynonym().type != Types::DesignEntity::If) {
		throw QP::QueryException("Invalid first argument type.");
	}

	if (args.at(2).getType() != Types::ReferenceType::Wildcard) {
		throw QP::QueryException("Incorrect third argument type.");
	}

	if (args.at(3).getType() != Types::ReferenceType::Wildcard) {
		throw QP::QueryException("Incorrect fourth argument type.");
	}

	Types::ArgumentDispatchKey var_key = args.at(1).getType();
	if (args.at(1).getType() == Types::ReferenceType::Synonym) {
		var_key = args.at(1).getSynonym().type;
	}

	auto map_iter = map.find(var_key);
	if (map_iter == map.end()) {
		throw QP::QueryException("Incorrect second argument type.");
	}
	return {type, map_iter->second(args)};
}

#endif  // SPA_SRC_QP_DISPATCHER_DISPATCHPROCESSORS_TPP