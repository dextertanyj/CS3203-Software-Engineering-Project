#include "StatementInfo.h"

#include <cassert>

PKB::StatementInfo::StatementInfo(StmtRef index, StmtType type) : index(index), type(type) { assert(this->index > 0); }

StmtRef PKB::StatementInfo::getIdentifier() const { return index; }

StmtType PKB::StatementInfo::getType() const { return type; }
