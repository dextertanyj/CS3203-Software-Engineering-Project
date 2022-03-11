#ifndef SPA_SRC_QP_DISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_H

#include "QP/QP.h"
#include "QP/QueryTypes.h"
#include "QP/Relationship/CallsHandler.tpp"
#include "QP/Relationship/FollowsDispatcher.tpp"
#include "QP/Relationship/Modifies.h"
#include "QP/Relationship/ParentDispatcher.tpp"
#include "QP/Relationship/Pattern.h"
#include "QP/Relationship/Uses.h"

struct QP::Dispatcher {
	Types::ArgumentDispatchMap dispatch_map = {
		{Types::ClauseType::Call, QP::Relationship::CallsHandler<Types::ClauseType::Call>::dispatcher},
		{Types::ClauseType::CallT, QP::Relationship::CallsHandler<Types::ClauseType::CallT>::dispatcher},
		{Types::ClauseType::Follows, QP::Relationship::FollowsDispatcher<Types::ClauseType::Follows>::dispatcher},
		{Types::ClauseType::FollowsT, QP::Relationship::FollowsDispatcher<Types::ClauseType::FollowsT>::dispatcher},
		{Types::ClauseType::UnknownModifies, QP::Relationship::Modifies::dispatcher},
		{Types::ClauseType::Parent, QP::Relationship::ParentDispatcher<Types::ClauseType::Parent>::dispatcher},
		{Types::ClauseType::ParentT, QP::Relationship::ParentDispatcher<Types::ClauseType::ParentT>::dispatcher},
		{Types::ClauseType::PatternAssign, QP::Relationship::Pattern::dispatcher},
		{Types::ClauseType::UnknownUses, QP::Relationship::Uses::dispatcher}};
	unordered_map<string, Types::ClauseType> clause_map = {
		{"Calls", Types::ClauseType::Call},
		{"Calls*", Types::ClauseType::CallT},
		{"Follows", Types::ClauseType::Follows},
		{"Follows*", Types::ClauseType::FollowsT},
		{"Modifies", Types::ClauseType::UnknownModifies},
		{"Parent", Types::ClauseType::Parent},
		{"Parent*", Types::ClauseType::ParentT},
		{"Uses", Types::ClauseType::UnknownUses},
	};
};

#endif  // SPA_SRC_QP_DISPATCHER_H
