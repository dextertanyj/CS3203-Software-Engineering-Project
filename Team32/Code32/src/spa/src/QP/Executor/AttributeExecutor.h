#ifndef SPA_SRC_QP_EXECUTOR_ATTRIBUTEEXECUTOR_H
#define SPA_SRC_QP_EXECUTOR_ATTRIBUTEEXECUTOR_H

#include <unordered_set>

#include "QP/ClauseArgument.h"
#include "QP/Executor/Executor.h"
#include "QP/StorageAdapter.h"
#include "QP/Types.h"

namespace QP::Executor::AttributeExecutor {

using namespace std;
// Constant extractors
unordered_set<Types::Name> extractName(const StorageAdapter& store, const ClauseArgument& argument);
unordered_set<Types::Number> extractNumber(const StorageAdapter& store, const ClauseArgument& argument);

// Select executors
unordered_set<Types::Number> selectStatements(const StorageAdapter& store, const ClauseArgument& argument);
unordered_set<Types::Number> selectConstants(const StorageAdapter& store, const ClauseArgument& argument);
unordered_set<Types::Name> selectProcedures(const StorageAdapter& store, const ClauseArgument& argument);
unordered_set<Types::Name> selectVariables(const StorageAdapter& store, const ClauseArgument& argument);

// Attribute mappers
template <typename T>
T identity(const StorageAdapter& /*store*/, const T& value);

Types::Name callToProcedure(const StorageAdapter& store, const Types::Number& value);

template <QP::Types::ClauseType T>
Types::Name statementToVariable(const StorageAdapter& store, const Types::Number& value);
}

#endif  // SPA_SRC_QP_EXECUTOR_ATTRIBUTEEXECUTOR_H
