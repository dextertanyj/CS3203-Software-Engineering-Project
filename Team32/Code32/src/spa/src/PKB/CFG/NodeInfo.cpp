#include "NodeInfo.h"

PKB::NodeInfo::NodeInfo(shared_ptr<StmtInfo> info) {
	this->index = info->getIdentifier();
	this->type = info->getType();
}

StmtRef PKB::NodeInfo::getIdentifier() const { return this->index; }

StmtType PKB::NodeInfo::getType() const { return this->type; }

StmtRef PKB::NodeInfo::getUniqueIndex() const { return this->uniqueIndex; }

void PKB::NodeInfo::setUniqueIndex(StmtRef unique_index) { this->uniqueIndex = unique_index; }
