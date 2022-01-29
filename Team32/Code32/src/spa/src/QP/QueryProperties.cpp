#include "QueryProperties.h"

QueryProperties::QueryProperties(DeclarationList declarationList,
                                 Declaration select,
                                 SuchThatClauseList suchThatClauseList,
                                 PatternClauseList patternClauseList)
    : declarationList(declarationList),
      select(select),
      suchThatClauseList(suchThatClauseList),
      patternClauseList(patternClauseList) {
}
