#ifndef SPA_SRC_QP_QUERYRESULT_H
#define SPA_SRC_QP_QUERYRESULT_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Common/TypeDefs.h"
#include "QP/QP.h"
#include "QP/QueryTypes.h"
#include "QP/ResultTable.h"

using QP::Types::ResultColumn;
using QP::Types::ResultRow;
using QP::Types::ResultTable;

class QP::QueryResult {
public:
	QueryResult();
	explicit QueryResult(bool result);
	[[nodiscard]] bool getResult() const;
	ResultTable getTable();
	unordered_set<string> getSynonymsStored();
	ResultColumn getSynonymResult(const string& synonym);
	size_t getNumberOfRows();
	void addColumn(const string& synonym, const ResultColumn& column);
	void joinResult(QueryResult& query_result);
	void filterBySelect(const QP::Types::DeclarationList& select_list);

private:
	bool result;
	ResultTable table;
};

#endif  // SPA_SRC_QP_QUERYRESULT_H
