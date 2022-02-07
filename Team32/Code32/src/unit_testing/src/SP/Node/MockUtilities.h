#ifndef SPA_MOCKUTILITIES_H
#define SPA_MOCKUTILITIES_H

#include "Common/ArithmeticProcessor/ArithmeticExpression.h"
#include "SP/Node/RelationalExpressionNode.h"
#include "SP/Node/StatementListNode.h"

#include <string>

ArithmeticProcessor::ArithmeticExpression createArithmeticExpression(vector<string> str_list);
unique_ptr<RelationalExpressionNode> createRelationalExpression(string str);
unique_ptr<StatementListNode> createStatementList(string str, int statement_count);

#endif //SPA_MOCKUTILITIES_H
