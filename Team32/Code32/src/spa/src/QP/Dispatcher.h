#ifndef SPA_SRC_QP_DISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_H

#include "QP/QP.h"
#include "QP/QueryTypes.h"
#include "QP/Relationship/Calls.h"
#include "QP/Relationship/Modifies.h"
#include "QP/Relationship/Pattern.h"
#include "QP/Relationship/CallsT.h"

struct QP::Dispatcher {
	Types::ArgumentDispatchMap dispatch_map = {
		{Types::ClauseType::Call, QP::Relationship::Calls::dispatcher},
		{Types::ClauseType::CallT, QP::Relationship::CallsT::dispatcher},
		{Types::ClauseType::UnknownModifies, QP::Relationship::Modifies::dispatcher},
		{Types::ClauseType::PatternAssign, QP::Relationship::Pattern::dispatcher}
	};
};

#endif  // SPA_SRC_QP_DISPATCHER_H
