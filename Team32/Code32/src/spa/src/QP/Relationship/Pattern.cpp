#include "QP/Relationship/Pattern.h"

QP::Relationship::Pattern::Pattern(ReferenceArgument syn_assign, ReferenceArgument ent_ref, ReferenceArgument expression)
	: syn_assign(std::move(syn_assign)), ent_ref(std::move(ent_ref)), expression(std::move(expression)) {}

ReferenceArgument QP::Relationship::Pattern::getSynAssign() { return this->syn_assign; }

ReferenceArgument QP::Relationship::Pattern::getEntRef() { return this->ent_ref; }

ReferenceType QP::Relationship::Pattern::getExpressionType() { return this->expression.getType(); }

Common::ExpressionProcessor::Expression QP::Relationship::Pattern::getExpression() { return this->expression.getExpression(); }

vector<string> QP::Relationship::Pattern::getDeclarationSymbols() {
	vector<string> declaration_symbols = {this->syn_assign.getSynonym().symbol};
	if (this->ent_ref.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->ent_ref.getSynonym().symbol);
	}
	return declaration_symbols;
}

static QP::QueryResult executeTrivialNameWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& /*assign*/,
                                                  const ReferenceArgument& name, const ReferenceArgument& /*wildcard*/) {
	return QP::QueryResult(!pkb.getStmtsWithPatternLHS(name.getName()).empty());
}

static QP::QueryResult executeTrivialSynonymOrWildcardWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& /*assign*/,
                                                               const ReferenceArgument& /*synonym_or_wildcard*/,
                                                               const ReferenceArgument& /*wildcard*/) {
	auto stmt_set = pkb.getStatements();
	for (auto const& stmt : stmt_set) {
		if (QP::Utilities::checkStmtTypeMatch(stmt, DesignEntity::Assign)) {
			return QP::QueryResult(true);
		}
	}
	return {};
}

static QP::QueryResult executeTrivialNameExpression(PKB::StorageAccessInterface& pkb, const ReferenceArgument& /*assign*/,
                                                    const ReferenceArgument& name, const ReferenceArgument& expression) {
	return QP::QueryResult(
		pkb.patternExists(name.getName(), expression.getExpression(), expression.getType() == QP::Types::ReferenceType::ExactExpression));
}

static QP::QueryResult executeTrivialSynonymOrWildcardExpression(PKB::StorageAccessInterface& pkb, const ReferenceArgument& /*assign*/,
                                                                 const ReferenceArgument& /*synonym_or_wildcard*/,
                                                                 const ReferenceArgument& expression) {
	return QP::QueryResult(
		!pkb.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == QP::Types::ReferenceType::ExactExpression).empty());
}

QP::QueryResult QP::Relationship::Pattern::executeTrivial(PKB::StorageAccessInterface& pkb) {
	if (ent_ref.getType() == Types::ReferenceType::Name) {
		switch (expression.getType()) {
			case Types::ReferenceType::Wildcard:
				return executeTrivialNameWildcard(pkb, syn_assign, ent_ref, expression);
			case Types::ReferenceType::SubExpression:
			case Types::ReferenceType::ExactExpression:
				return executeTrivialNameExpression(pkb, syn_assign, ent_ref, expression);
			default:
				throw logic_error("Incorrect arguments received.");
		}
	}
	switch (expression.getType()) {
		case Types::ReferenceType::Wildcard:
			return executeTrivialSynonymOrWildcardWildcard(pkb, syn_assign, ent_ref, expression);
		case Types::ReferenceType::SubExpression:
		case Types::ReferenceType::ExactExpression:
			return executeTrivialSynonymOrWildcardExpression(pkb, syn_assign, ent_ref, expression);
		default:
			throw logic_error("Incorrect arguments received.");
	}
}

static QP::QueryResult executeNameWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& assign, const ReferenceArgument& name,
                                           const ReferenceArgument& /*wildcard*/) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	auto results = pkb.getStmtsWithPatternLHS(name.getName());
	for (auto const& entry : results) {
		statement_result.push_back(to_string(entry->getIdentifier()));
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);

	return result;
}

