#ifndef SPA_SRC_PKB_PROCEDUREINFO_H
#define SPA_SRC_PKB_PROCEDUREINFO_H

#include <cassert>
#include <memory>
#include <vector>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"

class PKB::ProcedureInfo {
public:
	[[nodiscard]] ProcRef getIdentifier() const;
	[[nodiscard]] std::vector<std::shared_ptr<StmtInfo>> getStatements() const;

private:
	ProcedureInfo(ProcRef name, std::vector<std::shared_ptr<StmtInfo>> statements);
	ProcRef name;
	std::vector<std::shared_ptr<StmtInfo>> statements;

	friend class PKB::InfoStore<ProcRef, std::vector<std::shared_ptr<StmtInfo>>, ProcedureInfo>;
};

#endif  // SPA_SRC_PKB_PROCEDUREINFO_H
