#include "QP/QueryProperties.h"

QueryProperties::QueryProperties(DeclarationList declarationList,
	                               Declaration select,
	                               SuchThatClauseList suchThatClauseList,
	                               PatternClauseList patternClauseList)
		: declarationList(declarationList),
		  select(select),
		  suchThatClauseList(suchThatClauseList),
		  patternClauseList(patternClauseList) {
}

DeclarationList QueryProperties::getDeclarationList() {
	return declarationList;
}

Declaration QueryProperties::getSelect() {
	return select;
}

SuchThatClauseList QueryProperties::getSuchThatClauseList() {
	return suchThatClauseList;
}

PatternClauseList QueryProperties::getPatternClauseList() {
	return patternClauseList;
}
