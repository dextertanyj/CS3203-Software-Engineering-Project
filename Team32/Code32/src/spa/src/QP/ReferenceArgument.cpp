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
	visit(Visitor{[&type](const StmtRef& /*unused*/) { type = QP::Types::ReferenceType::StatementIndex; },
	              [&type](const QP::Types::Declaration& /*unused*/) { type = QP::Types::ReferenceType::Synonym; },
	              [&type](const string& /*unused*/) { type = QP::Types::ReferenceType::Name; },
	              [&type](const pair<Common::ExpressionProcessor::Expression, bool>& arg) {
					  type = arg.second ? QP::Types::ReferenceType::ExactExpression : QP::Types::ReferenceType::SubExpression;
				  },
	              [&type](const SynonymAttribute& /*unused*/) { type = QP::Types::ReferenceType::Attribute; },
	              [&type](const monostate& /*unused*/) { type = QP::Types::ReferenceType::Wildcard; }},
	      value);
	return type;
}

QP::Types::Declaration QP::Types::ReferenceArgument::getSynonym() const {
	Declaration synonym;
	visit(Visitor{[](auto) { throw QP::ReferenceArgumentException("Synonym not stored."); },
	              [&synonym](Declaration arg) { synonym = move(arg); }},
	      value);
	return synonym;
}

QP::Types::SynonymAttribute QP::Types::ReferenceArgument::getAttribute() const {
	SynonymAttribute attribute;
	visit(Visitor{[](auto) { throw QP::ReferenceArgumentException("Synonym not stored."); },
	              [&attribute](SynonymAttribute arg) { attribute = move(arg); }},
	      value);
	return attribute;
}

string QP::Types::ReferenceArgument::getName() const {
	string name;
	visit(Visitor{[](auto) { throw QP::ReferenceArgumentException("Name not stored."); }, [&name](string arg) { name = move(arg); }},
	      value);
	return name;
}

StmtRef QP::Types::ReferenceArgument::getStatementIndex() const {
	StmtRef index = 0;
	visit(
		Visitor{[](auto) { throw QP::ReferenceArgumentException("Statement index not stored."); }, [&index](StmtRef arg) { index = arg; }},
		value);
	return index;
}

Common::ExpressionProcessor::Expression QP::Types::ReferenceArgument::getExpression() const {
	Common::ExpressionProcessor::Expression expr = Common::ExpressionProcessor::Expression(nullptr, {}, {});
	visit(Visitor{[](auto) { throw QP::ReferenceArgumentException("Expression not stored."); },
	              [&expr](pair<Common::ExpressionProcessor::Expression, bool> arg) { expr = move(arg.first); }},
	      value);
	return expr;
}