#include "StatementInfo.h"

PKB::StatementInfo::StatementInfo(StmtRef index, StmtType type) : index(index), type(type) {
	if (this->index <= 0) {
		throw invalid_argument("Invalid statement index.");
	}
}

StmtRef PKB::StatementInfo::getIdentifier() const { return index; }

StmtType PKB::StatementInfo::getType() const { return type; }
