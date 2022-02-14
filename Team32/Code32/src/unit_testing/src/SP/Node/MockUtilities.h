#ifndef SPA_MOCKUTILITIES_H
#define SPA_MOCKUTILITIES_H

#include <memory>
#include <string>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "SP/Node/StatementListNode.h"
#include "SP/SP.h"

Common::ExpressionProcessor::Expression createArithmeticExpression(vector<string> str_list);
Common::ExpressionProcessor::Expression createConditionalExpression(vector<string> str_list);
unique_ptr<SP::Node::StatementListNode> createStatementList(string str, int statement_count);

#endif  // SPA_MOCKUTILITIES_H
