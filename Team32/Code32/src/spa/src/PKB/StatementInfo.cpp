#include "PKB/StatementInfo.h"

#include <cassert>

StmtRef PKB::StatementInfo::getIdentifier() const { return index; }

StmtType PKB::StatementInfo::getType() const { return type; }

PKB::StatementInfo::StatementInfo(StmtRef index, StmtType type) : index(index), type(type) { assert(this->index > 0); }
