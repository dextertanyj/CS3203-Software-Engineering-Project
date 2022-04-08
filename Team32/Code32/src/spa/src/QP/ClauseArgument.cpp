#include "QP/ClauseArgument.h"

#include <cassert>
#include <utility>
#include <variant>

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
ArgumentType QP::ClauseArgument::getType() const {
	ArgumentType type = ArgumentType::Wildcard;
	visit(Visitor{[&](const StmtRef& /*unused*/) { type = ArgumentType::Number; },
	              [&](const Declaration& /*unused*/) { type = ArgumentType::Synonym; },
	              [&](const string& /*unused*/) { type = ArgumentType::Name; },
	              [&](const pair<Common::ExpressionProcessor::Expression, bool>& arg) {
					  type = arg.second ? ArgumentType::ExactExpression : ArgumentType::SubExpression;
				  },
	              [&](const Attribute& /*unused*/) { type = ArgumentType::Attribute; },
	              [&](const monostate& /*unused*/) { type = ArgumentType::Wildcard; }},
	      value);
	return type;
}

Declaration QP::ClauseArgument::getSynonym() const {
	Declaration synonym;
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](Declaration arg) { synonym = move(arg); }, [&](Attribute arg) { synonym = move(arg.synonym); }},
	      value);
	return synonym;
}

string QP::ClauseArgument::getSynonymSymbol() const {
	string symbol;
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](const Declaration& arg) { symbol = arg.symbol; }, [&](const Attribute& arg) { symbol = arg.synonym.symbol; }},
	      value);
	return symbol;
}

DesignEntity QP::ClauseArgument::getSynonymType() const {
	DesignEntity type;
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](const Declaration& arg) { type = arg.type; }, [&](const Attribute& arg) { type = arg.synonym.type; }},
	      value);
	return type;
}

Attribute QP::ClauseArgument::getAttribute() const {
	Attribute attribute;
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](Attribute arg) { attribute = move(arg); }},
	      value);
	return attribute;
}

AttributeType QP::ClauseArgument::getAttributeType() const {
	AttributeType type;
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](const Attribute& arg) { type = arg.attribute; }},
	      value);
	return type;
}

string QP::ClauseArgument::getName() const {
	string name;
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](string arg) { name = move(arg); }},
	      value);
	return name;
}

StmtRef QP::ClauseArgument::getStatementIndex() const {
	StmtRef index = 0;
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](StmtRef arg) { index = arg; }},
	      value);
	return index;
}

Common::EP::Expression QP::ClauseArgument::getExpression() const {
	Common::EP::Expression expr = Common::EP::Expression(nullptr, {}, {});
	visit(Visitor{[](auto) { assert(false); },  // NOLINT(bugprone-lambda-function-name)
	              [&](pair<Common::EP::Expression, bool> arg) { expr = move(arg.first); }},
	      value);
	return expr;
}

bool QP::ClauseArgument::operator==(const ClauseArgument& other) const { return value == other.value; }
