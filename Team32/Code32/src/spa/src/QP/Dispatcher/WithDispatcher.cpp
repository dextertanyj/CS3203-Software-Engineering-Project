#include "QP/Dispatcher/WithDispatcher.h"

#include <utility>

#include "QP/Executor/AttributeExecutor.tpp"
#include "QP/Executor/WithExecutor.tpp"

using namespace QP::Types;
using namespace QP::Executor;

template <typename TAttribute, typename TLeft, typename TRight>
using FunctionPair = pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>;

// Executor maps

template <typename TAttribute, typename TLeft, typename TRight>
static unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>> getAttributeExecutorMap() {
	static const unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>> map = {
		{ReferenceType::Name,
	     FunctionPair<TAttribute, TLeft, TRight>(WithExecutor::executeTrivialAttributeConstant<TAttribute, TLeft, TRight>,
	                                             WithExecutor::executeAttributeConstant<TAttribute, TLeft, TRight>)},
		{ReferenceType::StatementIndex,
	     FunctionPair<TAttribute, TLeft, TRight>(WithExecutor::executeTrivialAttributeConstant<TAttribute, TLeft, TRight>,
	                                             WithExecutor::executeAttributeConstant<TAttribute, TLeft, TRight>)},
		{ReferenceType::Attribute,
	     FunctionPair<TAttribute, TLeft, TRight>(WithExecutor::executeTrivialAttributeAttribute<TAttribute, TLeft, TRight>,
	                                             WithExecutor::executeAttributeAttribute<TAttribute, TLeft, TRight>)}};
	return map;
}

template <typename TAttribute, typename TLeft, typename TRight>
static unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>> getConstantExecutorMap() {
	static const unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>> map = {
		{ReferenceType::Name, WithExecutor::executeTrivialConstantConstant<TAttribute, TLeft, TRight>},
		{ReferenceType::StatementIndex, WithExecutor::executeTrivialConstantConstant<TAttribute, TLeft, TRight>},
		{ReferenceType::Attribute,
	     FunctionPair<TAttribute, TLeft, TRight>(WithExecutor::executeTrivialAttributeAttribute<TAttribute, TLeft, TRight>,
	                                             WithExecutor::executeAttributeAttribute<TAttribute, TLeft, TRight>)}};
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
	auto inner_map = map.at(lhs);
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
	static auto left_attribute_map = getAttributeMap<TAttribute, TLeft>();
	static auto right_attribute_map = getAttributeMap<TAttribute, TRight>();
	WithClauseArgumentDispatchKey lhs_key = arguments.at(0).getType();
	if (arguments.at(0).getType() == ReferenceType::Attribute) {
		lhs_key = pair{arguments.at(0).getAttribute().synonym.type, arguments.at(0).getAttribute().attribute};
	}
	WithClauseArgumentDispatchKey rhs_key = arguments.at(1).getType();
	if (arguments.at(1).getType() == ReferenceType::Attribute) {
		rhs_key = pair{arguments.at(1).getAttribute().synonym.type, arguments.at(1).getAttribute().attribute};
	}
	auto lhs_executors_iter = left_attribute_map.find(lhs_key);
	if (lhs_executors_iter == left_attribute_map.end()) {
		throw QP::QueryDispatchException("Incorrect argument type.");
	}
	auto rhs_executors_iter = right_attribute_map.find(rhs_key);
	if (rhs_executors_iter == right_attribute_map.end()) {
		throw QP::QueryDispatchException("Incorrect argument type.");
	}
	auto lhs_executors = lhs_executors_iter->second;
	auto rhs_executors = rhs_executors_iter->second;
	auto executor = getExecutor<TAttribute, TLeft, TRight>(arguments);
	ExecutorSet result;
	visit(Visitor{
			  [=, lhs = arguments.at(0), rhs = arguments.at(1), &result](const WithExecutorFunction<TAttribute, TLeft, TRight>& executor) {
				  result = [=](const QP::StorageAdapter& store) { return executor(store, lhs, rhs, lhs_executors, rhs_executors); };
			  },
			  [=, lhs = arguments.at(0), rhs = arguments.at(1), &result](const FunctionPair<TAttribute, TLeft, TRight>& executors) {
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

QP::Types::ExecutorSetBundle QP::Dispatcher::WithDispatcher::dispatcher(const vector<Types::ReferenceArgument>& arguments) {
	assert(arguments.size() == 2);
	WithClauseBasicDispatchKey lhs = arguments.at(0).getType();
	if (arguments.at(0).getType() == ReferenceType::Attribute) {
		lhs = arguments.at(0).getAttribute().attribute;
	}
	WithClauseBasicDispatchKey rhs = arguments.at(1).getType();
	if (arguments.at(1).getType() == ReferenceType::Attribute) {
		rhs = arguments.at(1).getAttribute().attribute;
	}
	auto handler_iter = handler_map.find(lhs);
	if (handler_iter == handler_map.end()) {
		throw QP::QueryDispatchException("Incorrect argument type.");
	}
	auto inner_map = handler_iter->second;
	auto inner_handler_iter = inner_map.find(rhs);
	if (inner_handler_iter == inner_map.end()) {
		throw QP::QueryDispatchException("Incorrect argument type.");
	}
	return {ClauseType::With, inner_handler_iter->second(arguments)};
}