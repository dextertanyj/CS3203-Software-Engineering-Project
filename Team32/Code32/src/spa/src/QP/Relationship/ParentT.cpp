#include "ParentT.h"

ParentT::ParentT(QueryStmtRef parentStmt, QueryStmtRef childStmt)
{
}

QueryStmtRef ParentT::getParentStmt()
{
	return QueryStmtRef();
}

QueryStmtRef ParentT::getChildStmt()
{
	return QueryStmtRef();
}

QueryResult ParentT::execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map)
{
	return QueryResult();
}

vector<string> ParentT::getDeclarationSymbols()
{
	return vector<string>();
}

QueryResult ParentT::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map)
{
	return QueryResult();
}

QueryResult ParentT::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map)
{
	return QueryResult();
}
