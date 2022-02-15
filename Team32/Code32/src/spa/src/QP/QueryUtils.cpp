#include "QP/QueryUtils.h"

QueryUtils::designEntToStmtType = {
	{ DesignEntity::read, StmtType::Read },
	{ DesignEntity::print, StmtType::Print },
	{ DesignEntity::call, StmtType::Call },
	{ DesignEntity::while_, StmtType::WhileStmt },
	{ DesignEntity::if_, StmtType::IfStmt },
	{ DesignEntity::assign, StmtType::Assign },
};
