#ifndef SPA_MOCKLEXER_H
#define SPA_MOCKLEXER_H

#include <string>
#include <vector>

#include "Common/ExpressionProcessor/LexerInterface.h"

using namespace std;

class MockLexer : public Common::EP::LexerInterface {
public:
	explicit MockLexer(vector<string> tokens);
	string readToken() override;
	string peekToken() override;

private:
	vector<string> tokens;
	size_t index;
};

#endif  // SPA_MOCKLEXER_H
