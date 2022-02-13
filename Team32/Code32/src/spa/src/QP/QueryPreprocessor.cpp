#include "QP/QueryPreprocessor.h"

#include <regex>

#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/QueryTypeDefs.h"

QueryProperties QueryPreprocessor::parseQuery(string query) {

	tokenizeQuery(query);
	return parseQuery();
}

void QueryPreprocessor::tokenizeQuery(string query) {

	regex invalidCharsRegex = regex(R"([^a-zA-Z0-9\s,"_\(\);\+\-\*\/%])");
	regex queryTokenRegex = regex(R"([a-zA-Z][a-zA-Z0-9]*|[0-9]+|\(|\)|;|\+|-|\*|\/|%|_|,|\")");

	if (regex_search(query, invalidCharsRegex)) {
		throw QueryTokenizationException("Query included invalid characters");
	}

	auto words_begin = sregex_iterator(
		query.begin(),
		query.end(),
		queryTokenRegex
	);
	auto words_end = sregex_iterator();
	int prev_pos = 0;
	for (sregex_iterator i = words_begin; i != words_end; ++i) {
		smatch match = *i;
		if (
			match.str() == "*" && !this->queryTokens.empty() && 
			(this->queryTokens.back() == "Parent" || this->queryTokens.back() == "Follows") && 
			prev_pos + this->queryTokens.back().length() == match.position()
		) {
			string combinedToken = this->queryTokens.back() + "*";
			this->queryTokens.pop_back();
			this->queryTokens.push_back(combinedToken);
		}
		else {
			this->queryTokens.push_back(match.str());
		}
		prev_pos = match.position();
	}
}

QueryProperties QueryPreprocessor::parseQuery() {
	int tokenIndex = 0;

	// Used to check if synonyms for "Select" has been parsed
	this->select.symbol = "";
	while (tokenIndex < this->queryTokens.size()) {
		if (this->select.symbol == "") {
			
			if (this->queryTokens[tokenIndex] == "Select") {
				parseSelect(++tokenIndex);
			}
			else {
				parseDeclaration(tokenIndex);
			}
		}
		else if (this->queryTokens[tokenIndex] == "such" && this->queryTokens[tokenIndex+1] == "that") {
			tokenIndex += 2;
			parseSuchThat(tokenIndex);
		}
		else if (this->queryTokens[tokenIndex] == "pattern") {
			parsePattern(++tokenIndex);
		}
		else {
			throw QueryException("Unexpected token: " + this->queryTokens[tokenIndex]);
		}
	}

    if (this->select.symbol == "") {
          throw QueryException("Missing Select statement");
    }
	return QueryProperties(
		this->declarationList,
		this->select,
		this->suchThatClauseList,
		this->patternClauseList
	);
}

void QueryPreprocessor::parseDeclaration(int& tokenIndex) {
	Declaration declaration;
	declaration.symbol = "";
	
	declaration.type = parseDesignEntity(tokenIndex);
	while (tokenIndex < this->queryTokens.size()) {
		if (this->queryTokens[tokenIndex] == ";" && !declaration.symbol.empty()) {
			tokenIndex++;
			return;
		}
		else if (this->queryTokens[tokenIndex] == "," && !declaration.symbol.empty()) {
			declaration.symbol = "";
			tokenIndex++;
		}
		// if token satisfies "synonym : IDENT" and expecting a synonym
		else if (isIdentOrName(this->queryTokens[tokenIndex]) && declaration.symbol.empty()) {
			declaration.symbol = this->queryTokens[tokenIndex];
			for (Declaration existingDeclaration : this->declarationList) {
				if (existingDeclaration.symbol == declaration.symbol) {
					throw QueryException("Duplicate synonym symbol in declarations");
				}
			}
			tokenIndex++;
			this->declarationList.push_back(declaration);
		}
		else {
			throw QueryException("Unexpected query token design entity: " + this->queryTokens[tokenIndex]);
		}
		
	}
	throw QueryException("Unexpected end of query");
}

void QueryPreprocessor::parseSelect(int& tokenIndex) {
	if (tokenIndex >= this->queryTokens.size()) {
		throw QueryException("Unexpected end of query");
	}
	for (Declaration declaration : this->declarationList) {
		if (declaration.symbol == this->queryTokens[tokenIndex]) {
			this->select.type = declaration.type;
			this->select.symbol = this->queryTokens[tokenIndex];
			tokenIndex++;
			return;
		}
	}
	throw QueryException("Undeclared query synonym");
}

void QueryPreprocessor::parseSuchThat(int& tokenIndex) {
	SuchThatClause suchThatClause = { parseRelation(tokenIndex) };
	this->suchThatClauseList.push_back(suchThatClause);
	if (tokenIndex < this->queryTokens.size() && this->queryTokens[tokenIndex] == "and") {
		parseSuchThat(++tokenIndex);
	}
}

