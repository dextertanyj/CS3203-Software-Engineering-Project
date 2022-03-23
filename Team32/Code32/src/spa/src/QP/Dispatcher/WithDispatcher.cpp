#include "QP/Dispatcher/WithDispatcher.h"

#include <utility>

#include "QP/Executor/AttributeExecutor.h"
#include "QP/Executor/WithExecutor.h"

using namespace QP::Types;
using namespace QP::Executor;

namespace std {
template <>
struct hash<pair<QP::Types::DesignEntity, QP::Types::AttributeType>> {
	size_t operator()(const pair<QP::Types::DesignEntity, QP::Types::AttributeType>& pair) const {
		std::size_t seed = PAIR_SEED;
		Common::Hash::combineHash(seed, pair.first);
		Common::Hash::combineHash(seed, pair.second);
		return seed;
	}
};
}

namespace {
template <typename TAttribute, typename TLeft, typename TRight>
const unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>> attribute_executor_map = {
	{ReferenceType::Name, pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>(
							  QP::Executor::WithExecutor::executeTrivialAttributeConstant<TAttribute, TLeft, TRight>,
							  QP::Executor::WithExecutor::executeAttributeConstant<TAttribute, TLeft, TRight>)},
	{ReferenceType::StatementIndex, pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>(
										QP::Executor::WithExecutor::executeTrivialAttributeConstant<TAttribute, TLeft, TRight>,
										QP::Executor::WithExecutor::executeAttributeConstant<TAttribute, TLeft, TRight>)},
	{ReferenceType::Attribute, pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>(
								   QP::Executor::WithExecutor::executeTrivialAttributeAttribute<TAttribute, TLeft, TRight>,
								   QP::Executor::WithExecutor::executeAttributeAttribute<TAttribute, TLeft, TRight>)}};

template <typename TAttribute, typename TLeft, typename TRight>
const unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>> constant_executor_map = {
	{ReferenceType::Name, QP::Executor::WithExecutor::executeTrivialConstantConstant<TAttribute, TLeft, TRight>},
	{ReferenceType::StatementIndex, QP::Executor::WithExecutor::executeTrivialConstantConstant<TAttribute, TLeft, TRight>},
	{ReferenceType::Attribute, pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>(
								   QP::Executor::WithExecutor::executeTrivialAttributeAttribute<TAttribute, TLeft, TRight>,
								   QP::Executor::WithExecutor::executeAttributeAttribute<TAttribute, TLeft, TRight>)}};

template <typename TAttribute, typename TLeft, typename TRight>
const unordered_map<ReferenceType, unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>>> executor_map = {
	{ReferenceType::Name, constant_executor_map<TAttribute, TLeft, TRight>},
	{ReferenceType::StatementIndex, constant_executor_map<TAttribute, TLeft, TRight>},
	{ReferenceType::Attribute, attribute_executor_map<TAttribute, TLeft, TRight>}};

const unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Number, Number>> number_attribute_map = {
	{ReferenceType::StatementIndex, {AttributeExecutor::extractNumber, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::Stmt, AttributeType::Index},
     {AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::Assign, AttributeType::Index},
     {AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::Read, AttributeType::Index},
     {AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::Print, AttributeType::Index},
     {AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::Call, AttributeType::Index},
     {AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::If, AttributeType::Index},
     {AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::While, AttributeType::Index},
     {AttributeExecutor::selectStatements, AttributeExecutor::identity<Number>}},
	{DispatchAttributeKey{DesignEntity::Constant, AttributeType::Value},
     {AttributeExecutor::selectConstants, AttributeExecutor::identity<Number>}}};

const unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Name, Number>> name_attribute_number_map = {
	{DispatchAttributeKey{DesignEntity::Read, AttributeType::Variable},
     {AttributeExecutor::selectStatements, AttributeExecutor::statementToVariable<QP::Types::ClauseType::ModifiesS>}},
	{DispatchAttributeKey{DesignEntity::Call, AttributeType::Name},
     {AttributeExecutor::selectStatements, AttributeExecutor::callToProcedure}},
	{DispatchAttributeKey{DesignEntity::Print, AttributeType::Variable},
     {AttributeExecutor::selectStatements, AttributeExecutor::statementToVariable<QP::Types::ClauseType::UsesS>}}};

const unordered_map<WithClauseArgumentDispatchKey, WithInternalExecutors<Name, Name>> name_attribute_name_map = {
	{ReferenceType::Name, {AttributeExecutor::extractName, AttributeExecutor::identity<Name>}},
	{DispatchAttributeKey{DesignEntity::Variable, AttributeType::Name},
     {AttributeExecutor::selectVariables, AttributeExecutor::identity<Name>}},
	{DispatchAttributeKey{DesignEntity::Procedure, AttributeType::Name},
     {AttributeExecutor::selectProcedures, AttributeExecutor::identity<Name>}},
};

template <typename TAttribute, typename TLeft, typename TRight>
WithExecutorFunctionSet<TAttribute, TLeft, TRight> getExecutor(const vector<ReferenceArgument>& arguments) {
	ReferenceType lhs = arguments.at(0).getType();
	ReferenceType rhs = arguments.at(1).getType();
	unordered_map<ReferenceType, WithExecutorFunctionSet<TAttribute, TLeft, TRight>> inner_map =
		executor_map<TAttribute, TLeft, TRight>.at(lhs);
	auto executor = inner_map.at(rhs);
	return executor;
}

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
ExecutorSet processorHandler(const vector<ReferenceArgument>& arguments) {
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
			  [&](const WithExecutorFunction<TAttribute, TLeft, TRight>& executor) {
				  result = [&](const QP::StorageAdapter& store) {
					  return executor(store, arguments.at(0), arguments.at(1), lhs_executors, rhs_executors);
				  };
			  },
			  [&](const pair<WithExecutorFunction<TAttribute, TLeft, TRight>, WithExecutorFunction<TAttribute, TLeft, TRight>>& executors) {
				  result = pair{[&](const QP::StorageAdapter& store) {
									return executors.first(store, arguments.at(0), arguments.at(1), lhs_executors, rhs_executors);
								},
		                        [&](const QP::StorageAdapter& store) {
									return executors.second(store, arguments.at(0), arguments.at(1), lhs_executors, rhs_executors);
								}};
			  },
		  },
	      executor);
	return result;
}

