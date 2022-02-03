#ifndef SPA_PARSEEXCEPTION_H
#define SPA_PARSEEXCEPTION_H

using namespace std;

struct ParseException : public runtime_error {
	using runtime_error::runtime_error;
};


#endif  // SPA_PARSEEXCEPTION_H
