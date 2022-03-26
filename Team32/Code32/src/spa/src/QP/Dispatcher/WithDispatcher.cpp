#include "QP/Dispatcher/WithDispatcher.h"

#include <utility>

#include "QP/Executor/AttributeExecutor.h"
#include "QP/Executor/WithExecutor.tpp"

using namespace QP::Types;
using namespace QP::Executor;

namespace {

// Executor maps

template <typename TAttribute, typename TLeft, typename TRight>
static unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>> getAttributeExecutorMap() {
	static const unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>> map = {
		{ReferenceType::Name, pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>(
								  QP::Executor::WithExecutor::executeTrivialAttributeConstant<TAttribute, TLeft, TRight>,
								  QP::Executor::WithExecutor::executeAttributeConstant<TAttribute, TLeft, TRight>)},
		{ReferenceType::StatementIndex,
	     pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>(
			 QP::Executor::WithExecutor::executeTrivialAttributeConstant<TAttribute, TLeft, TRight>,
			 QP::Executor::WithExecutor::executeAttributeConstant<TAttribute, TLeft, TRight>)},
		{ReferenceType::Attribute, pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>(
									   QP::Executor::WithExecutor::executeTrivialAttributeAttribute<TAttribute, TLeft, TRight>,
									   QP::Executor::WithExecutor::executeAttributeAttribute<TAttribute, TLeft, TRight>)}};
	return map;
}

template <typename TAttribute, typename TLeft, typename TRight>
static unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>> getConstantExecutorMap() {
	static const unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>> map = {
		{ReferenceType::Name, QP::Executor::WithExecutor::executeTrivialConstantConstant<TAttribute, TLeft, TRight>},
		{ReferenceType::StatementIndex, QP::Executor::WithExecutor::executeTrivialConstantConstant<TAttribute, TLeft, TRight>},
		{ReferenceType::Attribute, pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>(
									   QP::Executor::WithExecutor::executeTrivialAttributeAttribute<TAttribute, TLeft, TRight>,
									   QP::Executor::WithExecutor::executeAttributeAttribute<TAttribute, TLeft, TRight>)}};
	return map;
}

template <typename TAttribute, typename TLeft, typename TRight>
static unordered_map<ReferenceType, unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>>> getExecutorMap() {
	static const unordered_map<ReferenceType, unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>>> map = {
		{ReferenceType::Name, getConstantExecutorMap<TAttribute, TLeft, TRight>()},
		{ReferenceType::StatementIndex, getConstantExecutorMap<TAttribute, TLeft, TRight>()},
		{ReferenceType::Attribute, getAttributeExecutorMap<TAttribute, TLeft, TRight>()}};
	return map;
}

template <typename TAttribute, typename TLeft, typename TRight>
WithExecutorFunctionSet<TAttribute, TLeft, TRight> getExecutor(const vector<ReferenceArgument>& arguments) {
	static const unordered_map<ReferenceType, unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>>> map =
		getExecutorMap<TAttribute, TLeft, TRight>();
	ReferenceType lhs = arguments.at(0).getType();
	ReferenceType rhs = arguments.at(1).getType();
	unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>> inner_map = map.at(lhs);
	auto executor = inner_map.at(rhs);
	return executor;
}

// Attribute selection maps

const unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Number, Number>> number_attribute_map = {
	{ReferenceType::StatementIndex, {AttributeExecutor::extractNumber, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::Stmt, AttributeType::NumberIdentifier},
     {AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::Assign, AttributeType::NumberIdentifier},
     {AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::Read, AttributeType::NumberIdentifier},
     {AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::Print, AttributeType::NumberIdentifier},
     {AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::Call, AttributeType::NumberIdentifier},
     {AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::If, AttributeType::NumberIdentifier},
     {AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::While, AttributeType::NumberIdentifier},
     {AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::Constant, AttributeType::NumberIdentifier},
     {AttributeExecutor::selectConstants, AttributeExecutor::identity<Number>}}};

const unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Name, Number>> name_attribute_number_map = {
	{DispatchAttributeKey{DesignEntity::Read, AttributeType::VariableName},
     {AttributeExecutor::selectStatements, AttributeExecutor::statementToVariable<QP::Types::ClauseType::ModifiesS>}},
	{DispatchAttributeKey{DesignEntity::Call, AttributeType::ProcedureName},
     {AttributeExecutor::selectStatements, AttributeExecutor::callToProcedure}},
	{DispatchAttributeKey{DesignEntity::Print, AttributeType::VariableName},
     {AttributeExecutor::selectStatements, AttributeExecutor::statementToVariable<QP::Types::ClauseType::UsesS>}}};

const unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Name, Name>> name_attribute_name_map = {
	{ReferenceType::Name, {AttributeExecutor::extractName, AttributeExecutor::identity<Name>}},
	{DispatchAttributeKey{DesignEntity::Variable, AttributeType::NameIdentifier},
     {AttributeExecutor::selectVariables, AttributeExecutor::identity<Name>}},
	{DispatchAttributeKey{DesignEntity::Procedure, AttributeType::NameIdentifier},
     {AttributeExecutor::selectProcedures, AttributeExecutor::identity<Name>}},
};

template <typename TAttribute, typename T>
unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<TAttribute, T>> getAttributeMap() = delete;
template <>
inline unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Number, Number>> getAttributeMap() {
	return number_attribute_map;
}
template <>
inline unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Name, Number>> getAttributeMap() {
	return name_attribute_number_map;
}
template <>
inline unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Name, Name>> getAttributeMap() {
	return name_attribute_name_map;
}

template <typename TAttribute, typename TLeft, typename TRight>
ExecutorSet dispatchHandler(const vector<ReferenceArgument>& arguments) {
	WithClauseArgumentDispatchKey lhs_key = arguments.at(0).getType();
	if (arguments.at(0).getType() == ReferenceType::Attribute) {
		lhs_key = pair{arguments.at(0).getAttribute().synonym.type, arguments.at(0).getAttribute().attribute};
	}
	auto lhs_executors = getAttributeMap<TAttribute, TLeft>().at(lhs_key);
	WithClauseArgumentDispatchKey rhs_key = arguments.at(1).getType();
	if (arguments.at(1).getType() == ReferenceType::Attribute) {
		rhs_key = pair{arguments.at(1).getAttribute().synonym.type, arguments.at(1).getAttribute().attribute};
	}
	auto rhs_executors = getAttributeMap<TAttribute, TRight>().at(rhs_key);
	auto executor = getExecutor<TAttribute, TLeft, TRight>(arguments);
	ExecutorSet result;
	visit(Visitor{
			  [=, lhs = arguments.at(0), rhs = arguments.at(1), &result](const WithExecutorFunction<TAttribute, TLeft, TRight>& executor) {
				  result = [=](const QP::StorageAdapter& store) { return executor(store, lhs, rhs, lhs_executors, rhs_executors); };
			  },
			  [=, lhs = arguments.at(0), rhs = arguments.at(1), &result](
				  const pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>& executors) {
				  result = pair{
					  [=](const QP::StorageAdapter& store) { return executors.first(store, lhs, rhs, lhs_executors, rhs_executors); },
					  [=](const QP::StorageAdapter& store) { return executors.second(store, lhs, rhs, lhs_executors, rhs_executors); }};
			  },
		  },
	      executor);
	return result;
}

// Dispatch handler maps

const unordered_map<WithClauseBasicDispatchKey, function<ExecutorSet(const vector<ReferenceArgument>&)>> number_handler_map = {
	{ReferenceType::StatementIndex, dispatchHandler<Number, Number, Number>},
	{AttributeType::NumberIdentifier, dispatchHandler<Number, Number, Number>},
	{AttributeType::NumberIdentifier, dispatchHandler<Number, Number, Number>}};

const unordered_map<WithClauseBasicDispatchKey, function<ExecutorSet(const vector<ReferenceArgument>&)>> name_handler_map = {
	{ReferenceType::Name, dispatchHandler<Name, Name, Name>},
	{AttributeType::NameIdentifier, dispatchHandler<Name, Name, Name>},
	{AttributeType::VariableName, dispatchHandler<Name, Name, Number>},
	{AttributeType::ProcedureName, dispatchHandler<Name, Name, Number>}};

const unordered_map<WithClauseBasicDispatchKey, function<ExecutorSet(const vector<ReferenceArgument>&)>> variable_handler_map = {
	{ReferenceType::Name, dispatchHandler<Name, Number, Name>},
	{AttributeType::NameIdentifier, dispatchHandler<Name, Number, Name>},
	{AttributeType::VariableName, dispatchHandler<Name, Number, Number>},
	{AttributeType::ProcedureName, dispatchHandler<Name, Number, Number>}};

const unordered_map<WithClauseBasicDispatchKey,
                    unordered_map<WithClauseBasicDispatchKey, function<ExecutorSet(const vector<ReferenceArgument>&)>>>
	handler_map = {{ReferenceType::StatementIndex, number_handler_map},
                   {AttributeType::NumberIdentifier, number_handler_map},
                   {ReferenceType::Name, name_handler_map},
                   {AttributeType::NameIdentifier, name_handler_map},
                   {AttributeType::ProcedureName, variable_handler_map},
                   {AttributeType::VariableName, variable_handler_map}};
}

QP::Types::ExecutorSetBundle QP::Dispatcher::WithDispatcher::dispatcher(const vector<Types::ReferenceArgument>& arguments) {
	if (arguments.size() != 2) {
		throw QP::QueryException("Incorrect number of arguments.");
	}
	WithClauseBasicDispatchKey lhs = arguments.at(0).getType();
	if (arguments.at(0).getType() == ReferenceType::Attribute) {
		lhs = arguments.at(0).getAttribute().attribute;
	}
	WithClauseBasicDispatchKey rhs = arguments.at(1).getType();
	if (arguments.at(1).getType() == ReferenceType::Attribute) {
		rhs = arguments.at(1).getAttribute().attribute;
	}
	return {ClauseType::With, handler_map.at(lhs).at(rhs)(arguments)};
}