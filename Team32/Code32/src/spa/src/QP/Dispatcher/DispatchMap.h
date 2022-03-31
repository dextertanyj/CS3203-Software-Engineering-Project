#ifndef SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H
#define SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H

#include "QP/Dispatcher/AffectsDispatcher.tpp"
#include "QP/Dispatcher/CallsDispatcher.tpp"
#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Dispatcher/FollowsNextDispatcher.tpp"
#include "QP/Dispatcher/ModifiesDispatcher.h"
#include "QP/Dispatcher/ParentDispatcher.tpp"
#include "QP/Dispatcher/PatternAssignDispatcher.h"
#include "QP/Dispatcher/PatternIfDispatcher.h"
#include "QP/Dispatcher/PatternWhileDispatcher.h"
#include "QP/Dispatcher/UsesDispatcher.h"
#include "QP/Dispatcher/WithDispatcher.h"
#include "QP/QP.h"
#include "QP/Types.h"

#define ASSIGN_WHILE_PATTERN_ARGUMENT_COUNT (2)
#define IF_PATTERN_ARGUMENT_COUNT (3)

struct QP::Dispatcher::DispatchMap {
	Types::ArgumentDispatchMap dispatch_map = {
		{Types::ClauseType::Affects, QP::Dispatcher::AffectsDispatcher::dispatcher<Types::ClauseType::Affects>},
		{Types::ClauseType::AffectsT, QP::Dispatcher::AffectsDispatcher::dispatcher<Types::ClauseType::AffectsT>},
		{Types::ClauseType::Calls, QP::Dispatcher::CallsDispatcher::dispatcher<Types::ClauseType::Calls>},
		{Types::ClauseType::CallsT, QP::Dispatcher::CallsDispatcher::dispatcher<Types::ClauseType::CallsT>},
		{Types::ClauseType::Follows, QP::Dispatcher::FollowsNextDispatcher::dispatcher<Types::ClauseType::Follows>},
		{Types::ClauseType::FollowsT, QP::Dispatcher::FollowsNextDispatcher::dispatcher<Types::ClauseType::FollowsT>},
		{Types::ClauseType::UnknownModifies, QP::Dispatcher::ModifiesDispatcher::dispatcher},
		{Types::ClauseType::Next, QP::Dispatcher::FollowsNextDispatcher::dispatcher<Types::ClauseType::Next>},
		{Types::ClauseType::NextT, QP::Dispatcher::FollowsNextDispatcher::dispatcher<Types::ClauseType::NextT>},
		{Types::ClauseType::Parent, QP::Dispatcher::ParentDispatcher::dispatcher<Types::ClauseType::Parent>},
		{Types::ClauseType::ParentT, QP::Dispatcher::ParentDispatcher::dispatcher<Types::ClauseType::ParentT>},
		{Types::ClauseType::PatternAssign, QP::Dispatcher::PatternAssignDispatcher::dispatcher},
		{Types::ClauseType::PatternWhile, QP::Dispatcher::PatternWhileDispatcher::dispatcher},
		{Types::ClauseType::PatternIf, QP::Dispatcher::PatternIfDispatcher::dispatcher},
		{Types::ClauseType::UnknownUses, QP::Dispatcher::UsesDispatcher::dispatcher},
		{Types::ClauseType::With, QP::Dispatcher::WithDispatcher::dispatcher},
	};

	unordered_map<string, Types::ClauseType> clause_map = {
		{"Affects", Types::ClauseType::Affects},
		{"Affects*", Types::ClauseType::AffectsT},
		{"Calls", Types::ClauseType::Calls},
		{"Calls*", Types::ClauseType::CallsT},
		{"Follows", Types::ClauseType::Follows},
		{"Follows*", Types::ClauseType::FollowsT},
		{"Modifies", Types::ClauseType::UnknownModifies},
		{"Next", Types::ClauseType::Next},
		{"Next*", Types::ClauseType::NextT},
		{"Parent", Types::ClauseType::Parent},
		{"Parent*", Types::ClauseType::ParentT},
		{"Uses", Types::ClauseType::UnknownUses},
	};

	unordered_set<Types::ReferenceType> name_wildcard = {Types::ReferenceType::Name, Types::ReferenceType::Wildcard};
	unordered_set<Types::ReferenceType> statement = {Types::ReferenceType::StatementIndex, Types::ReferenceType::Wildcard,
	                                                 Types::ReferenceType::Synonym};
	unordered_set<Types::ReferenceType> entity = {Types::ReferenceType::Name, Types::ReferenceType::Wildcard,
	                                              Types::ReferenceType::Synonym};
	unordered_set<Types::ReferenceType> statement_entity = {Types::ReferenceType::Name, Types::ReferenceType::StatementIndex,
	                                                        Types::ReferenceType::Wildcard, Types::ReferenceType::Synonym};
	unordered_set<Types::ReferenceType> expression = {Types::ReferenceType::ExactExpression, Types::ReferenceType::SubExpression,
	                                                  Types::ReferenceType::Wildcard};
	unordered_set<Types::ReferenceType> wildcard = {Types::ReferenceType::Wildcard};
	unordered_set<Types::ReferenceType> attribute = {Types::ReferenceType::Attribute, Types::ReferenceType::Name,
	                                                 Types::ReferenceType::StatementIndex};

