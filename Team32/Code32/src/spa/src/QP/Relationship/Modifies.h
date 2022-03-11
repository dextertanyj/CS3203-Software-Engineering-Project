#ifndef SPA_SRC_QP_RELATIONSHIP_MODIFIES_H
#define SPA_SRC_QP_RELATIONSHIP_MODIFIES_H

#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/ModifiesP.h"
#include "QP/Relationship/ModifiesS.h"
#include "QP/Relationship/Relationship.h"

namespace QP::Relationship::Modifies {
extern const Types::ArgumentDispatcher dispatcher;

namespace {
const unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactoryBundle> name_map = {
	{Types::ReferenceType::Name, pair{Types::ClauseType::ModifiesP,
                                      [](vector<Types::ReferenceArgument> args) {
										  return [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
											  return Relationship::ModifiesP::executeTrivialNameName(pkb, procedure, variable);
										  };
									  }}},
	{Types::ReferenceType::Wildcard, pair{Types::ClauseType::ModifiesP,
                                          [](vector<Types::ReferenceArgument> args) {
											  return [procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
												  return Relationship::ModifiesP::executeTrivialNameWildcardOrSynonym(pkb, procedure);
											  };
										  }}},
	{Types::DesignEntity::Variable, pair{Types::ClauseType::ModifiesP,
                                         [](vector<Types::ReferenceArgument> args) {
											 return pair{[procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
															 return Relationship::ModifiesP::executeTrivialNameWildcardOrSynonym(pkb,
		                                                                                                                         procedure);
														 },
	                                                     [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
															 return Relationship::ModifiesP::executeNameSynonym(pkb, procedure, variable);
														 }};
										 }}},
};

const unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactoryBundle> index_map = {
	{Types::ReferenceType::Name, pair{Types::ClauseType::ModifiesS,
                                      [](vector<Types::ReferenceArgument> args) {
										  return [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
											  return Relationship::ModifiesS::executeTrivialIndexName(pkb, procedure, variable);
										  };
									  }}},
	{Types::ReferenceType::Wildcard, pair{Types::ClauseType::ModifiesS,
                                          [](vector<Types::ReferenceArgument> args) {
											  return [procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
												  return Relationship::ModifiesS::executeTrivialIndexWildcardOrSynonym(pkb, procedure);
											  };
										  }}},
	{Types::DesignEntity::Variable, pair{Types::ClauseType::ModifiesS,
                                         [](vector<Types::ReferenceArgument> args) {
											 return pair{[procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
															 return Relationship::ModifiesS::executeTrivialIndexWildcardOrSynonym(
																 pkb, procedure);
														 },
	                                                     [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
															 return Relationship::ModifiesS::executeIndexSynonym(pkb, procedure, variable);
														 }};
										 }}},
};

const unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactoryBundle> procedure_map = {
	{Types::ReferenceType::Name, pair{Types::ClauseType::ModifiesP,
                                      [](vector<Types::ReferenceArgument> args) {
										  return pair{[variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
														  return Relationship::ModifiesP::executeTrivialSynonymName(pkb, variable);
													  },
	                                                  [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
														  return Relationship::ModifiesP::executeSynonymName(pkb, procedure, variable);
													  }};
									  }}},
	{Types::ReferenceType::Wildcard, pair{Types::ClauseType::ModifiesP,
                                          [](vector<Types::ReferenceArgument> args) {
											  return pair{[](PKB::StorageAccessInterface& pkb) {
															  return Relationship::ModifiesP::executeTrivialSynonymWildcardOrSynonym(pkb);
														  },
	                                                      [procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
															  return Relationship::ModifiesP::executeSynonymWildcard(pkb, procedure);
														  }};
										  }}},
	{Types::DesignEntity::Variable, pair{Types::ClauseType::ModifiesP,
                                         [](vector<Types::ReferenceArgument> args) {
											 return pair{[](PKB::StorageAccessInterface& pkb) {
															 return Relationship::ModifiesP::executeTrivialSynonymWildcardOrSynonym(pkb);
														 },
	                                                     [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
															 return Relationship::ModifiesP::executeSynonymSynonym(pkb, procedure,
		                                                                                                           variable);
														 }};
										 }}},
};

const unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactoryBundle> statement_map = {
	{Types::ReferenceType::Name, pair{Types::ClauseType::ModifiesS,
                                      [](vector<Types::ReferenceArgument> args) {
										  return pair{[statement = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
														  return Relationship::ModifiesS::executeTrivialSynonymName(pkb, statement,
		                                                                                                            variable);
													  },
	                                                  [statement = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
														  return Relationship::ModifiesS::executeSynonymName(pkb, statement, variable);
													  }};
									  }}},
	{Types::ReferenceType::Wildcard, pair{Types::ClauseType::ModifiesS,
                                          [](vector<Types::ReferenceArgument> args) {
											  return pair{[statement = args.at(0)](PKB::StorageAccessInterface& pkb) {
															  return Relationship::ModifiesS::executeTrivialSynonymWildcardOrSynonym(
																  pkb, statement);
														  },
	                                                      [statement = args.at(0)](PKB::StorageAccessInterface& pkb) {
															  return Relationship::ModifiesS::executeSynonymWildcard(pkb, statement);
														  }};
										  }}},
	{Types::DesignEntity::Variable, pair{Types::ClauseType::ModifiesS,
                                         [](vector<Types::ReferenceArgument> args) {
											 return pair{
												 [statement = args.at(0)](PKB::StorageAccessInterface& pkb) {
													 return Relationship::ModifiesS::executeTrivialSynonymWildcardOrSynonym(pkb, statement);
												 },
												 [statement = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
													 return Relationship::ModifiesS::executeSynonymSynonym(pkb, statement, variable);
												 }};
										 }}},
};

const unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactoryBundle>>
	argument_dispatch_map = {{Types::ReferenceType::Name, name_map},          {Types::ReferenceType::StatementIndex, index_map},
                             {Types::DesignEntity::Procedure, procedure_map}, {Types::DesignEntity::Stmt, statement_map},
                             {Types::DesignEntity::Call, statement_map},      {Types::DesignEntity::Assign, statement_map},
                             {Types::DesignEntity::Read, statement_map},      {Types::DesignEntity::While, statement_map},
                             {Types::DesignEntity::If, statement_map}};
}  // namespace
};  // namespace QP::Relationship::Modifies

#endif  // SPA_SRC_QP_RELATIONSHIP_MODIFIES_H
