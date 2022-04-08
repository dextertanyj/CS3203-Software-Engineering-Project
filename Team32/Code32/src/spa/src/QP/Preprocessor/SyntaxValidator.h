#ifndef SPA_SRC_QP_PREPROCESSOR_SYNTAXVALIDATOR_H
#define SPA_SRC_QP_PREPROCESSOR_SYNTAXVALIDATOR_H

#include <vector>

#include "QP/ClauseArgument.h"
#include "QP/Preprocessor/Preprocessor.h"
#include "QP/Types.h"

namespace QP::Preprocessor::SyntaxValidator {
bool validateArgumentsSyntaxDispatch(Types::ClauseType type, const std::vector<ClauseArgument>& arguments);
bool validateArgumentsSyntax(Types::ClauseType type, const std::vector<ClauseArgument>& arguments);
bool validatePatternArgumentsSyntax(const std::vector<ClauseArgument>& arguments);
};

#endif  // SPA_SRC_QP_PREPROCESSOR_SYNTAXVALIDATOR_H
