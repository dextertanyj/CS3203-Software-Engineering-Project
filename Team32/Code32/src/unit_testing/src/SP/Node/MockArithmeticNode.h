#ifndef SPA_MOCKARITHMETICNODE_H
#define SPA_MOCKARITHMETICNODE_H

#include "Common/ArithmeticProcessor/ArithmeticExpression.h"
#include "SP/Node/RelationalExpressionNode.h"
#include "SP/Node/StatementListNode.h"

#include <string>

ArithmeticProcessor::ArithmeticExpression getArithmeticExpression(vector<string> str_list);
unique_ptr<RelationalExpressionNode> getRelationalExpression(string str);
unique_ptr<StatementListNode> getStatementList(string str, int statement_count);

#endif //SPA_MOCKARITHMETICNODE_H
