#ifndef SPA_SRC_QP_DISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_H

#include "QP/Dispatcher/CallsDispatcher.tpp"
#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Dispatcher/FollowsDispatcher.tpp"
#include "QP/Dispatcher/ModifiesDispatcher.h"
#include "QP/Dispatcher/ParentDispatcher.tpp"
#include "QP/Dispatcher/PatternAssignDispatcher.h"
#include "QP/Dispatcher/UsesDispatcher.h"
#include "QP/QP.h"
#include "QP/QueryTypes.h"

struct QP::Dispatcher::DispatchMap {
	Types::ArgumentDispatchMap dispatch_map = {
		{Types::ClauseType::Calls, QP::Dispatcher::CallsDispatcher<Types::ClauseType::Calls>::dispatcher},
		{Types::ClauseType::CallsT, QP::Dispatcher::CallsDispatcher<Types::ClauseType::CallsT>::dispatcher},
		{Types::ClauseType::Follows, QP::Dispatcher::FollowsDispatcher<Types::ClauseType::Follows>::dispatcher},
		{Types::ClauseType::FollowsT, QP::Dispatcher::FollowsDispatcher<Types::ClauseType::FollowsT>::dispatcher},
		{Types::ClauseType::UnknownModifies, QP::Dispatcher::ModifiesDispatcher::dispatcher},
		{Types::ClauseType::Parent, QP::Dispatcher::ParentDispatcher<Types::ClauseType::Parent>::dispatcher},
		{Types::ClauseType::ParentT, QP::Dispatcher::ParentDispatcher<Types::ClauseType::ParentT>::dispatcher},
		{Types::ClauseType::PatternAssign, QP::Dispatcher::PatternAssignDispatcher::dispatcher},
		{Types::ClauseType::UnknownUses, QP::Dispatcher::UsesDispatcher::dispatcher}};
	unordered_map<string, Types::ClauseType> clause_map = {
		{"Calls", Types::ClauseType::Calls},
		{"Calls*", Types::ClauseType::CallsT},
		{"Follows", Types::ClauseType::Follows},
		{"Follows*", Types::ClauseType::FollowsT},
		{"Modifies", Types::ClauseType::UnknownModifies},
		{"Parent", Types::ClauseType::Parent},
		{"Parent*", Types::ClauseType::ParentT},
		{"Uses", Types::ClauseType::UnknownUses},
	};
};

#endif  // SPA_SRC_QP_DISPATCHER_H
