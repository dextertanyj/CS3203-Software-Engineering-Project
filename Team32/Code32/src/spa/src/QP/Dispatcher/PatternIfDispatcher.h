#ifndef SPA_SRC_QP_DISPATCHER_PATTERNIFDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_PATTERNIFDISPATCHER_H

#include <unordered_map>
#include <utility>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Executor/PatternContainerStatementExecutor.tpp"
#include "QP/ReferenceArgument.h"
#include "QP/Types.h"

using QP::Types::ClauseType;

namespace QP::Dispatcher::PatternIfDispatcher {
extern const QP::Types::ArgumentDispatcher dispatcher;

namespace {
const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> name_map = {
	{Types::ReferenceType::Wildcard, [](const vector<Types::ReferenceArgument>& args) {
		 return pair{[variable = args.at(1)](const QP::StorageAdapter& storage) {
						 return Executor::PatternContainerStatementExecutor::executeTrivialName<ClauseType::PatternIf>(storage, variable);
					 },
	                 [ifStmt = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
						 return Executor::PatternContainerStatementExecutor::executeName<ClauseType::PatternIf>(storage, ifStmt, variable);
					 }};
	 }}};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> wildcard_map = {
	{Types::ReferenceType::Wildcard,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{
			 [](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor::executeTrivialWildcardOrSynonym<ClauseType::PatternIf>(storage);
			 },
			 [ifStmt = args.at(0)](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor::executeWildcard<ClauseType::PatternIf>(storage, ifStmt);
			 }};
	 }},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> variable_synonym_map = {
	{Types::ReferenceType::Wildcard,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{
			 [ifStmt = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor::executeTrivialWildcardOrSynonym<ClauseType::PatternIf>(storage);
			 },
			 [ifStmt = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor::executeSynonym<ClauseType::PatternIf>(storage, ifStmt, variable);
			 }};
	 }},
};

const unordered_map<
	QP::Types::ArgumentDispatchKey,
	unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>>
	synonym_map = {
		{Types::ReferenceType::Name, {{Types::ReferenceType::Wildcard, name_map}}},
		{Types::ReferenceType::Wildcard, {{Types::ReferenceType::Wildcard, wildcard_map}}},
		{Types::DesignEntity::Variable, {{Types::ReferenceType::Wildcard, variable_synonym_map}}},
};

unordered_map<QP::Types::ArgumentDispatchKey,
              unordered_map<QP::Types::ArgumentDispatchKey,
                            unordered_map<QP::Types::ArgumentDispatchKey,
                                          unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>>>
	argument_dispatch_map = {{Types::DesignEntity::If, synonym_map}};
}
};

#endif  // SPA_SRC_QP_DISPATCHER_PATTERNIFDISPATCHER_H