void QueryPreprocessor::parsePattern(int& tokenIndex) {
	PatternClause patternClause = {};
	bool hasSynonym = false;
	for (Declaration declaration : this->declarationList) {
		if (declaration.type == DesignEntity::assign && declaration.symbol == this->queryTokens[tokenIndex]) {
			patternClause.synonym = declaration;
			hasSynonym = true;
			break;
		}
	}
	if (!hasSynonym) {
		throw QueryException("Missing query synonym: " + this->queryTokens[tokenIndex]);
	} else { tokenIndex++; }

	matchTokenOrThrow("(", tokenIndex);
	set<DesignEntity> allowedDesignEntities = { DesignEntity::variable };
	patternClause.entRef = parseQueryEntRef(tokenIndex, allowedDesignEntities);
	matchTokenOrThrow(",", tokenIndex);
	if (this->queryTokens[tokenIndex] == "_") {
		tokenIndex++;
		if (this->queryTokens[tokenIndex] == "\"") {
			patternClause.expressionType = ExpressionType::expressionUnderscore;
			patternClause.expression = &parseExpression(tokenIndex);
		}
		else {
			patternClause.expressionType = ExpressionType::underscore;
		}
	}
	else if (this->queryTokens[tokenIndex] == "\"") {
		patternClause.expressionType = ExpressionType::expression;
		patternClause.expression = &parseExpression(tokenIndex);
	}
	
	if (patternClause.expressionType == ExpressionType::expressionUnderscore) { 
		matchTokenOrThrow("_", tokenIndex); 
	}
	matchTokenOrThrow(")", tokenIndex);

	if (tokenIndex < this->queryTokens.size() && this->queryTokens[tokenIndex] == "and") {
		parsePattern(++tokenIndex);
	}

	this->patternClauseList.push_back(patternClause);
}

DesignEntity QueryPreprocessor::parseDesignEntity(int& tokenIndex) {
	DesignEntity designEntity;
	if (this->queryTokens[tokenIndex] == "stmt") {
		designEntity = DesignEntity::stmt;
	}
	else if (this->queryTokens[tokenIndex] == "read") {
		designEntity = DesignEntity::read;
	}
	else if (this->queryTokens[tokenIndex] == "print") {
		designEntity = DesignEntity::print;
	}
	else if (this->queryTokens[tokenIndex] == "call") {
		designEntity = DesignEntity::call;
	}
	else if (this->queryTokens[tokenIndex] == "while") {
		designEntity = DesignEntity::while_;
	}
	else if (this->queryTokens[tokenIndex] == "if") {
		designEntity = DesignEntity::if_;
	}
	else if (this->queryTokens[tokenIndex] == "assign") {
		designEntity = DesignEntity::assign;
	}
	else if (this->queryTokens[tokenIndex] == "variable") {
		designEntity = DesignEntity::variable;
	}
	else if (this->queryTokens[tokenIndex] == "constant") {
		designEntity = DesignEntity::constant;
	}
	else if (this->queryTokens[tokenIndex] == "procedure") {
		designEntity = DesignEntity::procedure;
	}
	else {
		throw QueryException("Unexpected query design entity: " + this->queryTokens[tokenIndex]);
	}
	tokenIndex++;
	return designEntity;

}

Relation* QueryPreprocessor::parseRelation(int& tokenIndex) {
	if (this->queryTokens[tokenIndex] == "Follows" || this->queryTokens[tokenIndex] == "Follows*") {
		return parseFollows(tokenIndex);
	}
	else if (this->queryTokens[tokenIndex] == "Modifies") {
		tokenIndex++;
		try {
			int tempTokenIndex = tokenIndex;
			Relation* relation = parseModifiesS(tempTokenIndex);
			tokenIndex = tempTokenIndex;
			return relation;
		}
		catch (QueryException e) {
			return parseModifiesP(tokenIndex);
		}
	}
	else if (this->queryTokens[tokenIndex] == "Parent" || this->queryTokens[tokenIndex] == "Parent*") {
		return parseParent(tokenIndex);
	}
	else if (this->queryTokens[tokenIndex] == "Uses") {
		tokenIndex++;
		try {
			int tempTokenIndex = tokenIndex;
			Relation* relation = parseUsesS(tempTokenIndex);
			tokenIndex = tempTokenIndex;
			return relation;
		}
		catch (QueryException e) {
			return parseUsesP(tokenIndex);
		}
	}
	else {
		throw QueryException("Unexpected query token relation: " + this->queryTokens[tokenIndex]);
	}

}

