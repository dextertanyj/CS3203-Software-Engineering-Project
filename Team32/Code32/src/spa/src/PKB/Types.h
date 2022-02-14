#ifndef SPA_TYPES_H
#define SPA_TYPES_H

#include <unordered_map>

#include "Common/TypeDefs.h"

typedef unordered_map<StmtRef, shared_ptr<StmtInfo>> StatementStore;

#endif  // SPA_TYPES_H
