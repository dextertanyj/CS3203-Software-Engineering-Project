#ifndef SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H
#define SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H

#include "QP/QP.h"
#include "QP/Types.h"

#define ASSIGN_WHILE_PATTERN_ARGUMENT_COUNT (2)
#define IF_PATTERN_ARGUMENT_COUNT (3)

namespace QP::Dispatcher::DispatchMap {

using namespace QP::Types;

extern const ArgumentDispatchMap dispatch_map;
extern const std::unordered_map<std::string, ClauseType> clause_map;
extern const std::unordered_map<ClauseType, std::vector<std::unordered_set<ReferenceType>>> syntax_map;
extern const std::unordered_map<size_t, std::vector<std::unordered_set<ReferenceType>>> pattern_syntax_map;
extern const std::unordered_map<DesignEntity, ClauseType> pattern_clause_map;
extern const std::unordered_map<std::string, AttributeToken> attribute_token_map;
extern const std::unordered_map<std::pair<DesignEntity, AttributeToken>, AttributeType> attribute_map;
extern const std::unordered_map<std::string, DesignEntity> design_entity_map;
};

#endif  // SPA_SRC_QP_DISPATCHER_DISPATCHMAP_H