Follows* QueryPreprocessor::parseFollows(int& tokenIndex) {
	bool isStar = false;
	if (this->queryTokens[tokenIndex] == "Follows*") {
		isStar = true;
	}
	tokenIndex++;
	set<DesignEntity> allowedDesignEntities = {
		DesignEntity::stmt,
		DesignEntity::read,
		DesignEntity::print,
		DesignEntity::call,
		DesignEntity::while_,
		DesignEntity::if_,
		DesignEntity::assign,
	};
	matchTokenOrThrow("(", tokenIndex);
	QueryStmtRef ref1 = parseQueryStmtRef(tokenIndex, allowedDesignEntities);
	matchTokenOrThrow(",", tokenIndex);
	QueryStmtRef ref2 = parseQueryStmtRef(tokenIndex, allowedDesignEntities);
	matchTokenOrThrow(")", tokenIndex);
	return new Follows(isStar, ref1, ref2);
}

Parent* QueryPreprocessor::parseParent(int& tokenIndex) {
	bool isStar = false;
	if (this->queryTokens[tokenIndex] == "Parent*") {
		isStar = true;
	}
	tokenIndex++;
	set<DesignEntity> allowedDesignEntities = {
		DesignEntity::stmt,
		DesignEntity::read,
		DesignEntity::print,
		DesignEntity::call,
		DesignEntity::while_,
		DesignEntity::if_,
		DesignEntity::assign,
	};
	matchTokenOrThrow("(", tokenIndex);
	QueryStmtRef ref1 = parseQueryStmtRef(tokenIndex, allowedDesignEntities);
	matchTokenOrThrow(",", tokenIndex);
	QueryStmtRef ref2 = parseQueryStmtRef(tokenIndex, allowedDesignEntities);
	matchTokenOrThrow(")", tokenIndex);
	return new Parent(isStar, ref1, ref2);
}

UsesP* QueryPreprocessor::parseUsesP(int& tokenIndex) {
	matchTokenOrThrow("(", tokenIndex);
	set<DesignEntity> ref1AllowedDesignEntities = {
		DesignEntity::call,
		DesignEntity::procedure
	};
	QueryEntRef ref1 = parseQueryEntRef(tokenIndex, ref1AllowedDesignEntities);
	// 1st entRef cannot be a wildcard
	if (ref1.type == EntRefType::underscore) {
		throw QueryException("Ambiguous wildcard _");
	}
	matchTokenOrThrow(",", tokenIndex);
	set<DesignEntity> ref2AllowedDesignEntities = {DesignEntity::variable};
	QueryEntRef ref2 = parseQueryEntRef(tokenIndex, ref2AllowedDesignEntities);
	matchTokenOrThrow(")", tokenIndex);
	return new UsesP(ref1, ref2);
}

UsesS* QueryPreprocessor::parseUsesS(int& tokenIndex) {
	matchTokenOrThrow("(", tokenIndex);
	set<DesignEntity> ref1AllowedDesignEntities = {
		DesignEntity::assign,
		DesignEntity::print,
		DesignEntity::if_,
		DesignEntity::while_,
		DesignEntity::stmt
	};
	QueryStmtRef ref1 = parseQueryStmtRef(tokenIndex, ref1AllowedDesignEntities);
	// 1st stmeRef cannot be a wildcard
	if (ref1.type == StmtRefType::underscore) {
		throw QueryException("Ambiguous wildcard _");
	}
	matchTokenOrThrow(",", tokenIndex);
	set<DesignEntity> ref2AllowedDesignEntities = { DesignEntity::variable };
	QueryEntRef ref2 = parseQueryEntRef(tokenIndex, ref2AllowedDesignEntities);
	matchTokenOrThrow(")", tokenIndex);
	return new UsesS(ref1, ref2);
}

ModifiesP* QueryPreprocessor::parseModifiesP(int& tokenIndex) {
	matchTokenOrThrow("(", tokenIndex);
	set<DesignEntity> ref1AllowedDesignEntities = {
		DesignEntity::call,
		DesignEntity::procedure
	};
	QueryEntRef ref1 = parseQueryEntRef(tokenIndex, ref1AllowedDesignEntities);
	// 1st entRef cannot be a wildcard
	if (ref1.type == EntRefType::underscore) {
		throw QueryException("Ambiguous wildcard _");
	}
	matchTokenOrThrow(",", tokenIndex);
	set<DesignEntity> ref2AllowedDesignEntities = { DesignEntity::variable };
	QueryEntRef ref2 = parseQueryEntRef(tokenIndex, ref2AllowedDesignEntities);
	matchTokenOrThrow(")", tokenIndex);
	return new ModifiesP(ref1, ref2);
}

