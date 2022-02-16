#ifndef SPA_MOCKUTILITIES_H
#define SPA_MOCKUTILITIES_H

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"

class MockUtilities {
public:
    static shared_ptr<StmtInfo> createStmtInfo(int stmtNo, StmtType type);
	static PKB generateFollowsTestPKB();
	static PKB generateParentTestPKB();
    static StatementStore generateStatementStore();
};


#endif //SPA_MOCKUTILITIES_H
