#include "PKB/ProcedureInfo.h"

PKB::ProcedureInfo::ProcedureInfo(ProcRef name, vector<shared_ptr<StmtInfo>> statements)
	: name(std::move(name)), statements(std::move(statements)) {}

ProcRef PKB::ProcedureInfo::getIdentifier() const { return name; }

vector<shared_ptr<StmtInfo>> PKB::ProcedureInfo::getStatements() const { return statements; }
