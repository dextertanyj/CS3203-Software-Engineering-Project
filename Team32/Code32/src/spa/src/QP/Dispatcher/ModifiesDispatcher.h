#ifndef SPA_SRC_QP_RELATIONSHIP_MODIFIES_H
#define SPA_SRC_QP_RELATIONSHIP_MODIFIES_H

#include <unordered_map>
#include <utility>

#include "QP/Executor/ProcedureVariableExecutor.tpp"
#include "QP/Executor/StatementVariableExecutor.tpp"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"

namespace QP::Dispatcher::ModifiesDispatcher {
extern const QP::Types::ArgumentDispatcher dispatcher;

namespace {
const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> name_map = {
	{QP::Types::ReferenceType::Name,
     pair{QP::Types::ClauseType::ModifiesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
				  return Executor::ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeTrivialNameName(storage, procedure,
		                                                                                                           variable);
			  };
		  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::ModifiesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return [procedure = args.at(0)](QP::StorageAdapter& storage) {
				  return Executor::ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeTrivialNameWildcardOrSynonym(storage,
		                                                                                                                        procedure);
			  };
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::ModifiesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[procedure = args.at(0)](QP::StorageAdapter& storage) {
							  return Executor::ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeTrivialNameWildcardOrSynonym(
								  storage, procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
							  return Executor::ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeNameSynonym(
								  storage, procedure, variable);
						  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> index_map = {
	{QP::Types::ReferenceType::Name,
     pair{QP::Types::ClauseType::ModifiesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
				  return Executor::StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeTrivialIndexName(storage, procedure,
		                                                                                                            variable);
			  };
		  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::ModifiesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return [procedure = args.at(0)](QP::StorageAdapter& storage) {
				  return Executor::StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeTrivialIndexWildcardOrSynonym(storage,
		                                                                                                                         procedure);
			  };
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::ModifiesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{
				  [procedure = args.at(0)](QP::StorageAdapter& storage) {
					  return Executor::StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeTrivialIndexWildcardOrSynonym(
						  storage, procedure);
				  },
				  [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
					  return Executor::StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeIndexSynonym(storage, procedure,
		                                                                                                            variable);
				  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> procedure_map = {
	{QP::Types::ReferenceType::Name,
     pair{QP::Types::ClauseType::ModifiesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[variable = args.at(1)](QP::StorageAdapter& storage) {
							  return Executor::ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeTrivialSynonymName(storage,
		                                                                                                                          variable);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
							  return Executor::ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeSynonymName(
								  storage, procedure, variable);
						  }};
		  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::ModifiesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{
				  [](QP::StorageAdapter& storage) {
					  return Executor::ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeTrivialSynonymWildcardOrSynonym(
						  storage);
				  },
				  [procedure = args.at(0)](QP::StorageAdapter& storage) {
					  return Executor::ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeSynonymWildcard(storage, procedure);
				  }};
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::ModifiesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{
				  [](QP::StorageAdapter& storage) {
					  return Executor::ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeTrivialSynonymWildcardOrSynonym(
						  storage);
				  },
				  [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
					  return Executor::ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeSynonymSynonym(storage, procedure,
		                                                                                                              variable);
				  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> statement_map = {
	{QP::Types::ReferenceType::Name,
     pair{QP::Types::ClauseType::ModifiesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[statement = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
							  return Executor::StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeTrivialSynonymName(
								  storage, statement, variable);
						  },
	                      [statement = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
							  return Executor::StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeSynonymName(
								  storage, statement, variable);
						  }};
		  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::ModifiesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{
				  [statement = args.at(0)](QP::StorageAdapter& storage) {
					  return Executor::StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeTrivialSynonymWildcardOrSynonym(
						  storage, statement);
				  },
				  [statement = args.at(0)](QP::StorageAdapter& storage) {
					  return Executor::StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeSynonymWildcard(storage, statement);
				  }};
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::ModifiesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{
				  [statement = args.at(0)](QP::StorageAdapter& storage) {
					  return Executor::StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeTrivialSynonymWildcardOrSynonym(
						  storage, statement);
				  },
				  [statement = args.at(0), variable = args.at(1)](QP::StorageAdapter& storage) {
					  return Executor::StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeSynonymSynonym(storage, statement,
		                                                                                                              variable);
				  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle>>
	argument_dispatch_map = {{QP::Types::ReferenceType::Name, name_map},          {QP::Types::ReferenceType::StatementIndex, index_map},
                             {QP::Types::DesignEntity::Procedure, procedure_map}, {QP::Types::DesignEntity::Stmt, statement_map},
                             {QP::Types::DesignEntity::Call, statement_map},      {QP::Types::DesignEntity::Assign, statement_map},
                             {QP::Types::DesignEntity::Read, statement_map},      {QP::Types::DesignEntity::While, statement_map},
                             {QP::Types::DesignEntity::If, statement_map}};
}  // namespace
};  // namespace QP::Relationship::ModifiesDispatcher

#endif  // SPA_SRC_QP_RELATIONSHIP_MODIFIES_H
