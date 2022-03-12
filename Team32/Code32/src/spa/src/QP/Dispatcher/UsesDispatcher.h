#ifndef SPA_SRC_QP_RELATIONSHIP_USES_H
#define SPA_SRC_QP_RELATIONSHIP_USES_H

#include <unordered_map>
#include <utility>

#include "QP/Dispatcher/Dispatcher.h"
#include "QP/Executor/ProcedureVariableExecutor.tpp"
#include "QP/Executor/StatementVariableExecutor.tpp"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"

namespace QP::Dispatcher::UsesDispatcher {
extern const QP::Types::ArgumentDispatcher dispatcher;

namespace {
const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> name_map = {
	{QP::Types::ReferenceType::Name,
     pair{QP::Types::ClauseType::UsesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
				  return Executor::ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeTrivialNameName(storage, procedure,
		                                                                                                       variable);
			  };
		  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::UsesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return [procedure = args.at(0)](QP::StorageAdapter& storage) {
				  return Executor::ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeTrivialNameWildcardOrSynonym(storage,
		                                                                                                                    procedure);
			  };
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::UsesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[procedure = args.at(0)](QP::StorageAdapter& storage) {
							  return Executor::ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeTrivialNameWildcardOrSynonym(
								  storage, procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
							  return Executor::ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeNameSynonym(storage, procedure,
		                                                                                                               variable);
						  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> index_map = {
	{QP::Types::ReferenceType::Name,
     pair{QP::Types::ClauseType::UsesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
				  return Executor::StatementVariableExecutor<Types::ClauseType::UsesS>::executeTrivialIndexName(storage, procedure,
		                                                                                                        variable);
			  };
		  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::UsesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return [procedure = args.at(0)](QP::StorageAdapter& storage) {
				  return Executor::StatementVariableExecutor<Types::ClauseType::UsesS>::executeTrivialIndexWildcardOrSynonym(storage,
		                                                                                                                     procedure);
			  };
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::UsesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[procedure = args.at(0)](QP::StorageAdapter& storage) {
							  return Executor::StatementVariableExecutor<Types::ClauseType::UsesS>::executeTrivialIndexWildcardOrSynonym(
								  storage, procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
							  return Executor::StatementVariableExecutor<Types::ClauseType::UsesS>::executeIndexSynonym(storage, procedure,
		                                                                                                                variable);
						  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> procedure_map = {
	{QP::Types::ReferenceType::Name,
     pair{QP::Types::ClauseType::UsesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[variable = args.at(1)](QP::StorageAdapter& storage) {
							  return Executor::ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeTrivialSynonymName(storage,
		                                                                                                                      variable);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
							  return Executor::ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeSynonymName(storage, procedure,
		                                                                                                               variable);
						  }};
		  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::UsesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{
				  [](QP::StorageAdapter& storage) {
					  return Executor::ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeTrivialSynonymWildcardOrSynonym(storage);
				  },
				  [procedure = args.at(0)](QP::StorageAdapter& storage) {
					  return Executor::ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeSynonymWildcard(storage, procedure);
				  }};
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::UsesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{
				  [](QP::StorageAdapter& storage) {
					  return Executor::ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeTrivialSynonymWildcardOrSynonym(storage);
				  },
				  [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
					  return Executor::ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeSynonymSynonym(storage, procedure,
		                                                                                                          variable);
				  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> statement_map = {
	{QP::Types::ReferenceType::Name,
     pair{QP::Types::ClauseType::UsesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[statement = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
							  return Executor::StatementVariableExecutor<Types::ClauseType::UsesS>::executeTrivialSynonymName(
								  storage, statement, variable);
						  },
	                      [statement = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
							  return Executor::StatementVariableExecutor<Types::ClauseType::UsesS>::executeSynonymName(storage, statement,
		                                                                                                               variable);
						  }};
		  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::UsesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[statement = args.at(0)](QP::StorageAdapter& storage) {
							  return Executor::StatementVariableExecutor<Types::ClauseType::UsesS>::executeTrivialSynonymWildcardOrSynonym(
								  storage, statement);
						  },
	                      [statement = args.at(0)](QP::StorageAdapter& storage) {
							  return Executor::StatementVariableExecutor<Types::ClauseType::UsesS>::executeSynonymWildcard(storage,
		                                                                                                                   statement);
						  }};
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::UsesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[statement = args.at(0)](QP::StorageAdapter& storage) {
							  return Executor::StatementVariableExecutor<Types::ClauseType::UsesS>::executeTrivialSynonymWildcardOrSynonym(
								  storage, statement);
						  },
	                      [statement = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
							  return Executor::StatementVariableExecutor<Types::ClauseType::UsesS>::executeSynonymSynonym(
								  storage, statement, variable);
						  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle>>
	argument_dispatch_map = {{QP::Types::ReferenceType::Name, name_map},          {QP::Types::ReferenceType::StatementIndex, index_map},
                             {QP::Types::DesignEntity::Procedure, procedure_map}, {QP::Types::DesignEntity::Stmt, statement_map},
                             {QP::Types::DesignEntity::Call, statement_map},      {QP::Types::DesignEntity::Assign, statement_map},
                             {QP::Types::DesignEntity::Print, statement_map},     {QP::Types::DesignEntity::While, statement_map},
                             {QP::Types::DesignEntity::If, statement_map}};
}  // namespace
}  // namespace QP::Dispatcher::UsesDispatcher

#endif  // SPA_SRC_QP_RELATIONSHIP_USES_H
