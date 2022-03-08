#ifndef SPA_NODEINFO_H
#define SPA_NODEINFO_H

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"

class PKB::NodeInfo {
public:
	NodeInfo() = default;
	NodeInfo(shared_ptr<StmtInfo> info);
	[[nodiscard]] StmtRef getIdentifier() const;
	[[nodiscard]] StmtType getType() const;
	void setUniqueIndex(StmtRef unique_index);
	StmtRef getUniqueIndex() const;

private:
	StmtRef index;
	StmtType type;
	StmtRef uniqueIndex;
};

#endif  // SPA_NODEINFO_H
