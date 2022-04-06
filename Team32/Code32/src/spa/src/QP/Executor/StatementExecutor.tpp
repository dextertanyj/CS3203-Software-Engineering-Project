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
QueryResult executeTrivialIndexIndex(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	return QueryResult(store.checkStatementRelation<T>(lhs.getStatementIndex(), rhs.getStatementIndex()));
}

template <ClauseType T>
QueryResult executeTrivialIndexWildcard(const StorageAdapter& store, const ReferenceArgument& lhs) {
	return QueryResult(!store.getReverseStatements<T>(lhs.getStatementIndex()).empty());
}

template <ClauseType T>
QueryResult executeTrivialIndexSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	StmtInfoPtrSet rhs_set = store.getReverseStatements<T>(lhs.getStatementIndex());
	if (rhs.getSynonym().type == DesignEntity::Stmt) {
		return QueryResult(!rhs_set.empty());
	}

	for (auto const& rhs_statement : rhs_set) {
		if (Utilities::checkStmtTypeMatch(rhs_statement, rhs.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeTrivialWildcardIndex(const StorageAdapter& store, const ReferenceArgument& rhs) {
	return QueryResult(!store.getForwardStatements<T>(rhs.getStatementIndex()).empty());
}

template <ClauseType T>
QueryResult executeTrivialWildcardWildcard(const StorageAdapter& store) {
	StmtInfoPtrSet statements = store.getStatements();
	for (auto const& statement : statements) {
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
	for (auto const& statement : statements) {
		// Avoid call to PKB unless statement can contain others.
		if (statement->getType() != StmtType::If && statement->getType() != StmtType::WhileStmt) {
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
QueryResult executeTrivialWildcardSynonym(const StorageAdapter& store, const ReferenceArgument& rhs) {
	StmtInfoPtrSet statements = store.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, rhs.getSynonym().type)) {
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
QueryResult executeTrivialSynonymIndex(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	StmtInfoPtrSet lhs_set = store.getForwardStatements<T>(rhs.getStatementIndex());
	for (auto const& lhs_statement : lhs_set) {
		if (Utilities::checkStmtTypeMatch(lhs_statement, lhs.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeTrivialSynonymWildcard(const StorageAdapter& store, const ReferenceArgument& lhs) {
	StmtInfoPtrSet statements = store.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
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
static QueryResult executeTrivialStatementSynonymSynonym(const StorageAdapter& store, const ReferenceArgument& lhs,
                                                         const ReferenceArgument& rhs) {
	StmtInfoPtrSet statements = store.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = store.getReverseStatements<T>(statement->getIdentifier());
		bool match = any_of(rhs_set.begin(), rhs_set.end(), [&rhs](const auto& rhs_statement) {
			return Utilities::checkStmtTypeMatch(rhs_statement, rhs.getSynonym().type);
		});
		if (match) {
			return QueryResult(true);
		}
	}

	return {};
}

template <ClauseType T>
QueryResult executeTrivialSynonymSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return {};
	}

	return executeTrivialStatementSynonymSynonym<T>(store, lhs, rhs);
}

template <ClauseType T>
static QueryResult executeTrivialSameSynonymSynonym(const StorageAdapter& store, const ReferenceArgument& synonym) {
	StmtInfoPtrSet statements = store.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, synonym.getSynonym().type)) {
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
inline QueryResult executeTrivialSynonymSynonym<ClauseType::NextT>(const StorageAdapter& store, const ReferenceArgument& lhs,
                                                                   const ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return executeTrivialSameSynonymSynonym<ClauseType::NextT>(store, lhs);
	}
	return executeTrivialStatementSynonymSynonym<ClauseType::NextT>(store, lhs, rhs);
}

template <>
inline QueryResult executeTrivialSynonymSynonym<ClauseType::Affects>(const StorageAdapter& store, const ReferenceArgument& lhs,
                                                                     const ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return executeTrivialSameSynonymSynonym<ClauseType::Affects>(store, lhs);
	}
	return executeTrivialStatementSynonymSynonym<ClauseType::Affects>(store, lhs, rhs);
}

template <>
inline QueryResult executeTrivialSynonymSynonym<ClauseType::AffectsT>(const StorageAdapter& store, const ReferenceArgument& lhs,
                                                                      const ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return executeTrivialSameSynonymSynonym<ClauseType::AffectsT>(store, lhs);
	}
	return executeTrivialStatementSynonymSynonym<ClauseType::AffectsT>(store, lhs, rhs);
}

// Executors
template <ClauseType T>
QueryResult executeIndexSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	QueryResult result = QueryResult({rhs.getSynonym().symbol});

	StmtInfoPtrSet statements = store.getReverseStatements<T>(lhs.getStatementIndex());
	for (auto const& statement : statements) {
		if (Utilities::checkStmtTypeMatch(statement, rhs.getSynonym().type)) {
			result.addRow({to_string(statement->getIdentifier())});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeWildcardSynonym(const StorageAdapter& store, const ReferenceArgument& rhs) {
	QueryResult result = QueryResult({rhs.getSynonym().symbol});
	StmtInfoPtrSet statements = store.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, rhs.getSynonym().type)) {
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
QueryResult executeSynonymIndex(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	QueryResult result = QueryResult({lhs.getSynonym().symbol});
	StmtInfoPtrSet lhs_set = store.getForwardStatements<T>(rhs.getStatementIndex());
	vector<string> column;
	for (auto const& lhs_statement : lhs_set) {
		if (Utilities::checkStmtTypeMatch(lhs_statement, lhs.getSynonym().type)) {
			result.addRow({to_string(lhs_statement->getIdentifier())});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& store, const ReferenceArgument& lhs) {
	QueryResult result = QueryResult({lhs.getSynonym().symbol});
	StmtInfoPtrSet statements = store.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = store.getReverseStatements<T>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			result.addRow({to_string(statement->getIdentifier())});
		}
	}
	return result;
}

static void checkSet(StmtRef lhs, const ReferenceArgument& rhs, const StmtInfoPtrSet& rhs_set, QueryResult& result) {
	for (auto const& rhs_statement : rhs_set) {
		if (!Utilities::checkStmtTypeMatch(rhs_statement, rhs.getSynonym().type)) {
			continue;
		}
		result.addRow({to_string(lhs), to_string(rhs_statement->getIdentifier())});
	}
}

template <ClauseType T>
static QueryResult executeStatementSynonymSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	QueryResult result = QueryResult({lhs.getSynonym().symbol, rhs.getSynonym().symbol});
	StmtInfoPtrSet statements = store.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = store.getReverseStatements<T>(statement->getIdentifier());
		checkSet(statement->getIdentifier(), rhs, rhs_set, result);
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return {};
	}
	return executeStatementSynonymSynonym<T>(store, lhs, rhs);
}

template <ClauseType T>
static QueryResult executeSameSynonymSynonym(const StorageAdapter& store, const ReferenceArgument& synonym) {
	QueryResult result = QueryResult({synonym.getSynonym().symbol});
	StmtInfoPtrSet statements = store.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, synonym.getSynonym().type)) {
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
inline QueryResult executeSynonymSynonym<ClauseType::NextT>(const StorageAdapter& store, const ReferenceArgument& lhs,
                                                            const ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return executeSameSynonymSynonym<ClauseType::NextT>(store, lhs);
	}
	return executeStatementSynonymSynonym<ClauseType::NextT>(store, lhs, rhs);
}

template <>
inline QueryResult executeSynonymSynonym<ClauseType::Affects>(const StorageAdapter& store, const ReferenceArgument& lhs,
                                                              const ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return executeSameSynonymSynonym<ClauseType::Affects>(store, lhs);
	}
	return executeStatementSynonymSynonym<ClauseType::Affects>(store, lhs, rhs);
}

template <>
inline QueryResult executeSynonymSynonym<ClauseType::AffectsT>(const StorageAdapter& store, const ReferenceArgument& lhs,
                                                               const ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return executeSameSynonymSynonym<ClauseType::AffectsT>(store, lhs);
	}
	return executeStatementSynonymSynonym<ClauseType::AffectsT>(store, lhs, rhs);
}

// Optimized Executors

inline set<StmtRef, greater<>> buildCandidates(const vector<string>& existing_results) {
	set<StmtRef, greater<>> candidates;
	transform(existing_results.begin(), existing_results.end(), inserter(candidates, candidates.end()),
	          [](const auto& result) { return (stoull(result)); });
	return candidates;
}

template <ClauseType T>
QueryResult executeWildcardSynonymOptimized(const StorageAdapter& store, const QueryResult& existing_result, const ReferenceArgument& rhs) {
	if (!existing_result.containsSynonym(rhs.getSynonymSymbol())) {
		return executeWildcardSynonym<T>(store, rhs);
	}

	auto existing_results = existing_result.getSynonymResult(rhs.getSynonymSymbol());
	QueryResult result = QueryResult({rhs.getSynonymSymbol()});
	auto candidates = buildCandidates(existing_results);

	for (auto const& candidate : candidates) {
		StmtInfoPtrSet rhs_set = store.getForwardStatements<T>(candidate);
		if (!rhs_set.empty()) {
			result.addRow({to_string(candidate)});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymWildcardOptimized(const StorageAdapter& store, const QueryResult& existing_result, const ReferenceArgument& lhs) {
	if (!existing_result.containsSynonym(lhs.getSynonymSymbol())) {
		return executeSynonymWildcard<T>(store, lhs);
	}

	auto existing_results = existing_result.getSynonymResult(lhs.getSynonymSymbol());
	QueryResult result = QueryResult({lhs.getSynonymSymbol()});
	auto candidates = buildCandidates(existing_results);

	for (auto const& candidate : candidates) {
		StmtInfoPtrSet rhs_set = store.getReverseStatements<T>(candidate);
		if (!rhs_set.empty()) {
			result.addRow({to_string(candidate)});
		}
	}

	return result;
}

template <ClauseType T>
static QueryResult executeSameSynonymSynonymOptimized(const StorageAdapter& store, const QueryResult& existing_result,
                                                      const ReferenceArgument& synonym) {
	auto existing_results = existing_result.getSynonymResult(synonym.getSynonymSymbol());
	QueryResult result = QueryResult({synonym.getSynonymSymbol()});
	auto candidates = buildCandidates(existing_results);

	for (auto const& candidate : candidates) {
		StmtInfoPtrSet other_set = store.getReverseStatements<T>(candidate);
		auto satisfied = any_of(other_set.begin(), other_set.end(), [&](const auto& info) { return info->getIdentifier() == candidate; });
		if (satisfied) {
			result.addRow({to_string(candidate)});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymSynonymOptimized(const StorageAdapter& store, const QueryResult& existing_result, const ReferenceArgument& lhs,
                                           const ReferenceArgument& rhs) {
	bool has_left = existing_result.containsSynonym(lhs.getSynonymSymbol());
	bool has_right = existing_result.containsSynonym(rhs.getSynonymSymbol());
	if (!has_left && !has_right) {
		return executeSynonymSynonym<T>(store, lhs, rhs);
	}

	if (lhs.getSynonymSymbol() == rhs.getSynonymSymbol()) {
		return executeSameSynonymSynonymOptimized<T>(store, existing_result, lhs);
	}

	vector<string> existing_results;
	QueryResult result;
	ReferenceArgument other;
	StmtInfoPtrSet (StorageAdapter::*getter)(StmtRef) const;

	if (has_left) {
		other = rhs;
		result = QueryResult({lhs.getSynonymSymbol(), rhs.getSynonymSymbol()});
		existing_results = existing_result.getSynonymResult(lhs.getSynonymSymbol());
		getter = &StorageAdapter::getReverseStatements<T>;
	} else {
		other = lhs;
		result = QueryResult({rhs.getSynonymSymbol(), lhs.getSynonymSymbol()});
		existing_results = existing_result.getSynonymResult(rhs.getSynonymSymbol());
		getter = &StorageAdapter::getForwardStatements<T>;
	}

	set<StmtRef, greater<>> candidates = buildCandidates(existing_results);

	for (const auto& candidate : candidates) {
		StmtInfoPtrSet other_set = (&store->*getter)(candidate);
		checkSet(candidate, other, other_set, result);
	}

	return result;
}

// Executor Set Factories

template <ClauseType T>
ExecutorSet executorFactoryIndexIndex(const vector<ReferenceArgument>& args) {
	return [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& store) { return executeTrivialIndexIndex<T>(store, lhs, rhs); };
}

template <ClauseType T>
ExecutorSet executorFactoryIndexWildcard(const vector<ReferenceArgument>& args) {
	return [lhs = args.at(0)](const StorageAdapter& store) { return executeTrivialIndexWildcard<T>(store, lhs); };
}

template <ClauseType T>
ExecutorSet executorFactoryIndexSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& store) {
		return executeTrivialIndexSynonym<T>(store, lhs, rhs);
	};
	Types::Executor executor = [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& store) {
		return executeIndexSynonym<T>(store, lhs, rhs);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardIndex(const vector<ReferenceArgument>& args) {
	return [rhs = args.at(1)](const StorageAdapter& store) { return executeTrivialWildcardIndex<T>(store, rhs); };
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardWildcard(const vector<ReferenceArgument>& /*args*/) {
	return [](const StorageAdapter& store) { return executeTrivialWildcardWildcard<T>(store); };
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [rhs = args.at(1)](const StorageAdapter& store) {
		return executeTrivialWildcardSynonym<T>(store, rhs);
	};
	Types::Executor executor = [rhs = args.at(1)](const StorageAdapter& store) { return executeWildcardSynonym<T>(store, rhs); };
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymIndex(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& store) {
		return executeTrivialSynonymIndex<T>(store, lhs, rhs);
	};
	Types::Executor executor = [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& store) {
		return executeSynonymIndex<T>(store, lhs, rhs);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [lhs = args.at(0)](const StorageAdapter& store) {
		return executeTrivialSynonymWildcard<T>(store, lhs);
	};
	Types::Executor executor = [lhs = args.at(0)](const StorageAdapter& store) { return executeSynonymWildcard<T>(store, lhs); };
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& store) {
		return executeTrivialSynonymSynonym<T>(store, lhs, rhs);
	};
	Types::Executor executor = [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& store) {
		return executeSynonymSynonym<T>(store, lhs, rhs);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardSynonymOptimized(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [rhs = args.at(1)](const StorageAdapter& store) {
		return executeTrivialWildcardSynonym<T>(store, rhs);
	};
	Types::OptimizedExecutor executor = [rhs = args.at(1)](const StorageAdapter& store, const QueryResult& result) {
		return executeWildcardSynonymOptimized<T>(store, result, rhs);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymWildcardOptimized(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [lhs = args.at(0)](const StorageAdapter& store) {
		return executeTrivialSynonymWildcard<T>(store, lhs);
	};
	Types::OptimizedExecutor executor = [lhs = args.at(0)](const StorageAdapter& store, const QueryResult& result) {
		return executeSynonymWildcardOptimized<T>(store, result, lhs);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymSynonymOptimized(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& store) {
		return executeTrivialSynonymSynonym<T>(store, lhs, rhs);
	};
	Types::OptimizedExecutor executor = [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& store, const QueryResult& result) {
		return executeSynonymSynonymOptimized<T>(store, result, lhs, rhs);
	};
	return pair{trivial_executor, executor};
}

}

#endif  // SPA_STATEMENTEXECUTOR_TPP
