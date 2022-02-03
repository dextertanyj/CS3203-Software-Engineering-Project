#include "SP/Node/ConstantNode.h"

#include "Common/Converter.h"
#include "SP/ParseException.h"

ConstantNode::ConstantNode(int value) : value(value) {}

unique_ptr<ConstantNode> ConstantNode::parseConstant(Lexer& lex) {
	string token = lex.read_token();
	try {
		int value = Converter::convertInteger(token);
		return make_unique<ConstantNode>(value);
	} catch (Converter::ConversionException& e) {
		throw ParseException("Invalid constant value");
	}
}