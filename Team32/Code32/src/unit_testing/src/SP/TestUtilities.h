#ifndef SPA_TESTUTILITIES_H
#define SPA_TESTUTILITIES_H

#include <memory>
#include <string>
#include <vector>

#include "../Common/ExpressionProcessor/MockLexer.h"
#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "SP/Node/StatementListNode.h"
#include "SP/SP.h"

using namespace std;

namespace SP::TestUtilities {
inline Common::EP::Expression createArithmeticExpression(vector<string> str_list) {
	MockLexer lex = MockLexer(move(str_list));
	Common::EP::Expression expression =
		Common::EP::ExpressionParser{lex, Common::EP::ExpressionType::Arithmetic}.parse();
	return expression;
};

inline Common::EP::Expression createRelationalExpression(vector<string> str_list) {
	MockLexer lex = MockLexer(move(str_list));
	Common::EP::Expression expression =
		Common::EP::ExpressionParser{lex, Common::EP::ExpressionType::Relational}.parse();
	return expression;
};

inline Common::EP::Expression createConditionalExpression(vector<string> str_list) {
	MockLexer lex = MockLexer(move(str_list));
	Common::EP::Expression expression =
		Common::EP::ExpressionParser{lex, Common::EP::ExpressionType::Logical}.parse();
	return expression;
};

inline unique_ptr<SP::Node::StatementListNode> createStatementList(string str, StmtRef statement_count) {
	SP::Lexer lex;
	lex.initialize(move(str));
	unique_ptr<SP::Node::StatementListNode> node = SP::Node::StatementListNode::parse(lex, statement_count);
	return node;
};
}  // namespace SP::TestUtilities

#endif  // SPA_TESTUTILITIES_H
