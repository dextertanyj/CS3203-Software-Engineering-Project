#include "ModifiesDispatcher.h"

#include <utility>

#include "DispatchProcessors.h"

using namespace QP::Executor;
using namespace QP::Types;

const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> name_map = {
	{ReferenceType::Name, pair{ClauseType::ModifiesP,
                               [](const vector<ReferenceArgument>& args) {
								   return [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
									   return ProcedureVariableExecutor<ClauseType::ModifiesP>::executeTrivialNameName(storage, procedure,
		                                                                                                               variable);
								   };
							   }}},
	{ReferenceType::Wildcard, pair{ClauseType::ModifiesP,
                                   [](const vector<ReferenceArgument>& args) {
									   return [procedure = args.at(0)](const QP::StorageAdapter& storage) {
										   return ProcedureVariableExecutor<ClauseType::ModifiesP>::executeTrivialNameWildcardOrSynonym(
											   storage, procedure);
									   };
								   }}},
	{DesignEntity::Variable,
     pair{ClauseType::ModifiesP,
          [](const vector<ReferenceArgument>& args) {
			  return pair{[procedure = args.at(0)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::ModifiesP>::executeTrivialNameWildcardOrSynonym(storage,
		                                                                                                                   procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::ModifiesP>::executeNameSynonym(storage, procedure, variable);
						  }};
		  }}},
};

const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> index_map = {
	{ReferenceType::Name, pair{ClauseType::ModifiesS,
                               [](const vector<ReferenceArgument>& args) {
								   return [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
									   return StatementVariableExecutor<ClauseType::ModifiesS>::executeTrivialIndexName(storage, procedure,
		                                                                                                                variable);
								   };
							   }}},
	{ReferenceType::Wildcard, pair{ClauseType::ModifiesS,
                                   [](const vector<ReferenceArgument>& args) {
									   return [procedure = args.at(0)](const QP::StorageAdapter& storage) {
										   return StatementVariableExecutor<ClauseType::ModifiesS>::executeTrivialIndexWildcardOrSynonym(
											   storage, procedure);
									   };
								   }}},
	{DesignEntity::Variable,
     pair{ClauseType::ModifiesS,
          [](const vector<ReferenceArgument>& args) {
			  return pair{[procedure = args.at(0)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor<ClauseType::ModifiesS>::executeTrivialIndexWildcardOrSynonym(storage,
		                                                                                                                    procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor<ClauseType::ModifiesS>::executeIndexSynonym(storage, procedure, variable);
						  }};
		  }}},
};

const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> procedure_map = {
	{ReferenceType::Name,
     pair{ClauseType::ModifiesP,
          [](const vector<ReferenceArgument>& args) {
			  return pair{[variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::ModifiesP>::executeTrivialSynonymName(storage, variable);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::ModifiesP>::executeSynonymName(storage, procedure, variable);
						  }};
		  }}},
	{ReferenceType::Wildcard,
     pair{ClauseType::ModifiesP,
          [](const vector<ReferenceArgument>& args) {
			  return pair{[](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::ModifiesP>::executeTrivialSynonymWildcardOrSynonym(storage);
						  },
	                      [procedure = args.at(0)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::ModifiesP>::executeSynonymWildcard(storage, procedure);
						  }};
		  }}},
	{DesignEntity::Variable,
     pair{ClauseType::ModifiesP,
          [](const vector<ReferenceArgument>& args) {
			  return pair{[](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::ModifiesP>::executeTrivialSynonymWildcardOrSynonym(storage);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor<ClauseType::ModifiesP>::executeSynonymSynonym(storage, procedure, variable);
						  }};
		  }}},
};

const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> statement_map = {
	{ReferenceType::Name, pair{ClauseType::ModifiesS,
                               [](const vector<ReferenceArgument>& args) {
								   return pair{[statement = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
												   return StatementVariableExecutor<ClauseType::ModifiesS>::executeTrivialSynonymName(
													   storage, statement, variable);
											   },
	                                           [statement = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
												   return StatementVariableExecutor<ClauseType::ModifiesS>::executeSynonymName(
													   storage, statement, variable);
											   }};
							   }}},
	{ReferenceType::Wildcard,
     pair{ClauseType::ModifiesS,
          [](const vector<ReferenceArgument>& args) {
			  return pair{[statement = args.at(0)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor<ClauseType::ModifiesS>::executeTrivialSynonymWildcardOrSynonym(storage,
		                                                                                                                      statement);
						  },
	                      [statement = args.at(0)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor<ClauseType::ModifiesS>::executeSynonymWildcard(storage, statement);
						  }};
		  }}},
	{DesignEntity::Variable,
     pair{ClauseType::ModifiesS,
          [](const vector<ReferenceArgument>& args) {
			  return pair{[statement = args.at(0)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor<ClauseType::ModifiesS>::executeTrivialSynonymWildcardOrSynonym(storage,
		                                                                                                                      statement);
						  },
	                      [statement = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor<ClauseType::ModifiesS>::executeSynonymSynonym(storage, statement, variable);
						  }};
		  }}},
};

const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>> argument_dispatch_map = {
	{ReferenceType::Name, name_map},     {ReferenceType::StatementIndex, index_map}, {DesignEntity::Procedure, procedure_map},
	{DesignEntity::Stmt, statement_map}, {DesignEntity::Call, statement_map},        {DesignEntity::Assign, statement_map},
	{DesignEntity::Read, statement_map}, {DesignEntity::While, statement_map},       {DesignEntity::If, statement_map}};

const ArgumentDispatcher QP::Dispatcher::ModifiesDispatcher::dispatcher = [](const vector<QP::Types::ReferenceArgument>& args) {
	return QP::Dispatcher::DispatchProcessors::processDoubleArgument(argument_dispatch_map, args);
};