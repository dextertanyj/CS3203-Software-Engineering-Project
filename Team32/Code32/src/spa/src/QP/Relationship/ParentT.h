#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENTT_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENTT_H_

#include "QP/Relationship/Parent.h"
#include "QP/QueryTypeDefs.h"

class ParentT : public Parent {
public:
	using Parent::Parent;

	QueryResult execute(PKB& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) override;

private:
	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENTT_H_
