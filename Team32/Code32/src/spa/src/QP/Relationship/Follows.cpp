#include "QP/Relationship/Follows.h"

#include <utility>

Follows::Follows(QueryStmtRef left_stmt, QueryStmtRef right_stmt)
	: left_stmt(std::move(std::move(left_stmt))), right_stmt(std::move(std::move(right_stmt))) {}

QueryStmtRef Follows::getLeftStmt() {
	return left_stmt;
}

QueryStmtRef Follows::getRightStmt() {
	return right_stmt;
}

QueryResult Follows::execute(PKB& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) {
	return is_trivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

vector<string> Follows::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->left_stmt.type == StmtRefType::Synonym) {
		declaration_symbols.push_back(this->left_stmt.stmt_ref);
	}
	if (this->right_stmt.type == StmtRefType::Synonym) {
		declaration_symbols.push_back(this->right_stmt.stmt_ref);
	}
	return declaration_symbols;
}

QueryResult Follows::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (left_stmt.type == StmtRefType::StmtNumber && right_stmt.type == StmtRefType::StmtNumber) {
		return QueryResult(pkb.checkFollows(stoul(left_stmt.stmt_ref), stoul(right_stmt.stmt_ref)));
	}
	if (left_stmt.type == StmtRefType::StmtNumber && right_stmt.type == StmtRefType::Underscore) {
		shared_ptr<StmtInfo> stmt = pkb.getFollower(stoul(left_stmt.stmt_ref));
		return QueryResult(stmt.get() != nullptr);
	} else if (left_stmt.type == StmtRefType::StmtNumber && right_stmt.type == StmtRefType::Synonym) {
		shared_ptr<StmtInfo> stmt = pkb.getFollower(stoul(left_stmt.stmt_ref));
		DesignEntity designEntity = map[right_stmt.stmt_ref];

		if (QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
			return QueryResult(true);
		}
	} else if (left_stmt.type == StmtRefType::Underscore && right_stmt.type == StmtRefType::StmtNumber) {
		shared_ptr<StmtInfo> stmt = pkb.getPreceding(stoul(right_stmt.stmt_ref));
		return QueryResult(stmt.get() != nullptr);
	} else if (left_stmt.type == StmtRefType::Underscore && right_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		for (auto const& stmt : stmtSet) {
			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt.get()->reference);
			if (follower.get() != nullptr) {
				return QueryResult(true);
			}
		}
	} else if (left_stmt.type == StmtRefType::Underscore && right_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[right_stmt.stmt_ref];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			shared_ptr<StmtInfo> preceding = pkb.getPreceding(stmt.get()->reference);
			if (preceding != nullptr) {
				return QueryResult(true);
			}
		}
	} else if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::StmtNumber) {
		shared_ptr<StmtInfo> preceding = pkb.getPreceding(stoul(right_stmt.stmt_ref));
		DesignEntity designEntity = map[left_stmt.stmt_ref];
		if (!QueryUtils::checkStmtTypeMatch(preceding, designEntity)) {
			return QueryResult();
		}
		return QueryResult(true);
	} else if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[left_stmt.stmt_ref];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt.get()->reference);
			if (follower != nullptr) {
				return QueryResult(true);
			}
		}
	} else if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::Synonym) {
		if (left_stmt.stmt_ref == right_stmt.stmt_ref) {
			return QueryResult();
		}

		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity leftDesignEntity = map[left_stmt.stmt_ref];
		DesignEntity rightDesignEntity = map[right_stmt.stmt_ref];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, leftDesignEntity)) {
				continue;
			}

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt.get()->reference);
			if (QueryUtils::checkStmtTypeMatch(follower, rightDesignEntity)) {
				return QueryResult(true);
			}
		}
	}

	return {};
}

QueryResult Follows::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::StmtNumber) {
		shared_ptr<StmtInfo> preceding = pkb.getPreceding(stoul(right_stmt.stmt_ref));
		DesignEntity design_entity = map[left_stmt.stmt_ref];
		vector<string> column;
		if (!QueryUtils::checkStmtTypeMatch(preceding, design_entity)) {
			return {};
		}
		column.push_back(to_string(preceding->reference));
		QueryResult result = QueryResult();
		result.addColumn(left_stmt.stmt_ref, column);
		return result;
	}
	if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[left_stmt.stmt_ref];
		vector<string> column;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt.get()->reference);
			if (follower.get() != nullptr) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(left_stmt.stmt_ref, column);
		return result;
	} else if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::Synonym) {
		if (left_stmt.stmt_ref == right_stmt.stmt_ref) {
			return QueryResult();
		}

		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity leftDesignEntity = map[left_stmt.stmt_ref];
		DesignEntity rightDesignEntity = map[right_stmt.stmt_ref];
		vector<string> leftColumn;
		vector<string> rightColumn;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, leftDesignEntity)) {
				continue;
			}

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt.get()->reference);
			if (QueryUtils::checkStmtTypeMatch(follower, rightDesignEntity)) {
				leftColumn.push_back(to_string(stmt.get()->reference));
				rightColumn.push_back(to_string(follower.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(left_stmt.stmt_ref, leftColumn);
		result.addColumn(right_stmt.stmt_ref, rightColumn);
		return result;
	} else if (left_stmt.type == StmtRefType::Underscore && right_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[right_stmt.stmt_ref];
		vector<string> column;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			shared_ptr<StmtInfo> preceding = pkb.getPreceding(stmt.get()->reference);
			if (preceding != nullptr) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(right_stmt.stmt_ref, column);
		return result;
	} else if (left_stmt.type == StmtRefType::StmtNumber && right_stmt.type == StmtRefType::Synonym) {
		shared_ptr<StmtInfo> stmt = pkb.getFollower(stoul(left_stmt.stmt_ref));
		DesignEntity designEntity = map[right_stmt.stmt_ref];
		vector<string> column;

		if (QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
			column.push_back(to_string(stmt.get()->reference));
		}
		QueryResult result = QueryResult();
		result.addColumn(right_stmt.stmt_ref, column);
		return result;
	}

	return {};
}
