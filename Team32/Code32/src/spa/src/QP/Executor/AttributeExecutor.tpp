#ifndef SPA_SRC_QP_EXECUTOR_ATTRIBUTEEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_ATTRIBUTEEXECUTOR_TPP

#include "QP/Executor/AttributeExecutor.h"

#include <cassert>

#include "Common/TypeDefs.h"

template <typename T>
T QP::Executor::AttributeExecutor::identity(const QP::StorageAdapter& /*store*/, const T& value) {
	return value;
}

template <QP::Types::ClauseType T>
QP::Types::Name QP::Executor::AttributeExecutor::statementToVariable(const QP::StorageAdapter& store, const QP::Types::Number& value) {
	auto variable_set = store.getVariableByStatement<T>(value);

	assert(variable_set.size() == 1);

	return *variable_set.begin();
}

#endif  // SPA_SRC_QP_EXECUTOR_ATTRIBUTEEXECUTOR_TPP