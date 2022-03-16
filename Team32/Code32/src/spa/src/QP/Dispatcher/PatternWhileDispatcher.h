#ifndef SPA_PATTERNWHILEDISPATCHER_H
#define SPA_PATTERNWHILEDISPATCHER_H

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
const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> map = {
	{Types::ReferenceType::Name,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{
			 [variable = args.at(1)](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeTrivialName(storage, variable);
			 },
			 [whileStmt = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
				 return Executor::PatternContainerStatementExecutor<ClauseType::PatternWhile>::executeName(storage, whileStmt, variable);
			 }};
	 }},
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
	{Types::ReferenceType::Synonym,
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
	synonym_map = {{Types::DesignEntity::While, map}};

unordered_map<QP::Types::ArgumentDispatchKey,
              unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>>
	argument_dispatch_map = {{Types::DesignEntity::While, synonym_map}};
}  // namespace
};  // namespace QP::Dispatcher::PatternWhileDispatcher

#endif  // SPA_PATTERNWHILEDISPATCHER_H
