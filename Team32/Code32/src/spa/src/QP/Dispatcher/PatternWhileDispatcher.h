#ifndef SPA_SRC_QP_DISPATCHER_PATTERNWHILEDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_PATTERNWHILEDISPATCHER_H

#include <unordered_map>
#include <utility>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Executor/PatternContainerStatementExecutor.tpp"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"

using QP::Types::ClauseType;

namespace QP::Dispatcher::PatternWhileDispatcher {
extern const QP::Types::ArgumentDispatcher dispatcher;

namespace {
const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> name_map = {
	{Types::ReferenceType::Wildcard, [](const vector<Types::ReferenceArgument>& args) {
		 return pair{
			 [variable = args.at(1)](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeTrivialName(storage, variable);
			 },
			 [whileStmt = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeName(storage, whileStmt, variable);
			 }};
	 }}};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> wildcard_map = {
	{Types::ReferenceType::Wildcard,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{
			 [](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeTrivialWildcardOrSynonym(storage);
			 },
			 [whileStmt = args.at(0)](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeWildcard(storage, whileStmt);
			 }};
	 }},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> variable_synonym_map = {
	{Types::ReferenceType::Wildcard,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{
			 [whileStmt = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeTrivialWildcardOrSynonym(storage);
			 },
			 [whileStmt = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeSynonym(storage, whileStmt, variable);
			 }};
	 }},
};

const unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>
	synonym_map = {{Types::ReferenceType::Name, name_map},
                   {Types::ReferenceType::Wildcard, wildcard_map},
                   {Types::DesignEntity::Variable, variable_synonym_map}};

unordered_map<QP::Types::ArgumentDispatchKey,
              unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>>
	argument_dispatch_map = {{Types::DesignEntity::While, synonym_map}};
}  // namespace
};

#endif  // SPA_SRC_QP_DISPATCHER_PATTERNWHILEDISPATCHER_H
