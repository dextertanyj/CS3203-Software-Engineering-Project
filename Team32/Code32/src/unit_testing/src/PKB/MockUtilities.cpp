#include "MockUtilities.h"

shared_ptr<StmtInfo> MockUtilities::createStmtInfo(int stmtNo, StmtType type) {
    StmtInfo s = {stmtNo, type};
    shared_ptr<StmtInfo> stmt_info = make_shared<StmtInfo>(s);
    return stmt_info;
}
