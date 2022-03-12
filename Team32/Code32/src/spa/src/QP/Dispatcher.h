#ifndef SPA_SRC_QP_DISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_H

#include "QP/QP.h"
#include "QP/QueryTypes.h"
#include "QP/Relationship/Calls.h"
#include "QP/Relationship/CallsT.h"
#include "QP/Relationship/Follows.h"
#include "QP/Relationship/FollowsT.h"
#include "QP/Relationship/Modifies.h"
#include "QP/Relationship/Parent.h"
#include "QP/Relationship/ParentT.h"
#include "QP/Relationship/Pattern.h"
#include "QP/Relationship/Uses.h"

struct QP::Dispatcher {
	Types::ArgumentDispatchMap dispatch_map = {{Types::ClauseType::Call, QP::Relationship::Calls::dispatcher},
	                                           {Types::ClauseType::CallT, QP::Relationship::CallsT::dispatcher},
	                                           {Types::ClauseType::Follows, QP::Relationship::Follows::dispatcher},
	                                           {Types::ClauseType::FollowsT, QP::Relationship::FollowsT::dispatcher},
	                                           {Types::ClauseType::UnknownModifies, QP::Relationship::Modifies::dispatcher},
	                                           {Types::ClauseType::Parent, QP::Relationship::Parent::dispatcher},
	                                           {Types::ClauseType::ParentT, QP::Relationship::ParentT::dispatcher},
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
