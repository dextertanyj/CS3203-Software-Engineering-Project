#ifndef SPA_SRC_QP_RESULTTABLE_H
#define SPA_SRC_QP_RESULTTABLE_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Common/Hash.h"
#include "QP/QP.h"
#include "QP/Types.h"

using QP::Types::ResultColumn;
using QP::Types::ResultRow;

class QP::ResultTable {
public:
	ResultTable();
	explicit ResultTable(std::vector<std::string> synonyms_stored);
	[[nodiscard]] size_t getNumberOfRows() const;
	[[nodiscard]] size_t getNumberOfColumns() const;
	[[nodiscard]] std::vector<ResultRow> getTable() const;
	[[nodiscard]] std::vector<std::string> getSynonymsStored() const;
	[[nodiscard]] std::unordered_map<std::string, size_t> getSynonymsStoredMap() const;
	[[nodiscard]] ResultColumn getColumn(const std::string& synonym) const;
	[[nodiscard]] ResultRow getRow(size_t row_number) const;
	[[nodiscard]] ResultRow getRowWithOrder(const std::vector<std::string>& synonyms, size_t row_number) const;
	void insertRow(const ResultRow& row);
	void removeRow(size_t row_number);
	ResultTable filterBySelect(const QP::Types::DeclarationList& select_list);
	bool containsRow(const ResultRow& row);
	static ResultTable joinTables(const ResultTable& table_one, const ResultTable& table_two);

private:
	std::unordered_map<std::string, size_t> synonyms_to_index_map;
	std::vector<std::string> synonyms_stored;
	std::vector<ResultRow> table;

	static ResultTable intersectTables(ResultTable superset_table, const ResultTable& subset_table);
	static ResultTable hashJoinTables(const ResultTable& table_one, const ResultTable& table_two);
	static ResultTable loopJoinTables(const ResultTable& table_one, const ResultTable& table_two);
	static std::unordered_multimap<ResultRow, size_t> buildHashTable(ResultTable& table, const std::vector<std::string>& key_synonyms);
};

#endif  // SPA_SRC_QP_RESULTTABLE_H
