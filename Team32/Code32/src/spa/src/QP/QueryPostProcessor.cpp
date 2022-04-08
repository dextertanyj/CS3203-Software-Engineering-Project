#include "QP/QueryPostProcessor.h"

#include <algorithm>
#include <functional>
#include <iterator>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "Common/Hash.h"
#include "QP/ClauseArgument.h"
#include "QP/Executor/AttributeExecutor.h"
#include "QP/Types.h"

using namespace std;
#define TUPLE_SEPERATOR " "
#define TRUE "TRUE"
#define FALSE "FALSE"

using QP::Types::DeclarationList;

QP::QueryPostProcessor::QueryPostProcessor(const StorageAdapter& store) : store(store) {}

vector<string> QP::QueryPostProcessor::processResult(QueryProperties& query_properties, QueryResult& query_result) const {
	DeclarationList select_list = query_properties.getSelectSynonymList();
	if (select_list.empty()) {
		return processBooleanResult(query_result);
	}

	return processStandardResult(query_properties, query_result);
}

template <QP::Types::ClauseType T>
static string statementToVariable(const QP::StorageAdapter& store, const string& value) {
	return QP::Executor::AttributeExecutor::statementToVariable<T>(store, stoul(value));
}

static string callToProcedure(const QP::StorageAdapter& store, const string& value) {
	return QP::Executor::AttributeExecutor::callToProcedure(store, stoul(value));
}

static const unordered_map<QP::Types::DispatchAttributeKey, function<string(const QP::StorageAdapter&, const string&)>> transform_map = {
	{{QP::Types::DesignEntity::Read, QP::Types::AttributeType::VariableName}, statementToVariable<QP::Types::ClauseType::ModifiesS>},
	{{QP::Types::DesignEntity::Call, QP::Types::AttributeType::ProcedureName}, callToProcedure},
	{{QP::Types::DesignEntity::Print, QP::Types::AttributeType::VariableName}, statementToVariable<QP::Types::ClauseType::UsesS>}};

static inline string applyTransform(const QP::StorageAdapter& store, const QP::ClauseArgument& argument, string result) {
	if (argument.getType() == QP::Types::ArgumentType::Synonym) {
		return result;
	}
	auto iter = transform_map.find({argument.getSynonymType(), argument.getAttributeType()});
	if (iter == transform_map.end()) {
		return result;
	}
	return iter->second(store, result);
}

vector<string> QP::QueryPostProcessor::processStandardResult(QueryProperties& query_properties, QueryResult& query_result) const {
	if (!query_result.getResult()) {
		return {};
	}

	Types::SelectList select_list = query_properties.getSelectList();
	vector<string> synonyms;
	synonyms.reserve(select_list.size());
	transform(select_list.begin(), select_list.end(), back_inserter(synonyms),
	          [](const ClauseArgument& argument) { return argument.getSynonymSymbol(); });

	unordered_set<string> result_set;
	size_t table_size = query_result.getNumberOfRows();
	for (size_t i = 0; i < table_size; i++) {
		ResultRow row = query_result.getRowWithOrder(synonyms, i);
		string row_string;

		for (size_t j = 0; j < select_list.size(); j++) {
			string value = applyTransform(store, select_list.at(j), row.at(j));
			row_string.append(value);
			row_string.append(TUPLE_SEPERATOR);
		}

		// Remove additional separator.
		row_string.pop_back();

		result_set.emplace(row_string);
	}

	vector<string> results;
	results.reserve(result_set.size());
	results.insert(results.end(), result_set.begin(), result_set.end());

	return results;
}

vector<string> QP::QueryPostProcessor::processBooleanResult(QueryResult& query_result) {
	string result = query_result.getResult() ? TRUE : FALSE;
	return {result};
}
