#include "QP/QueryPreprocessor.h"

#include <optional>
#include <regex>
#include <utility>

#include "Common/Converter.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/QueryTypes.h"
#include "QP/Relationship/Pattern.h"

QP::QueryProperties QP::QueryPreprocessor::parseQuery(string query) {
	tokenizeQuery(std::move(query));
	return parseQuery();
}

void QP::QueryPreprocessor::tokenizeQuery(string query) {
	regex invalid_chars_regex = regex(R"([^a-zA-Z0-9\s,"_\(\);\+\-\*\/%])");
	regex query_token_regex = regex(R"([a-zA-Z][a-zA-Z0-9]*|[0-9]+|\(|\)|;|\+|-|\*|\/|%|_|,|\")");

	if (regex_search(query, invalid_chars_regex)) {
		throw QueryTokenizationException("Illegal token encountered.");
	}

	auto words_begin = sregex_iterator(query.begin(), query.end(), query_token_regex);

	/*Check to make sure that there are no spaces between Parent|Follows and *.
	  If so, combine the tokens into Parent* or Follows* */
	auto words_end = sregex_iterator();
	size_t prev_pos = 0;
	for (sregex_iterator i = words_begin; i != words_end; ++i) {
		smatch match = *i;
		if (match.str() == "*" && !this->query_tokens.empty() &&
		    (this->query_tokens.back() == "Parent" || this->query_tokens.back() == "Follows" || this->query_tokens.back() == "Calls") &&
		    prev_pos + this->query_tokens.back().length() == static_cast<size_t>(match.position())) {
			string combined_token = this->query_tokens.back() + "*";
			this->query_tokens.pop_back();
			this->query_tokens.push_back(combined_token);
		} else {
			this->query_tokens.push_back(match.str());
		}
		prev_pos = match.position();
	}
}

QP::QueryProperties QP::QueryPreprocessor::parseQuery() {
	size_t token_index = 0;

	// Used to check if synonyms for "Select" has been parsed
	while (token_index < this->query_tokens.size()) {
		if (this->select_list.empty()) {
			if (this->query_tokens[token_index] == "Select") {
				parseSelect(++token_index);
			} else {
				parseDeclaration(token_index);
			}
		} else if (this->query_tokens[token_index] == "such" && this->query_tokens[token_index + 1] == "that") {
			token_index += 2;
			parseSuchThat(token_index);
		} else if (this->query_tokens[token_index] == "pattern") {
			parsePattern(++token_index);
		} else {
			throw QueryException("Unexpected token encountered: " + this->query_tokens[token_index] + ".");
		}
	}

	if (this->select_list[0].symbol.empty()) {
		throw QueryException("Missing Select clause.");
	}
	return {this->declaration_list, this->select_list, this->clause_list};
}

void QP::QueryPreprocessor::parseDeclaration(size_t& token_index) {
	Declaration declaration;
	declaration.symbol = "";

	declaration.type = parseDesignEntity(token_index);
	while (token_index < this->query_tokens.size()) {
		if (this->query_tokens[token_index] == ";" && !declaration.symbol.empty()) {
			token_index++;
			return;
		}
		if (this->query_tokens[token_index] == "," && !declaration.symbol.empty()) {
			declaration.symbol = "";
			token_index++;
		}
		// if token satisfies "synonym : IDENT" and expecting a synonym
		else if (isIdentOrName(this->query_tokens[token_index]) && declaration.symbol.empty()) {
			declaration.symbol = this->query_tokens[token_index];
			for (const Declaration& existing_declaration : this->declaration_list) {
				if (existing_declaration.symbol == declaration.symbol) {
					throw QueryException("Duplicate synonym symbol in declarations.");
				}
			}
			token_index++;
			this->declaration_list.push_back(declaration);
		} else {
			throw QueryException("Unexpected query token design entity: " + this->query_tokens[token_index] + ".");
		}
	}
	throw QueryException("Unexpected end of query.");
}

