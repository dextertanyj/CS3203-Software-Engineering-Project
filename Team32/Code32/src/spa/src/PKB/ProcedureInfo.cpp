#include "PKB/ProcedureInfo.h"

using namespace std;

PKB::ProcedureInfo::ProcedureInfo(ProcRef name, vector<shared_ptr<StmtInfo>> statements)
	: name(move(name)), statements(move(statements)) {
	assert(!this->name.empty());
}

ProcRef PKB::ProcedureInfo::getIdentifier() const { return name; }

vector<shared_ptr<StmtInfo>> PKB::ProcedureInfo::getStatements() const { return statements; }
