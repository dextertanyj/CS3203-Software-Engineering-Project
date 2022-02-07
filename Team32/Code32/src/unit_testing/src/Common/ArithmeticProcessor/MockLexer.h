#ifndef SPA_MOCKLEXER_H
#define SPA_MOCKLEXER_H

#include "Common/ArithmeticProcessor/LexerInterface.h"

#include <string>
#include <vector>

using namespace std;

class MockLexer : public Common::ArithmeticProcessor::LexerInterface {
public:
	MockLexer(vector<string> tokens);
	string readToken() override;
	string peekToken() override;

private:
	vector<string> tokens;
	int index;
};

#endif  // SPA_MOCKLEXER_H
