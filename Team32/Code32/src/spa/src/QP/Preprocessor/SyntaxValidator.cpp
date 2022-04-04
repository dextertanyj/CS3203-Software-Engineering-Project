#include "QP/Preprocessor/SyntaxValidator.h"

#include <cassert>
#include <unordered_set>

#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

#define ASSIGN_WHILE_PATTERN_ARGUMENT_COUNT (3)
#define IF_PATTERN_ARGUMENT_COUNT (4)

using namespace std;
using namespace QP::Types;

static const unordered_set<ReferenceType> name_expression_wildcard = {ReferenceType::Name, ReferenceType::ExactExpression,
                                                                      ReferenceType::SubExpression, ReferenceType::Wildcard};
static const unordered_set<ReferenceType> statement = {ReferenceType::StatementIndex, ReferenceType::Wildcard, ReferenceType::Synonym};
static const unordered_set<ReferenceType> entity = {ReferenceType::Name, ReferenceType::Wildcard, ReferenceType::Synonym};
static const unordered_set<ReferenceType> statement_entity = {ReferenceType::Name, ReferenceType::StatementIndex, ReferenceType::Wildcard,
                                                              ReferenceType::Synonym};
static const unordered_set<ReferenceType> expression = {ReferenceType::ExactExpression, ReferenceType::SubExpression,
                                                        ReferenceType::Wildcard};
static const unordered_set<ReferenceType> wildcard = {ReferenceType::Wildcard};
static const unordered_set<ReferenceType> attribute = {ReferenceType::Attribute, ReferenceType::Name, ReferenceType::StatementIndex};
static const unordered_set<ReferenceType> synonym = {ReferenceType::Synonym};

const unordered_set<ClauseType> pattern_clauses = {ClauseType::PatternAssign, ClauseType::PatternIf, ClauseType::PatternWhile};

const unordered_map<ClauseType, vector<unordered_set<ReferenceType>>> syntax_argument_map = {
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

const unordered_map<size_t, vector<unordered_set<ReferenceType>>> pattern_syntax_argument_map = {
	{ASSIGN_WHILE_PATTERN_ARGUMENT_COUNT, {synonym, entity, name_expression_wildcard}},
	{IF_PATTERN_ARGUMENT_COUNT, {synonym, entity, wildcard, wildcard}},
};

bool QP::Preprocessor::SyntaxValidator::validateArgumentsSyntax(ClauseType type, const vector<ReferenceArgument>& arguments) {
	if (pattern_clauses.find(type) != pattern_clauses.end()) {
		return validatePatternArgumentsSyntax(arguments);
	}
	return validateStandardArgumentsSyntax(type, arguments);
}

bool QP::Preprocessor::SyntaxValidator::validateStandardArgumentsSyntax(ClauseType type, const vector<ReferenceArgument>& arguments) {
	assert(syntax_argument_map.find(type) != syntax_argument_map.end());
	auto expected_types = syntax_argument_map.at(type);
	if (expected_types.size() != arguments.size()) {
		return false;
	}
	for (int i = 0; i < arguments.size(); i++) {
		auto expected = expected_types.at(i);
		if (expected.find(arguments.at(i).getType()) == expected.end()) {
			return false;
		}
	}
	return true;
}

bool QP::Preprocessor::SyntaxValidator::validatePatternArgumentsSyntax(const vector<ReferenceArgument>& arguments) {
	auto iter = pattern_syntax_argument_map.find(arguments.size());
	if (iter == pattern_syntax_argument_map.end()) {
		return false;
	}
	auto expected_types = iter->second;
	for (int i = 0; i < arguments.size(); i++) {
		auto expected = expected_types.at(i);
		if (expected.find(arguments.at(i).getType()) == expected.end()) {
			return false;
		}
	}
	return true;
}
