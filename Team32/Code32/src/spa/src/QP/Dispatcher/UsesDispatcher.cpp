#include "QP/Dispatcher/UsesDispatcher.h"

#include <unordered_map>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Dispatcher/VariableDispatcher.tpp"

using namespace std;
using namespace QP::Dispatcher;
using namespace QP::Types;

static const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>> argument_dispatch_map = {
	{ReferenceType::Name, VariableDispatcher::getNameMap<ClauseType::UsesP>()},
	{ReferenceType::StatementIndex, VariableDispatcher::getIndexMap<ClauseType::UsesS>()},
	{DesignEntity::Procedure, VariableDispatcher::getProcedureMap<ClauseType::UsesP>()},
	{DesignEntity::Stmt, VariableDispatcher::getStatementMap<ClauseType::UsesS>()},
	{DesignEntity::Call, VariableDispatcher::getStatementMap<ClauseType::UsesS>()},
	{DesignEntity::Assign, VariableDispatcher::getStatementMap<ClauseType::UsesS>()},
	{DesignEntity::Print, VariableDispatcher::getStatementMap<ClauseType::UsesS>()},
	{DesignEntity::While, VariableDispatcher::getStatementMap<ClauseType::UsesS>()},
	{DesignEntity::If, VariableDispatcher::getStatementMap<ClauseType::UsesS>()}};

ExecutorSetBundle QP::Dispatcher::UsesDispatcher::dispatcher(const vector<ReferenceArgument>& args) {
	return DispatchProcessors::processArgument(argument_dispatch_map, args);
}
