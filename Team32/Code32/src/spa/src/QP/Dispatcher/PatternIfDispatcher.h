#ifndef SPA_PATTERNIFDISPATCHER_H
#define SPA_PATTERNIFDISPATCHER_H

#include <unordered_map>
#include <utility>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Executor/PatternContainerStatementExecutor.tpp"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"

using QP::Types::ClauseType;

namespace QP::Dispatcher::PatternIfDispatcher {
extern const QP::Types::ArgumentDispatcher dispatcher;

namespace {
const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> argument_dispatch_map = {
	{Types::ReferenceType::Name,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{[variable = args.at(1)](const QP::StorageAdapter& storage) {
						 return Executor::PatternContainerStatementExecutor<ClauseType::PatternIf>::executeTrivialName(storage, variable);
					 },
	                 [ifStmt = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
						 return Executor::PatternContainerStatementExecutor<ClauseType::PatternIf>::executeName(storage, ifStmt, variable);
					 }};
	 }},
	{Types::ReferenceType::Wildcard,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{
			 [](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor<ClauseType::PatternIf>::executeTrivialWildcardOrSynonym(storage);
			 },
			 [ifStmt = args.at(0)](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor<ClauseType::PatternIf>::executeWildcard(storage, ifStmt);
			 }};
	 }},
	{Types::ReferenceType::Synonym,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{
			 [ifStmt = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor<ClauseType::PatternIf>::executeTrivialWildcardOrSynonym(storage);
			 },
			 [ifStmt = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor<ClauseType::PatternIf>::executeSynonym(storage, ifStmt, variable);
			 }};
	 }},
};
}  // namespace
};  // namespace QP::Dispatcher::PatternIfDispatcher

#endif  // SPA_PATTERNIFDISPATCHER_H
