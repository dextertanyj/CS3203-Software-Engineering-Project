#include "ReferenceArgument.h"

#include <utility>

using namespace std;
using namespace QP::Types;

QP::ReferenceArgument::ReferenceArgument() = default;

QP::ReferenceArgument::ReferenceArgument(Declaration synonym) : value(synonym) {}

QP::ReferenceArgument::ReferenceArgument(Attribute attribute) : value(attribute) {}

QP::ReferenceArgument::ReferenceArgument(string name) : value(name) {}

QP::ReferenceArgument::ReferenceArgument(StmtRef statement_index) : value(statement_index) {}

QP::ReferenceArgument::ReferenceArgument(Common::ExpressionProcessor::Expression expression, bool exact)
	: value(pair<Common::ExpressionProcessor::Expression, bool>({move(expression), exact})) {}

ReferenceType QP::ReferenceArgument::getType() const {
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

Declaration QP::ReferenceArgument::getSynonym() const {
	Declaration synonym;
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](Declaration arg) { synonym = move(arg); }, [&](Attribute arg) { synonym = move(arg.synonym); }},
	      value);
	return synonym;
}

string QP::ReferenceArgument::getSynonymSymbol() const {
	string symbol;
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](const Declaration& arg) { symbol = arg.symbol; }, [&](const Attribute& arg) { symbol = arg.synonym.symbol; }},
	      value);
	return symbol;
}

DesignEntity QP::ReferenceArgument::getSynonymType() const {
	DesignEntity type;
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](const Declaration& arg) { type = arg.type; }, [&](const Attribute& arg) { type = arg.synonym.type; }},
	      value);
	return type;
}

Attribute QP::ReferenceArgument::getAttribute() const {
	Attribute attribute;
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](Attribute arg) { attribute = move(arg); }},
	      value);
	return attribute;
}

AttributeType QP::ReferenceArgument::getAttributeType() const {
	AttributeType type;
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](const Attribute& arg) { type = arg.attribute; }},
	      value);
	return type;
}

string QP::ReferenceArgument::getName() const {
	string name;
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](string arg) { name = move(arg); }},
	      value);
	return name;
}

StmtRef QP::ReferenceArgument::getStatementIndex() const {
	StmtRef index = 0;
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](StmtRef arg) { index = arg; }},
	      value);
	return index;
}

Common::ExpressionProcessor::Expression QP::ReferenceArgument::getExpression() const {
	Common::ExpressionProcessor::Expression expr = Common::ExpressionProcessor::Expression(nullptr, {}, {});
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](pair<Common::ExpressionProcessor::Expression, bool> arg) { expr = move(arg.first); }},
	      value);
	return expr;
}

bool QP::ReferenceArgument::operator==(const ReferenceArgument& other) const { return value == other.value; }
