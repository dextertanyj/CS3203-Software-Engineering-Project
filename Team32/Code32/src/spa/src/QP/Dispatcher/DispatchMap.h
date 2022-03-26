#ifndef SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H
#define SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H

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

struct QP::Dispatcher::DispatchMap {
	Types::ArgumentDispatchMap dispatch_map = {
		{Types::ClauseType::Calls, QP::Dispatcher::CallsDispatcher<Types::ClauseType::Calls>::dispatcher},
		{Types::ClauseType::CallsT, QP::Dispatcher::CallsDispatcher<Types::ClauseType::CallsT>::dispatcher},
		{Types::ClauseType::Follows, QP::Dispatcher::FollowsNextDispatcher<Types::ClauseType::Follows>::dispatcher},
		{Types::ClauseType::FollowsT, QP::Dispatcher::FollowsNextDispatcher<Types::ClauseType::FollowsT>::dispatcher},
		{Types::ClauseType::UnknownModifies, QP::Dispatcher::ModifiesDispatcher::dispatcher},
		{Types::ClauseType::Next, QP::Dispatcher::FollowsNextDispatcher<Types::ClauseType::Next>::dispatcher},
		{Types::ClauseType::NextT, QP::Dispatcher::FollowsNextDispatcher<Types::ClauseType::NextT>::dispatcher},
		{Types::ClauseType::Parent, QP::Dispatcher::ParentDispatcher<Types::ClauseType::Parent>::dispatcher},
		{Types::ClauseType::ParentT, QP::Dispatcher::ParentDispatcher<Types::ClauseType::ParentT>::dispatcher},
		{Types::ClauseType::PatternAssign, QP::Dispatcher::PatternAssignDispatcher::dispatcher},
		{Types::ClauseType::PatternWhile, QP::Dispatcher::PatternWhileDispatcher::dispatcher},
		{Types::ClauseType::PatternIf, QP::Dispatcher::PatternIfDispatcher::dispatcher},
		{Types::ClauseType::UnknownUses, QP::Dispatcher::UsesDispatcher::dispatcher},
		{Types::ClauseType::With, QP::Dispatcher::WithDispatcher::dispatcher}};

	unordered_map<string, Types::ClauseType> clause_map = {
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

	unordered_map<Types::DesignEntity, Types::ClauseType> pattern_clause_map = {
		{Types::DesignEntity::If, Types::ClauseType::PatternIf},
		{Types::DesignEntity::While, Types::ClauseType::PatternWhile},
	};

	unordered_map<pair<Types::DesignEntity, string>, Types::AttributeType> attribute_map = {
		{{Types::DesignEntity::Stmt, "stmt#"}, Types::AttributeType::NumberIdentifier},
		{{Types::DesignEntity::Read, "stmt#"}, Types::AttributeType::NumberIdentifier},
		{{Types::DesignEntity::Print, "stmt#"}, Types::AttributeType::NumberIdentifier},
		{{Types::DesignEntity::Call, "stmt#"}, Types::AttributeType::NumberIdentifier},
		{{Types::DesignEntity::While, "stmt#"}, Types::AttributeType::NumberIdentifier},
		{{Types::DesignEntity::If, "stmt#"}, Types::AttributeType::NumberIdentifier},
		{{Types::DesignEntity::Assign, "stmt#"}, Types::AttributeType::NumberIdentifier},
		{{Types::DesignEntity::Read, "varName"}, Types::AttributeType::VariableName},
		{{Types::DesignEntity::Print, "varName"}, Types::AttributeType::VariableName},
		{{Types::DesignEntity::Call, "procName"}, Types::AttributeType::ProcedureName},
		{{Types::DesignEntity::Procedure, "procName"}, Types::AttributeType::NameIdentifier},
		{{Types::DesignEntity::Variable, "varName"}, Types::AttributeType::NameIdentifier},
		{{Types::DesignEntity::Constant, "value"}, Types::AttributeType::NumberIdentifier}};

	unordered_map<string, Types::DesignEntity> design_entity_map = {
		{"stmt", Types::DesignEntity::Stmt},         {"read", Types::DesignEntity::Read},
		{"print", Types::DesignEntity::Print},       {"call", Types::DesignEntity::Call},
		{"while", Types::DesignEntity::While},       {"if", Types::DesignEntity::If},
		{"assign", Types::DesignEntity::Assign},     {"variable", Types::DesignEntity::Variable},
		{"constant", Types::DesignEntity::Constant}, {"procedure", Types::DesignEntity::Procedure},
	};
};

#endif  // SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H
