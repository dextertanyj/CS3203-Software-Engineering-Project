#ifndef SPA_MOCKUTILITIES_H
#define SPA_MOCKUTILITIES_H

#include "Common/TypeDefs.h"

class MockUtilities {
public:
    static shared_ptr<StmtInfo> createStmtInfo(int stmtNo, StmtType type);
};


#endif //SPA_MOCKUTILITIES_H
