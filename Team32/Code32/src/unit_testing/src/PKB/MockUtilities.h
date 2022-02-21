#ifndef SPA_MOCKUTILITIES_H
#define SPA_MOCKUTILITIES_H

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"

class MockUtilities {
public:
    static shared_ptr<StmtInfo> createStmtInfo(StmtRef stmtNo, StmtType type);
	static Storage generateFollowsTestPKB();
	static Storage generateParentTestPKB();
    static Storage generateUsesTestPKB();
    static Storage generateModifyTestPKB();
    static StatementStore generateStatementStore();
};


#endif //SPA_MOCKUTILITIES_H
