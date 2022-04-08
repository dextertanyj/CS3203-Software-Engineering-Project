#include "QP/Dispatcher/ModifiesDispatcher.h"

#include <unordered_map>

#include "QP/Dispatcher/DispatchProcessors.tpp"
#include "QP/Dispatcher/VariableDispatcher.tpp"

using namespace std;
using namespace QP::Dispatcher::VariableDispatcher;
using namespace QP::Types;

static const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>> argument_dispatch_map = {
	{ArgumentType::Name, getNameMap<ClauseType::ModifiesP>()},           {ArgumentType::Number, getIndexMap<ClauseType::ModifiesS>()},
	{DesignEntity::Procedure, getProcedureMap<ClauseType::ModifiesP>()}, {DesignEntity::Stmt, getStatementMap<ClauseType::ModifiesS>()},
	{DesignEntity::Call, getStatementMap<ClauseType::ModifiesS>()},      {DesignEntity::Assign, getStatementMap<ClauseType::ModifiesS>()},
	{DesignEntity::Read, getStatementMap<ClauseType::ModifiesS>()},      {DesignEntity::While, getStatementMap<ClauseType::ModifiesS>()},
	{DesignEntity::If, getStatementMap<ClauseType::ModifiesS>()}};

ExecutorSetBundle QP::Dispatcher::ModifiesDispatcher::dispatcher(const vector<ClauseArgument>& args) {
	return QP::Dispatcher::DispatchProcessors::processArgument(argument_dispatch_map, args);
}