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
extern const QP::Types::ArgumentDispatcher dispatcher;

namespace {
const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> name_map = {
	{QP::Types::ReferenceType::Name, pair{QP::Types::ClauseType::UsesP,
                                          [](vector<QP::Types::ReferenceArgument> args) {
											  return [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
												  return QP::Relationship::UsesP::executeTrivialNameName(pkb, procedure, variable);
											  };
										  }}},
	{QP::Types::ReferenceType::Wildcard, pair{QP::Types::ClauseType::UsesP,
                                              [](vector<QP::Types::ReferenceArgument> args) {
												  return [procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
													  return QP::Relationship::UsesP::executeTrivialNameWildcardOrSynonym(pkb, procedure);
												  };
											  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::UsesP,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
							  return QP::Relationship::UsesP::executeTrivialNameWildcardOrSynonym(pkb, procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
							  return QP::Relationship::UsesP::executeNameSynonym(pkb, procedure, variable);
						  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> index_map = {
	{QP::Types::ReferenceType::Name, pair{QP::Types::ClauseType::UsesS,
                                          [](vector<QP::Types::ReferenceArgument> args) {
											  return [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
												  return QP::Relationship::UsesS::executeTrivialIndexName(pkb, procedure, variable);
											  };
										  }}},
	{QP::Types::ReferenceType::Wildcard, pair{QP::Types::ClauseType::UsesS,
                                              [](vector<QP::Types::ReferenceArgument> args) {
												  return [procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
													  return QP::Relationship::UsesS::executeTrivialIndexWildcardOrSynonym(pkb, procedure);
												  };
											  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::UsesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
							  return QP::Relationship::UsesS::executeTrivialIndexWildcardOrSynonym(pkb, procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
							  return QP::Relationship::UsesS::executeIndexSynonym(pkb, procedure, variable);
						  }};
		  }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> procedure_map = {
	{QP::Types::ReferenceType::Name, pair{QP::Types::ClauseType::UsesP,
                                          [](vector<QP::Types::ReferenceArgument> args) {
											  return pair{
												  [variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
													  return QP::Relationship::UsesP::executeTrivialSynonymName(pkb, variable);
												  },
												  [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
													  return QP::Relationship::UsesP::executeSynonymName(pkb, procedure, variable);
												  }};
										  }}},
	{QP::Types::ReferenceType::Wildcard, pair{QP::Types::ClauseType::UsesP,
                                              [](vector<QP::Types::ReferenceArgument> args) {
												  return pair{
													  [](PKB::StorageAccessInterface& pkb) {
														  return QP::Relationship::UsesP::executeTrivialSynonymWildcardOrSynonym(pkb);
													  },
													  [procedure = args.at(0)](PKB::StorageAccessInterface& pkb) {
														  return QP::Relationship::UsesP::executeSynonymWildcard(pkb, procedure);
													  }};
											  }}},
	{QP::Types::DesignEntity::Variable, pair{QP::Types::ClauseType::UsesP,
                                             [](vector<QP::Types::ReferenceArgument> args) {
												 return pair{
													 [](PKB::StorageAccessInterface& pkb) {
														 return QP::Relationship::UsesP::executeTrivialSynonymWildcardOrSynonym(pkb);
													 },
													 [procedure = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
														 return QP::Relationship::UsesP::executeSynonymSynonym(pkb, procedure, variable);
													 }};
											 }}},
};

const unordered_map<QP::Types::ArgumentDispatchKey, QP::Types::ExecutorSetFactoryBundle> statement_map = {
	{QP::Types::ReferenceType::Name, pair{QP::Types::ClauseType::UsesS,
                                          [](vector<QP::Types::ReferenceArgument> args) {
											  return pair{
												  [statement = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
													  return QP::Relationship::UsesS::executeTrivialSynonymName(pkb, statement, variable);
												  },
												  [statement = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
													  return QP::Relationship::UsesS::executeSynonymName(pkb, statement, variable);
												  }};
										  }}},
	{QP::Types::ReferenceType::Wildcard, pair{QP::Types::ClauseType::UsesS,
                                              [](vector<QP::Types::ReferenceArgument> args) {
												  return pair{[statement = args.at(0)](PKB::StorageAccessInterface& pkb) {
																  return QP::Relationship::UsesS::executeTrivialSynonymWildcardOrSynonym(
																	  pkb, statement);
															  },
	                                                          [statement = args.at(0)](PKB::StorageAccessInterface& pkb) {
																  return QP::Relationship::UsesS::executeSynonymWildcard(pkb, statement);
															  }};
											  }}},
	{QP::Types::DesignEntity::Variable,
     pair{QP::Types::ClauseType::UsesS,
          [](vector<QP::Types::ReferenceArgument> args) {
			  return pair{[statement = args.at(0)](PKB::StorageAccessInterface& pkb) {
							  return QP::Relationship::UsesS::executeTrivialSynonymWildcardOrSynonym(pkb, statement);
						  },
	                      [statement = args.at(0), variable = args.at(1)](PKB::StorageAccessInterface& pkb) {
							  return QP::Relationship::UsesS::executeSynonymSynonym(pkb, statement, variable);
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
