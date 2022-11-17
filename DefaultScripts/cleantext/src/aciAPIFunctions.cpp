#include <cleantext.h>

extern "C" {
	__declspec(dllexport) aci::InterpreterObject ** generateObjects(int & _count) {
		_count = 1;
		aci::InterpreterObject ** ret = new aci::InterpreterObject *[1];
		ret[0] = new cleantext;
		return ret;
	}
}