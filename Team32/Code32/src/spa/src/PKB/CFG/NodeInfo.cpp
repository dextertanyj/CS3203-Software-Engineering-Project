#include "NodeInfo.h"

PKB::NodeInfo::NodeInfo(StmtRef identifier, StmtType type) {
	this->index = identifier;
	this->type = type;
}

StmtRef PKB::NodeInfo::getIdentifier() const { return this->index; }

StmtType PKB::NodeInfo::getType() const { return this->type; }

StmtRef PKB::NodeInfo::getUniqueIndex() const { return this->uniqueIndex; }

void PKB::NodeInfo::setUniqueIndex(StmtRef unique_index) { this->uniqueIndex = unique_index; }

