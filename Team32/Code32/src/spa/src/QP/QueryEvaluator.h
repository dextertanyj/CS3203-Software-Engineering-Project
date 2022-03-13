#ifndef SPA_SRC_QP_QUERYEVALUATOR_H
#define SPA_SRC_QP_QUERYEVALUATOR_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "PKB/Storage.h"
#include "QP/QP.h"
#include "QP/QueryGraph.h"
#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"

using namespace std;
using QP::Types::ClauseList;
using QP::Types::ConnectedSynonyms;
using QP::Types::Declaration;
using QP::Types::DeclarationList;

class QP::QueryEvaluator {
public:
	explicit QueryEvaluator(PKB::StorageAccessInterface& pkb);
	QueryResult executeQuery(QueryProperties& query_properties);
	static vector<pair<ClauseList, DeclarationList>> splitClauses(QueryProperties& query_properties, ConnectedSynonyms& connected_synonyms);

private:
	PKB::StorageAccessInterface& pkb;
	QueryResult executeGroupWithSelected(ClauseList& clauses, DeclarationList& select_list);
	QueryResult executeGroupWithoutSelected(ClauseList& clauses, DeclarationList& select_list);
	QueryResult executeTrivialGroup(ClauseList& clauses);
	QueryResult executeNonTrivialGroup(ClauseList& clauses, DeclarationList& select_list);
	QueryResult executeNoClauses(const Declaration& select);
	QueryResult getSpecificStmtType(const Declaration& declaration);
	QueryResult getConstants(const string& symbol);
	QueryResult getVariables(const string& symbol);
	QueryResult getProcedures(const string& symbol);
	static QueryGraph buildGraph(QueryProperties& query_properties);
};

#endif  // SPA_SRC_QP_QUERYEVALUATOR_H
