#ifndef SPA_MOCKUTILITIES_H
#define SPA_MOCKUTILITIES_H

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"

class MockUtilities {
public:
    static shared_ptr<StmtInfo> createStmtInfo(StmtRef stmtNo, StmtType type);
	static PKB::Storage generateFollowsTestPKB();
	static PKB::Storage generateParentTestPKB();
    static PKB::Storage generateUsesTestPKB();
    static PKB::Storage generateModifyTestPKB();
    static PKB::StatementStore generateStatementStore();
};


#endif //SPA_MOCKUTILITIES_H
