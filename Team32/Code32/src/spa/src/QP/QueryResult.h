#ifndef SPA_SRC_QP_QUERYRESULT_H
#define SPA_SRC_QP_QUERYRESULT_H

#include <string>
#include <vector>

#include "Common/TypeDefs.h"
#include "QP/QP.h"
#include "QP/Types.h"
#include "QP/ResultTable.h"
#include "QP/Types.h"

using QP::Types::ResultColumn;
using QP::Types::ResultRow;
using QP::Types::ResultTable;

class QP::QueryResult {
public:
	QueryResult();
	explicit QueryResult(bool result);
	explicit QueryResult(vector<string> synonyms);
	[[nodiscard]] bool getResult() const;
	ResultTable getTable();
	ResultColumn getSynonymResult(const string& synonym);
	ResultRow getRowWithOrder(const vector<string>& synonyms, size_t row_number);
	size_t getNumberOfRows();
	void addRow(const ResultRow& row);
	void joinResult(QueryResult& query_result);
	void filterBySelect(const QP::Types::DeclarationList& select_list);

private:
	bool result;
	ResultTable table;
};

#endif  // SPA_SRC_QP_QUERYRESULT_H
