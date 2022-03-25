#include "QP/QueryPostProcessor.h"

#include <algorithm>
#include <string>

#include "QP/Executor/AttributeExecutor.h"
#include "QP/Types.h"

#define TUPLE_SEPERATOR " "
#define TRUE "TRUE"
#define FALSE "FALSE"

using namespace std;
using QP::Types::Declaration;
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
string statementToVariable(const QP::StorageAdapter& store, const string& value) {
	return QP::Executor::AttributeExecutor::statementToVariable<T>(store, stoul(value));
}

string callToProcedure(const QP::StorageAdapter& store, const string& value) {
	return QP::Executor::AttributeExecutor::callToProcedure(store, stoul(value));
}

const unordered_map<QP::Types::DispatchAttributeKey, function<string(const QP::StorageAdapter&, const string&)>> transform_map = {
	{{QP::Types::DesignEntity::Read, QP::Types::AttributeType::VariableName}, statementToVariable<QP::Types::ClauseType::ModifiesS>},
	{{QP::Types::DesignEntity::Call, QP::Types::AttributeType::ProcedureName}, callToProcedure},
	{{QP::Types::DesignEntity::Print, QP::Types::AttributeType::VariableName}, statementToVariable<QP::Types::ClauseType::UsesS>}};

static inline string applyTransform(const QP::StorageAdapter& store, const QP::Types::ReferenceArgument& argument, string result) {
	if (argument.getType() == QP::Types::ReferenceType::Synonym) {
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

	size_t table_size = query_result.getNumberOfRows();
	vector<string> result(table_size);
	Types::SelectList select_list = query_properties.getSelectList();
	vector<string> synonyms(select_list.size());
	transform(select_list.begin(), select_list.end(), synonyms.begin(),
	          [](const Types::ReferenceArgument& argument) { return argument.getSynonymSymbol(); });

	for (size_t i = 0; i < table_size; i++) {
		ResultRow row = query_result.getRowWithOrder(synonyms, i);
		string row_string;

		for (size_t j = 0; j < select_list.size(); j++) {
			string value = applyTransform(store, select_list.at(j), row.at(j));
			row_string.append(value);
			row_string.append(TUPLE_SEPERATOR);
		}
		row_string.pop_back();
		result[i] = row_string;
	}

	return result;
}

vector<string> QP::QueryPostProcessor::processBooleanResult(QueryResult& query_result) {
	string result = query_result.getResult() ? TRUE : FALSE;
	return {result};
}
