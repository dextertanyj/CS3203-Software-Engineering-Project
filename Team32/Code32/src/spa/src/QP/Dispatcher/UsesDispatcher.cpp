#include "UsesDispatcher.h"

#include <utility>

#include "DispatchProcessors.h"

using namespace QP::Executor;
using namespace QP::Types;

const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> name_map = {
	{ReferenceType::Name, pair{ClauseType::UsesP,
                               [](vector<ReferenceArgument> args) {
								   return [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
									   return ProcedureVariableExecutor<ClauseType::UsesP>::executeTrivialNameName(storage, procedure,
		                                                                                                           variable);
								   };
							   }}},
	{ReferenceType::Wildcard, pair{ClauseType::UsesP,
                                   [](vector<ReferenceArgument> args) {
									   return [procedure = args.at(0)](const QP::StorageAdapter& storage) {
										   return ProcedureVariableExecutor<ClauseType::UsesP>::executeTrivialNameWildcardOrSynonym(
											   storage, procedure);
									   };
								   }}},
	{DesignEntity::Variable,
     pair{ClauseType::UsesP,
          [](vector<ReferenceArgument> args) {
			  return pair{[procedure = args.at(0)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::UsesP>::executeTrivialNameWildcardOrSynonym(storage, procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::UsesP>::executeNameSynonym(storage, procedure, variable);
						  }};
		  }}},
};

const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> index_map = {
	{ReferenceType::Name, pair{ClauseType::UsesS,
                               [](vector<ReferenceArgument> args) {
								   return [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
									   return StatementVariableExecutor<ClauseType::UsesS>::executeTrivialIndexName(storage, procedure,
		                                                                                                            variable);
								   };
							   }}},
	{ReferenceType::Wildcard, pair{ClauseType::UsesS,
                                   [](vector<ReferenceArgument> args) {
									   return [procedure = args.at(0)](const QP::StorageAdapter& storage) {
										   return StatementVariableExecutor<ClauseType::UsesS>::executeTrivialIndexWildcardOrSynonym(
											   storage, procedure);
									   };
								   }}},
	{DesignEntity::Variable,
     pair{ClauseType::UsesS,
          [](vector<ReferenceArgument> args) {
			  return pair{[procedure = args.at(0)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor<ClauseType::UsesS>::executeTrivialIndexWildcardOrSynonym(storage, procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor<ClauseType::UsesS>::executeIndexSynonym(storage, procedure, variable);
						  }};
		  }}},
};

const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> procedure_map = {
	{ReferenceType::Name,
     pair{ClauseType::UsesP,
          [](vector<ReferenceArgument> args) {
			  return pair{[variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::UsesP>::executeTrivialSynonymName(storage, variable);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::UsesP>::executeSynonymName(storage, procedure, variable);
						  }};
		  }}},
	{ReferenceType::Wildcard,
     pair{ClauseType::UsesP,
          [](vector<ReferenceArgument> args) {
			  return pair{[](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::UsesP>::executeTrivialSynonymWildcardOrSynonym(storage);
						  },
	                      [procedure = args.at(0)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::UsesP>::executeSynonymWildcard(storage, procedure);
						  }};
		  }}},
	{DesignEntity::Variable,
     pair{ClauseType::UsesP,
          [](vector<ReferenceArgument> args) {
			  return pair{[](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::UsesP>::executeTrivialSynonymWildcardOrSynonym(storage);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::UsesP>::executeSynonymSynonym(storage, procedure, variable);
						  }};
		  }}},
};

const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> statement_map = {
	{ReferenceType::Name,
     pair{ClauseType::UsesS,
          [](vector<ReferenceArgument> args) {
			  return pair{[statement = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor<ClauseType::UsesS>::executeTrivialSynonymName(storage, statement, variable);
						  },
	                      [statement = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor<ClauseType::UsesS>::executeSynonymName(storage, statement, variable);
						  }};
		  }}},
	{ReferenceType::Wildcard,
     pair{ClauseType::UsesS,
          [](vector<ReferenceArgument> args) {
			  return pair{[statement = args.at(0)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor<ClauseType::UsesS>::executeTrivialSynonymWildcardOrSynonym(storage,
		                                                                                                                  statement);
						  },
	                      [statement = args.at(0)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor<ClauseType::UsesS>::executeSynonymWildcard(storage, statement);
						  }};
		  }}},
	{DesignEntity::Variable,
     pair{ClauseType::UsesS,
          [](vector<ReferenceArgument> args) {
			  return pair{[statement = args.at(0)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor<ClauseType::UsesS>::executeTrivialSynonymWildcardOrSynonym(storage,
		                                                                                                                  statement);
						  },
	                      [statement = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor<ClauseType::UsesS>::executeSynonymSynonym(storage, statement, variable);
						  }};
		  }}},
};

const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>> argument_dispatch_map = {
	{ReferenceType::Name, name_map},      {ReferenceType::StatementIndex, index_map}, {DesignEntity::Procedure, procedure_map},
	{DesignEntity::Stmt, statement_map},  {DesignEntity::Call, statement_map},        {DesignEntity::Assign, statement_map},
	{DesignEntity::Print, statement_map}, {DesignEntity::While, statement_map},       {DesignEntity::If, statement_map}};

const ArgumentDispatcher QP::Dispatcher::UsesDispatcher::dispatcher = [](vector<QP::Types::ReferenceArgument> args) {
	return QP::Dispatcher::DispatchProcessors::processDoubleArgument(argument_dispatch_map, move(args));
};