#include "QueryPreprocessor.h"
#include <regex>

#include "QueryTypeDefs.h"
#include "Relationship/StmtEntRelation.h"
#include "Relationship/StmtsRelation.h"

QueryProperties QueryPreprocessor::parseQuery(string query) {

	tokenizeQuery(query);
	return parseQuery();
}

void QueryPreprocessor::tokenizeQuery(string query) {

	regex invalidCharsRegex = regex(R"([^a-zA-Z0-9 ,"_\(\);\+\-\*\/%\n])");
	regex queryTokenRegex = regex(R"((Follows|Parent)\*|such that|[a-zA-Z][a-zA-Z0-9]*|[0-9]+|\(|\)|;|\\+|-|\*|\/|%|_|,|\")");

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
		if (this->declarationList.empty() && this->queryTokens[tokenIndex] != "Select") {
			parseDeclaration(tokenIndex);
		} 
		else if (this->select.symbol == "") {
			
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
					throw QueryException("Duplicate synonym symbol");
				}
			}
			tokenIndex++;
			this->declarationList.push_back(declaration);
		}
		else {
			throw QueryException("Unexpected query token design entity" + this->queryTokens[tokenIndex]);
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

	if (this->queryTokens[tokenIndex] == "read") {
		designEntity = DesignEntity::read;
	}
	else if (this->queryTokens[tokenIndex] == "print") {
		designEntity = DesignEntity::print;
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
		throw QueryException("Unexpected query design entity");
	}
	tokenIndex++;
	return designEntity;

}

Relation QueryPreprocessor::parseRelation(int& tokenIndex) {
	Relationship relationship;
	if (this->queryTokens[tokenIndex] == "Follows") {
		tokenIndex++;
		return parseFollows(tokenIndex);
	}
	else if (this->queryTokens[tokenIndex] == "Follows*") {
		tokenIndex++;
		return parseFollowsT(tokenIndex);
	}
	else if (this->queryTokens[tokenIndex] == "Parent") {
		tokenIndex++;
		return parseParent(tokenIndex);
	}
	else if (this->queryTokens[tokenIndex] == "Parent*") {
		tokenIndex++;
		return parseParentT(tokenIndex);
	}
	else if (this->queryTokens[tokenIndex] == "Uses") {
		tokenIndex++;
		return parseUses(tokenIndex);
	}
	else if (this->queryTokens[tokenIndex] == "Modifies") {
		tokenIndex++;
		return parseModifies(tokenIndex);
	}
	else {
		throw QueryException("Unexpected query token relation " + this->queryTokens[tokenIndex]);
	}

}

Relation QueryPreprocessor::parseFollows(int& tokenIndex) {
	Relationship relationship = Relationship::Follows;
	matchTokenOrThrow("(", tokenIndex);
	QueryStmtRef ref1 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(",", tokenIndex);
	QueryStmtRef ref2 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(")", tokenIndex);
	Relation relation = StmtsRelation(relationship, ref1, ref2);
	return relation;
}

Relation QueryPreprocessor::parseFollowsT(int& tokenIndex) {
	Relationship relationship = Relationship::FollowsT;
	matchTokenOrThrow("(", tokenIndex);
	QueryStmtRef ref1 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(",", tokenIndex);
	QueryStmtRef ref2 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(")", tokenIndex);
	Relation relation = StmtsRelation(relationship, ref1, ref2);
	return relation;
}

Relation QueryPreprocessor::parseParent(int& tokenIndex) {
	Relationship relationship = Relationship::Parent;
	matchTokenOrThrow("(", tokenIndex);
	QueryStmtRef ref1 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(",", tokenIndex);
	QueryStmtRef ref2 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(")", tokenIndex);
	Relation relation = StmtsRelation(relationship, ref1, ref2);
	return relation;
}

Relation QueryPreprocessor::parseParentT(int& tokenIndex) {
	Relationship relationship = Relationship::ParentT;
	matchTokenOrThrow("(", tokenIndex);
	QueryStmtRef ref1 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(",", tokenIndex);
	QueryStmtRef ref2 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(")", tokenIndex);
	Relation relation = StmtsRelation(relationship, ref1, ref2);
	return relation;
}

Relation QueryPreprocessor::parseUses(int& tokenIndex) {
	Relationship relationship = Relationship::UsesS;
	matchTokenOrThrow("(", tokenIndex);
	QueryStmtRef ref1 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(",", tokenIndex);
	QueryEntRef ref2 = parseQueryEntRef(tokenIndex);
	matchTokenOrThrow(")", tokenIndex);
	Relation relation = StmtEntRelation(relationship, ref1, ref2);
	return relation;
}

Relation QueryPreprocessor::parseModifies(int& tokenIndex) {
	Relationship relationship = Relationship::ModifiesS;
	matchTokenOrThrow("(", tokenIndex);
	QueryStmtRef ref1 = parseQueryStmtRef(tokenIndex);
	matchTokenOrThrow(",", tokenIndex);
	QueryEntRef ref2 = parseQueryEntRef(tokenIndex);
	matchTokenOrThrow(")", tokenIndex);
	Relation relation = StmtEntRelation(relationship, ref1, ref2);
	return relation;
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
	if (tokenIndex + 4 < this->queryTokens.size()) {
		string joinedExpression;
		for (int i = 0; i <= 4; ++i) {
			joinedExpression += this->queryTokens[tokenIndex + i];
		}
		if (regex_match(joinedExpression, regex("^_\"([a-zA-Z][a-zA-Z0-9]*|[0-9]+)\"_"))) {
			tokenIndex+=5;
			return joinedExpression;
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