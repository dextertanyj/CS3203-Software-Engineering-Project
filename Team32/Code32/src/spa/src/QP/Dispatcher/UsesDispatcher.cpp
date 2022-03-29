#include "QP/Dispatcher/UsesDispatcher.h"

#include <unordered_map>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/ProcedureVariableExecutor.tpp"
#include "QP/Executor/StatementVariableExecutor.tpp"

using namespace QP::Executor;
using namespace QP::Types;

static ExecutorSet executorFactoryNameName(const vector<ReferenceArgument>& args) {
	return [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
		return ProcedureVariableExecutor::executeTrivialNameName<ClauseType::UsesP>(storage, procedure, variable);
	};
}

static ExecutorSet executorFactoryNameWildcard(const vector<ReferenceArgument>& args) {
	return [procedure = args.at(0)](const QP::StorageAdapter& storage) {
		return ProcedureVariableExecutor::executeTrivialNameWildcardOrSynonym<ClauseType::UsesP>(storage, procedure);
	};
}

static ExecutorSet executorFactoryNameSynonym(const vector<ReferenceArgument>& args) {
	Executor trivial_executor = [procedure = args.at(0)](const QP::StorageAdapter& storage) {
		return ProcedureVariableExecutor::executeTrivialNameWildcardOrSynonym<ClauseType::UsesP>(storage, procedure);
	};
	Executor executor = [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
		return ProcedureVariableExecutor::executeNameSynonym<ClauseType::UsesP>(storage, procedure, variable);
	};
	return pair{trivial_executor, executor};
}

static ExecutorSet executorFactoryIndexName(const vector<ReferenceArgument>& args) {
	return [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
		return StatementVariableExecutor::executeTrivialIndexName<ClauseType::UsesS>(storage, procedure, variable);
	};
}

static ExecutorSet executorFactoryIndexWildcard(const vector<ReferenceArgument>& args) {
	return [procedure = args.at(0)](const QP::StorageAdapter& storage) {
		return StatementVariableExecutor::executeTrivialIndexWildcardOrSynonym<ClauseType::UsesS>(storage, procedure);
	};
}

static ExecutorSet executorFactoryIndexSynonym(const vector<ReferenceArgument>& args) {
	Executor trivial_executor = [procedure = args.at(0)](const QP::StorageAdapter& storage) {
		return StatementVariableExecutor::executeTrivialIndexWildcardOrSynonym<ClauseType::UsesS>(storage, procedure);
	};
	Executor executor = [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
		return StatementVariableExecutor::executeIndexSynonym<ClauseType::UsesS>(storage, procedure, variable);
	};
	return pair{trivial_executor, executor};
}

static ExecutorSet executorFactoryProcedureName(const vector<ReferenceArgument>& args) {
	Executor trivial_executor = [variable = args.at(1)](const QP::StorageAdapter& storage) {
		return ProcedureVariableExecutor::executeTrivialSynonymName<ClauseType::UsesP>(storage, variable);
	};
	Executor executor = [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
		return ProcedureVariableExecutor::executeSynonymName<ClauseType::UsesP>(storage, procedure, variable);
	};
	return pair{trivial_executor, executor};
}

static ExecutorSet executorFactoryProcedureWildcard(const vector<ReferenceArgument>& args) {
	Executor trivial_executor = [](const QP::StorageAdapter& storage) {
		return ProcedureVariableExecutor::executeTrivialSynonymWildcardOrSynonym<ClauseType::UsesP>(storage);
	};
	Executor executor = [procedure = args.at(0)](const QP::StorageAdapter& storage) {
		return ProcedureVariableExecutor::executeSynonymWildcard<ClauseType::UsesP>(storage, procedure);
	};
	return pair{trivial_executor, executor};
}