const unordered_map<WithClauseBasicDispatchKey, function<ExecutorSet(const vector<ReferenceArgument>&)>> number_handler_map = {
	{ReferenceType::StatementIndex, processorHandler<Number, Number, Number>},
	{AttributeType::Index, processorHandler<Number, Number, Number>},
	{AttributeType::Value, processorHandler<Number, Number, Number>}};

const unordered_map<WithClauseBasicDispatchKey, function<ExecutorSet(const vector<ReferenceArgument>&)>> name_handler_map = {
	{ReferenceType::Name, processorHandler<Name, Name, Name>},
	{AttributeType::Name, processorHandler<Name, Name, Name>},
	{AttributeType::Variable, processorHandler<Name, Name, Number>}};

const unordered_map<WithClauseBasicDispatchKey, function<ExecutorSet(const vector<ReferenceArgument>&)>> variable_handler_map = {
	{ReferenceType::Name, processorHandler<Name, Name, Name>},
	{AttributeType::Name, processorHandler<Name, Name, Name>},
	{AttributeType::Variable, processorHandler<Name, Number, Name>}};

const unordered_map<WithClauseBasicDispatchKey,
                    unordered_map<WithClauseBasicDispatchKey, function<ExecutorSet(const vector<ReferenceArgument>&)>>>
	handler_map = {{ReferenceType::StatementIndex, number_handler_map},
                   {AttributeType::Index, number_handler_map},
                   {AttributeType::Value, number_handler_map},
                   {ReferenceType::Name, name_handler_map},
                   {AttributeType::Name, name_handler_map},
                   {AttributeType::Variable, variable_handler_map}};

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