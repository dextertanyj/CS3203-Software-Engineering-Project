#ifndef SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H
#define SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H

#include <string>
#include <unordered_map>
#include <utility>

#include "QP/Preprocessor/Preprocessor.h"
#include "QP/Types.h"

namespace QP::Preprocessor::Maps {

using namespace QP::Types;

extern const ArgumentDispatchMap dispatch_map;
extern const std::unordered_map<std::string, ClauseType> such_that_clause_map;
extern const std::unordered_map<DesignEntity, ClauseType> pattern_clause_map;
extern const std::unordered_map<std::string, DesignEntity> design_entity_map;
extern const std::unordered_map<std::string, AttributeToken> attribute_token_map;
extern const std::unordered_map<std::pair<DesignEntity, AttributeToken>, AttributeType> attribute_map;
};

#endif  // SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H
