#ifndef SPA_MOCKLEXER_H
#define SPA_MOCKLEXER_H

#include <string>
#include <vector>

#include "Common/ExpressionProcessor/LexerInterface.h"

using namespace std;

class MockLexer : public Common::ExpressionProcessor::LexerInterface {
public:
	MockLexer(vector<string> tokens);
	string readToken() override;
	string peekToken() override;

private:
	vector<string> tokens;
	int index;
};

#endif  // SPA_MOCKLEXER_H
