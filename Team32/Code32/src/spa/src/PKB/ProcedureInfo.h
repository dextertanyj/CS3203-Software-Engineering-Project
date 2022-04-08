#ifndef SPA_SRC_PKB_PROCEDUREINFO_H
#define SPA_SRC_PKB_PROCEDUREINFO_H

#include <vector>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"

class PKB::ProcedureInfo {
public:
	[[nodiscard]] ProcRef getIdentifier() const;
	[[nodiscard]] std::vector<StmtInfoPtr> getStatements() const;

private:
	ProcRef name;
	std::vector<StmtInfoPtr> statements;

	ProcedureInfo(ProcRef name, std::vector<StmtInfoPtr> statements);

	friend class PKB::InfoStore<ProcRef, std::vector<StmtInfoPtr>, ProcedureInfo>;
};

#endif  // SPA_SRC_PKB_PROCEDUREINFO_H
