#ifndef SPA_SRC_QP_RELATIONSHIP_RELATIONSHIP_H
#define SPA_SRC_QP_RELATIONSHIP_RELATIONSHIP_H

namespace QP::Relationship {
class Follows;
class FollowsT;
class ModifiesP;
class ModifiesS;
class Parent;
class ParentT;
class Pattern;
class Relation;
class UsesP;
class UsesS;
class Calls;
class CallsT;
template <class T>
class CallDispatcherTemplate;
template <class T>
class ParentDispatcherTemplate;
namespace Modifies {};
}  // namespace QP::Relationship

#endif  // SPA_SRC_QP_RELATIONSHIP_RELATIONSHIP_H
