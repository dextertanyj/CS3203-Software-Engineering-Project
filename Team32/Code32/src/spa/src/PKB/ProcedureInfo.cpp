#include "PKB/ProcedureInfo.h"

#include <cassert>
#include <utility>

using namespace std;

ProcRef PKB::ProcedureInfo::getIdentifier() const { return name; }

vector<StmtInfoPtr> PKB::ProcedureInfo::getStatements() const { return statements; }

PKB::ProcedureInfo::ProcedureInfo(ProcRef name, vector<StmtInfoPtr> statements) : name(move(name)), statements(move(statements)) {
	assert(!this->name.empty());
}
