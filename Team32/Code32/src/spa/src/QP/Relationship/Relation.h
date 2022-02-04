#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_

#include "QP/QueryResult.h"

class Relation {
public:
	virtual QueryResult execute() = 0;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_
