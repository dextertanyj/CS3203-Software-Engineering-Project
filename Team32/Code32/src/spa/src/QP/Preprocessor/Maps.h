#ifndef SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H
#define SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H

#include "QP/QP.h"
#include "QP/Types.h"

namespace QP::Preprocessor::Maps {

using namespace QP::Types;

extern const ArgumentDispatchMap dispatch_map;
extern const std::unordered_map<std::string, ClauseType> clause_map;
extern const std::unordered_map<DesignEntity, ClauseType> pattern_clause_map;
extern const std::unordered_map<std::string, DesignEntity> design_entity_map;
extern const std::unordered_map<std::string, AttributeToken> attribute_token_map;
extern const std::unordered_map<std::pair<DesignEntity, AttributeToken>, AttributeType> attribute_map;
};

#endif  // SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H
