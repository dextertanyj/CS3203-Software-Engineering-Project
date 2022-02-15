#include "FollowsT.h"

FollowsT::FollowsT(QueryStmtRef leftStmt, QueryStmtRef rightStmt)
{
}

QueryStmtRef FollowsT::getLeftStmt()
{
	return QueryStmtRef();
}

QueryStmtRef FollowsT::getRightStmt()
{
	return QueryStmtRef();
}

QueryResult FollowsT::execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map)
{
	return QueryResult();
}

vector<string> FollowsT::getDeclarationSymbols()
{
	return vector<string>();
}

QueryResult FollowsT::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map)
{
	return QueryResult();
}

QueryResult FollowsT::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map)
{
	return QueryResult();
}
