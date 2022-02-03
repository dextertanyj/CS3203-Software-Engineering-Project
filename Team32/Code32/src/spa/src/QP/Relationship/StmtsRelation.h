#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_STMTSRELATION_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_STMTSRELATION_H_

#include "Relation.h"

class StmtsRelation : public Relation {
public:
	StmtsRelation(Relationship relationship, QueryStmtRef stmtOne, QueryStmtRef stmtTwo);

private:
	static Relationship validateRelationship(Relationship relationship);
	QueryStmtRef stmtOne;
	QueryStmtRef stmtTwo;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_STMTSRELATION_H_
