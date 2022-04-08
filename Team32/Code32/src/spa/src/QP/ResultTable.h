#ifndef SPA_SRC_QP_RESULTTABLE_H
#define SPA_SRC_QP_RESULTTABLE_H

#include <string>
#include <unordered_map>
#include <vector>

#include "Common/Hash.h"
#include "QP/QP.h"
#include "QP/Types.h"

class QP::ResultTable {
public:
	ResultTable();
	explicit ResultTable(std::vector<std::string> synonyms_stored);

	void insertRow(const Types::ResultRow& row);
	ResultTable filterBySelect(const Types::DeclarationList& select_list);

	[[nodiscard]] size_t getNumberOfRows() const;
	[[nodiscard]] size_t getNumberOfColumns() const;
	[[nodiscard]] std::vector<std::string> getSynonymsStored() const;
	[[nodiscard]] std::unordered_map<std::string, size_t> getSynonymsStoredMap() const;
	[[nodiscard]] bool containsRow(const Types::ResultRow& row) const;
	[[nodiscard]] Types::ResultRow getRow(size_t row_number) const;
	[[nodiscard]] Types::ResultRow getRowWithOrder(const std::vector<std::string>& synonyms, size_t row_number) const;
	[[nodiscard]] Types::ResultColumn getColumn(const std::string& synonym) const;
	[[nodiscard]] std::vector<Types::ResultRow> getTable() const;

	static ResultTable joinTables(const ResultTable& table_one, const ResultTable& table_two);

private:
	std::unordered_map<std::string, size_t> synonyms_to_index_map;
	std::vector<std::string> synonyms_stored;
	std::vector<Types::ResultRow> table;

	static ResultTable intersectTables(const ResultTable& superset_table, const ResultTable& subset_table);
	static ResultTable hashJoinTables(const ResultTable& table_one, const ResultTable& table_two);
	static ResultTable loopJoinTables(const ResultTable& table_one, const ResultTable& table_two);
	static std::unordered_multimap<Types::ResultRow, size_t> buildHashTable(ResultTable& table,
	                                                                        const std::vector<std::string>& key_synonyms);
};

#endif  // SPA_SRC_QP_RESULTTABLE_H
