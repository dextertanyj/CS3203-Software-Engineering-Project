#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENTT_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENTT_H_

#include "QP/Relationship/Parent.h"
#include "QP/QueryTypeDefs.h"

class ParentT : public Parent {
public:
	using Parent::Parent;

	QueryResult execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map);
	bool getIsStar();

private:
	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_PARENTT_H_
