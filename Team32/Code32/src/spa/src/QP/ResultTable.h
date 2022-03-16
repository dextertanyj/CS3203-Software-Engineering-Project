#ifndef SPA_SRC_QP_RESULTTABLE_H
#define SPA_SRC_QP_RESULTTABLE_H

#include "QP/QP.h"
#include "QP/QueryTypes.h"

using QP::Types::ResultRow;
using QP::Types::ResultColumn;

class QP::Types::ResultTable {
public:
	ResultTable();
	ResultTable(unordered_map<string, vector<string>> table);
	size_t getTableSize();
	ResultColumn getColumn(const string& synonym);
	bool contains(const ResultRow& row);
	bool isRowMatch(const ResultRow& row, size_t row_number);
	void removeRow(size_t row_number);
	void removeDuplicateRows();
	void insertColumn(const string& synonym, const ResultColumn& column);
	ResultTable getSubTableWithRow(const ResultRow& row);

private:
	unordered_map<string, vector<string>> table;
};

#endif  // SPA_SRC_QP_RESULTTABLE_H
