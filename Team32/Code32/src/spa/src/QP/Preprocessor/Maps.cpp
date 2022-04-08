#include "QP/Preprocessor/Maps.h"

#include <set>

#include "Common/Hash.h"
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

using namespace QP::Dispatcher;

namespace QP::Preprocessor::Maps {

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

const std::unordered_map<std::string, ClauseType> such_that_clause_map = {
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

const std::unordered_map<DesignEntity, ClauseType> pattern_clause_map = {
	{DesignEntity::Assign, ClauseType::PatternAssign},
	{DesignEntity::If, ClauseType::PatternIf},
	{DesignEntity::While, ClauseType::PatternWhile},
};

const std::unordered_map<std::string, AttributeToken> attribute_token_map = {{"stmt#", AttributeToken::StatementIndex},
                                                                             {"varName", AttributeToken::VariableName},
                                                                             {"procName", AttributeToken::ProcedureName},
                                                                             {"value", AttributeToken::Value}};

const std::unordered_map<std::pair<DesignEntity, AttributeToken>, AttributeType> attribute_map = {
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

const std::unordered_map<std::string, DesignEntity> design_entity_map = {
	{"stmt", DesignEntity::Stmt},           {"read", DesignEntity::Read},         {"print", DesignEntity::Print},
	{"call", DesignEntity::Call},           {"while", DesignEntity::While},       {"if", DesignEntity::If},
	{"assign", DesignEntity::Assign},       {"variable", DesignEntity::Variable}, {"constant", DesignEntity::Constant},
	{"procedure", DesignEntity::Procedure},
};

}