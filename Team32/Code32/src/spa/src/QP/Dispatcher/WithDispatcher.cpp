#include "QP/Dispatcher/WithDispatcher.h"

#include <unordered_map>

#include "QP/Executor/AttributeExecutor.tpp"
#include "QP/Executor/WithExecutor.tpp"

using namespace QP::Executor;
using namespace QP::Types;

/*
 * Selection and attribute transformation maps
 *
 * These maps select the correct synonym selection function and attribute mapping function to obtain the respective synonym-attribute
 * combination.
 */

static const unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Number, Number>> number_attribute_map = {
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

static const unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Name, Number>> name_attribute_number_map = {
	{DispatchAttributeKey{DesignEntity::Read, AttributeType::VariableName},
     {AttributeExecutor::selectStatements, AttributeExecutor::statementToVariable<QP::Types::ClauseType::ModifiesS>}},
	{DispatchAttributeKey{DesignEntity::Call, AttributeType::ProcedureName},
     {AttributeExecutor::selectStatements, AttributeExecutor::callToProcedure}},
	{DispatchAttributeKey{DesignEntity::Print, AttributeType::VariableName},
     {AttributeExecutor::selectStatements, AttributeExecutor::statementToVariable<QP::Types::ClauseType::UsesS>}}};

static const unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Name, Name>> name_attribute_name_map = {
	{ReferenceType::Name, {AttributeExecutor::extractName, AttributeExecutor::identity<Name>}},
	{DispatchAttributeKey{DesignEntity::Variable, AttributeType::NameIdentifier},
     {AttributeExecutor::selectVariables, AttributeExecutor::identity<Name>}},
	{DispatchAttributeKey{DesignEntity::Procedure, AttributeType::NameIdentifier},
     {AttributeExecutor::selectProcedures, AttributeExecutor::identity<Name>}},
};

template <typename TAttribute, typename T>
const unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<TAttribute, T>>& getAttributeMap() = delete;
template <>
inline const unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Number, Number>>& getAttributeMap() {
	return number_attribute_map;
}
template <>
inline const unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Name, Number>>& getAttributeMap() {
	return name_attribute_number_map;
}
template <>
inline const unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Name, Name>>& getAttributeMap() {
	return name_attribute_name_map;
}

/*
 * Executor maps
 *
 * These maps select the executor to use based on the attribute type, left synonym type and right synonym type.
 */

template <typename T>
using ReferenceMap = unordered_map<ReferenceType, T>;

template <typename TAttribute, typename TLeft, typename TRight>
using ExecutorPair = pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>;

template <typename TAttribute, typename TLeft, typename TRight>
static const ReferenceMap<WithExecutorFunctionSet<TAttribute, TLeft, TRight>>& getAttributeExecutorMap() {
	static const ReferenceMap<WithExecutorFunctionSet<TAttribute, TLeft, TRight>> map = {
		{ReferenceType::Name,
	     ExecutorPair<TAttribute, TLeft, TRight>(WithExecutor::executeTrivialAttributeConstant<TAttribute, TLeft, TRight>,
	                                             WithExecutor::executeAttributeConstant<TAttribute, TLeft, TRight>)},
		{ReferenceType::StatementIndex,
	     ExecutorPair<TAttribute, TLeft, TRight>(WithExecutor::executeTrivialAttributeConstant<TAttribute, TLeft, TRight>,
	                                             WithExecutor::executeAttributeConstant<TAttribute, TLeft, TRight>)},
		{ReferenceType::Attribute,
	     ExecutorPair<TAttribute, TLeft, TRight>(WithExecutor::executeTrivialAttributeAttribute<TAttribute, TLeft, TRight>,
	                                             WithExecutor::executeAttributeAttribute<TAttribute, TLeft, TRight>)}};
	return map;
}

template <typename TAttribute, typename TLeft, typename TRight>
static const ReferenceMap<WithExecutorFunctionSet<TAttribute, TLeft, TRight>>& getConstantExecutorMap() {
	static const ReferenceMap<WithExecutorFunctionSet<TAttribute, TLeft, TRight>> map = {
		{ReferenceType::Name, WithExecutor::executeTrivialConstantConstant<TAttribute, TLeft, TRight>},
		{ReferenceType::StatementIndex, WithExecutor::executeTrivialConstantConstant<TAttribute, TLeft, TRight>},
		{ReferenceType::Attribute,
	     ExecutorPair<TAttribute, TLeft, TRight>(WithExecutor::executeTrivialAttributeAttribute<TAttribute, TLeft, TRight>,
	                                             WithExecutor::executeAttributeAttribute<TAttribute, TLeft, TRight>)}};
	return map;
}

template <typename TAttribute, typename TLeft, typename TRight>
static const ReferenceMap<ReferenceMap<WithExecutorFunctionSet<TAttribute, TLeft, TRight>>>& getExecutorMap() {
	static const ReferenceMap<ReferenceMap<WithExecutorFunctionSet<TAttribute, TLeft, TRight>>> map = {
		{ReferenceType::Name, getConstantExecutorMap<TAttribute, TLeft, TRight>()},
		{ReferenceType::StatementIndex, getConstantExecutorMap<TAttribute, TLeft, TRight>()},
		{ReferenceType::Attribute, getAttributeExecutorMap<TAttribute, TLeft, TRight>()}};
	return map;
}

template <typename TAttribute, typename TLeft, typename TRight>
WithExecutorFunctionSet<TAttribute, TLeft, TRight> getExecutor(const vector<QP::ReferenceArgument>& args) {
	static const ReferenceMap<ReferenceMap<WithExecutorFunctionSet<TAttribute, TLeft, TRight>>> map =
		getExecutorMap<TAttribute, TLeft, TRight>();
	ReferenceType lhs = args.at(0).getType();
	ReferenceType rhs = args.at(1).getType();
	unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>> inner_map = map.at(lhs);
	auto executor = inner_map.at(rhs);
	return executor;
}

/**
 * Selects the corresponding executor set for the given arguments.
 *
 * Retrieves the selection and attribute mapping functions for the left and right arguments.
 * Combines them together using the appropriate set of trivial and non-trivial executors.
 *
 * @tparam TAttribute the type (Name | Number) of the attribute being compared.
 * @tparam TLeft the type of the left argument.
 * @tparam TRight the type of the right argument.
 * @param arguments the left and right arguments being compared.
 * @return the corresponding executor set.
 */
template <typename TAttribute, typename TLeft, typename TRight>
ExecutorSet dispatchHandler(const vector<QP::ReferenceArgument>& args) {
	static const auto left_attribute_map = getAttributeMap<TAttribute, TLeft>();
	static const auto right_attribute_map = getAttributeMap<TAttribute, TRight>();
	WithClauseArgumentDispatchKey lhs_key = args.at(0).getType();
	if (args.at(0).getType() == ReferenceType::Attribute) {
		lhs_key = pair{args.at(0).getAttribute().synonym.type, args.at(0).getAttribute().attribute};
	}
	WithClauseArgumentDispatchKey rhs_key = args.at(1).getType();
	if (args.at(1).getType() == ReferenceType::Attribute) {
		rhs_key = pair{args.at(1).getAttribute().synonym.type, args.at(1).getAttribute().attribute};
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
	auto executor = getExecutor<TAttribute, TLeft, TRight>(args);

	// Curries the corresponding left- and right-hand side executors together with the comparison executor.
	ExecutorSet result;
	auto trivial_executor_visitor = [=, lhs = args.at(0), rhs = args.at(1),
	                                 &result](const WithExecutorFunction<TAttribute, TLeft, TRight>& executor) {
		result = [=](const QP::StorageAdapter& store) { return executor(store, lhs, rhs, lhs_executors, rhs_executors); };
	};
	auto executor_visitor = [=, lhs = args.at(0), rhs = args.at(1), &result](const ExecutorPair<TAttribute, TLeft, TRight>& executors) {
		result = pair{[=](const QP::StorageAdapter& store) { return executors.first(store, lhs, rhs, lhs_executors, rhs_executors); },
		              [=](const QP::StorageAdapter& store) { return executors.second(store, lhs, rhs, lhs_executors, rhs_executors); }};
	};
	visit(Visitor{trivial_executor_visitor, executor_visitor}, executor);

	return result;
}

/*
 * Dispatch handler maps
 *
 * These maps select the appropriate dispatch handler to use based on the attribute type, left synonym type and right synonym type.
 */

const unordered_map<WithClauseBasicDispatchKey, function<ExecutorSet(const vector<QP::ReferenceArgument>&)>> number_handler_map = {
	{ReferenceType::StatementIndex, dispatchHandler<Number, Number, Number>},
	{AttributeType::NumberIdentifier, dispatchHandler<Number, Number, Number>},
	{AttributeType::NumberIdentifier, dispatchHandler<Number, Number, Number>}};

const unordered_map<WithClauseBasicDispatchKey, function<ExecutorSet(const vector<QP::ReferenceArgument>&)>> name_handler_map = {
	{ReferenceType::Name, dispatchHandler<Name, Name, Name>},
	{AttributeType::NameIdentifier, dispatchHandler<Name, Name, Name>},
	{AttributeType::VariableName, dispatchHandler<Name, Name, Number>},
	{AttributeType::ProcedureName, dispatchHandler<Name, Name, Number>}};

const unordered_map<WithClauseBasicDispatchKey, function<ExecutorSet(const vector<QP::ReferenceArgument>&)>> variable_handler_map = {
	{ReferenceType::Name, dispatchHandler<Name, Number, Name>},
	{AttributeType::NameIdentifier, dispatchHandler<Name, Number, Name>},
	{AttributeType::VariableName, dispatchHandler<Name, Number, Number>},
	{AttributeType::ProcedureName, dispatchHandler<Name, Number, Number>}};

const unordered_map<WithClauseBasicDispatchKey,
                    unordered_map<WithClauseBasicDispatchKey, function<ExecutorSet(const vector<QP::ReferenceArgument>&)>>>
	handler_map = {{ReferenceType::StatementIndex, number_handler_map},
                   {AttributeType::NumberIdentifier, number_handler_map},
                   {ReferenceType::Name, name_handler_map},
                   {AttributeType::NameIdentifier, name_handler_map},
                   {AttributeType::ProcedureName, variable_handler_map},
                   {AttributeType::VariableName, variable_handler_map}};

QP::Types::ExecutorSetBundle QP::Dispatcher::WithDispatcher::dispatcher(const vector<ReferenceArgument>& args) {
	assert(args.size() == 2);
	WithClauseBasicDispatchKey lhs = args.at(0).getType();
	if (args.at(0).getType() == ReferenceType::Attribute) {
		lhs = args.at(0).getAttribute().attribute;
	}
	WithClauseBasicDispatchKey rhs = args.at(1).getType();
	if (args.at(1).getType() == ReferenceType::Attribute) {
		rhs = args.at(1).getAttribute().attribute;
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
	return {ClauseType::With, inner_handler_iter->second(args)};
}