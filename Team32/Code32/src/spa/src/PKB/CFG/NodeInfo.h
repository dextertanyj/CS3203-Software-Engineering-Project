#ifndef SPA_NODEINFO_H
#define SPA_NODEINFO_H

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"

class PKB::NodeInfo {
public:
	NodeInfo() = default;
	NodeInfo(StmtRef identifier, StmtType type);
	[[nodiscard]] StmtRef getIdentifier() const;
	[[nodiscard]] StmtType getType() const;
	void setUniqueIndex(size_t unique_index);
	StmtRef getUniqueIndex() const;
	virtual bool isDummy() const;

private:
	StmtRef index;
	StmtType type;
	size_t uniqueIndex;
};

#endif  // SPA_NODEINFO_H