void QP::QueryPreprocessor::parseSelect(size_t& token_index) {
	if (token_index >= this->query_tokens.size()) {
		throw QueryException("Unexpected end of query.");
	}
	for (const Declaration& declaration : this->declaration_list) {
		if (declaration.symbol == this->query_tokens[token_index]) {
			this->select_list.push_back({declaration.type, this->query_tokens[token_index]});
			token_index++;
			return;
		}
	}
	throw QueryException("Undeclared query synonym.");
}

void QP::QueryPreprocessor::parseSuchThat(size_t& token_index) {
	Clause clause = {parseRelation(token_index)};
	this->clause_list.push_back(clause);
	if (token_index < this->query_tokens.size() && this->query_tokens[token_index] == "and") {
		parseSuchThat(++token_index);
	}
}

void QP::QueryPreprocessor::parsePattern(size_t& token_index) {
	ReferenceArgument synonym;
	ReferenceArgument ent_ref;
	ReferenceArgument expr;
	bool has_synonym = false;
	for (const Declaration& declaration : this->declaration_list) {
		if (declaration.type == DesignEntity::Assign && declaration.symbol == this->query_tokens[token_index]) {
			synonym = ReferenceArgument(declaration);
			has_synonym = true;
			break;
		}
	}
	if (!has_synonym) {
		throw QueryException("Missing query synonym: " + this->query_tokens[token_index] + ".");
	}
	token_index++;

	matchTokenOrThrow("(", token_index);
	set<DesignEntity> allowed_design_entities = {DesignEntity::Variable};
	ent_ref = parseQueryEntRef(token_index, allowed_design_entities);
	matchTokenOrThrow(",", token_index);
	if (this->query_tokens[token_index] == "_") {
		token_index++;
		if (this->query_tokens[token_index] == "\"") {
			Common::ExpressionProcessor::Expression expression = parseExpression(token_index);
			expr = ReferenceArgument(expression, false);
			matchTokenOrThrow("_", token_index);
		}
	} else if (this->query_tokens[token_index] == "\"") {
		Common::ExpressionProcessor::Expression expression = parseExpression(token_index);
		expr = ReferenceArgument(expression, true);
	} else {
		throw QueryException("Unexpected query expression: " + this->query_tokens[token_index] + ".");
	}

	matchTokenOrThrow(")", token_index);

	if (token_index < this->query_tokens.size() && this->query_tokens[token_index] == "and") {
		parsePattern(++token_index);
	}
	unique_ptr<Relationship::Relation> relation = make_unique<Relationship::Pattern>(synonym, ent_ref, expr);
	Clause clause = {std::move(relation)};
	this->clause_list.push_back(clause);
}

DesignEntity QP::QueryPreprocessor::parseDesignEntity(size_t& token_index) {
	DesignEntity design_entity;
	if (this->query_tokens[token_index] == "stmt") {
		design_entity = DesignEntity::Stmt;
	} else if (this->query_tokens[token_index] == "read") {
		design_entity = DesignEntity::Read;
	} else if (this->query_tokens[token_index] == "print") {
		design_entity = DesignEntity::Print;
	} else if (this->query_tokens[token_index] == "call") {
		design_entity = DesignEntity::Call;
	} else if (this->query_tokens[token_index] == "while") {
		design_entity = DesignEntity::While;
	} else if (this->query_tokens[token_index] == "if") {
		design_entity = DesignEntity::If;
	} else if (this->query_tokens[token_index] == "assign") {
		design_entity = DesignEntity::Assign;
	} else if (this->query_tokens[token_index] == "variable") {
		design_entity = DesignEntity::Variable;
	} else if (this->query_tokens[token_index] == "constant") {
		design_entity = DesignEntity::Constant;
	} else if (this->query_tokens[token_index] == "procedure") {
		design_entity = DesignEntity::Procedure;
	} else {
		throw QueryException("Unexpected query design entity: " + this->query_tokens[token_index] + ".");
	}
	token_index++;
	return design_entity;
}

