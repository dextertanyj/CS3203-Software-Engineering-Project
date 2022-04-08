#include "QP/Preprocessor/SyntaxValidator.h"

#include <cassert>
#include <unordered_set>

#include "QP/ClauseArgument.h"
#include "QP/Types.h"

#define ASSIGN_WHILE_PATTERN_ARGUMENT_COUNT (3)
#define IF_PATTERN_ARGUMENT_COUNT (4)

using namespace std;
using namespace QP::Types;

static const unordered_set<ArgumentType> name_expression_wildcard = {ArgumentType::Name, ArgumentType::ExactExpression,
                                                                     ArgumentType::SubExpression, ArgumentType::Wildcard};
static const unordered_set<ArgumentType> statement = {ArgumentType::Number, ArgumentType::Wildcard, ArgumentType::Synonym};
static const unordered_set<ArgumentType> entity = {ArgumentType::Name, ArgumentType::Wildcard, ArgumentType::Synonym};
static const unordered_set<ArgumentType> statement_entity = {ArgumentType::Name, ArgumentType::Number, ArgumentType::Wildcard,
                                                             ArgumentType::Synonym};
static const unordered_set<ArgumentType> expression = {ArgumentType::ExactExpression, ArgumentType::SubExpression, ArgumentType::Wildcard};
static const unordered_set<ArgumentType> wildcard = {ArgumentType::Wildcard};
static const unordered_set<ArgumentType> attribute = {ArgumentType::Attribute, ArgumentType::Name, ArgumentType::Number};
static const unordered_set<ArgumentType> synonym = {ArgumentType::Synonym};

const unordered_set<ClauseType> pattern_clauses = {ClauseType::PatternAssign, ClauseType::PatternIf, ClauseType::PatternWhile};

const unordered_map<ClauseType, vector<unordered_set<ArgumentType>>> syntax_argument_map = {
	{ClauseType::Affects, {statement, statement}},
	{ClauseType::AffectsT, {statement, statement}},
	{ClauseType::Calls, {entity, entity}},
	{ClauseType::CallsT, {entity, entity}},
	{ClauseType::Follows, {statement, statement}},
	{ClauseType::FollowsT, {statement, statement}},
	{ClauseType::UnknownModifies, {statement_entity, entity}},
	{ClauseType::Next, {statement, statement}},
	{ClauseType::NextT, {statement, statement}},
	{ClauseType::Parent, {statement, statement}},
	{ClauseType::ParentT, {statement, statement}},
	{ClauseType::UnknownUses, {statement_entity, entity}},
	{ClauseType::With, {attribute, attribute}}};

const unordered_map<size_t, vector<unordered_set<ArgumentType>>> pattern_syntax_argument_map = {
	{ASSIGN_WHILE_PATTERN_ARGUMENT_COUNT, {synonym, entity, name_expression_wildcard}},
	{IF_PATTERN_ARGUMENT_COUNT, {synonym, entity, wildcard, wildcard}},
};

bool QP::Preprocessor::SyntaxValidator::validateArgumentsSyntax(ClauseType type, const vector<ClauseArgument>& arguments) {
	if (pattern_clauses.find(type) != pattern_clauses.end()) {
		return validatePatternArgumentsSyntax(arguments);
	}
	return validateStandardArgumentsSyntax(type, arguments);
}

bool QP::Preprocessor::SyntaxValidator::validateStandardArgumentsSyntax(ClauseType type, const vector<ClauseArgument>& arguments) {
	assert(syntax_argument_map.find(type) != syntax_argument_map.end());
	auto expected_types = syntax_argument_map.at(type);
	if (expected_types.size() != arguments.size()) {
		return false;
	}
	for (int i = 0; i < arguments.size(); i++) {
		auto expected = expected_types[i];
		if (expected.find(arguments[i].getType()) == expected.end()) {
			return false;
		}
	}
	return true;
}

bool QP::Preprocessor::SyntaxValidator::validatePatternArgumentsSyntax(const vector<ClauseArgument>& arguments) {
	auto iter = pattern_syntax_argument_map.find(arguments.size());
	if (iter == pattern_syntax_argument_map.end()) {
		return false;
	}
	auto expected_types = iter->second;
	for (int i = 0; i < arguments.size(); i++) {
		auto expected = expected_types[i];
		if (expected.find(arguments[i].getType()) == expected.end()) {
			return false;
		}
	}
	return true;
}
