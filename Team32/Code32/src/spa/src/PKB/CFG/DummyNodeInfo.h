#ifndef SPA_DUMMYNODEINFO_H
#define SPA_DUMMYNODEINFO_H

#include "Common/TypeDefs.h"
#include "NodeInfo.h"
#include "PKB/PKB.h"

class PKB::DummyNodeInfo : public PKB::NodeInfo {
public:
	DummyNodeInfo();
	bool isDummy() const override;
};

#endif  // SPA_DUMMYNODEINFO_H