unique_ptr<QP::Relationship::Relation> QP::QueryPreprocessor::parseRelation(size_t& token_index) {
	if (this->query_tokens[token_index] == "Follows" || this->query_tokens[token_index] == "Follows*") {
		return parseFollows(token_index);
	}
	if (this->query_tokens[token_index] == "Modifies") {
		token_index++;
		try {
			size_t temp_token_index = token_index;
			unique_ptr<Relationship::Relation> relation = parseModifiesS(temp_token_index);
			token_index = temp_token_index;
			return relation;
		} catch (const QueryException& e) {
			return parseModifiesP(token_index);
		}
	} else if (this->query_tokens[token_index] == "Parent" || this->query_tokens[token_index] == "Parent*") {
		return parseParent(token_index);
	} else if (this->query_tokens[token_index] == "Uses") {
		token_index++;
		try {
			size_t temp_token_index = token_index;
			unique_ptr<Relationship::Relation> relation = parseUsesS(temp_token_index);
			token_index = temp_token_index;
			return relation;
		} catch (const QueryException& e) {
			return parseUsesP(token_index);
		}
	} else if (this->query_tokens[token_index] == "Calls" || this->query_tokens[token_index] == "Calls*") {
		return parseCalls(token_index);
	} else {
		throw QueryException("Unexpected query token relation: " + this->query_tokens[token_index] + ".");
	}
}

unique_ptr<QP::Relationship::Follows> QP::QueryPreprocessor::parseFollows(size_t& token_index) {
	bool is_star = false;
	if (this->query_tokens[token_index] == "Follows*") {
		is_star = true;
	}
	token_index++;
	set<DesignEntity> allowed_design_entities = {
		DesignEntity::Stmt,  DesignEntity::Read, DesignEntity::Print,  DesignEntity::Call,
		DesignEntity::While, DesignEntity::If,   DesignEntity::Assign,
	};
	matchTokenOrThrow("(", token_index);
	ReferenceArgument ref1 = parseQueryStmtRef(token_index, allowed_design_entities);
	matchTokenOrThrow(",", token_index);
	ReferenceArgument ref2 = parseQueryStmtRef(token_index, allowed_design_entities);
	matchTokenOrThrow(")", token_index);
	if (is_star) {
		return make_unique<Relationship::FollowsT>(ref1, ref2);
	}
	return make_unique<Relationship::Follows>(ref1, ref2);
}

unique_ptr<QP::Relationship::Parent> QP::QueryPreprocessor::parseParent(size_t& token_index) {
	bool is_star = false;
	if (this->query_tokens[token_index] == "Parent*") {
		is_star = true;
	}
	token_index++;
	set<DesignEntity> allowed_design_entities = {
		DesignEntity::Stmt,  DesignEntity::Read, DesignEntity::Print,  DesignEntity::Call,
		DesignEntity::While, DesignEntity::If,   DesignEntity::Assign,
	};
	matchTokenOrThrow("(", token_index);
	ReferenceArgument ref1 = parseQueryStmtRef(token_index, allowed_design_entities);
	matchTokenOrThrow(",", token_index);
	ReferenceArgument ref2 = parseQueryStmtRef(token_index, allowed_design_entities);
	matchTokenOrThrow(")", token_index);
	if (is_star) {
		return make_unique<Relationship::ParentT>(ref1, ref2);
	}
	return make_unique<Relationship::Parent>(ref1, ref2);
}

