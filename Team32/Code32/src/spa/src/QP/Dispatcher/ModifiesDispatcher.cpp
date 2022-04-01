#include "QP/Dispatcher/ModifiesDispatcher.h"

#include <unordered_map>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Dispatcher/VariableDispatcher.tpp"

using namespace QP::Dispatcher;
using namespace QP::Types;

static const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>> argument_dispatch_map = {
	{ReferenceType::Name, VariableDispatcher::getNameMap<ClauseType::ModifiesP>()},
	{ReferenceType::StatementIndex, VariableDispatcher::getIndexMap<ClauseType::ModifiesS>()},
	{DesignEntity::Procedure, VariableDispatcher::getProcedureMap<ClauseType::ModifiesP>()},
	{DesignEntity::Stmt, VariableDispatcher::getStatementMap<ClauseType::ModifiesS>()},
	{DesignEntity::Call, VariableDispatcher::getStatementMap<ClauseType::ModifiesS>()},
	{DesignEntity::Assign, VariableDispatcher::getStatementMap<ClauseType::ModifiesS>()},
	{DesignEntity::Read, VariableDispatcher::getStatementMap<ClauseType::ModifiesS>()},
	{DesignEntity::While, VariableDispatcher::getStatementMap<ClauseType::ModifiesS>()},
	{DesignEntity::If, VariableDispatcher::getStatementMap<ClauseType::ModifiesS>()}};

ExecutorSetBundle QP::Dispatcher::ModifiesDispatcher::dispatcher(const vector<ReferenceArgument>& args) {
	return QP::Dispatcher::DispatchProcessors::processArgument(argument_dispatch_map, args);
}