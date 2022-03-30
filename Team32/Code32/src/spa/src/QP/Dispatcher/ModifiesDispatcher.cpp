#include "QP/Dispatcher/ModifiesDispatcher.h"

#include <utility>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Executor/ProcedureVariableExecutor.tpp"
#include "QP/Executor/StatementVariableExecutor.tpp"

using namespace QP::Executor;
using namespace QP::Types;

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> name_map = {
	{ReferenceType::Name, {ClauseType::ModifiesP, ProcedureVariableExecutor::executorFactoryNameName<ClauseType::ModifiesP>}},
	{ReferenceType::Wildcard, {ClauseType::ModifiesP, ProcedureVariableExecutor::executorFactoryNameWildcard<ClauseType::ModifiesP>}},
	{DesignEntity::Variable, {ClauseType::ModifiesP, ProcedureVariableExecutor::executorFactoryNameSynonym<ClauseType::ModifiesP>}},
};

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> index_map = {
	{ReferenceType::Name, {ClauseType::ModifiesS, StatementVariableExecutor::executorFactoryIndexName<ClauseType::ModifiesS>}},
	{ReferenceType::Wildcard, {ClauseType::ModifiesS, StatementVariableExecutor::executorFactoryIndexWildcard<ClauseType::ModifiesS>}},
	{DesignEntity::Variable, {ClauseType::ModifiesS, StatementVariableExecutor::executorFactoryIndexSynonym<ClauseType::ModifiesS>}},
};

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> procedure_map = {
	{ReferenceType::Name, {ClauseType::ModifiesP, ProcedureVariableExecutor::executorFactorySynonymName<ClauseType::ModifiesP>}},
	{ReferenceType::Wildcard, {ClauseType::ModifiesP, ProcedureVariableExecutor::executorFactorySynonymWildcard<ClauseType::ModifiesP>}},
	{DesignEntity::Variable, {ClauseType::ModifiesP, ProcedureVariableExecutor::executorFactorySynonymSynonym<ClauseType::ModifiesP>}},
};

static const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> statement_map = {
	{ReferenceType::Name, {ClauseType::ModifiesS, StatementVariableExecutor::executorFactorySynonymName<ClauseType::ModifiesS>}},
	{ReferenceType::Wildcard, {ClauseType::ModifiesS, StatementVariableExecutor::executorFactorySynonymWildcard<ClauseType::ModifiesS>}},
	{DesignEntity::Variable, {ClauseType::ModifiesS, StatementVariableExecutor::executorFactorySynonymSynonym<ClauseType::ModifiesS>}},
};

const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>> argument_dispatch_map = {
	{ReferenceType::Name, name_map},     {ReferenceType::StatementIndex, index_map}, {DesignEntity::Procedure, procedure_map},
	{DesignEntity::Stmt, statement_map}, {DesignEntity::Call, statement_map},        {DesignEntity::Assign, statement_map},
	{DesignEntity::Read, statement_map}, {DesignEntity::While, statement_map},       {DesignEntity::If, statement_map}};

ExecutorSetBundle QP::Dispatcher::ModifiesDispatcher::dispatcher(const vector<ReferenceArgument>& args) {
	return QP::Dispatcher::DispatchProcessors::processArgument(argument_dispatch_map, args);
}