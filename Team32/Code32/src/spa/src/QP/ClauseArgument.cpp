#include "QP/ClauseArgument.h"

#include <utility>

using namespace std;
using namespace QP::Types;

QP::ClauseArgument::ClauseArgument() = default;

QP::ClauseArgument::ClauseArgument(Declaration synonym) : value(synonym) {}

QP::ClauseArgument::ClauseArgument(Attribute attribute) : value(attribute) {}

QP::ClauseArgument::ClauseArgument(string name) : value(name) {}

QP::ClauseArgument::ClauseArgument(StmtRef statement_index) : value(statement_index) {}

QP::ClauseArgument::ClauseArgument(Common::ExpressionProcessor::Expression expression, bool exact)
	: value(pair<Common::ExpressionProcessor::Expression, bool>({move(expression), exact})) {}

/**
 * Returns the type of argument stored.
 *
 * The type is determined through the use of exhaustive overloaded visitor functions that capture the argument type to an external variable.
 */
ReferenceType QP::ClauseArgument::getType() const {
	ReferenceType type = ReferenceType::Wildcard;
	visit(Visitor{[&](const StmtRef& /*unused*/) { type = ReferenceType::StatementIndex; },
	              [&](const Declaration& /*unused*/) { type = ReferenceType::Synonym; },
	              [&](const string& /*unused*/) { type = ReferenceType::Name; },
	              [&](const pair<Common::ExpressionProcessor::Expression, bool>& arg) {
					  type = arg.second ? ReferenceType::ExactExpression : ReferenceType::SubExpression;
				  },
	              [&](const Attribute& /*unused*/) { type = ReferenceType::Attribute; },
	              [&](const monostate& /*unused*/) { type = ReferenceType::Wildcard; }},
	      value);
	return type;
}

Declaration QP::ClauseArgument::getSynonym() const {
	Declaration synonym;
	visit(Visitor{[](auto) { throw QP::ClauseArgumentException("Synonym not stored."); }, [&](Declaration arg) { synonym = move(arg); },
	              [&](Attribute arg) { synonym = move(arg.synonym); }},
	      value);
	return synonym;
}

string QP::ClauseArgument::getSynonymSymbol() const {
	string symbol;
	visit(Visitor{[](auto) { throw QP::ClauseArgumentException("Synonym not stored."); },
	              [&](const Declaration& arg) { symbol = arg.symbol; }, [&](const Attribute& arg) { symbol = arg.synonym.symbol; }},
	      value);
	return symbol;
}

DesignEntity QP::ClauseArgument::getSynonymType() const {
	DesignEntity type;
	visit(Visitor{[](auto) { throw QP::ClauseArgumentException("Synonym not stored."); },
	              [&](const Declaration& arg) { type = arg.type; }, [&](const Attribute& arg) { type = arg.synonym.type; }},
	      value);
	return type;
}

Attribute QP::ClauseArgument::getAttribute() const {
	Attribute attribute;
	visit(
		Visitor{[](auto) { throw QP::ClauseArgumentException("Attribute not stored."); }, [&](Attribute arg) { attribute = move(arg); }},
		value);
	return attribute;
}

AttributeType QP::ClauseArgument::getAttributeType() const {
	AttributeType type;
	visit(Visitor{[](auto) { throw QP::ClauseArgumentException("Attribute not stored."); },
	              [&](const Attribute& arg) { type = arg.attribute; }},
	      value);
	return type;
}

string QP::ClauseArgument::getName() const {
	string name;
	visit(Visitor{[](auto) { throw QP::ClauseArgumentException("Name not stored."); }, [&](string arg) { name = move(arg); }}, value);
	return name;
}

StmtRef QP::ClauseArgument::getStatementIndex() const {
	StmtRef index = 0;
	visit(Visitor{[](auto) { throw QP::ClauseArgumentException("Statement index not stored."); }, [&](StmtRef arg) { index = arg; }},
	      value);
	return index;
}

Common::ExpressionProcessor::Expression QP::ClauseArgument::getExpression() const {
	Common::ExpressionProcessor::Expression expr = Common::ExpressionProcessor::Expression(nullptr, {}, {});
	visit(Visitor{[](auto) { throw QP::ClauseArgumentException("Expression not stored."); },
	              [&](pair<Common::ExpressionProcessor::Expression, bool> arg) { expr = move(arg.first); }},
	      value);
	return expr;
}

bool QP::ClauseArgument::operator==(const ClauseArgument& other) const { return value == other.value; }
