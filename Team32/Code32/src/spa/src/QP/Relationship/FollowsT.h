#ifndef SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H
#define SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Follows.h"

class QP::Relationship::FollowsT : public Follows {
public:
	using Follows::Follows;

	QueryResult execute(PKB::Storage& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) override;

private:
	QueryResult executeTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H
