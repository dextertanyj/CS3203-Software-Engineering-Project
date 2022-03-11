#ifndef SPA_SRC_QP_RELATIONSHIP_USES_H
#define SPA_SRC_QP_RELATIONSHIP_USES_H

#include <unordered_map>
#include <utility>

#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relationship.h"
#include "QP/Relationship/UsesP.h"
#include "QP/Relationship/UsesS.h"

namespace QP::Relationship::Uses {
extern const Types::ArgumentDispatcher dispatcher;

namespace {
const unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactoryBundle> name_map = {
	{Types::ReferenceType::Name, pair{Types::ClauseType::UsesP,
                                      [](vector<Types::ReferenceArgument> args) {
										  return [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
											  return Relationship::UsesP::executeTrivialNameName(pkb, procedure, variable);
										  };
									  }}},
	{Types::ReferenceType::Wildcard, pair{Types::ClauseType::UsesP,
                                          [](vector<Types::ReferenceArgument> args) {
											  return [procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
												  return Relationship::UsesP::executeTrivialNameWildcardOrSynonym(pkb, procedure);
											  };
										  }}},
	{Types::DesignEntity::Variable, pair{Types::ClauseType::UsesP,
                                         [](vector<Types::ReferenceArgument> args) {
											 return pair{[procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
															 return Relationship::UsesP::executeTrivialNameWildcardOrSynonym(pkb,
		                                                                                                                     procedure);
														 },
	                                                     [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
															 return Relationship::UsesP::executeNameSynonym(pkb, procedure, variable);
														 }};
										 }}},
};

const unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactoryBundle> index_map = {
	{Types::ReferenceType::Name, pair{Types::ClauseType::UsesS,
                                      [](vector<Types::ReferenceArgument> args) {
										  return [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
											  return Relationship::UsesS::executeTrivialIndexName(pkb, procedure, variable);
										  };
									  }}},
	{Types::ReferenceType::Wildcard, pair{Types::ClauseType::UsesS,
                                          [](vector<Types::ReferenceArgument> args) {
											  return [procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
												  return Relationship::UsesS::executeTrivialIndexWildcardOrSynonym(pkb, procedure);
											  };
										  }}},
	{Types::DesignEntity::Variable, pair{Types::ClauseType::UsesS,
                                         [](vector<Types::ReferenceArgument> args) {
											 return pair{[procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
															 return Relationship::UsesS::executeTrivialIndexWildcardOrSynonym(pkb,
		                                                                                                                      procedure);
														 },
	                                                     [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
															 return Relationship::UsesS::executeIndexSynonym(pkb, procedure, variable);
														 }};
										 }}},
};

const unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactoryBundle> procedure_map = {
	{Types::ReferenceType::Name, pair{Types::ClauseType::UsesP,
                                      [](vector<Types::ReferenceArgument> args) {
										  return pair{[variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
														  return Relationship::UsesP::executeTrivialSynonymName(pkb, variable);
													  },
	                                                  [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
														  return Relationship::UsesP::executeSynonymName(pkb, procedure, variable);
													  }};
									  }}},
	{Types::ReferenceType::Wildcard, pair{Types::ClauseType::UsesP,
                                          [](vector<Types::ReferenceArgument> args) {
											  return pair{[](PKB::StorageAccessInterface& pkb) {
															  return Relationship::UsesP::executeTrivialSynonymWildcardOrSynonym(pkb);
														  },
	                                                      [procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
															  return Relationship::UsesP::executeSynonymWildcard(pkb, procedure);
														  }};
										  }}},
	{Types::DesignEntity::Variable, pair{Types::ClauseType::UsesP,
                                         [](vector<Types::ReferenceArgument> args) {
											 return pair{[](PKB::StorageAccessInterface& pkb) {
															 return Relationship::UsesP::executeTrivialSynonymWildcardOrSynonym(pkb);
														 },
	                                                     [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
															 return Relationship::UsesP::executeSynonymSynonym(pkb, procedure, variable);
														 }};
										 }}},
};

const unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactoryBundle> statement_map = {
	{Types::ReferenceType::Name, pair{Types::ClauseType::UsesS,
                                      [](vector<Types::ReferenceArgument> args) {
										  return pair{[statement = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
														  return Relationship::UsesS::executeTrivialSynonymName(pkb, statement, variable);
													  },
	                                                  [statement = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
														  return Relationship::UsesS::executeSynonymName(pkb, statement, variable);
													  }};
									  }}},
	{Types::ReferenceType::Wildcard, pair{Types::ClauseType::UsesS,
                                          [](vector<Types::ReferenceArgument> args) {
											  return pair{[statement = args.at(0)](PKB::StorageAccessInterface& pkb) {
															  return Relationship::UsesS::executeTrivialSynonymWildcardOrSynonym(pkb,
		                                                                                                                         statement);
														  },
	                                                      [statement = args.at(0)](PKB::StorageAccessInterface& pkb) {
															  return Relationship::UsesS::executeSynonymWildcard(pkb, statement);
														  }};
										  }}},
	{Types::DesignEntity::Variable, pair{Types::ClauseType::UsesS,
                                         [](vector<Types::ReferenceArgument> args) {
											 return pair{[statement = args.at(0)](PKB::StorageAccessInterface& pkb) {
															 return Relationship::UsesS::executeTrivialSynonymWildcardOrSynonym(pkb,
		                                                                                                                        statement);
														 },
	                                                     [statement = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
															 return Relationship::UsesS::executeSynonymSynonym(pkb, statement, variable);
														 }};
										 }}},
};

const unordered_map<Types::ArgumentDispatchKey, unordered_map<Types::ArgumentDispatchKey, Types::ExecutorSetFactoryBundle>>
	argument_dispatch_map = {{Types::ReferenceType::Name, name_map},          {Types::ReferenceType::StatementIndex, index_map},
                             {Types::DesignEntity::Procedure, procedure_map}, {Types::DesignEntity::Stmt, statement_map},
                             {Types::DesignEntity::Call, statement_map},      {Types::DesignEntity::Assign, statement_map},
                             {Types::DesignEntity::Print, statement_map},     {Types::DesignEntity::While, statement_map},
                             {Types::DesignEntity::If, statement_map}};
}  // namespace
}  // namespace QP::Relationship::Uses

#endif  // SPA_SRC_QP_RELATIONSHIP_USES_H
