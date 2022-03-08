#include "QP/Relationship/Follows.h"

#include <utility>

QP::Relationship::Follows::Follows(ReferenceArgument left_stmt, ReferenceArgument right_stmt)
	: left_stmt(std::move(std::move(left_stmt))), right_stmt(std::move(std::move(right_stmt))) {}

ReferenceArgument QP::Relationship::Follows::getLeftStmt() { return left_stmt; }

ReferenceArgument QP::Relationship::Follows::getRightStmt() { return right_stmt; }

vector<string> QP::Relationship::Follows::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->left_stmt.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->left_stmt.getSynonym().symbol);
	}
	if (this->right_stmt.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->right_stmt.getSynonym().symbol);
	}
	return declaration_symbols;
}

QP::QueryResult QP::Relationship::Follows::executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& /*map*/) {
	if (left_stmt.getType() == ReferenceType::StatementIndex && right_stmt.getType() == ReferenceType::StatementIndex) {
		return QueryResult(pkb.checkFollows(left_stmt.getStatementIndex(), right_stmt.getStatementIndex()));
	}
	if (left_stmt.getType() == ReferenceType::StatementIndex && right_stmt.getType() == ReferenceType::Wildcard) {
		shared_ptr<StmtInfo> stmt = pkb.getFollower(left_stmt.getStatementIndex());
		return QueryResult(stmt != nullptr);
	}
	if (left_stmt.getType() == ReferenceType::StatementIndex && right_stmt.getType() == ReferenceType::Synonym) {
		shared_ptr<StmtInfo> stmt = pkb.getFollower(left_stmt.getStatementIndex());
		DesignEntity design_entity = right_stmt.getSynonym().type;

		if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			return QueryResult(true);
		}
	} else if (left_stmt.getType() == ReferenceType::Wildcard && right_stmt.getType() == ReferenceType::StatementIndex) {
		shared_ptr<StmtInfo> stmt = pkb.getPreceding(right_stmt.getStatementIndex());
		return QueryResult(stmt != nullptr);
	} else if (left_stmt.getType() == ReferenceType::Wildcard && right_stmt.getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		for (auto const& stmt : stmt_set) {
			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt->getIdentifier());
			if (follower != nullptr) {
				return QueryResult(true);
			}
		}
	} else if (left_stmt.getType() == ReferenceType::Wildcard && right_stmt.getType() == ReferenceType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = right_stmt.getSynonym().type;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> preceding = pkb.getPreceding(stmt->getIdentifier());
			if (preceding != nullptr) {
				return QueryResult(true);
			}
		}
	} else if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::StatementIndex) {
		shared_ptr<StmtInfo> preceding = pkb.getPreceding(right_stmt.getStatementIndex());
		DesignEntity design_entity = left_stmt.getSynonym().type;
		if (!Utilities::checkStmtTypeMatch(preceding, design_entity)) {
			return {};
		}
		return QueryResult(true);
	} else if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = left_stmt.getSynonym().type;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt->getIdentifier());
			if (follower != nullptr) {
				return QueryResult(true);
			}
		}
	} else if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::Synonym) {
		if (left_stmt.getSynonym().symbol == right_stmt.getSynonym().symbol) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity left_design_entity = left_stmt.getSynonym().type;
		DesignEntity right_design_entity = right_stmt.getSynonym().type;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, left_design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt->getIdentifier());
			if (Utilities::checkStmtTypeMatch(follower, right_design_entity)) {
				return QueryResult(true);
			}
		}
	}

	return {};
}

QP::QueryResult QP::Relationship::Follows::executeNonTrivial(PKB::StorageAccessInterface& pkb,
                                                             unordered_map<string, DesignEntity>& /*map*/) {
	if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::StatementIndex) {
		shared_ptr<StmtInfo> preceding = pkb.getPreceding(right_stmt.getStatementIndex());
		DesignEntity design_entity = left_stmt.getSynonym().type;
		vector<string> column;
		if (!Utilities::checkStmtTypeMatch(preceding, design_entity)) {
			return {};
		}
		column.push_back(to_string(preceding->getIdentifier()));
		QueryResult result = QueryResult();
		result.addColumn(left_stmt.getSynonym().symbol, column);
		return result;
	}
	if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = left_stmt.getSynonym().type;
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt->getIdentifier());
			if (follower != nullptr) {
				column.push_back(to_string(stmt->getIdentifier()));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(left_stmt.getSynonym().symbol, column);
		return result;
	}
	if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::Synonym) {
		if (left_stmt.getSynonym().symbol == right_stmt.getSynonym().symbol) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity left_design_entity = left_stmt.getSynonym().type;
		DesignEntity right_design_entity = right_stmt.getSynonym().type;
		vector<string> left_column;
		vector<string> right_column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, left_design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt->getIdentifier());
			if (Utilities::checkStmtTypeMatch(follower, right_design_entity)) {
				left_column.push_back(to_string(stmt->getIdentifier()));
				right_column.push_back(to_string(follower->getIdentifier()));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(left_stmt.getSynonym().symbol, left_column);
		result.addColumn(right_stmt.getSynonym().symbol, right_column);
		return result;
	}
	if (left_stmt.getType() == ReferenceType::Wildcard && right_stmt.getType() == ReferenceType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = right_stmt.getSynonym().type;
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> preceding = pkb.getPreceding(stmt->getIdentifier());
			if (preceding != nullptr) {
				column.push_back(to_string(stmt->getIdentifier()));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(right_stmt.getSynonym().symbol, column);
		return result;
	}
	if (left_stmt.getType() == ReferenceType::StatementIndex && right_stmt.getType() == ReferenceType::Synonym) {
		shared_ptr<StmtInfo> stmt = pkb.getFollower(left_stmt.getStatementIndex());
		DesignEntity design_entity = right_stmt.getSynonym().type;
		vector<string> column;

		if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			column.push_back(to_string(stmt->getIdentifier()));
		}
		QueryResult result = QueryResult();
		result.addColumn(right_stmt.getSynonym().symbol, column);
		return result;
	}

	return {};
}
