#include "QP/Dispatcher/UsesDispatcher.h"

#include <unordered_map>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/ProcedureVariableExecutor.tpp"
#include "QP/Executor/StatementVariableExecutor.tpp"

using namespace QP::Executor;
using namespace QP::Types;

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> name_map = {
	{ReferenceType::Name, {ClauseType::UsesP, ProcedureVariableExecutor::executorFactoryNameName<ClauseType::UsesP>}},
	{ReferenceType::Wildcard, {ClauseType::UsesP, ProcedureVariableExecutor::executorFactoryNameWildcard<ClauseType::UsesP>}},
	{DesignEntity::Variable, {ClauseType::UsesP, ProcedureVariableExecutor::executorFactoryNameSynonym<ClauseType::UsesP>}},
};

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> index_map = {
	{ReferenceType::Name, {ClauseType::UsesS, StatementVariableExecutor::executorFactoryIndexName<ClauseType::UsesS>}},
	{ReferenceType::Wildcard, {ClauseType::UsesS, StatementVariableExecutor::executorFactoryIndexWildcard<ClauseType::UsesS>}},
	{DesignEntity::Variable, {ClauseType::UsesS, StatementVariableExecutor::executorFactoryIndexSynonym<ClauseType::UsesS>}},
};

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> procedure_map = {
	{ReferenceType::Name, {ClauseType::UsesP, ProcedureVariableExecutor::executorFactorySynonymName<ClauseType::UsesP>}},
	{ReferenceType::Wildcard, {ClauseType::UsesP, ProcedureVariableExecutor::executorFactorySynonymWildcard<ClauseType::UsesP>}},
	{DesignEntity::Variable, {ClauseType::UsesP, ProcedureVariableExecutor::executorFactorySynonymSynonym<ClauseType::UsesP>}},
};

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> statement_map = {
	{ReferenceType::Name, {ClauseType::UsesS, StatementVariableExecutor::executorFactorySynonymName<ClauseType::UsesS>}},
	{ReferenceType::Wildcard, {ClauseType::UsesS, StatementVariableExecutor::executorFactorySynonymWildcard<ClauseType::UsesS>}},
	{DesignEntity::Variable, {ClauseType::UsesS, StatementVariableExecutor::executorFactorySynonymSynonym<ClauseType::UsesS>}},
};

static const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>> argument_dispatch_map = {
	{ReferenceType::Name, name_map},      {ReferenceType::StatementIndex, index_map}, {DesignEntity::Procedure, procedure_map},
	{DesignEntity::Stmt, statement_map},  {DesignEntity::Call, statement_map},        {DesignEntity::Assign, statement_map},
	{DesignEntity::Print, statement_map}, {DesignEntity::While, statement_map},       {DesignEntity::If, statement_map}};

ExecutorSetBundle QP::Dispatcher::UsesDispatcher::dispatcher(const vector<ReferenceArgument>& args) {
	return DispatchProcessors::processArgument(argument_dispatch_map, args);
}