ModifiesS* QueryPreprocessor::parseModifiesS(int& tokenIndex) {
	matchTokenOrThrow("(", tokenIndex);
	set<DesignEntity> ref1AllowedDesignEntities = {
		DesignEntity::assign,
		DesignEntity::read,
		DesignEntity::if_,
		DesignEntity::while_,
		DesignEntity::stmt
	};
	QueryStmtRef ref1 = parseQueryStmtRef(tokenIndex, ref1AllowedDesignEntities);
	// 1st stmeRef cannot be a wildcard
	if (ref1.type == StmtRefType::underscore) {
		throw QueryException("Ambiguous wildcard _");
	}
	matchTokenOrThrow(",", tokenIndex);
	set<DesignEntity> ref2AllowedDesignEntities = { DesignEntity::variable };
	QueryEntRef ref2 = parseQueryEntRef(tokenIndex, ref2AllowedDesignEntities);
	matchTokenOrThrow(")", tokenIndex);
	return new ModifiesS(ref1, ref2);
}

QueryEntRef QueryPreprocessor::parseQueryEntRef(int& tokenIndex, set<DesignEntity> acceptedDesignEntities) {
	QueryEntRef entRef;
	if (this->queryTokens[tokenIndex] == "_") {
		entRef.type = EntRefType::underscore;
		entRef.entRef = "_";
	}
	else if (tokenIndex + 2 < this->queryTokens.size() &&
		this->queryTokens[tokenIndex] == "\"" &&
		isIdentOrName(this->queryTokens[tokenIndex + 1]) &&
		this->queryTokens[tokenIndex + 2] == "\"") {

		entRef.type = EntRefType::varName;
		entRef.entRef = this->queryTokens[tokenIndex + 1];
		tokenIndex += 2;
	}
	else {
		bool hasSynonym = false;
		for (Declaration declaration : this->declarationList) {
			if (declaration.symbol == this->queryTokens[tokenIndex] && acceptedDesignEntities.count(declaration.type) != 0) {
				entRef.type = EntRefType::synonym;
				entRef.entRef = this->queryTokens[tokenIndex];
				hasSynonym = true;
				break;
			}
		}
		if (!hasSynonym) {
			throw QueryException("Unexpected query token entref " + this->queryTokens[tokenIndex]);
		}
	}

	tokenIndex++;
	return entRef;
}

QueryStmtRef QueryPreprocessor::parseQueryStmtRef(int& tokenIndex, set<DesignEntity> acceptedDesignEntities) {
	QueryStmtRef stmtRef;
	if (this->queryTokens[tokenIndex] == "_") {
		stmtRef.type = StmtRefType::underscore;
		stmtRef.stmtRef = "_";
	}
	else if (regex_match(this->queryTokens[tokenIndex], regex("^[0-9]+$"))) {

		stmtRef.type = StmtRefType::stmtNumber;
		stmtRef.stmtRef = this->queryTokens[tokenIndex];
	}
	else {
		bool hasSynonym = false;
		for (Declaration declaration : this->declarationList) {
			if (declaration.symbol == this->queryTokens[tokenIndex] && acceptedDesignEntities.count(declaration.type) != 0) {
				stmtRef.type = StmtRefType::synonym;
				stmtRef.stmtRef = this->queryTokens[tokenIndex];
				hasSynonym = true;
				break;
			}
		}
		if (!hasSynonym) {
			throw QueryException("Unexpected query token stmtref " + this->queryTokens[tokenIndex]);
		}
	}
	tokenIndex++;
	return stmtRef;
}

Common::ExpressionProcessor::Expression QueryPreprocessor::parseExpression(int& tokenIndex) {
	matchTokenOrThrow("\"", tokenIndex);
	vector<string> expression;
	while (this->queryTokens[tokenIndex] != "\"") {
		expression.push_back(this->queryTokens[tokenIndex]);
		tokenIndex++;
	}
	expression.push_back(";");
	QueryExpressionLexer lexer = QueryExpressionLexer(expression);
	Common::ExpressionProcessor::Expression arithmeticExpression = Common::ExpressionProcessor::Expression::parse(lexer, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);
	matchTokenOrThrow("\"", tokenIndex);
	return arithmeticExpression;
}

bool QueryPreprocessor::isIdentOrName(string token) {
	return regex_match(token, regex("^[a-zA-Z][a-zA-Z0-9]*$"));
}

void QueryPreprocessor::matchTokenOrThrow(string token, int& tokenIndex) {
	if (tokenIndex < this->queryTokens.size() && this->queryTokens[tokenIndex] == token) {
		tokenIndex++; // Skip token
	}
	else { throw QueryException("Missing token " + token + this->queryTokens[tokenIndex-1]+ this->queryTokens[tokenIndex] + this->queryTokens[tokenIndex+1]); }
}