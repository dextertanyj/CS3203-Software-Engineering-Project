#ifndef SPA_SRC_QP_DISPATCHER_PATTERNASSIGNDISPATCHER_H
#define SPA_SRC_QP_DISPATCHER_PATTERNASSIGNDISPATCHER_H

#include <unordered_map>
#include <utility>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Executor/PatternAssignExecutor.h"
#include "QP/Executor/ProcedureVariableExecutor.tpp"
#include "QP/Executor/StatementVariableExecutor.tpp"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"

namespace QP::Dispatcher::PatternAssignDispatcher {
extern const QP::Types::ArgumentDispatcher dispatcher;

namespace {
using namespace Executor::PatternAssignExecutor;

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> name_map = {
	{Types::ReferenceType::Wildcard,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{[variable = args.at(1)](const QP::StorageAdapter& storage) { return executeTrivialNameWildcard(storage, variable); },
	                 [assign = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
						 return executeNameWildcard(storage, assign, variable);
					 }};
	 }},
	{Types::ReferenceType::ExactExpression,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{[variable = args.at(1), expression = args.at(2)](const QP::StorageAdapter& storage) {
						 return executeTrivialNameExpression(storage, variable, expression);
					 },
	                 [assign = args.at(0), variable = args.at(1), expression = args.at(2)](const QP::StorageAdapter& storage) {
						 return executeNameExpression(storage, assign, variable, expression);
					 }};
	 }},
	{Types::ReferenceType::SubExpression,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{[variable = args.at(1), expression = args.at(2)](const QP::StorageAdapter& storage) {
						 return executeTrivialNameExpression(storage, variable, expression);
					 },
	                 [assign = args.at(0), variable = args.at(1), expression = args.at(2)](const QP::StorageAdapter& storage) {
						 return executeNameExpression(storage, assign, variable, expression);
					 }};
	 }},
};
const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> wildcard_map = {
	{Types::ReferenceType::Wildcard,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{[](const QP::StorageAdapter& storage) { return executeTrivialSynonymOrWildcardWildcard(storage); },
	                 [assign = args.at(0)](const QP::StorageAdapter& storage) { return executeWildcardWildcard(storage, assign); }};
	 }},
	{Types::ReferenceType::ExactExpression,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{[expression = args.at(2)](const QP::StorageAdapter& storage) {
						 return executeTrivialSynonymOrWildcardExpression(storage, expression);
					 },
	                 [assign = args.at(0), expression = args.at(2)](const QP::StorageAdapter& storage) {
						 return executeWildcardExpression(storage, assign, expression);
					 }};
	 }},
	{Types::ReferenceType::SubExpression,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{[expression = args.at(2)](const QP::StorageAdapter& storage) {
						 return executeTrivialSynonymOrWildcardExpression(storage, expression);
					 },
	                 [assign = args.at(0), expression = args.at(2)](const QP::StorageAdapter& storage) {
						 return executeWildcardExpression(storage, assign, expression);
					 }};
	 }},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory> variable_synonym_map = {
	{Types::ReferenceType::Wildcard,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{[](const QP::StorageAdapter& storage) { return executeTrivialSynonymOrWildcardWildcard(storage); },
	                 [assign = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
						 return executeSynonymWildcard(storage, assign, variable);
					 }};
	 }},
	{Types::ReferenceType::ExactExpression,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{[expression = args.at(2)](const QP::StorageAdapter& storage) {
						 return executeTrivialSynonymOrWildcardExpression(storage, expression);
					 },
	                 [assign = args.at(0), variable = args.at(1), expression = args.at(2)](const QP::StorageAdapter& storage) {
						 return executeSynonymExpression(storage, assign, variable, expression);
					 }};
	 }},
	{Types::ReferenceType::SubExpression,
     [](const vector<Types::ReferenceArgument>& args) {
		 return pair{[expression = args.at(2)](const QP::StorageAdapter& storage) {
						 return executeTrivialSynonymOrWildcardExpression(storage, expression);
					 },
	                 [assign = args.at(0), variable = args.at(1), expression = args.at(2)](const QP::StorageAdapter& storage) {
						 return executeSynonymExpression(storage, assign, variable, expression);
					 }};
	 }},
};

const unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>
	synonym_map = {{Types::ReferenceType::Name, name_map},
                   {Types::ReferenceType::Wildcard, wildcard_map},
                   {Types::DesignEntity::Variable, variable_synonym_map}};

unordered_map<QP::Types::ArgumentDispatchKey,
              unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactory>>>
	argument_dispatch_map = {{Types::DesignEntity::Assign, synonym_map}};
}
};

#endif  // SPA_SRC_QP_DISPATCHER_PATTERNASSIGNDISPATCHER_H
