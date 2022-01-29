#ifndef SPA_VALIDATOR_H
#define SPA_VALIDATOR_H

#include <string>

using namespace std;

namespace Validator {
	bool validateName(const string& name);
	bool validateInteger(const string& integer);
}

#endif //SPA_VALIDATOR_H
