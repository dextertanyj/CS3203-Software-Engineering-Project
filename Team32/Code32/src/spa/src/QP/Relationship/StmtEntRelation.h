#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_STMTENTRELATION_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_STMTENTRELATION_H_

#include "Relation.h"

class StmtEntRelation : public Relation {
public:
	StmtEntRelation(Relationship relationship, QueryStmtRef stmt, QueryEntRef ent);

private:
	static Relationship validateRelationship(Relationship relationship);
	QueryStmtRef stmt;
	QueryEntRef ent;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_STMTENTRELATION_H_
