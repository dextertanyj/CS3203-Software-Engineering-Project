#ifndef SPA_SRC_QP_RELATIONSHIP_MODIFIES_H
#define SPA_SRC_QP_RELATIONSHIP_MODIFIES_H

#include <unordered_map>
#include <utility>

#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/ProcedureVariableExecutor.tpp"
#include "QP/Relationship/Relationship.h"
#include "QP/Relationship/StatementVariableExecutor.tpp"

namespace QP::Relationship::Modifies {
extern const QP::Types::ArgumentDispatcher dispatcher;

namespace {
const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> name_map = {
	{QP::Types::ReferenceType::Name, pair{QP::Types::ClauseType::ModifiesP,
                                          [](vector<QP::Types::ReferenceArgument> args) {
											  return [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
												  return ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeTrivialNameName(
													  pkb, procedure, variable);
											  };
										  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::ModifiesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return [procedure = args.at(0)](QP::StorageAdapter& pkb) {
				  return ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeTrivialNameWildcardOrSynonym(pkb, procedure);
			  };
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::ModifiesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[procedure = args.at(0)](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeTrivialNameWildcardOrSynonym(
								  pkb, procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeNameSynonym(pkb, procedure, variable);
						  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> index_map = {
	{QP::Types::ReferenceType::Name, pair{QP::Types::ClauseType::ModifiesS,
                                          [](vector<QP::Types::ReferenceArgument> args) {
											  return [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
												  return StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeTrivialIndexName(
													  pkb, procedure, variable);
											  };
										  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::ModifiesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return [procedure = args.at(0)](QP::StorageAdapter& pkb) {
				  return StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeTrivialIndexWildcardOrSynonym(pkb, procedure);
			  };
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::ModifiesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[procedure = args.at(0)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeTrivialIndexWildcardOrSynonym(
								  pkb, procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeIndexSynonym(pkb, procedure, variable);
						  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> procedure_map = {
	{QP::Types::ReferenceType::Name,
     pair{QP::Types::ClauseType::ModifiesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeTrivialSynonymName(pkb, variable);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeSynonymName(pkb, procedure, variable);
						  }};
		  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::ModifiesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeTrivialSynonymWildcardOrSynonym(pkb);
						  },
	                      [procedure = args.at(0)](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeSynonymWildcard(pkb, procedure);
						  }};
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::ModifiesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeTrivialSynonymWildcardOrSynonym(pkb);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return ProcedureVariableExecutor<Types::ClauseType::ModifiesP>::executeSynonymSynonym(pkb, procedure,
		                                                                                                            variable);
						  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> statement_map = {
	{QP::Types::ReferenceType::Name,
     pair{QP::Types::ClauseType::ModifiesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[statement = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeTrivialSynonymName(pkb, statement,
		                                                                                                                variable);
						  },
	                      [statement = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeSynonymName(pkb, statement, variable);
						  }};
		  }}},
	{QP::Types::ReferenceType::Wildcard,
     pair{QP::Types::ClauseType::ModifiesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[statement = args.at(0)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeTrivialSynonymWildcardOrSynonym(
								  pkb, statement);
						  },
	                      [statement = args.at(0)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeSynonymWildcard(pkb, statement);
						  }};
		  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::ModifiesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[statement = args.at(0)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeTrivialSynonymWildcardOrSynonym(
								  pkb, statement);
						  },
	                      [statement = args.at(0), variable = args.at(1)](QP::StorageAdapter& pkb) {
							  return StatementVariableExecutor<Types::ClauseType::ModifiesS>::executeSynonymSynonym(pkb, statement,
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
};  // namespace QP::Relationship::Modifies

#endif  // SPA_SRC_QP_RELATIONSHIP_MODIFIES_H