static ExecutorSet executorFactoryProcedureSynonym(const vector<ReferenceArgument>& args) {
	Executor trivial_executor = [](const QP::StorageAdapter& storage) {
		return ProcedureVariableExecutor::executeTrivialSynonymWildcardOrSynonym<ClauseType::UsesP>(storage);
	};
	Executor executor = [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
		return ProcedureVariableExecutor::executeSynonymSynonym<ClauseType::UsesP>(storage, procedure, variable);
	};
	return pair{trivial_executor, executor};
}

static ExecutorSet executorFactoryStatementName(const vector<ReferenceArgument>& args) {
	Executor trivial_executor = [statement = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
		return StatementVariableExecutor::executeTrivialSynonymName<ClauseType::UsesS>(storage, statement, variable);
	};
	Executor executor = [statement = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
		return StatementVariableExecutor::executeSynonymName<ClauseType::UsesS>(storage, statement, variable);
	};
	return pair{trivial_executor, executor};
}

static ExecutorSet executorFactoryStatementWildcard(const vector<ReferenceArgument>& args) {
	Executor trivial_executor = [statement = args.at(0)](const QP::StorageAdapter& storage) {
		return StatementVariableExecutor::executeTrivialSynonymWildcardOrSynonym<ClauseType::UsesS>(storage, statement);
	};
	Executor executor = [statement = args.at(0)](const QP::StorageAdapter& storage) {
		return StatementVariableExecutor::executeSynonymWildcard<ClauseType::UsesS>(storage, statement);
	};
	return pair{trivial_executor, executor};
}

static ExecutorSet executorFactoryStatementSynonym(const vector<ReferenceArgument>& args) {
	Executor trivial_executor = [statement = args.at(0)](const QP::StorageAdapter& storage) {
		return StatementVariableExecutor::executeTrivialSynonymWildcardOrSynonym<ClauseType::UsesS>(storage, statement);
	};
	Executor executor = [statement = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
		return StatementVariableExecutor::executeSynonymSynonym<ClauseType::UsesS>(storage, statement, variable);
	};
	return pair{trivial_executor, executor};
}

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> name_map = {
	{ReferenceType::Name, {ClauseType::UsesP, executorFactoryNameName}},
	{ReferenceType::Wildcard, {ClauseType::UsesP, executorFactoryNameWildcard}},
	{DesignEntity::Variable, {ClauseType::UsesP, executorFactoryNameSynonym}},
};

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> index_map = {
	{ReferenceType::Name, {ClauseType::UsesS, executorFactoryIndexName}},
	{ReferenceType::Wildcard, {ClauseType::UsesS, executorFactoryIndexWildcard}},
	{DesignEntity::Variable, {ClauseType::UsesS, executorFactoryIndexSynonym}},
};

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> procedure_map = {
	{ReferenceType::Name, {ClauseType::UsesP, executorFactoryProcedureName}},
	{ReferenceType::Wildcard, {ClauseType::UsesP, executorFactoryProcedureWildcard}},
	{DesignEntity::Variable, {ClauseType::UsesP, executorFactoryProcedureSynonym}},
};

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> statement_map = {
	{ReferenceType::Name, {ClauseType::UsesS, executorFactoryStatementName}},
	{ReferenceType::Wildcard, {ClauseType::UsesS, executorFactoryStatementWildcard}},
	{DesignEntity::Variable, {ClauseType::UsesS, executorFactoryStatementSynonym}},
};

static const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>> argument_dispatch_map = {
	{ReferenceType::Name, name_map},      {ReferenceType::StatementIndex, index_map}, {DesignEntity::Procedure, procedure_map},
	{DesignEntity::Stmt, statement_map},  {DesignEntity::Call, statement_map},        {DesignEntity::Assign, statement_map},
	{DesignEntity::Print, statement_map}, {DesignEntity::While, statement_map},       {DesignEntity::If, statement_map}};

ExecutorSetBundle QP::Dispatcher::UsesDispatcher::dispatcher(const vector<Types::ReferenceArgument>& args) {
	return QP::Dispatcher::DispatchProcessors::processArgument(argument_dispatch_map, args);
};