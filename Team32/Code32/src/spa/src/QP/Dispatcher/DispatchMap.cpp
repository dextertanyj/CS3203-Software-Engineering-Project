#include "QP/Dispatcher/DispatchMap.h"

#include "QP/Dispatcher/AffectsDispatcher.tpp"
#include "QP/Dispatcher/CallsDispatcher.tpp"
#include "QP/Dispatcher/FollowsNextDispatcher.tpp"
#include "QP/Dispatcher/ModifiesDispatcher.h"
#include "QP/Dispatcher/ParentDispatcher.tpp"
#include "QP/Dispatcher/PatternAssignDispatcher.h"
#include "QP/Dispatcher/PatternIfDispatcher.h"
#include "QP/Dispatcher/PatternWhileDispatcher.h"
#include "QP/Dispatcher/UsesDispatcher.h"
#include "QP/Dispatcher/WithDispatcher.h"

namespace QP::Dispatcher::DispatchMap {

const ArgumentDispatchMap dispatch_map = {
	{ClauseType::Affects, AffectsDispatcher::dispatcher<ClauseType::Affects>},
	{ClauseType::AffectsT, AffectsDispatcher::dispatcher<ClauseType::AffectsT>},
	{ClauseType::Calls, CallsDispatcher::dispatcher<ClauseType::Calls>},
	{ClauseType::CallsT, CallsDispatcher::dispatcher<ClauseType::CallsT>},
	{ClauseType::Follows, FollowsNextDispatcher::dispatcher<ClauseType::Follows>},
	{ClauseType::FollowsT, FollowsNextDispatcher::dispatcher<ClauseType::FollowsT>},
	{ClauseType::UnknownModifies, ModifiesDispatcher::dispatcher},
	{ClauseType::Next, FollowsNextDispatcher::dispatcher<ClauseType::Next>},
	{ClauseType::NextT, FollowsNextDispatcher::dispatcher<ClauseType::NextT>},
	{ClauseType::Parent, ParentDispatcher::dispatcher<ClauseType::Parent>},
	{ClauseType::ParentT, ParentDispatcher::dispatcher<ClauseType::ParentT>},
	{ClauseType::PatternAssign, PatternAssignDispatcher::dispatcher},
	{ClauseType::PatternWhile, PatternWhileDispatcher::dispatcher},
	{ClauseType::PatternIf, PatternIfDispatcher::dispatcher},
	{ClauseType::UnknownUses, UsesDispatcher::dispatcher},
	{ClauseType::With, WithDispatcher::dispatcher},
};

const unordered_map<string, ClauseType> clause_map = {
	{"Affects", ClauseType::Affects},
	{"Affects*", ClauseType::AffectsT},
	{"Calls", ClauseType::Calls},
	{"Calls*", ClauseType::CallsT},
	{"Follows", ClauseType::Follows},
	{"Follows*", ClauseType::FollowsT},
	{"Modifies", ClauseType::UnknownModifies},
	{"Next", ClauseType::Next},
	{"Next*", ClauseType::NextT},
	{"Parent", ClauseType::Parent},
	{"Parent*", ClauseType::ParentT},
	{"Uses", ClauseType::UnknownUses},
};

static const unordered_set<ReferenceType> name_wildcard = {ReferenceType::Name, ReferenceType::Wildcard};
static const unordered_set<ReferenceType> statement = {ReferenceType::StatementIndex, ReferenceType::Wildcard, ReferenceType::Synonym};
static const unordered_set<ReferenceType> entity = {ReferenceType::Name, ReferenceType::Wildcard, ReferenceType::Synonym};
static const unordered_set<ReferenceType> statement_entity = {ReferenceType::Name, ReferenceType::StatementIndex, ReferenceType::Wildcard,
                                                              ReferenceType::Synonym};
static const unordered_set<ReferenceType> expression = {ReferenceType::ExactExpression, ReferenceType::SubExpression,
                                                        ReferenceType::Wildcard};
static const unordered_set<ReferenceType> wildcard = {ReferenceType::Wildcard};
static const unordered_set<ReferenceType> attribute = {ReferenceType::Attribute, ReferenceType::Name, ReferenceType::StatementIndex};

const unordered_map<ClauseType, vector<unordered_set<ReferenceType>>> syntax_map = {
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
	{ClauseType::PatternAssign, {statement, entity, expression}},
	{ClauseType::PatternWhile, {statement, entity, wildcard}},
	{ClauseType::PatternIf, {statement, entity, wildcard, wildcard}},
	{ClauseType::UnknownUses, {statement_entity, entity}},
	{ClauseType::With, {attribute, attribute}}};

const unordered_map<size_t, vector<unordered_set<ReferenceType>>> pattern_syntax_map = {
	{ASSIGN_WHILE_PATTERN_ARGUMENT_COUNT, {entity, name_wildcard}},
	{IF_PATTERN_ARGUMENT_COUNT, {entity, wildcard, wildcard}},
};

const unordered_map<DesignEntity, ClauseType> pattern_clause_map = {
	{DesignEntity::If, ClauseType::PatternIf},
	{DesignEntity::While, ClauseType::PatternWhile},
};

const unordered_map<string, AttributeToken> attribute_token_map = {{"stmt#", AttributeToken::StatementIndex},
                                                                   {"varName", AttributeToken::VariableName},
                                                                   {"procName", AttributeToken::ProcedureName},
                                                                   {"value", AttributeToken::Value}};

const unordered_map<pair<DesignEntity, AttributeToken>, AttributeType> attribute_map = {
	{{DesignEntity::Stmt, AttributeToken::StatementIndex}, AttributeType::NumberIdentifier},
	{{DesignEntity::Read, AttributeToken::StatementIndex}, AttributeType::NumberIdentifier},
	{{DesignEntity::Print, AttributeToken::StatementIndex}, AttributeType::NumberIdentifier},
	{{DesignEntity::Call, AttributeToken::StatementIndex}, AttributeType::NumberIdentifier},
	{{DesignEntity::While, AttributeToken::StatementIndex}, AttributeType::NumberIdentifier},
	{{DesignEntity::If, AttributeToken::StatementIndex}, AttributeType::NumberIdentifier},
	{{DesignEntity::Assign, AttributeToken::StatementIndex}, AttributeType::NumberIdentifier},
	{{DesignEntity::Read, AttributeToken::VariableName}, AttributeType::VariableName},
	{{DesignEntity::Print, AttributeToken::VariableName}, AttributeType::VariableName},
	{{DesignEntity::Call, AttributeToken::ProcedureName}, AttributeType::ProcedureName},
	{{DesignEntity::Procedure, AttributeToken::ProcedureName}, AttributeType::NameIdentifier},
	{{DesignEntity::Variable, AttributeToken::VariableName}, AttributeType::NameIdentifier},
	{{DesignEntity::Constant, AttributeToken::Value}, AttributeType::NumberIdentifier}};

const unordered_map<string, DesignEntity> design_entity_map = {
	{"stmt", DesignEntity::Stmt},           {"read", DesignEntity::Read},         {"print", DesignEntity::Print},
	{"call", DesignEntity::Call},           {"while", DesignEntity::While},       {"if", DesignEntity::If},
	{"assign", DesignEntity::Assign},       {"variable", DesignEntity::Variable}, {"constant", DesignEntity::Constant},
	{"procedure", DesignEntity::Procedure},
};

}