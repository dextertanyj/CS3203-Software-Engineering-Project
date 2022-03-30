#ifndef SPA_STATEMENTEXECUTOR_TPP
#define SPA_STATEMENTEXECUTOR_TPP

#include "QP/Executor/StatementExecutor.h"

#include "QP/QueryUtils.h"

namespace QP::Executor::StatementExecutor {

// Trivial Executors
template <ClauseType T>
QueryResult executeTrivialIndexIndex(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	return QueryResult(storage.checkStatementRelation<T>(lhs.getStatementIndex(), rhs.getStatementIndex()));
}

template <ClauseType T>
QueryResult executeTrivialIndexWildcard(const StorageAdapter& storage, const ReferenceArgument& lhs) {
	return QueryResult(!storage.getReverseStatements<T>(lhs.getStatementIndex()).empty());
}

template <ClauseType T>
QueryResult executeTrivialIndexSynonym(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	StmtInfoPtrSet rhs_set = storage.getReverseStatements<T>(lhs.getStatementIndex());
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
QueryResult executeTrivialWildcardIndex(const StorageAdapter& storage, const ReferenceArgument& rhs) {
	return QueryResult(!storage.getForwardStatements<T>(rhs.getStatementIndex()).empty());
}

template <ClauseType T>
QueryResult executeTrivialWildcardWildcard(const StorageAdapter& storage) {
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		StmtInfoPtrSet rhs_set = storage.getReverseStatements<T>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <>
inline QueryResult executeTrivialWildcardWildcard<ClauseType::Parent>(const StorageAdapter& storage) {
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		// Avoid call to PKB unless statement can contain others.
		if (statement->getType() != StmtType::IfStmt && statement->getType() != StmtType::WhileStmt) {
			continue;
		}
		StmtInfoPtrSet rhs_set = storage.getReverseStatements<ClauseType::Parent>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <>
inline QueryResult executeTrivialWildcardWildcard<ClauseType::ParentT>(const StorageAdapter& storage) {
	return executeTrivialWildcardWildcard<ClauseType::Parent>(storage);
}

template <ClauseType T>
QueryResult executeTrivialWildcardSynonym(const StorageAdapter& storage, const ReferenceArgument& rhs) {
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, rhs.getSynonym().type)) {
			continue;
		}
		StmtInfoPtrSet lhs_set = storage.getForwardStatements<T>(statement->getIdentifier());
		if (!lhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeTrivialSynonymIndex(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	StmtInfoPtrSet lhs_set = storage.getForwardStatements<T>(rhs.getStatementIndex());
	for (auto const& lhs_statement : lhs_set) {
		if (Utilities::checkStmtTypeMatch(lhs_statement, lhs.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeTrivialSynonymWildcard(const StorageAdapter& storage, const ReferenceArgument& lhs) {
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = storage.getReverseStatements<T>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
static QueryResult executeTrivialStatementSynonymSynonym(const StorageAdapter& storage, const ReferenceArgument& lhs,
                                                         const ReferenceArgument& rhs) {
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = storage.getReverseStatements<T>(statement->getIdentifier());
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
QueryResult executeTrivialSynonymSynonym(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return {};
	}

	return executeTrivialStatementSynonymSynonym<T>(storage, lhs, rhs);
}

// Next* specialization to account for semantically correct same synonym case.
template <>
inline QueryResult executeTrivialSynonymSynonym<ClauseType::NextT>(const StorageAdapter& storage, const ReferenceArgument& lhs,
                                                                   const ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol != rhs.getSynonym().symbol) {
		return executeTrivialStatementSynonymSynonym<ClauseType::NextT>(storage, lhs, rhs);
	}
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet others = storage.getReverseStatements<ClauseType::NextT>(statement->getIdentifier());
		if (others.find(statement) != others.end()) {
			return QueryResult(true);
		}
	}
	return {};
}

// Executors
template <ClauseType T>
QueryResult executeIndexSynonym(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	QueryResult result = QueryResult({rhs.getSynonym().symbol});

	StmtInfoPtrSet statements = storage.getReverseStatements<T>(lhs.getStatementIndex());
	for (auto const& statement : statements) {
		if (Utilities::checkStmtTypeMatch(statement, rhs.getSynonym().type)) {
			result.addRow({to_string(statement->getIdentifier())});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeWildcardSynonym(const StorageAdapter& storage, const ReferenceArgument& rhs) {
	QueryResult result = QueryResult({rhs.getSynonym().symbol});
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, rhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet lhs_set = storage.getForwardStatements<T>(statement->getIdentifier());
		if (!lhs_set.empty()) {
			result.addRow({to_string(statement->getIdentifier())});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymIndex(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	QueryResult result = QueryResult({lhs.getSynonym().symbol});
	StmtInfoPtrSet lhs_set = storage.getForwardStatements<T>(rhs.getStatementIndex());
	vector<string> column;
	for (auto const& lhs_statement : lhs_set) {
		if (Utilities::checkStmtTypeMatch(lhs_statement, lhs.getSynonym().type)) {
			result.addRow({to_string(lhs_statement->getIdentifier())});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& storage, const ReferenceArgument& lhs) {
	QueryResult result = QueryResult({lhs.getSynonym().symbol});
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = storage.getReverseStatements<T>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			result.addRow({to_string(statement->getIdentifier())});
		}
	}
	return result;
}

template <ClauseType T>
static QueryResult executeStatementSynonymSynonym(const StorageAdapter& storage, const ReferenceArgument& lhs,
                                                  const ReferenceArgument& rhs) {
	QueryResult result = QueryResult({lhs.getSynonym().symbol, rhs.getSynonym().symbol});
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = storage.getReverseStatements<T>(statement->getIdentifier());
		for (auto const& rhs_statement : rhs_set) {
			if (!Utilities::checkStmtTypeMatch(rhs_statement, rhs.getSynonym().type)) {
				continue;
			}
			result.addRow({to_string(statement->getIdentifier()), to_string(rhs_statement->getIdentifier())});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& storage, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return {};
	}
	return executeStatementSynonymSynonym<T>(storage, lhs, rhs);
}

// Next* specialization to account for semantically correct same synonym case.
template <>
inline QueryResult executeSynonymSynonym<ClauseType::NextT>(const StorageAdapter& storage, const ReferenceArgument& lhs,
                                                            const ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol != rhs.getSynonym().symbol) {
		return executeStatementSynonymSynonym<ClauseType::NextT>(storage, lhs, rhs);
	}

	QueryResult result = QueryResult({lhs.getSynonym().symbol});
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet others = storage.getReverseStatements<ClauseType::NextT>(statement->getIdentifier());
		if (others.find(statement) == others.end()) {
			continue;
		}
		result.addRow({to_string(statement->getIdentifier())});
	}

	return result;
}

// Executor Set Factories

template <ClauseType T>
ExecutorSet executorFactoryIndexIndex(const vector<ReferenceArgument>& args) {
	return [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& storage) { return executeTrivialIndexIndex<T>(storage, lhs, rhs); };
}

template <ClauseType T>
ExecutorSet executorFactoryIndexWildcard(const vector<ReferenceArgument>& args) {
	return [lhs = args.at(0)](const StorageAdapter& storage) { return executeTrivialIndexWildcard<T>(storage, lhs); };
}

template <ClauseType T>
ExecutorSet executorFactoryIndexSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& storage) {
		return executeTrivialIndexSynonym<T>(storage, lhs, rhs);
	};
	Types::Executor executor = [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& storage) {
		return executeIndexSynonym<T>(storage, lhs, rhs);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardIndex(const vector<ReferenceArgument>& args) {
	return [rhs = args.at(1)](const StorageAdapter& storage) { return executeTrivialWildcardIndex<T>(storage, rhs); };
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardWildcard(const vector<ReferenceArgument>& /*args*/) {
	return [](const StorageAdapter& storage) { return executeTrivialWildcardWildcard<T>(storage); };
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [rhs = args.at(1)](const StorageAdapter& storage) {
		return executeTrivialWildcardSynonym<T>(storage, rhs);
	};
	Types::Executor executor = [rhs = args.at(1)](const StorageAdapter& storage) { return executeWildcardSynonym<T>(storage, rhs); };
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymIndex(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& storage) {
		return executeTrivialSynonymIndex<T>(storage, lhs, rhs);
	};
	Types::Executor executor = [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& storage) {
		return executeSynonymIndex<T>(storage, lhs, rhs);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [lhs = args.at(0)](const StorageAdapter& storage) {
		return executeTrivialSynonymWildcard<T>(storage, lhs);
	};
	Types::Executor executor = [lhs = args.at(0)](const StorageAdapter& storage) { return executeSynonymWildcard<T>(storage, lhs); };
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& storage) {
		return executeTrivialSynonymSynonym<T>(storage, lhs, rhs);
	};
	Types::Executor executor = [lhs = args.at(0), rhs = args.at(1)](const StorageAdapter& storage) {
		return executeSynonymSynonym<T>(storage, lhs, rhs);
	};
	return pair{trivial_executor, executor};
}

}

#endif  // SPA_STATEMENTEXECUTOR_TPP
