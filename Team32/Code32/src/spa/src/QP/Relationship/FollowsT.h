#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H_

#include "QP/Relationship/Follows.h"
#include "QP/QueryTypeDefs.h"

class FollowsT : public Follows {
public:
	using Follows::Follows;

	bool getIsStar();
	QueryResult execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map);

private:
	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H_
