#include "QP/Dispatcher/UsesDispatcher.h"

#include <unordered_map>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Dispatcher/VariableDispatcher.tpp"

using namespace std;
using namespace QP::Dispatcher::VariableDispatcher;
using namespace QP::Types;

static const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>> argument_dispatch_map = {
	{ArgumentType::Name, getNameMap<ClauseType::UsesP>()},           {ArgumentType::Number, getIndexMap<ClauseType::UsesS>()},
	{DesignEntity::Procedure, getProcedureMap<ClauseType::UsesP>()}, {DesignEntity::Stmt, getStatementMap<ClauseType::UsesS>()},
	{DesignEntity::Call, getStatementMap<ClauseType::UsesS>()},      {DesignEntity::Assign, getStatementMap<ClauseType::UsesS>()},
	{DesignEntity::Print, getStatementMap<ClauseType::UsesS>()},     {DesignEntity::While, getStatementMap<ClauseType::UsesS>()},
	{DesignEntity::If, getStatementMap<ClauseType::UsesS>()}};

ExecutorSetBundle QP::Dispatcher::UsesDispatcher::dispatcher(const vector<ClauseArgument>& args) {
	return DispatchProcessors::processArgument(argument_dispatch_map, args);
}
