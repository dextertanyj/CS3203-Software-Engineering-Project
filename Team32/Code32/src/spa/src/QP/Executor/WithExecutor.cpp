#include "QP/Executor/WithExecutor.h"

template <typename TAttribute, typename TValues>
struct HashInfo {
	std::string symbol;
	unordered_set<TValues> values;
	QP::Types::AttributeMapper<TAttribute, TValues> mapper;
};

template <typename TAttribute, typename TBuild, typename TProbe>
static QP::QueryResult find(const QP::StorageAdapter& storage, HashInfo<TAttribute, TBuild> build_info,
                            HashInfo<TAttribute, TProbe> probe_info) {
	unordered_set<TAttribute> attributes;
	for_each(build_info.values.begin(), build_info.values.end(),
	         [&](const auto& value) { attributes.insert(build_info.mapper(storage, value)); });
	for (const auto& probe : probe_info.values) {
		if (attributes.find(probe_info.mapper(storage, probe)) != attributes.end()) {
			return QP::QueryResult(true);
		}
	}
	return {};
}

template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult QP::Executor::WithExecutor::executeTrivialAttributeAttribute(
	const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs, const Types::ReferenceArgument& rhs,
	Types::WithInternalExecutors<TAttribute, TLeft> lhs_executors, Types::WithInternalExecutors<TAttribute, TRight> rhs_executors) {
	unordered_set<TLeft> left_values = lhs_executors.first(storage, lhs);
	unordered_set<TRight> right_values = rhs_executors.first(storage, rhs);
	unordered_set<TAttribute> attributes;
	if (left_values.size() < right_values.size()) {
		return find(storage, {lhs.getSynonym().symbol, move(left_values), lhs_executors.second},
		            {rhs.getSynonym().symbol, move(right_values), rhs_executors.second});
	}
	return find(storage, {rhs.getSynonym().symbol, move(right_values), rhs_executors.second},
	            {lhs.getSynonym().symbol, move(left_values), lhs_executors.second});
}

template <typename TAttribute, typename TLeft>
QP::QueryResult QP::Executor::WithExecutor::executeTrivialAttributeConstant(const QP::StorageAdapter& storage,
                                                                            const Types::ReferenceArgument& lhs,
                                                                            const Types::ReferenceArgument& rhs,
                                                                            Types::WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                                                            Types::WithConstantExtractor<TAttribute> rhs_executor) {
	unordered_set<TLeft> left_values = lhs_executors.first(storage, lhs);
	TAttribute right_value = rhs_executor(rhs);
	vector<string> left_column;
	for (const auto& left : left_values) {
		if (lhs_executors.second(storage, left) == right_value) {
			return QueryResult(true);
		}
	}
	return {};
}

template <typename TAttribute, typename TRight>
QP::QueryResult QP::Executor::WithExecutor::executeTrivialConstantAttribute(
	const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs, const Types::ReferenceArgument& rhs,
	Types::WithConstantExtractor<TAttribute> lhs_executor, Types::WithInternalExecutors<TAttribute, TRight> rhs_executors) {
	return executeTrivialAttributeConstant(storage, rhs, lhs, rhs_executors, lhs_executor);
}

template <typename TAttribute, typename TBuild, typename TProbe>
static QP::QueryResult hashJoin(const QP::StorageAdapter& storage, HashInfo<TAttribute, TBuild> build_info,
                                HashInfo<TAttribute, TProbe> probe_info) {
	unordered_multimap<TAttribute, TBuild> build_table;
	for (const auto& build : build_info.values) {
		build_table.insert(build_info.mapper(storage, build), build);
	}
	vector<string> build_column;
	vector<string> probe_column;
	for (const auto& probe : probe_info.values) {
		TAttribute attribute = probe_info.mapper(storage, probe);
		auto iter = build_table.equal_range(attribute);
		for_each(iter.first, iter.second, [&](const auto& pair) {
			build_column.push_back(pair.second);
			probe_column.push_back(probe);
		});
	}
	QP::QueryResult result;
	result.addColumn(build_info.symbol, build_column);
	result.addColumn(probe_info.symbol, probe_column);
	return result;
}

template <typename TAttribute, typename TLeft, typename TRight>
QP::QueryResult QP::Executor::WithExecutor::executeAttributeAttribute(const QP::StorageAdapter& storage,
                                                                      const Types::ReferenceArgument& lhs,
                                                                      const Types::ReferenceArgument& rhs,
                                                                      Types::WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                                                      Types::WithInternalExecutors<TAttribute, TRight> rhs_executors) {
	unordered_set<TLeft> left_values = lhs_executors.first(storage, lhs);
	unordered_set<TRight> right_values = rhs_executors.first(storage, rhs);
	// Reduce memory usage by building hash table on smaller set.
	if (left_values.size() < right_values.size()) {
		return hashJoin(storage, {lhs.getSynonym().symbol, move(left_values), lhs_executors.second},
		                {rhs.getSynonym().symbol, move(right_values), rhs_executors.second});
	}
	return hashJoin(storage, {rhs.getSynonym().symbol, move(right_values), rhs_executors.second},
	                {lhs.getSynonym().symbol, move(left_values), lhs_executors.second});
}

template <typename TAttribute, typename TLeft>
QP::QueryResult QP::Executor::WithExecutor::executeAttributeConstant(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
                                                                     const Types::ReferenceArgument& rhs,
                                                                     Types::WithInternalExecutors<TAttribute, TLeft> lhs_executors,
                                                                     Types::WithConstantExtractor<TAttribute> rhs_executor) {
	unordered_set<TLeft> left_values = lhs_executors.first(storage, lhs);
	TAttribute right_value = rhs_executor(rhs);
	vector<string> left_column;
	for (const auto& left : left_values) {
		if (lhs_executors.second(storage, left) == right_value) {
			left_column.push_back(left);
		}
	}
	QueryResult result;
	result.addColumn(lhs.getSynonym().symbol, left_column);
	return result;
}

template <typename TAttribute, typename TRight>
QP::QueryResult QP::Executor::WithExecutor::executeConstantAttribute(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
                                                                     const Types::ReferenceArgument& rhs,
                                                                     Types::WithConstantExtractor<TAttribute> lhs_executor,
                                                                     Types::WithInternalExecutors<TAttribute, TRight> rhs_executors) {
	return executeAttributeConstant(storage, rhs, lhs, rhs_executors, lhs_executor);
}