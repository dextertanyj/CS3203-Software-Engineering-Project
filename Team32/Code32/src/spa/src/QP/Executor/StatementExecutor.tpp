#ifndef SPA_STATEMENTEXECUTOR_TPP
#define SPA_STATEMENTEXECUTOR_TPP

#include "QP/Executor/StatementExecutor.h"

#include <algorithm>
#include <iterator>

#include "QP/QueryUtils.h"

namespace QP::Executor::StatementExecutor {

using namespace std;

// Trivial Executors
template <ClauseType T>
QueryResult executeTrivialIndexIndex(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs) {
	return QueryResult(store.checkStatementRelation<T>(lhs.getStatementIndex(), rhs.getStatementIndex()));
}

template <ClauseType T>
QueryResult executeTrivialIndexWildcard(const StorageAdapter& store, const ClauseArgument& lhs) {
	return QueryResult(!store.getReverseStatements<T>(lhs.getStatementIndex()).empty());
}

template <ClauseType T>
QueryResult executeTrivialIndexSynonym(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs) {
	StmtInfoPtrSet rhs_set = store.getReverseStatements<T>(lhs.getStatementIndex());
	if (rhs.getSynonymType() == DesignEntity::Stmt) {
		return QueryResult(!rhs_set.empty());
	}

	for (const auto& rhs_statement : rhs_set) {
		if (Utilities::checkStmtTypeMatch(rhs_statement, rhs.getSynonymType())) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeTrivialWildcardIndex(const StorageAdapter& store, const ClauseArgument& rhs) {
	return QueryResult(!store.getForwardStatements<T>(rhs.getStatementIndex()).empty());
}

template <ClauseType T>
QueryResult executeTrivialWildcardWildcard(const StorageAdapter& store) {
	StmtInfoPtrSet statements = store.getStatements();
	for (const auto& statement : statements) {
		StmtInfoPtrSet rhs_set = store.getReverseStatements<T>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <>
inline QueryResult executeTrivialWildcardWildcard<ClauseType::Parent>(const StorageAdapter& store) {
	StmtInfoPtrSet statements = store.getStatements();
	for (const auto& statement : statements) {
		// Avoid call to PKB unless statement can contain others.
		if (statement->getType() != StmtType::If && statement->getType() != StmtType::While) {
			continue;
		}
		StmtInfoPtrSet rhs_set = store.getReverseStatements<ClauseType::Parent>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <>
inline QueryResult executeTrivialWildcardWildcard<ClauseType::ParentT>(const StorageAdapter& store) {
	return executeTrivialWildcardWildcard<ClauseType::Parent>(store);
}

template <ClauseType T>
QueryResult executeTrivialWildcardSynonym(const StorageAdapter& store, const ClauseArgument& rhs) {
	StmtInfoPtrSet statements = store.getStatements();
	for (const auto& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, rhs.getSynonymType())) {
			continue;
		}
		StmtInfoPtrSet lhs_set = store.getForwardStatements<T>(statement->getIdentifier());
		if (!lhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeTrivialSynonymIndex(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs) {
	StmtInfoPtrSet lhs_set = store.getForwardStatements<T>(rhs.getStatementIndex());
	for (const auto& lhs_statement : lhs_set) {
		if (Utilities::checkStmtTypeMatch(lhs_statement, lhs.getSynonymType())) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeTrivialSynonymWildcard(const StorageAdapter& store, const ClauseArgument& lhs) {
	StmtInfoPtrSet statements = store.getStatements();
	for (const auto& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonymType())) {
			continue;
		}

		StmtInfoPtrSet rhs_set = store.getReverseStatements<T>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
static QueryResult executeTrivialStatementSynonymSynonym(const StorageAdapter& store, const ClauseArgument& lhs,
                                                         const ClauseArgument& rhs) {
	StmtInfoPtrSet statements = store.getStatements();
	for (const auto& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonymType())) {
			continue;
		}

		StmtInfoPtrSet rhs_set = store.getReverseStatements<T>(statement->getIdentifier());
		bool match = any_of(rhs_set.begin(), rhs_set.end(), [&rhs](const auto& rhs_statement) {
			return Utilities::checkStmtTypeMatch(rhs_statement, rhs.getSynonymType());
		});
		if (match) {
			return QueryResult(true);
		}
	}

	return {};
}

template <ClauseType T>
QueryResult executeTrivialSynonymSynonym(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs) {
	if (lhs.getSynonymSymbol() == rhs.getSynonymSymbol()) {
		return {};
	}

	return executeTrivialStatementSynonymSynonym<T>(store, lhs, rhs);
}

template <ClauseType T>
static QueryResult executeTrivialSameSynonymSynonym(const StorageAdapter& store, const ClauseArgument& synonym) {
	StmtInfoPtrSet statements = store.getStatements();
	for (const auto& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, synonym.getSynonymType())) {
			continue;
		}

		StmtInfoPtrSet others = store.getReverseStatements<T>(statement->getIdentifier());
		if (others.find(statement) != others.end()) {
			return QueryResult(true);
		}
	}
	return {};
}

// Specialization to account for semantically correct same synonym case.
template <>
inline QueryResult executeTrivialSynonymSynonym<ClauseType::NextT>(const StorageAdapter& store, const ClauseArgument& lhs,
                                                                   const ClauseArgument& rhs) {
	if (lhs.getSynonymSymbol() == rhs.getSynonymSymbol()) {
		return executeTrivialSameSynonymSynonym<ClauseType::NextT>(store, lhs);
	}
	return executeTrivialStatementSynonymSynonym<ClauseType::NextT>(store, lhs, rhs);
}

template <>
inline QueryResult executeTrivialSynonymSynonym<ClauseType::Affects>(const StorageAdapter& store, const ClauseArgument& lhs,
                                                                     const ClauseArgument& rhs) {
	if (lhs.getSynonymSymbol() == rhs.getSynonymSymbol()) {
		return executeTrivialSameSynonymSynonym<ClauseType::Affects>(store, lhs);
	}
	return executeTrivialStatementSynonymSynonym<ClauseType::Affects>(store, lhs, rhs);
}

template <>
inline QueryResult executeTrivialSynonymSynonym<ClauseType::AffectsT>(const StorageAdapter& store, const ClauseArgument& lhs,
                                                                      const ClauseArgument& rhs) {
	if (lhs.getSynonymSymbol() == rhs.getSynonymSymbol()) {
		return executeTrivialSameSynonymSynonym<ClauseType::AffectsT>(store, lhs);
	}
	return executeTrivialStatementSynonymSynonym<ClauseType::AffectsT>(store, lhs, rhs);
}

// Executors
template <ClauseType T>
QueryResult executeIndexSynonym(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs) {
	QueryResult result = QueryResult({rhs.getSynonymSymbol()});

	StmtInfoPtrSet statements = store.getReverseStatements<T>(lhs.getStatementIndex());
	for (const auto& statement : statements) {
		if (Utilities::checkStmtTypeMatch(statement, rhs.getSynonymType())) {
			result.addRow({to_string(statement->getIdentifier())});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeWildcardSynonym(const StorageAdapter& store, const ClauseArgument& rhs) {
	QueryResult result = QueryResult({rhs.getSynonymSymbol()});
	StmtInfoPtrSet statements = store.getStatements();
	for (const auto& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, rhs.getSynonymType())) {
			continue;
		}

		StmtInfoPtrSet lhs_set = store.getForwardStatements<T>(statement->getIdentifier());
		if (!lhs_set.empty()) {
			result.addRow({to_string(statement->getIdentifier())});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymIndex(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs) {
	QueryResult result = QueryResult({lhs.getSynonymSymbol()});
	StmtInfoPtrSet lhs_set = store.getForwardStatements<T>(rhs.getStatementIndex());
	vector<string> column;
	for (const auto& lhs_statement : lhs_set) {
		if (Utilities::checkStmtTypeMatch(lhs_statement, lhs.getSynonymType())) {
			result.addRow({to_string(lhs_statement->getIdentifier())});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& store, const ClauseArgument& lhs) {
	QueryResult result = QueryResult({lhs.getSynonymSymbol()});
	StmtInfoPtrSet statements = store.getStatements();
	for (const auto& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonymType())) {
			continue;
		}

		StmtInfoPtrSet rhs_set = store.getReverseStatements<T>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			result.addRow({to_string(statement->getIdentifier())});
		}
	}
	return result;
}

static void checkSet(StmtRef lhs, const ClauseArgument& rhs, const StmtInfoPtrSet& rhs_set, QueryResult& result) {
	for (const auto& rhs_statement : rhs_set) {
		if (!Utilities::checkStmtTypeMatch(rhs_statement, rhs.getSynonymType())) {
			continue;
		}
		result.addRow({to_string(lhs), to_string(rhs_statement->getIdentifier())});
	}
}

template <ClauseType T>
static QueryResult executeStatementSynonymSynonym(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs) {
	QueryResult result = QueryResult({lhs.getSynonymSymbol(), rhs.getSynonymSymbol()});
	StmtInfoPtrSet statements = store.getStatements();
	for (const auto& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonymType())) {
			continue;
		}

		StmtInfoPtrSet rhs_set = store.getReverseStatements<T>(statement->getIdentifier());
		checkSet(statement->getIdentifier(), rhs, rhs_set, result);
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs) {
	if (lhs.getSynonymSymbol() == rhs.getSynonymSymbol()) {
		return {};
	}
	return executeStatementSynonymSynonym<T>(store, lhs, rhs);
}

template <ClauseType T>
static QueryResult executeSameSynonymSynonym(const StorageAdapter& store, const ClauseArgument& synonym) {
	QueryResult result = QueryResult({synonym.getSynonymSymbol()});
	StmtInfoPtrSet statements = store.getStatements();
	for (const auto& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, synonym.getSynonymType())) {
			continue;
		}

		StmtInfoPtrSet others = store.getReverseStatements<T>(statement->getIdentifier());
		if (others.find(statement) == others.end()) {
			continue;
		}
		result.addRow({to_string(statement->getIdentifier())});
	}

	return result;
}

// Specialization to account for semantically correct same synonym case.
template <>
inline QueryResult executeSynonymSynonym<ClauseType::NextT>(const StorageAdapter& store, const ClauseArgument& lhs,
                                                            const ClauseArgument& rhs) {
	if (lhs.getSynonymSymbol() == rhs.getSynonymSymbol()) {
		return executeSameSynonymSynonym<ClauseType::NextT>(store, lhs);
	}
	return executeStatementSynonymSynonym<ClauseType::NextT>(store, lhs, rhs);
}

template <>
inline QueryResult executeSynonymSynonym<ClauseType::Affects>(const StorageAdapter& store, const ClauseArgument& lhs,
                                                              const ClauseArgument& rhs) {
	if (lhs.getSynonymSymbol() == rhs.getSynonymSymbol()) {
		return executeSameSynonymSynonym<ClauseType::Affects>(store, lhs);
	}
	return executeStatementSynonymSynonym<ClauseType::Affects>(store, lhs, rhs);
}

template <>
inline QueryResult executeSynonymSynonym<ClauseType::AffectsT>(const StorageAdapter& store, const ClauseArgument& lhs,
                                                               const ClauseArgument& rhs) {
	if (lhs.getSynonymSymbol() == rhs.getSynonymSymbol()) {
		return executeSameSynonymSynonym<ClauseType::AffectsT>(store, lhs);
	}
	return executeStatementSynonymSynonym<ClauseType::AffectsT>(store, lhs, rhs);
}

// Optimized Executors

template <class Comparator>
inline set<StmtRef, Comparator> getAndSortCandidates(const StorageAdapter& store, const ClauseArgument& arg) {
	StmtInfoPtrSet statements = store.getStatements();
	set<StmtRef, Comparator> result;
	for (const auto& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, arg.getSynonymType())) {
			continue;
		}
		result.emplace(statement->getIdentifier());
	}
	return result;
}

template <class Comparator>
inline set<StmtRef, Comparator> sortCandidates(const vector<string>& existing_results) {
	set<StmtRef, Comparator> candidates;
	transform(existing_results.begin(), existing_results.end(), inserter(candidates, candidates.end()),
	          [](const auto& result) { return (stoull(result)); });
	return candidates;
}

/**
 * If existing intermediate results contain the synonym in the argument, convert the existing results and sort them.
 * Otherwise, query the store for new candidates and sort them.
 */
template <class Comparator>
inline set<StmtRef, Comparator> buildCandidates(const StorageAdapter& store, const QueryResult& existing_result,
                                                const ClauseArgument& arg) {
	set<StmtRef, Comparator> candidates;

	if (!existing_result.containsSynonym(arg.getSynonymSymbol())) {
		candidates = getAndSortCandidates<Comparator>(store, arg);
	} else {
		auto existing_results = existing_result.getSynonymResult(arg.getSynonymSymbol());
		candidates = sortCandidates<Comparator>(existing_results);
	}

	return candidates;
}

template <ClauseType T>
QueryResult executeWildcardSynonymOptimized(const StorageAdapter& store, const QueryResult& existing_result, const ClauseArgument& rhs) {
	set<StmtRef, less<>> candidates = buildCandidates<less<>>(store, existing_result, rhs);

	QueryResult result = QueryResult({rhs.getSynonymSymbol()});
	for (const auto& candidate : candidates) {
		StmtInfoPtrSet rhs_set = store.getForwardStatements<T>(candidate);
		if (!rhs_set.empty()) {
			result.addRow({to_string(candidate)});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymWildcardOptimized(const StorageAdapter& store, const QueryResult& existing_result, const ClauseArgument& lhs) {
	set<StmtRef, greater<>> candidates = buildCandidates<greater<>>(store, existing_result, lhs);

	QueryResult result = QueryResult({lhs.getSynonymSymbol()});
	for (const auto& candidate : candidates) {
		StmtInfoPtrSet rhs_set = store.getReverseStatements<T>(candidate);
		if (!rhs_set.empty()) {
			result.addRow({to_string(candidate)});
		}
	}

	return result;
}

template <ClauseType T>
static QueryResult executeSameSynonymSynonymOptimized(const StorageAdapter& store, const QueryResult& existing_result,
                                                      const ClauseArgument& synonym) {
	set<StmtRef, greater<>> candidates = buildCandidates<greater<>>(store, existing_result, synonym);

	QueryResult result = QueryResult({synonym.getSynonymSymbol()});
	for (const auto& candidate : candidates) {
		StmtInfoPtrSet other_set = store.getReverseStatements<T>(candidate);
		auto satisfied = any_of(other_set.begin(), other_set.end(), [&](const auto& info) { return info->getIdentifier() == candidate; });
		if (satisfied) {
			result.addRow({to_string(candidate)});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymSynonymOptimized(const StorageAdapter& store, const QueryResult& existing_result, const ClauseArgument& lhs,
                                           const ClauseArgument& rhs) {
	if (lhs.getSynonymSymbol() == rhs.getSynonymSymbol()) {
		return executeSameSynonymSynonymOptimized<T>(store, existing_result, lhs);
	}

	bool has_left = existing_result.containsSynonym(lhs.getSynonymSymbol());
	bool has_right = existing_result.containsSynonym(rhs.getSynonymSymbol());

	// If the intermediate result contains the left synonym or does not contain the right synonym,
	// we attempt to use the reverse direction query. This provides preferential use of the cache
	// of the reverse direction query in the store.
	if (has_left || !has_right) {
		auto result = QueryResult({lhs.getSynonymSymbol(), rhs.getSynonymSymbol()});
		auto candidates = buildCandidates<greater<>>(store, existing_result, lhs);
		for (const auto& candidate : candidates) {
			StmtInfoPtrSet rhs_set = store.getReverseStatements<T>(candidate);
			checkSet(candidate, rhs, rhs_set, result);
		}
		return result;
	}

	auto result = QueryResult({rhs.getSynonymSymbol(), lhs.getSynonymSymbol()});
	auto candidates = buildCandidates<less<>>(store, existing_result, rhs);
	for (const auto& candidate : candidates) {
		StmtInfoPtrSet lhs_set = store.getForwardStatements<T>(candidate);
		checkSet(candidate, lhs, lhs_set, result);
	}
	return result;
}

// Executor Set Factories

template <ClauseType T>
ExecutorSet executorFactoryIndexIndex(const vector<ClauseArgument>& args) {
	return [lhs = args[0], rhs = args[1]](const StorageAdapter& store) { return executeTrivialIndexIndex<T>(store, lhs, rhs); };
}

template <ClauseType T>
ExecutorSet executorFactoryIndexWildcard(const vector<ClauseArgument>& args) {
	return [lhs = args[0]](const StorageAdapter& store) { return executeTrivialIndexWildcard<T>(store, lhs); };
}

template <ClauseType T>
ExecutorSet executorFactoryIndexSynonym(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [lhs = args[0], rhs = args[1]](const StorageAdapter& store) {
		return executeTrivialIndexSynonym<T>(store, lhs, rhs);
	};
	Types::Executor executor = [lhs = args[0], rhs = args[1]](const StorageAdapter& store) {
		return executeIndexSynonym<T>(store, lhs, rhs);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardIndex(const vector<ClauseArgument>& args) {
	return [rhs = args[1]](const StorageAdapter& store) { return executeTrivialWildcardIndex<T>(store, rhs); };
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardWildcard(const vector<ClauseArgument>& /*args*/) {
	return [](const StorageAdapter& store) { return executeTrivialWildcardWildcard<T>(store); };
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardSynonym(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [rhs = args[1]](const StorageAdapter& store) {
		return executeTrivialWildcardSynonym<T>(store, rhs);
	};
	Types::Executor executor = [rhs = args[1]](const StorageAdapter& store) { return executeWildcardSynonym<T>(store, rhs); };
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymIndex(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [lhs = args[0], rhs = args[1]](const StorageAdapter& store) {
		return executeTrivialSynonymIndex<T>(store, lhs, rhs);
	};
	Types::Executor executor = [lhs = args[0], rhs = args[1]](const StorageAdapter& store) {
		return executeSynonymIndex<T>(store, lhs, rhs);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [lhs = args[0]](const StorageAdapter& store) {
		return executeTrivialSynonymWildcard<T>(store, lhs);
	};
	Types::Executor executor = [lhs = args[0]](const StorageAdapter& store) { return executeSynonymWildcard<T>(store, lhs); };
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [lhs = args[0], rhs = args[1]](const StorageAdapter& store) {
		return executeTrivialSynonymSynonym<T>(store, lhs, rhs);
	};
	Types::Executor executor = [lhs = args[0], rhs = args[1]](const StorageAdapter& store) {
		return executeSynonymSynonym<T>(store, lhs, rhs);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardSynonymOptimized(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [rhs = args[1]](const StorageAdapter& store) {
		return executeTrivialWildcardSynonym<T>(store, rhs);
	};
	Types::OptimizedExecutor executor = [rhs = args[1]](const StorageAdapter& store, const QueryResult& result) {
		return executeWildcardSynonymOptimized<T>(store, result, rhs);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymWildcardOptimized(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [lhs = args[0]](const StorageAdapter& store) {
		return executeTrivialSynonymWildcard<T>(store, lhs);
	};
	Types::OptimizedExecutor executor = [lhs = args[0]](const StorageAdapter& store, const QueryResult& result) {
		return executeSynonymWildcardOptimized<T>(store, result, lhs);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymSynonymOptimized(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [lhs = args[0], rhs = args[1]](const StorageAdapter& store) {
		return executeTrivialSynonymSynonym<T>(store, lhs, rhs);
	};
	Types::OptimizedExecutor executor = [lhs = args[0], rhs = args[1]](const StorageAdapter& store, const QueryResult& result) {
		return executeSynonymSynonymOptimized<T>(store, result, lhs, rhs);
	};
	return pair{trivial_executor, executor};
}

}

#endif  // SPA_STATEMENTEXECUTOR_TPP
