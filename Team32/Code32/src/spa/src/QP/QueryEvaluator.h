#pragma once

class QueryProperties;
class QueryResult;

class QueryEvaluator {
public:
	static QueryResult* executeQuery(QueryProperties* queryProperties);
};
