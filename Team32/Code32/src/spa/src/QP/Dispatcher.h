#ifndef SPA_SRC_QP_DISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_H

#include "QP/QP.h"
#include "QP/QueryTypes.h"
#include "QP/Relationship/Calls.h"
#include "QP/Relationship/CallsT.h"

struct QP::Dispatcher {
	Types::ArgumentDispatchMap dispatch_map = {
		{Types::ClauseType::Call, QP::Relationship::Calls::dispatcher},
		{Types::ClauseType::CallT, QP::Relationship::CallsT::dispatcher},
	};
};

#endif  // SPA_SRC_QP_DISPATCHER_H