	unordered_map<Types::ClauseType, vector<unordered_set<Types::ReferenceType>>> syntax_map = {
		{Types::ClauseType::Calls, {entity, entity}},
		{Types::ClauseType::CallsT, {entity, entity}},
		{Types::ClauseType::Follows, {statement, statement}},
		{Types::ClauseType::FollowsT, {statement, statement}},
		{Types::ClauseType::UnknownModifies, {statement_entity, entity}},
		{Types::ClauseType::Next, {statement, statement}},
		{Types::ClauseType::NextT, {statement, statement}},
		{Types::ClauseType::Parent, {statement, statement}},
		{Types::ClauseType::ParentT, {statement, statement}},
		{Types::ClauseType::PatternAssign, {statement, entity, expression}},
		{Types::ClauseType::PatternWhile, {statement, entity, wildcard}},
		{Types::ClauseType::PatternIf, {statement, entity, wildcard, wildcard}},
		{Types::ClauseType::UnknownUses, {statement_entity, entity}},
		{Types::ClauseType::With, {attribute, attribute}}};

	unordered_map<size_t, vector<unordered_set<Types::ReferenceType>>> pattern_syntax_map = {
		{ASSIGN_WHILE_PATTERN_ARGUMENT_COUNT, {entity, name_wildcard}},
		{IF_PATTERN_ARGUMENT_COUNT, {entity, wildcard, wildcard}},
	};

	unordered_map<Types::DesignEntity, Types::ClauseType> pattern_clause_map = {
		{Types::DesignEntity::If, Types::ClauseType::PatternIf},
		{Types::DesignEntity::While, Types::ClauseType::PatternWhile},
	};

	unordered_map<string, Types::AttributeToken> attribute_token_map = {{"stmt#", Types::AttributeToken::StatementIndex},
	                                                                    {"varName", Types::AttributeToken::VariableName},
	                                                                    {"procName", Types::AttributeToken::ProcedureName},
	                                                                    {"value", Types::AttributeToken::Value}};

	unordered_map<pair<Types::DesignEntity, Types::AttributeToken>, Types::AttributeType> attribute_map = {
		{{Types::DesignEntity::Stmt, Types::AttributeToken::StatementIndex}, Types::AttributeType::NumberIdentifier},
		{{Types::DesignEntity::Read, Types::AttributeToken::StatementIndex}, Types::AttributeType::NumberIdentifier},
		{{Types::DesignEntity::Print, Types::AttributeToken::StatementIndex}, Types::AttributeType::NumberIdentifier},
		{{Types::DesignEntity::Call, Types::AttributeToken::StatementIndex}, Types::AttributeType::NumberIdentifier},
		{{Types::DesignEntity::While, Types::AttributeToken::StatementIndex}, Types::AttributeType::NumberIdentifier},
		{{Types::DesignEntity::If, Types::AttributeToken::StatementIndex}, Types::AttributeType::NumberIdentifier},
		{{Types::DesignEntity::Assign, Types::AttributeToken::StatementIndex}, Types::AttributeType::NumberIdentifier},
		{{Types::DesignEntity::Read, Types::AttributeToken::VariableName}, Types::AttributeType::VariableName},
		{{Types::DesignEntity::Print, Types::AttributeToken::VariableName}, Types::AttributeType::VariableName},
		{{Types::DesignEntity::Call, Types::AttributeToken::ProcedureName}, Types::AttributeType::ProcedureName},
		{{Types::DesignEntity::Procedure, Types::AttributeToken::ProcedureName}, Types::AttributeType::NameIdentifier},
		{{Types::DesignEntity::Variable, Types::AttributeToken::VariableName}, Types::AttributeType::NameIdentifier},
		{{Types::DesignEntity::Constant, Types::AttributeToken::Value}, Types::AttributeType::NumberIdentifier}};

	unordered_map<string, Types::DesignEntity> design_entity_map = {
		{"stmt", Types::DesignEntity::Stmt},         {"read", Types::DesignEntity::Read},
		{"print", Types::DesignEntity::Print},       {"call", Types::DesignEntity::Call},
		{"while", Types::DesignEntity::While},       {"if", Types::DesignEntity::If},
		{"assign", Types::DesignEntity::Assign},     {"variable", Types::DesignEntity::Variable},
		{"constant", Types::DesignEntity::Constant}, {"procedure", Types::DesignEntity::Procedure},
	};
};

#endif  // SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H
