#ifndef SPA_SRC_PKB_STATEMENTINFO_H
#define SPA_SRC_PKB_STATEMENTINFO_H

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"

class PKB::StatementInfo : public StatementInfoInterface {
public:
	[[nodiscard]] StmtRef getIdentifier() const override;
	[[nodiscard]] StmtType getType() const override;

private:
	StmtRef index;
	StmtType type;

	friend class PKB::InfoStore<StmtRef, StmtType, StatementInfo>;
	;

protected:
	StatementInfo(StmtRef index, StmtType type);
};

#endif  // SPA_SRC_PKB_STATEMENTINFO_H
