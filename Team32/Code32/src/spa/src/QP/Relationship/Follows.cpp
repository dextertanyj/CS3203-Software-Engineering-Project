#include "QP/Relationship/Follows.h"

#include <utility>

QP::Relationship::Follows::Follows(QueryStmtRef left_stmt, QueryStmtRef right_stmt)
	: left_stmt(std::move(std::move(left_stmt))), right_stmt(std::move(std::move(right_stmt))) {}

QueryStmtRef QP::Relationship::Follows::getLeftStmt() { return left_stmt; }

QueryStmtRef QP::Relationship::Follows::getRightStmt() { return right_stmt; }

QP::QueryResult QP::Relationship::Follows::execute(PKB::Storage& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) {
	return is_trivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

vector<string> QP::Relationship::Follows::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->left_stmt.type == StmtRefType::Synonym) {
		declaration_symbols.push_back(this->left_stmt.stmt_ref);
	}
	if (this->right_stmt.type == StmtRefType::Synonym) {
		declaration_symbols.push_back(this->right_stmt.stmt_ref);
	}
	return declaration_symbols;
}

QP::QueryResult QP::Relationship::Follows::executeTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) {
	if (left_stmt.type == StmtRefType::StmtNumber && right_stmt.type == StmtRefType::StmtNumber) {
		return QueryResult(pkb.checkFollows(stoul(left_stmt.stmt_ref), stoul(right_stmt.stmt_ref)));
	}
	if (left_stmt.type == StmtRefType::StmtNumber && right_stmt.type == StmtRefType::Underscore) {
		shared_ptr<StmtInfo> stmt = pkb.getFollower(stoul(left_stmt.stmt_ref));
		return QueryResult(stmt != nullptr);
	}
	if (left_stmt.type == StmtRefType::StmtNumber && right_stmt.type == StmtRefType::Synonym) {
		shared_ptr<StmtInfo> stmt = pkb.getFollower(stoul(left_stmt.stmt_ref));
		DesignEntity design_entity = map[right_stmt.stmt_ref];

		if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			return QueryResult(true);
		}
	} else if (left_stmt.type == StmtRefType::Underscore && right_stmt.type == StmtRefType::StmtNumber) {
		shared_ptr<StmtInfo> stmt = pkb.getPreceding(stoul(right_stmt.stmt_ref));
		return QueryResult(stmt != nullptr);
	} else if (left_stmt.type == StmtRefType::Underscore && right_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		for (auto const& stmt : stmt_set) {
			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt->reference);
			if (follower != nullptr) {
				return QueryResult(true);
			}
		}
	} else if (left_stmt.type == StmtRefType::Underscore && right_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[right_stmt.stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> preceding = pkb.getPreceding(stmt->reference);
			if (preceding != nullptr) {
				return QueryResult(true);
			}
		}
	} else if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::StmtNumber) {
		shared_ptr<StmtInfo> preceding = pkb.getPreceding(stoul(right_stmt.stmt_ref));
		DesignEntity design_entity = map[left_stmt.stmt_ref];
		if (!Utilities::checkStmtTypeMatch(preceding, design_entity)) {
			return {};
		}
		return QueryResult(true);
	} else if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[left_stmt.stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt->reference);
			if (follower != nullptr) {
				return QueryResult(true);
			}
		}
	} else if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::Synonym) {
		if (left_stmt.stmt_ref == right_stmt.stmt_ref) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity left_design_entity = map[left_stmt.stmt_ref];
		DesignEntity right_design_entity = map[right_stmt.stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, left_design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt->reference);
			if (Utilities::checkStmtTypeMatch(follower, right_design_entity)) {
				return QueryResult(true);
			}
		}
	}

	return {};
}

QP::QueryResult QP::Relationship::Follows::executeNonTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) {
	if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::StmtNumber) {
		shared_ptr<StmtInfo> preceding = pkb.getPreceding(stoul(right_stmt.stmt_ref));
		DesignEntity design_entity = map[left_stmt.stmt_ref];
		vector<string> column;
		if (!Utilities::checkStmtTypeMatch(preceding, design_entity)) {
			return {};
		}
		column.push_back(to_string(preceding->reference));
		QueryResult result = QueryResult();
		result.addColumn(left_stmt.stmt_ref, column);
		return result;
	}
	if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[left_stmt.stmt_ref];
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt->reference);
			if (follower != nullptr) {
				column.push_back(to_string(stmt->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(left_stmt.stmt_ref, column);
		return result;
	}
	if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::Synonym) {
		if (left_stmt.stmt_ref == right_stmt.stmt_ref) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity left_design_entity = map[left_stmt.stmt_ref];
		DesignEntity right_design_entity = map[right_stmt.stmt_ref];
		vector<string> left_column;
		vector<string> right_column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, left_design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt->reference);
			if (Utilities::checkStmtTypeMatch(follower, right_design_entity)) {
				left_column.push_back(to_string(stmt->reference));
				right_column.push_back(to_string(follower->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(left_stmt.stmt_ref, left_column);
		result.addColumn(right_stmt.stmt_ref, right_column);
		return result;
	}
	if (left_stmt.type == StmtRefType::Underscore && right_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[right_stmt.stmt_ref];
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> preceding = pkb.getPreceding(stmt->reference);
			if (preceding != nullptr) {
				column.push_back(to_string(stmt->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(right_stmt.stmt_ref, column);
		return result;
	}
	if (left_stmt.type == StmtRefType::StmtNumber && right_stmt.type == StmtRefType::Synonym) {
		shared_ptr<StmtInfo> stmt = pkb.getFollower(stoul(left_stmt.stmt_ref));
		DesignEntity design_entity = map[right_stmt.stmt_ref];
		vector<string> column;

		if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			column.push_back(to_string(stmt->reference));
		}
		QueryResult result = QueryResult();
		result.addColumn(right_stmt.stmt_ref, column);
		return result;
	}

	return {};
}
