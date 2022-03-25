#include "QP/Dispatcher/ModifiesDispatcher.h"

#include <utility>

#include "QP/Dispatcher/DispatchProcessors.tpp"

using namespace QP::Executor;
using namespace QP::Types;

const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> name_map = {
	{ReferenceType::Name, pair{ClauseType::ModifiesP,
                               [](const vector<ReferenceArgument>& args) {
								   return [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
									   return ProcedureVariableExecutor::executeTrivialNameName<ClauseType::ModifiesP>(storage, procedure,
		                                                                                                               variable);
								   };
							   }}},
	{ReferenceType::Wildcard, pair{ClauseType::ModifiesP,
                                   [](const vector<ReferenceArgument>& args) {
									   return [procedure = args.at(0)](const QP::StorageAdapter& storage) {
										   return ProcedureVariableExecutor::executeTrivialNameWildcardOrSynonym<ClauseType::ModifiesP>(
											   storage, procedure);
									   };
								   }}},
	{DesignEntity::Variable,
     pair{ClauseType::ModifiesP,
          [](const vector<ReferenceArgument>& args) {
			  return pair{[procedure = args.at(0)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor::executeTrivialNameWildcardOrSynonym<ClauseType::ModifiesP>(storage,
		                                                                                                                   procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor::executeNameSynonym<ClauseType::ModifiesP>(storage, procedure, variable);
						  }};
		  }}},
};

const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> index_map = {
	{ReferenceType::Name, pair{ClauseType::ModifiesS,
                               [](const vector<ReferenceArgument>& args) {
								   return [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
									   return StatementVariableExecutor::executeTrivialIndexName<ClauseType::ModifiesS>(storage, procedure,
		                                                                                                                variable);
								   };
							   }}},
	{ReferenceType::Wildcard, pair{ClauseType::ModifiesS,
                                   [](const vector<ReferenceArgument>& args) {
									   return [procedure = args.at(0)](const QP::StorageAdapter& storage) {
										   return StatementVariableExecutor::executeTrivialIndexWildcardOrSynonym<ClauseType::ModifiesS>(
											   storage, procedure);
									   };
								   }}},
	{DesignEntity::Variable,
     pair{ClauseType::ModifiesS,
          [](const vector<ReferenceArgument>& args) {
			  return pair{[procedure = args.at(0)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor::executeTrivialIndexWildcardOrSynonym<ClauseType::ModifiesS>(storage,
		                                                                                                                    procedure);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor::executeIndexSynonym<ClauseType::ModifiesS>(storage, procedure, variable);
						  }};
		  }}},
};

const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> procedure_map = {
	{ReferenceType::Name,
     pair{ClauseType::ModifiesP,
          [](const vector<ReferenceArgument>& args) {
			  return pair{[variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor::executeTrivialSynonymName<ClauseType::ModifiesP>(storage, variable);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor::executeSynonymName<ClauseType::ModifiesP>(storage, procedure, variable);
						  }};
		  }}},
	{ReferenceType::Wildcard,
     pair{ClauseType::ModifiesP,
          [](const vector<ReferenceArgument>& args) {
			  return pair{[](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor::executeTrivialSynonymWildcardOrSynonym<ClauseType::ModifiesP>(storage);
						  },
	                      [procedure = args.at(0)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor::executeSynonymWildcard<ClauseType::ModifiesP>(storage, procedure);
						  }};
		  }}},
	{DesignEntity::Variable,
     pair{ClauseType::ModifiesP,
          [](const vector<ReferenceArgument>& args) {
			  return pair{[](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor::executeTrivialSynonymWildcardOrSynonym<ClauseType::ModifiesP>(storage);
						  },
	                      [procedure = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return ProcedureVariableExecutor::executeSynonymSynonym<ClauseType::ModifiesP>(storage, procedure, variable);
						  }};
		  }}},
};

const unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle> statement_map = {
	{ReferenceType::Name, pair{ClauseType::ModifiesS,
                               [](const vector<ReferenceArgument>& args) {
								   return pair{[statement = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
												   return StatementVariableExecutor::executeTrivialSynonymName<ClauseType::ModifiesS>(
													   storage, statement, variable);
											   },
	                                           [statement = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
												   return StatementVariableExecutor::executeSynonymName<ClauseType::ModifiesS>(
													   storage, statement, variable);
											   }};
							   }}},
	{ReferenceType::Wildcard,
     pair{ClauseType::ModifiesS,
          [](const vector<ReferenceArgument>& args) {
			  return pair{[statement = args.at(0)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor::executeTrivialSynonymWildcardOrSynonym<ClauseType::ModifiesS>(storage,
		                                                                                                                      statement);
						  },
	                      [statement = args.at(0)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor::executeSynonymWildcard<ClauseType::ModifiesS>(storage, statement);
						  }};
		  }}},
	{DesignEntity::Variable,
     pair{ClauseType::ModifiesS,
          [](const vector<ReferenceArgument>& args) {
			  return pair{[statement = args.at(0)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor::executeTrivialSynonymWildcardOrSynonym<ClauseType::ModifiesS>(storage,
		                                                                                                                      statement);
						  },
	                      [statement = args.at(0), variable = args.at(1)](const QP::StorageAdapter& storage) {
							  return StatementVariableExecutor::executeSynonymSynonym<ClauseType::ModifiesS>(storage, statement, variable);
						  }};
		  }}},
};

const unordered_map<ArgumentDispatchKey, unordered_map<ArgumentDispatchKey, ExecutorSetFactoryBundle>> argument_dispatch_map = {
	{ReferenceType::Name, name_map},     {ReferenceType::StatementIndex, index_map}, {DesignEntity::Procedure, procedure_map},
	{DesignEntity::Stmt, statement_map}, {DesignEntity::Call, statement_map},        {DesignEntity::Assign, statement_map},
	{DesignEntity::Read, statement_map}, {DesignEntity::While, statement_map},       {DesignEntity::If, statement_map}};

const ArgumentDispatcher QP::Dispatcher::ModifiesDispatcher::dispatcher = [](const vector<QP::Types::ReferenceArgument>& args) {
	return QP::Dispatcher::DispatchProcessors::processArgument(argument_dispatch_map, args);
};