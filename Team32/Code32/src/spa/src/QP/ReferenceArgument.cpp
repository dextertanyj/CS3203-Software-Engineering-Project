#include "ReferenceArgument.h"

#include <utility>

QP::Types::ReferenceArgument::ReferenceArgument() = default;

QP::Types::ReferenceArgument::ReferenceArgument(QP::Types::Declaration synonym) : value(synonym) {}

QP::Types::ReferenceArgument::ReferenceArgument(QP::Types::Attribute attribute) : value(attribute) {}

QP::Types::ReferenceArgument::ReferenceArgument(string name) : value(name) {}

QP::Types::ReferenceArgument::ReferenceArgument(StmtRef statement_index) : value(statement_index) {}

QP::Types::ReferenceArgument::ReferenceArgument(Common::ExpressionProcessor::Expression expression, bool exact)
	: value(pair<Common::ExpressionProcessor::Expression, bool>({move(expression), exact})) {}

QP::Types::ReferenceType QP::Types::ReferenceArgument::getType() const {
	QP::Types::ReferenceType type = QP::Types::ReferenceType::Wildcard;
	visit(Visitor{[&](const StmtRef& /*unused*/) { type = QP::Types::ReferenceType::StatementIndex; },
	              [&](const QP::Types::Declaration& /*unused*/) { type = QP::Types::ReferenceType::Synonym; },
	              [&](const string& /*unused*/) { type = QP::Types::ReferenceType::Name; },
	              [&](const pair<Common::ExpressionProcessor::Expression, bool>& arg) {
					  type = arg.second ? QP::Types::ReferenceType::ExactExpression : QP::Types::ReferenceType::SubExpression;
				  },
	              [&](const Attribute& /*unused*/) { type = QP::Types::ReferenceType::Attribute; },
	              [&](const monostate& /*unused*/) { type = QP::Types::ReferenceType::Wildcard; }},
	      value);
	return type;
}

QP::Types::Declaration QP::Types::ReferenceArgument::getSynonym() const {
	Declaration synonym;
	visit(Visitor{[](auto) { throw QP::ReferenceArgumentException("Synonym not stored."); }, [&](Declaration arg) { synonym = move(arg); },
	              [&](Attribute arg) { synonym = move(arg.synonym); }},
	      value);
	return synonym;
}

string QP::Types::ReferenceArgument::getSynonymSymbol() const {
	string symbol;
	visit(Visitor{[](auto) { throw QP::ReferenceArgumentException("Synonym not stored."); },
	              [&](const Declaration& arg) { symbol = arg.symbol; }, [&](const Attribute& arg) { symbol = arg.synonym.symbol; }},
	      value);
	return symbol;
}

QP::Types::DesignEntity QP::Types::ReferenceArgument::getSynonymType() const {
	DesignEntity type;
	visit(Visitor{[](auto) { throw QP::ReferenceArgumentException("Synonym not stored."); },
	              [&](const Declaration& arg) { type = arg.type; }, [&](const Attribute& arg) { type = arg.synonym.type; }},
	      value);
	return type;
}

QP::Types::Attribute QP::Types::ReferenceArgument::getAttribute() const {
	Attribute attribute;
	visit(
		Visitor{[](auto) { throw QP::ReferenceArgumentException("Attribute not stored."); }, [&](Attribute arg) { attribute = move(arg); }},
		value);
	return attribute;
}

QP::Types::AttributeType QP::Types::ReferenceArgument::getAttributeType() const {
	AttributeType type;
	visit(Visitor{[](auto) { throw QP::ReferenceArgumentException("Attribute not stored."); },
	              [&](const Attribute& arg) { type = arg.attribute; }},
	      value);
	return type;
}

string QP::Types::ReferenceArgument::getName() const {
	string name;
	visit(Visitor{[](auto) { throw QP::ReferenceArgumentException("Name not stored."); }, [&](string arg) { name = move(arg); }}, value);
	return name;
}

StmtRef QP::Types::ReferenceArgument::getStatementIndex() const {
	StmtRef index = 0;
	visit(Visitor{[](auto) { throw QP::ReferenceArgumentException("Statement index not stored."); }, [&](StmtRef arg) { index = arg; }},
	      value);
	return index;
}

Common::ExpressionProcessor::Expression QP::Types::ReferenceArgument::getExpression() const {
	Common::ExpressionProcessor::Expression expr = Common::ExpressionProcessor::Expression(nullptr, {}, {});
	visit(Visitor{[](auto) { throw QP::ReferenceArgumentException("Expression not stored."); },
	              [&](pair<Common::ExpressionProcessor::Expression, bool> arg) { expr = move(arg.first); }},
	      value);
	return expr;
}

bool QP::Types::ReferenceArgument::operator==(const ReferenceArgument& other) const {
	ReferenceType type = getType();

	if (type != other.getType()) {
		return false;
	}

	switch (type) {
		case ReferenceType::StatementIndex:
			return getStatementIndex() == other.getStatementIndex();
		case ReferenceType::Synonym:
			return getSynonymSymbol() == other.getSynonymSymbol();
		case ReferenceType::Wildcard:
			return true;
		case ReferenceType::Name:
			return getName() == other.getName();
		case ReferenceType::ExactExpression:
		case ReferenceType::SubExpression:
			return getExpression() == other.getExpression();
		case ReferenceType::Attribute:
			return getAttribute() == other.getAttribute();
	}
}