static QP::QueryResult executeWildcardWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& assign,
                                               const ReferenceArgument& /*wildcard*/, const ReferenceArgument& /*wildcard*/) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	auto results = pkb.getStatements();
	for (auto const& entry : results) {
		if (QP::Utilities::checkStmtTypeMatch(entry, DesignEntity::Assign)) {
			statement_result.push_back(to_string(entry->getIdentifier()));
		}
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);

	return result;
}

static QP::QueryResult executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& assign,
                                              const ReferenceArgument& synonym, const ReferenceArgument& /*wildcard*/) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	vector<string> variable_result;
	for (auto const& var_ref : pkb.getVariables()) {
		auto stmt_set = pkb.getStmtsWithPatternLHS(var_ref);
		for (auto const& stmt : stmt_set) {
			statement_result.push_back(to_string(stmt->getIdentifier()));
			variable_result.push_back(var_ref);
		}
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);
	result.addColumn(synonym.getSynonym().symbol, variable_result);

	return result;
}

static QP::QueryResult executeNameExpression(PKB::StorageAccessInterface& pkb, const ReferenceArgument& assign,
                                             const ReferenceArgument& name, const ReferenceArgument& expression) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	auto results = pkb.getStmtsWithPattern(name.getName(), expression.getExpression(),
	                                       expression.getType() == QP::Types::ReferenceType::ExactExpression);
	for (auto const& entry : results) {
		statement_result.push_back(to_string(entry->getIdentifier()));
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);
	return result;
}

static QP::QueryResult executeWildcardExpression(PKB::StorageAccessInterface& pkb, const ReferenceArgument& assign,
                                                 const ReferenceArgument& wildcard, const ReferenceArgument& expression) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	auto result_pairs =
		pkb.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == QP::Types::ReferenceType::ExactExpression);
	for (auto const& pair : result_pairs) {
		statement_result.push_back(to_string(pair.first->getIdentifier()));
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);

	return result;
}

static QP::QueryResult executeSynonymExpression(PKB::StorageAccessInterface& pkb, const ReferenceArgument& assign,
                                                const ReferenceArgument& synonym, const ReferenceArgument& expression) {
	QP::QueryResult result = QP::QueryResult();
	vector<string> statement_result;
	vector<string> variable_result;
	auto result_pairs =
		pkb.getStmtsWithPatternRHS(expression.getExpression(), expression.getType() == QP::Types::ReferenceType::ExactExpression);
	for (auto const& pair : result_pairs) {
		statement_result.push_back(to_string(pair.first->getIdentifier()));
		variable_result.push_back(pair.second);
	}
	result.addColumn(assign.getSynonym().symbol, statement_result);
	result.addColumn(synonym.getSynonym().symbol, variable_result);

	return result;
}

QP::QueryResult QP::Relationship::Pattern::executeNonTrivial(PKB::StorageAccessInterface& pkb) {
	switch (ent_ref.getType()) {
		case Types::ReferenceType::Name:
			switch (expression.getType()) {
				case Types::ReferenceType::Wildcard:
					return executeNameWildcard(pkb, syn_assign, ent_ref, expression);
				case Types::ReferenceType::SubExpression:
				case Types::ReferenceType::ExactExpression:
					return executeNameExpression(pkb, syn_assign, ent_ref, expression);
				default:
					throw logic_error("Incorrect arguments received.");
			}
		case Types::ReferenceType::Wildcard:
			switch (expression.getType()) {
				case Types::ReferenceType::Wildcard:
					return executeWildcardWildcard(pkb, syn_assign, ent_ref, expression);
				case Types::ReferenceType::SubExpression:
				case Types::ReferenceType::ExactExpression:
					return executeWildcardExpression(pkb, syn_assign, ent_ref, expression);
				default:
					throw logic_error("Incorrect arguments received.");
			}
		case Types::ReferenceType::Synonym:
			switch (expression.getType()) {
				case Types::ReferenceType::Wildcard:
					return executeSynonymWildcard(pkb, syn_assign, ent_ref, expression);
				case Types::ReferenceType::SubExpression:
				case Types::ReferenceType::ExactExpression:
					return executeSynonymExpression(pkb, syn_assign, ent_ref, expression);
				default:
					throw logic_error("Incorrect arguments received.");
			}
		default:
			throw logic_error("Incorrect arguments received.");
	}

}