unique_ptr<QP::Relationship::UsesP> QP::QueryPreprocessor::parseUsesP(size_t& token_index) {
	matchTokenOrThrow("(", token_index);
	set<DesignEntity> ref1_allowed_design_entities = {DesignEntity::Procedure};
	ReferenceArgument ref1 = parseQueryEntRef(token_index, ref1_allowed_design_entities);
	// 1st entRef cannot be a wildcard
	if (ref1.getType() == ReferenceType::Wildcard) {
		throw QueryException("Ambiguous wildcard _.");
	}
	matchTokenOrThrow(",", token_index);
	set<DesignEntity> ref2_allowed_design_entities = {DesignEntity::Variable};
	ReferenceArgument ref2 = parseQueryEntRef(token_index, ref2_allowed_design_entities);
	matchTokenOrThrow(")", token_index);
	return make_unique<Relationship::UsesP>(ref1, ref2);
}

unique_ptr<QP::Relationship::UsesS> QP::QueryPreprocessor::parseUsesS(size_t& token_index) {
	matchTokenOrThrow("(", token_index);
	set<DesignEntity> ref1_allowed_design_entities = {DesignEntity::Assign, DesignEntity::Call,  DesignEntity::Print,
	                                                  DesignEntity::If,     DesignEntity::While, DesignEntity::Stmt};
	ReferenceArgument ref1 = parseQueryStmtRef(token_index, ref1_allowed_design_entities);
	// 1st stmeRef cannot be a wildcard
	if (ref1.getType() == ReferenceType::Wildcard) {
		throw QueryException("Ambiguous wildcard _.");
	}
	matchTokenOrThrow(",", token_index);
	set<DesignEntity> ref2_allowed_design_entities = {DesignEntity::Variable};
	ReferenceArgument ref2 = parseQueryEntRef(token_index, ref2_allowed_design_entities);
	matchTokenOrThrow(")", token_index);
	return make_unique<Relationship::UsesS>(ref1, ref2);
}

unique_ptr<QP::Relationship::ModifiesP> QP::QueryPreprocessor::parseModifiesP(size_t& token_index) {
	matchTokenOrThrow("(", token_index);
	set<DesignEntity> ref1_allowed_design_entities = {DesignEntity::Procedure};
	ReferenceArgument ref1 = parseQueryEntRef(token_index, ref1_allowed_design_entities);
	// 1st entRef cannot be a wildcard
	if (ref1.getType() == ReferenceType::Wildcard) {
		throw QueryException("Ambiguous wildcard _.");
	}
	matchTokenOrThrow(",", token_index);
	set<DesignEntity> ref2_allowed_design_entities = {DesignEntity::Variable};
	ReferenceArgument ref2 = parseQueryEntRef(token_index, ref2_allowed_design_entities);
	matchTokenOrThrow(")", token_index);
	return make_unique<Relationship::ModifiesP>(ref1, ref2);
}

unique_ptr<QP::Relationship::ModifiesS> QP::QueryPreprocessor::parseModifiesS(size_t& token_index) {
	matchTokenOrThrow("(", token_index);
	set<DesignEntity> ref1_allowed_design_entities = {DesignEntity::Assign, DesignEntity::Call,  DesignEntity::Read,
	                                                  DesignEntity::If,     DesignEntity::While, DesignEntity::Stmt};
	ReferenceArgument ref1 = parseQueryStmtRef(token_index, ref1_allowed_design_entities);
	// 1st stmeRef cannot be a wildcard
	if (ref1.getType() == ReferenceType::Wildcard) {
		throw QueryException("Ambiguous wildcard _.");
	}
	matchTokenOrThrow(",", token_index);
	set<DesignEntity> ref2_allowed_design_entities = {DesignEntity::Variable};
	ReferenceArgument ref2 = parseQueryEntRef(token_index, ref2_allowed_design_entities);
	matchTokenOrThrow(")", token_index);
	return make_unique<Relationship::ModifiesS>(ref1, ref2);
}

