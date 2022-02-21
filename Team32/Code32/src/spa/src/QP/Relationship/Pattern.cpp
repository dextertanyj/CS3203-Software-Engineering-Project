#include "QP/Relationship/Pattern.h"

Pattern::Pattern(Declaration syn_assign, QueryEntRef ent_ref, ExpressionType expression_type,
                 optional<Common::ExpressionProcessor::Expression> expression)
	: syn_assign(std::move(syn_assign)), ent_ref(std::move(ent_ref)), expression_type(expression_type), expression(std::move(expression)) {}

Declaration Pattern::getSynAssign() { return this->syn_assign; }

QueryEntRef Pattern::getEntRef() { return this->ent_ref; }

ExpressionType Pattern::getExpressionType() { return this->expression_type; }

Common::ExpressionProcessor::Expression Pattern::getExpression() { return this->expression.value(); }

QueryResult Pattern::execute(PKB::Storage& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) {
	return is_trivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

vector<string> Pattern::getDeclarationSymbols() {
	vector<string> declaration_symbols = {this->syn_assign.symbol};
	if (this->ent_ref.type == EntRefType::Synonym) {
		declaration_symbols.push_back(this->ent_ref.ent_ref);
	}
	return declaration_symbols;
}

QueryResult Pattern::executeTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& /*map*/) {
	StmtInfoPtrSet stmt_set;
	if (this->expression_type != ExpressionType::Underscore) {
		bool is_exact = this->expression_type != ExpressionType::ExpressionUnderscore;
		if (this->ent_ref.type == EntRefType::VarName) {
			return QueryResult(pkb.patternExists(this->ent_ref.ent_ref, getExpression(), is_exact));
		}
		auto stmt_pairs = pkb.getStmtsWithPatternRHS(getExpression(), is_exact);
		for (const auto& stmt_pair : stmt_pairs) {
			if (QueryUtils::checkStmtTypeMatch(stmt_pair.first, DesignEntity::Assign)) {
				return QueryResult(true);
			}
		}

	} else if (this->ent_ref.type == EntRefType::VarName) {
		stmt_set = pkb.getStmtsWithPatternLHS(this->ent_ref.ent_ref);
	} else {
		stmt_set = pkb.getStatements();
	}
	for (auto const& stmt : stmt_set) {
		if (QueryUtils::checkStmtTypeMatch(stmt, DesignEntity::Assign)) {
			return QueryResult(true);
		}
	}
	return QueryResult(false);
}

QueryResult Pattern::executeNonTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& /*map*/) {
	QueryResult result = QueryResult();

	if (this->ent_ref.type == EntRefType::Synonym) {
		vector<string> assign_result;
		vector<string> var_result;

		if (this->expression_type != ExpressionType::Underscore) {
			bool is_exact = this->expression_type != ExpressionType::ExpressionUnderscore;
			auto stmt_pairs = pkb.getStmtsWithPatternRHS(getExpression(), is_exact);
			for (auto const& pair : stmt_pairs) {
				assign_result.push_back(to_string(pair.first->reference));
				var_result.push_back(pair.second);
			}
		} else {
			VarRefSet var_set = pkb.getVariables();
			for (auto const& var_ref : var_set) {
				StmtInfoPtrSet stmt_set = pkb.getStmtsWithPatternLHS(var_ref);
				for (auto const& stmt : stmt_set) {
					assign_result.push_back(to_string(stmt->reference));
					var_result.push_back(var_ref);
				}
			}
		}

		result.addColumn(this->syn_assign.symbol, assign_result);
		result.addColumn(this->ent_ref.ent_ref, var_result);

		return result;
	}

	StmtInfoPtrSet stmt_set;
	if (this->expression_type != ExpressionType::Underscore) {
		bool is_exact = this->expression_type != ExpressionType::ExpressionUnderscore;
		if (this->ent_ref.type == EntRefType::VarName) {
			stmt_set = pkb.getStmtsWithPattern(this->ent_ref.ent_ref, getExpression(), is_exact);
		} else {
			auto stmt_pairs = pkb.getStmtsWithPatternRHS(getExpression(), is_exact);
			for (const auto& stmt_pair : stmt_pairs) {
				stmt_set.insert(stmt_pair.first);
			}
		}
	} else if (this->ent_ref.type == EntRefType::VarName) {
		stmt_set = pkb.getStmtsWithPatternLHS(this->ent_ref.ent_ref);
	} else {
		stmt_set = pkb.getStatements();
	}
	vector<string> assign_stmt_strings;
	for (auto const& stmt : stmt_set) {
		if (QueryUtils::checkStmtTypeMatch(stmt, DesignEntity::Assign)) {
			assign_stmt_strings.push_back(to_string(stmt->reference));
		}
	}
	result.addColumn(this->syn_assign.symbol, assign_stmt_strings);
	return result;
}
