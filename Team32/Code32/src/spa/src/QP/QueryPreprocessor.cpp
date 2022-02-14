#include "QP/QueryPreprocessor.h"

#include <regex>

#include "QP/QueryTypeDefs.h"

QueryProperties QueryPreprocessor::parseQuery(string query) {

	tokenizeQuery(query);
	return parseQuery();
}

void QueryPreprocessor::tokenizeQuery(string query) {

	regex invalidCharsRegex = regex(R"([^a-zA-Z0-9\s,"_\(\);\+\-\*\/%])");
	regex queryTokenRegex = regex(R"(such that|[a-zA-Z][a-zA-Z0-9]*|[0-9]+|\(|\)|;|\\+|-|\*|\/|%|_|,|\")");

	if (regex_search(query, invalidCharsRegex)) {
		throw TokenizationException("Query included invalid characters");
	}

	auto words_begin = sregex_iterator(
		query.begin(),
		query.end(),
		queryTokenRegex
	);
	auto words_end = sregex_iterator();

	for (sregex_iterator i = words_begin; i != words_end; ++i) {
		smatch match = *i;
		this->queryTokens.push_back(match.str());
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
		else if (this->queryTokens[tokenIndex] == "such that") {
			parseSuchThat(++tokenIndex);
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
		if (this->queryTokens[tokenIndex] == ";" && declaration.symbol != "") {
			tokenIndex++;
			return;
		}
		else if (this->queryTokens[tokenIndex] == "," && declaration.symbol != "") {
			Declaration declaration;
			declaration.symbol = "";
			tokenIndex++;
		}
		// if token satisfies "synonym : IDENT"
		else if (isIdentOrName(this->queryTokens[tokenIndex])) {
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
	SuchThatClause suchThatClause = { std::move(parseRelation(tokenIndex)) };
	this->suchThatClauseList.push_back(suchThatClause);
	if (tokenIndex < this->queryTokens.size() && this->queryTokens[tokenIndex] == "and") {
		parseSuchThat(++tokenIndex);
	}
}

void QueryPreprocessor::parsePattern(int& tokenIndex) {
	PatternClause patternClause;
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
	patternClause.entRef = parseQueryEntRef(tokenIndex);
	matchTokenOrThrow(",", tokenIndex);
	patternClause.expression = parseExpression(tokenIndex);
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

unique_ptr<Relation> QueryPreprocessor::parseRelation(int& tokenIndex) {
	if (this->queryTokens[tokenIndex] == "Follows") {
		tokenIndex++;
		return parseFollows(tokenIndex);
	}
	else if (this->queryTokens[tokenIndex] == "Modifies") {
		tokenIndex++;
		try {
			int tempTokenIndex = tokenIndex;
			unique_ptr<Relation> relation = parseModifiesP(tempTokenIndex);
			tokenIndex = tempTokenIndex;
			return relation;
		}
		catch (QueryException e) {
			return parseModifiesS(tokenIndex);
		}
	}
	else if (this->queryTokens[tokenIndex] == "Parent") {
		tokenIndex++;
		return parseParent(tokenIndex);
	}
	else if (this->queryTokens[tokenIndex] == "Uses") {
		tokenIndex++;
		try {
			int tempTokenIndex = tokenIndex;
			unique_ptr<Relation> relation = parseUsesP(tempTokenIndex);
			tokenIndex = tempTokenIndex;
			return relation;
		}
		catch (QueryException e) {
			return parseUsesS(tokenIndex);
		}
	}
	else {
		throw QueryException("Unexpected query token relation: " + this->queryTokens[tokenIndex]);
	}

}

unique_ptr<Follows> QueryPreprocessor::parseFollows(int& tokenIndex) {
	bool isStar = false;
	if (tokenIndex < this->queryTokens.size() && this->queryTokens[tokenIndex] == "*") {
		isStar = true;
		tokenIndex++; // Skip * token
	}
	matchTokenOrThrow("(", tokenIndex);
	QueryStmtRef ref1 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(",", tokenIndex);
	QueryStmtRef ref2 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(")", tokenIndex);
	return make_unique<Follows>(isStar, ref1, ref2);
}

unique_ptr<Parent> QueryPreprocessor::parseParent(int& tokenIndex) {
	bool isStar = false;
	if (tokenIndex < this->queryTokens.size() && this->queryTokens[tokenIndex] == "*") {
		isStar = true;
		tokenIndex++; // Skip * token
	}
	matchTokenOrThrow("(", tokenIndex);
	QueryStmtRef ref1 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(",", tokenIndex);
	QueryStmtRef ref2 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(")", tokenIndex);
	return make_unique<Parent>(isStar, ref1, ref2);
}

unique_ptr<UsesP> QueryPreprocessor::parseUsesP(int& tokenIndex) {
	matchTokenOrThrow("(", tokenIndex);
	QueryEntRef ref1 = parseQueryEntRef(tokenIndex);
	matchTokenOrThrow(",", tokenIndex);
	QueryEntRef ref2 = parseQueryEntRef(tokenIndex);
	matchTokenOrThrow(")", tokenIndex);
	return make_unique<UsesP>(ref1, ref2);
}

unique_ptr<UsesS> QueryPreprocessor::parseUsesS(int& tokenIndex) {
	matchTokenOrThrow("(", tokenIndex);
	QueryStmtRef ref1 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(",", tokenIndex);
	QueryEntRef ref2 = parseQueryEntRef(tokenIndex);
	matchTokenOrThrow(")", tokenIndex);
	return make_unique<UsesS>(ref1, ref2);
}

unique_ptr<ModifiesP> QueryPreprocessor::parseModifiesP(int& tokenIndex) {
	matchTokenOrThrow("(", tokenIndex);
	QueryEntRef ref1 = parseQueryEntRef(tokenIndex);
	matchTokenOrThrow(",", tokenIndex);
	QueryEntRef ref2 = parseQueryEntRef(tokenIndex);
	matchTokenOrThrow(")", tokenIndex);
	return make_unique<ModifiesP>(ref1, ref2);
}

unique_ptr<ModifiesS> QueryPreprocessor::parseModifiesS(int& tokenIndex) {
	matchTokenOrThrow("(", tokenIndex);
	QueryStmtRef ref1 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(",", tokenIndex);
	QueryEntRef ref2 = parseQueryEntRef(tokenIndex);
	matchTokenOrThrow(")", tokenIndex);
	return make_unique<ModifiesS>(ref1, ref2);
}

QueryEntRef QueryPreprocessor::parseQueryEntRef(int& tokenIndex) {
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
			if (declaration.symbol == this->queryTokens[tokenIndex]) {
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

QueryStmtRef QueryPreprocessor::parseQueryStmtRef(int& tokenIndex) {
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
			if (declaration.symbol == this->queryTokens[tokenIndex]) {
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

string QueryPreprocessor::parseExpression(int& tokenIndex) {
	if (this->queryTokens[tokenIndex] == "\"" || 
		(this->queryTokens[tokenIndex] == "_" && 
			this->queryTokens[tokenIndex+1] == "\"")) {
		string joinedExpression;
		while (!(this->queryTokens[tokenIndex] == "\"")) {
			joinedExpression += this->queryTokens[tokenIndex];
			tokenIndex++;
		}
		if (this->queryTokens[tokenIndex] == "_") {
			joinedExpression += this->queryTokens[tokenIndex];
			tokenIndex++;
		}
		for (int i = 0; i <= 4; ++i) {
			joinedExpression += this->queryTokens[tokenIndex + i];
		}
		if (regex_match(joinedExpression, regex("^_?\"(a-zA-z0-9+-\\*\\/%\\(\\))\"_?"))) {
			return joinedExpression;
		}
		else {
			throw QueryException("Unexpected query token expression: " + joinedExpression);
		}
	}
	if (this->queryTokens[tokenIndex] == "_") {
		tokenIndex++;
		return "_";
	}
	throw QueryException("Unexpected query token expression" + this->queryTokens[tokenIndex]);
}

bool QueryPreprocessor::isIdentOrName(string token) {
	return regex_match(token, regex("^[a-zA-Z][a-zA-Z0-9]*$"));
}

void QueryPreprocessor::matchTokenOrThrow(string token, int& tokenIndex) {
	if (tokenIndex < this->queryTokens.size() && this->queryTokens[tokenIndex] == token) {
		tokenIndex++; // Skip token
	}
	else { throw QueryException("Missing token " + token + std::to_string(tokenIndex)); }
}