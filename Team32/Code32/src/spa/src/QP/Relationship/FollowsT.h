#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H_

#include "QP/Relationship/Follows.h"
#include "QP/QueryTypeDefs.h"

class FollowsT : public Follows {
public:
	using Follows::Follows;

	QueryResult execute(PKB& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) override;

private:
	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H_
