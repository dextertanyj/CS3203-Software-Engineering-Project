#ifndef SPA_SRC_QP_EXECUTOR_ATTRIBUTEEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_ATTRIBUTEEXECUTOR_H

#include <unordered_set>

#include "QP/Executor/Executor.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"

namespace QP::Executor::AttributeExecutor {
// Constant extractors
Types::Name extractName(const Types::ReferenceArgument& argument);
Types::Number extractNumber(const Types::ReferenceArgument& argument);

// Select executors
std::unordered_set<Types::Number> selectStatements(const QP::StorageAdapter&, const Types::ReferenceArgument&);
std::unordered_set<Types::Number> selectConstants(const QP::StorageAdapter&, const Types::ReferenceArgument&);
std::unordered_set<Types::Name> selectProcedures(const QP::StorageAdapter&, const Types::ReferenceArgument&);
std::unordered_set<Types::Name> selectVariables(const QP::StorageAdapter&, const Types::ReferenceArgument&);

// Attribute mappers
template <typename T>
T identity(const QP::StorageAdapter& store, const T& value);
Types::Name callToProcedure(const QP::StorageAdapter& store, const StmtRef& value);
template <QP::Types::ClauseType T>
Types::Name statementToVariable(const QP::StorageAdapter& store, const StmtRef& value);
};

#endif  // SPA_SRC_QP_EXECUTOR_ATTRIBUTEEXECUTOR_H
