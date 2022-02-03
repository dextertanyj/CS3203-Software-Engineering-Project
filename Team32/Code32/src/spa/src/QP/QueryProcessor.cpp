#include "QueryProcessor.h"

#include <regex>

#include "QueryTypeDefs.h"

string QueryProcessor::processQuery(string query) {

	tokenizeQuery(query);
	QueryProperties queryProperties = parseQuery();
	this->queryResult = QueryEvaluator::executeQuery(queryProperties);

	return formatResult();
}

void QueryProcessor::tokenizeQuery(string query) {

	regex invalidCharsRegex = regex(R"([^a-zA-Z0-9 ,\(\);\+\-\*\/%\n])");
	regex queryTokenRegex = regex(R"((Follows|Parents)\*|such that|[a-zA-Z][a-zA-Z0-9]*|[0-9]+|\(|\)|;|\\+|-|\*|\/|%|_|,|\")");

	if (regex_search(query, invalidCharsRegex)) {
		throw QueryException("Query included invalid characters");
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

QueryProperties QueryProcessor::parseQuery() {
	int tokenIndex = 0;

	// Used to check if synonyms for "Select" has been parsed
	this->select.symbol = "";

	while (tokenIndex < this->queryTokens.size()) {
		if (this->declarationList.empty() && this->queryTokens[tokenIndex] != "Select") {
			parseDeclaration(tokenIndex);
		}
		else if (select.symbol == "") {
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
			throw QueryException("Unexpected token");
		}
	}

	return QueryProperties::QueryProperties(
		this->declarationList, 
		this->select, 
		this->suchThatClauseList, 
		this->patternClauseList
	);
}

string QueryProcessor::formatResult() {
	return "";
}


void QueryProcessor::parseDeclaration(int& tokenIndex) {
	Declaration declaration;
	declaration.symbol = "";

	declaration.type = parseDesignEntity(tokenIndex);

	while (++tokenIndex < this->queryTokens.size()) {
		if (this->queryTokens[tokenIndex] == ";" && declaration.symbol != "") {
			tokenIndex++;
		}
		else if (this->queryTokens[tokenIndex] == "," && declaration.symbol != "") {
			Declaration declaration;
			declaration.symbol = "";
		}
		// if token satisfies "synonym : IDENT"
		else if (isIdentOrName(this->queryTokens[tokenIndex])) {
			declaration.symbol = this->queryTokens[tokenIndex];
			for (Declaration existingDeclaration : this->declarationList) {
				if (existingDeclaration.symbol == declaration.symbol) {
					throw QueryException("Duplicate synonym symbol");
				}
			}
			this->declarationList.push_back(declaration);
		}
		else {
			throw QueryException("Unexpected query token");
		}
	}
	throw QueryException("Unexpected end of query");
}

void QueryProcessor::parseSelect(int& tokenIndex) {
	if (tokenIndex >= this->queryTokens.size()) {
		throw QueryException("Unexpected end of query");
	}
	for (Declaration declaration : this->declarationList) {
		if (declaration.symbol == this->queryTokens[tokenIndex]) {
			this->select.type = declaration.type;
			this->select.symbol = this->queryTokens[tokenIndex];
			tokenIndex++;
		}
	}
	throw QueryException("Undeclared query synonym");
}

void QueryProcessor::parseSuchThat(int& tokenIndex) {
	SuchThatClause suchThatClause = { parseRelation(tokenIndex) };
	this->suchThatClauseList.push_back(suchThatClause);
	if (tokenIndex < this->queryTokens.size() && this->queryTokens[tokenIndex] == "and") {
		parseSuchThat(++tokenIndex);
	}
}

void QueryProcessor::parsePattern(int& tokenIndex) {
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
		throw QueryException("Missing query synonym");
	}
	patternClause.entRef = parseQueryEntRef(tokenIndex);
	patternClause.expression = parseExpression(tokenIndex);

	if (tokenIndex < this->queryTokens.size() && this->queryTokens[tokenIndex] == "and") {
		parsePattern(++tokenIndex);
	}
	tokenIndex++;
}

DesignEntity QueryProcessor::parseDesignEntity(int& tokenIndex) {
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
		throw QueryException("Unexpected query token");
	}
	tokenIndex++;
	return designEntity;

}

Relation QueryProcessor::parseRelation(int& tokenIndex) {
	Relationship relationship;

	if (this->queryTokens[tokenIndex] == "Follows") {
		relationship = Relationship::Follows;
	}
	else if (this->queryTokens[tokenIndex] == "Follows*") {
		relationship = Relationship::FollowsT;
	}
	else if (this->queryTokens[tokenIndex] == "Parent") {
		relationship = Relationship::Parent;
	}
	else if (this->queryTokens[tokenIndex] == "Parent*") {
		relationship = Relationship::ParentT;
	}
	else if (this->queryTokens[tokenIndex] == "Uses") {
		relationship = Relationship::UsesS;
	}
	else if (this->queryTokens[tokenIndex] == "Modifies") {
		relationship = Relationship::ModifiesS;
	}
	else {
		throw QueryException("Unexpected query token");
	}

	Relation relation = Relation::Relation(Relationship::Follows);
	tokenIndex++;
	return relation;
}

QueryEntRef QueryProcessor::parseQueryEntRef(int& tokenIndex) {
	QueryEntRef entRef;
	if (this->queryTokens[tokenIndex] == "_") {
		entRef.type = EntRefType::underscore;
		entRef.entRef = "_";
	}
	else if (tokenIndex + 2 < this->queryTokens.size() && 
		this->queryTokens[tokenIndex] == "\"" && 
		isIdentOrName(this->queryTokens[tokenIndex + 1]) && 
		this->queryTokens[tokenIndex+2] == "\"") {

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
			throw QueryException("Unexpected query token");
		}
	}

	tokenIndex++;
	return entRef;
}

QueryStmtRef QueryProcessor::parseQueryStmtRef(int& tokenIndex) {
	QueryStmtRef stmtRef;
	if (this->queryTokens[tokenIndex] == "_") {
		stmtRef.type = StmtRefType::underscore;
		stmtRef.stmtRef = "_";
	}
	else if (regex_match(this->queryTokens[tokenIndex], regex("^[0-9]+$"))) {

		stmtRef.type = StmtRefType::stmtNumber;
		stmtRef.stmtRef = this->queryTokens[tokenIndex + 1];
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
			if (!hasSynonym) {
				throw QueryException("Unexpected query token");
			}
		}
	}
	tokenIndex++;
	return stmtRef;
}


string QueryProcessor::parseExpression(int& tokenIndex) {
	if (this->queryTokens[tokenIndex] == "_") {
		tokenIndex++;
		return "_";
	}
	if (tokenIndex + 4 < this->queryTokens.size()) {
		string joinedExpression;
		for (int i = 0; i <= 4; ++i) {
			joinedExpression += this->queryTokens[tokenIndex + i];
		}
		if (regex_match(joinedExpression, regex("^_\"([a-zA-Z][a-zA-Z0-9]*|[0-9]+)\"_"))) {
			tokenIndex++;
			return joinedExpression;
		}
	}
}

bool isIdentOrName(string token) {
	return regex_search(token, regex("^[a-zA-Z][a-zA-Z0-9]*$"));
}