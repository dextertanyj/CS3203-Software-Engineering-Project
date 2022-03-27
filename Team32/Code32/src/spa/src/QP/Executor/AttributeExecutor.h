#ifndef SPA_SRC_QP_EXECUTOR_ATTRIBUTEEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_ATTRIBUTEEXECUTOR_H

#include <unordered_set>

#include "QP/Executor/Executor.h"
#include "QP/ReferenceArgument.h"
#include "QP/StorageAdapter.h"

namespace QP::Executor::AttributeExecutor {
// Constant extractors
std::unordered_set<Types::Name> extractName(const QP::StorageAdapter& store, const Types::ReferenceArgument& argument);
std::unordered_set<Types::Number> extractNumber(const QP::StorageAdapter& store, const Types::ReferenceArgument& argument);

// Select executors
std::unordered_set<Types::Number> selectStatements(const QP::StorageAdapter& store, const Types::ReferenceArgument& argument);
std::unordered_set<Types::Number> selectConstants(const QP::StorageAdapter& store, const Types::ReferenceArgument& argument);
std::unordered_set<Types::Name> selectProcedures(const QP::StorageAdapter& store, const Types::ReferenceArgument& argument);
std::unordered_set<Types::Name> selectVariables(const QP::StorageAdapter& store, const Types::ReferenceArgument& argument);

// Attribute mappers
template <typename T>
T identity(const QP::StorageAdapter& /*store*/, const T& value);

Types::Name callToProcedure(const QP::StorageAdapter& store, const Types::Number& value);

template <QP::Types::ClauseType T>
Types::Name statementToVariable(const QP::StorageAdapter& store, const Types::Number& value);
}

#endif  // SPA_SRC_QP_EXECUTOR_ATTRIBUTEEXECUTOR_H
