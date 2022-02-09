#ifndef SPA_MOCKUTILITIES_H
#define SPA_MOCKUTILITIES_H

#include <memory>
#include <string>
#include <vector>

#include "Common/ArithmeticProcessor/ArithmeticExpression.h"
#include "SP/Node/RelationalExpressionNode.h"
#include "SP/Node/StatementListNode.h"
#include "SP/SP.h"

class MockCENode : public SP::Node::ConditionalExpressionNode {
public:
	MockCENode(SP::UsageInfo data, int& ctr) : data(move(data)), ctr(ctr){};
	SP::UsageInfo extract() override {
		ctr++;
		return data;
	};
	bool equals(shared_ptr<ConditionalExpressionNode> object) override { return false; };
	~MockCENode() override = default;

private:
	SP::UsageInfo data;
	int& ctr;
};

Common::ArithmeticProcessor::ArithmeticExpression createArithmeticExpression(vector<string> str_list);
unique_ptr<SP::Node::RelationalExpressionNode> createRelationalExpression(string str);
unique_ptr<SP::Node::StatementListNode> createStatementList(string str, int statement_count);

#endif  // SPA_MOCKUTILITIES_H
