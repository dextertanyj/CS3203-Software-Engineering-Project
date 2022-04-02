#ifndef SPA_SRC_QP_EXECUTOR_WITHEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_WITHEXECUTOR_TPP

#include "QP/Executor/WithExecutor.h"

#include <algorithm>

template <typename TAttribute, typename TValues>
struct HashInfo {
	std::string symbol;
	unordered_set<TValues> values;
	QP::Types::AttributeMapper<TAttribute, TValues> mapper;
};

template <typename T>
inline std::string safeToString(T value) {
	return std::to_string(value);
}

template <>
inline std::string safeToString(std::string value) {
	return value;
}

template <typename TAttribute, typename TBuild, typename TProbe>
static QP::QueryResult find(const QP::StorageAdapter& store, HashInfo<TAttribute, TBuild> build_info,
                            HashInfo<TAttribute, TProbe> probe_info) {
	unordered_set<TAttribute> attributes;
	for_each(build_info.values.begin(), build_info.values.end(),
	         [&](const auto& value) { attributes.insert(build_info.mapper(store, value)); });
	for (const auto& probe : probe_info.values) {
		if (attributes.find(probe_info.mapper(store, probe)) != attributes.end()) {
			return QP::QueryResult(true);
		}
	}
	return {};
}

template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult QP::Executor::WithExecutor::executeTrivialAttributeAttribute(
	const QP::StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs,
	Types::WithInternalExecutors<TAttribute, TLeft> lhs_executors, Types::WithInternalExecutors<TAttribute, TRight> rhs_executors) {
	unordered_set<TLeft> left_values = lhs_executors.first(store, lhs);
	unordered_set<TRight> right_values = rhs_executors.first(store, rhs);
	unordered_set<TAttribute> attributes;
	if (left_values.size() < right_values.size()) {
		return find<TAttribute, TLeft, TRight>(store, {lhs.getAttribute().synonym.symbol, move(left_values), lhs_executors.second},
		                                       {rhs.getAttribute().synonym.symbol, move(right_values), rhs_executors.second});
	}
	return find<TAttribute, TRight, TLeft>(store, {rhs.getAttribute().synonym.symbol, move(right_values), rhs_executors.second},
	                                       {lhs.getAttribute().synonym.symbol, move(left_values), lhs_executors.second});
}

template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult QP::Executor::WithExecutor::executeTrivialAttributeConstant(
	const QP::StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs,
	Types::WithInternalExecutors<TAttribute, TLeft> lhs_executors, Types::WithInternalExecutors<TAttribute, TRight> rhs_executors) {
	unordered_set<TLeft> left_values = lhs_executors.first(store, lhs);
	TAttribute right_value = rhs_executors.second(store, (*rhs_executors.first(store, rhs).begin()));
	for (const auto& left : left_values) {
		if (lhs_executors.second(store, left) == right_value) {
			return QueryResult(true);
		}
	}
	return {};
}

template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult QP::Executor::WithExecutor::executeTrivialConstantAttribute(
	const QP::StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs,
	Types::WithInternalExecutors<TAttribute, TLeft> lhs_executors, Types::WithInternalExecutors<TAttribute, TRight> rhs_executors) {
	return executeTrivialAttributeConstant(store, rhs, lhs, rhs_executors, lhs_executors);
}

template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult QP::Executor::WithExecutor::executeTrivialConstantConstant(const QP::StorageAdapter& store,
                                                                           const ReferenceArgument& lhs,
                                                                           const ReferenceArgument& rhs,
                                                                           Types::WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                                                           Types::WithInternalExecutors<TAttribute, TRight> rhs_executors) {
	TAttribute left_value = lhs_executors.second(store, (*lhs_executors.first(store, lhs).begin()));
	TAttribute right_value = rhs_executors.second(store, (*rhs_executors.first(store, rhs).begin()));
	if (left_value == right_value) {
		return QueryResult(true);
	}
	return {};
}

template <typename TAttribute, typename TBuild, typename TProbe>
static QP::QueryResult hashJoin(const QP::StorageAdapter& store, HashInfo<TAttribute, TBuild> build_info,
                                HashInfo<TAttribute, TProbe> probe_info) {
	unordered_multimap<TAttribute, TBuild> build_table;
	for (const auto& build : build_info.values) {
		build_table.insert({build_info.mapper(store, build), build});
	}
	QP::QueryResult result = QP::QueryResult({build_info.symbol, probe_info.symbol});
	for (const auto& probe : probe_info.values) {
		TAttribute attribute = probe_info.mapper(store, probe);
		auto iter = build_table.equal_range(attribute);
		for_each(iter.first, iter.second, [&](const auto& pair) { result.addRow({safeToString(pair.second), safeToString(probe)}); });
	}
	return result;
}

template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult QP::Executor::WithExecutor::executeAttributeAttribute(const QP::StorageAdapter& store, const ReferenceArgument& lhs,
                                                                      const ReferenceArgument& rhs,
                                                                      Types::WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                                                      Types::WithInternalExecutors<TAttribute, TRight> rhs_executors) {
	unordered_set<TLeft> left_values = lhs_executors.first(store, lhs);
	unordered_set<TRight> right_values = rhs_executors.first(store, rhs);
	// Reduce memory usage by building hash table on smaller set.
	if (left_values.size() < right_values.size()) {
		return hashJoin<TAttribute, TLeft, TRight>(store, {lhs.getAttribute().synonym.symbol, move(left_values), lhs_executors.second},
		                                           {rhs.getAttribute().synonym.symbol, move(right_values), rhs_executors.second});
	}
	return hashJoin<TAttribute, TRight, TLeft>(store, {rhs.getAttribute().synonym.symbol, move(right_values), rhs_executors.second},
	                                           {lhs.getAttribute().synonym.symbol, move(left_values), lhs_executors.second});
}

template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult QP::Executor::WithExecutor::executeAttributeConstant(const QP::StorageAdapter& store, const ReferenceArgument& lhs,
                                                                     const ReferenceArgument& rhs,
                                                                     Types::WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                                                     Types::WithInternalExecutors<TAttribute, TRight> rhs_executors) {
	unordered_set<TLeft> left_values = lhs_executors.first(store, lhs);
	TAttribute right_value = rhs_executors.second(store, (*rhs_executors.first(store, rhs).begin()));
	QueryResult result = QueryResult({lhs.getAttribute().synonym.symbol});
	for (const auto& left : left_values) {
		if (lhs_executors.second(store, left) == right_value) {
			result.addRow({safeToString(left)});
		}
	}
	return result;
}

template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult QP::Executor::WithExecutor::executeConstantAttribute(const QP::StorageAdapter& store, const ReferenceArgument& lhs,
                                                                     const ReferenceArgument& rhs,
                                                                     Types::WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                                                     Types::WithInternalExecutors<TAttribute, TRight> rhs_executors) {
	return executeAttributeConstant(store, rhs, lhs, rhs_executors, lhs_executors);
}

#endif  // SPA_SRC_QP_EXECUTOR_WITHEXECUTOR_TPP