unique_ptr<QP::Relationship::Calls> QP::QueryPreprocessor::parseCalls(size_t& token_index) {
	bool is_star = false;
	if (this->query_tokens[token_index] == "Calls*") {
		is_star = true;
	}
	token_index++;
	set<DesignEntity> allowed_design_entities = {DesignEntity::Procedure};
	matchTokenOrThrow("(", token_index);
	ReferenceArgument ref1 = parseQueryEntRef(token_index, allowed_design_entities);
	matchTokenOrThrow(",", token_index);
	ReferenceArgument ref2 = parseQueryEntRef(token_index, allowed_design_entities);
	matchTokenOrThrow(")", token_index);
	if (is_star) {
		return make_unique<Relationship::CallsT>(ref1, ref2);
	}
	return make_unique<Relationship::Calls>(ref1, ref2);
}

ReferenceArgument QP::QueryPreprocessor::parseQueryEntRef(size_t& token_index, const set<DesignEntity>& accepted_design_entities) {
	ReferenceArgument ent_ref;
	if (this->query_tokens[token_index] == "_") {
		++token_index;
		return ReferenceArgument();
	}
	if (token_index + 2 < this->query_tokens.size() && this->query_tokens[token_index] == "\"" &&
	    isIdentOrName(this->query_tokens[token_index + 1]) && this->query_tokens[token_index + 2] == "\"") {
		token_index += 3;
		return ReferenceArgument(this->query_tokens[token_index - 2]);
	}

	for (const Declaration& declaration : this->declaration_list) {
		if (declaration.symbol == this->query_tokens[token_index] && accepted_design_entities.count(declaration.type) != 0) {
			++token_index;
			return ReferenceArgument({declaration.type, this->query_tokens[token_index - 1]});
		}
	}
	throw QueryException("Unexpected query entity reference: " + this->query_tokens[token_index] + ".");
}

ReferenceArgument QP::QueryPreprocessor::parseQueryStmtRef(size_t& token_index, const set<DesignEntity>& accepted_design_entities) {
	if (this->query_tokens[token_index] == "_") {
		++token_index;
		return ReferenceArgument();
	}
	if (regex_match(this->query_tokens[token_index], regex("^[0-9]+$"))) {
		++token_index;
		return ReferenceArgument(Common::Converter::convertInteger(this->query_tokens[token_index - 1]));
	}
	for (const Declaration& declaration : this->declaration_list) {
		if (declaration.symbol == this->query_tokens[token_index] && accepted_design_entities.count(declaration.type) != 0) {
			++token_index;
			return ReferenceArgument({declaration.type, this->query_tokens[token_index - 1]});
		}
	}
	throw QueryException("Unexpected query statement reference: " + this->query_tokens[token_index] + ".");
}

Common::ExpressionProcessor::Expression QP::QueryPreprocessor::parseExpression(size_t& token_index) {
	matchTokenOrThrow("\"", token_index);
	vector<string> expression;
	while (this->query_tokens[token_index] != "\"") {
		expression.push_back(this->query_tokens[token_index]);
		token_index++;
	}
	QueryExpressionLexer lexer = QueryExpressionLexer(expression);
	Common::ExpressionProcessor::Expression arithmetic_expression =
		Common::ExpressionProcessor::Expression::parse(lexer, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	if (!lexer.readToken().empty()) {
		throw QueryException("Invalid expression.");
	}

	matchTokenOrThrow("\"", token_index);
	return arithmetic_expression;
}

bool QP::QueryPreprocessor::isIdentOrName(const string& token) { return regex_match(token, regex("^[a-zA-Z][a-zA-Z0-9]*$")); }

void QP::QueryPreprocessor::matchTokenOrThrow(const string& token, size_t& token_index) {
	if (token_index < this->query_tokens.size() && this->query_tokens[token_index] == token) {
		token_index++;  // Skip token
	} else {
		throw QueryException("Missing token: " + token + this->query_tokens[token_index - 1] + this->query_tokens[token_index] +
		                     this->query_tokens[token_index + 1] + ".");
	}
}