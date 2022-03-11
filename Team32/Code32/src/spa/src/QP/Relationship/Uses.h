#ifndef SPA_SRC_QP_RELATIONSHIP_USES_H
#define SPA_SRC_QP_RELATIONSHIP_USES_H

#include <unordered_map>
#include <utility>

#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/ProcedureVariableExecutor.tpp"
#include "QP/Relationship/Relationship.h"
#include "QP/Relationship/StatementVariableExecutor.tpp"

namespace QP::Relationship::Uses {
extern const QP::Types::ArgumentDispatcher dispatcher;

namespace {
const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> name_map = {
	{QP::Types::ReferenceType::Name, pair{QP::Types::ClauseType::UsesP,
                                          [](vector<QP::Types::ReferenceArgument> args) {
											  return [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
												  return ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeTrivialNameName(
													  pkb, procedure, variable);
											  };
										  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::UsesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return [procedure = args.at(0)](QP::StorageAdapter& pkb) {
				  return ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeTrivialNameWildcardOrSynonym(pkb, procedure);
			  };
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::UsesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[procedure = args.at(0)](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeTrivialNameWildcardOrSynonym(pkb,
		                                                                                                                      procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeNameSynonym(pkb, procedure, variable);
						  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> index_map = {
	{QP::Types::ReferenceType::Name, pair{QP::Types::ClauseType::UsesS,
                                          [](vector<QP::Types::ReferenceArgument> args) {
											  return [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
												  return StatementVariableExecutor<Types::ClauseType::UsesS>::executeTrivialIndexName(
													  pkb, procedure, variable);
											  };
										  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::UsesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return [procedure = args.at(0)](QP::StorageAdapter& pkb) {
				  return StatementVariableExecutor<Types::ClauseType::UsesS>::executeTrivialIndexWildcardOrSynonym(pkb, procedure);
			  };
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::UsesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[procedure = args.at(0)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::UsesS>::executeTrivialIndexWildcardOrSynonym(pkb,
		                                                                                                                       procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::UsesS>::executeIndexSynonym(pkb, procedure, variable);
						  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> procedure_map = {
	{QP::Types::ReferenceType::Name,
     pair{QP::Types::ClauseType::UsesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeTrivialSynonymName(pkb, variable);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeSynonymName(pkb, procedure, variable);
						  }};
		  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::UsesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeTrivialSynonymWildcardOrSynonym(pkb);
						  },
	                      [procedure = args.at(0)](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeSynonymWildcard(pkb, procedure);
						  }};
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::UsesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeTrivialSynonymWildcardOrSynonym(pkb);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::UsesP>::executeSynonymSynonym(pkb, procedure, variable);
						  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> statement_map = {
	{QP::Types::ReferenceType::Name,
     pair{QP::Types::ClauseType::UsesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[statement = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::UsesS>::executeTrivialSynonymName(pkb, statement,
		                                                                                                            variable);
						  },
	                      [statement = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::UsesS>::executeSynonymName(pkb, statement, variable);
						  }};
		  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::UsesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[statement = args.at(0)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::UsesS>::executeTrivialSynonymWildcardOrSynonym(pkb,
		                                                                                                                         statement);
						  },
	                      [statement = args.at(0)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::UsesS>::executeSynonymWildcard(pkb, statement);
						  }};
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::UsesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[statement = args.at(0)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::UsesS>::executeTrivialSynonymWildcardOrSynonym(pkb,
		                                                                                                                         statement);
						  },
	                      [statement = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::UsesS>::executeSynonymSynonym(pkb, statement, variable);
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
}  // namespace QP::Relationship::Uses

#endif  // SPA_SRC_QP_RELATIONSHIP_USES_H
