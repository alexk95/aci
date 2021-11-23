#include <aci/API.h>
#include <aci/InterpreterCore.h>
#include <cassert>

bool initialized{ false };

bool aci::API::initialize(AbstractPrinter * _printer, AbstractInterpreterNotifier * _notifier) {
	if (initialized) {
		assert(0);		// API already initialized
		return false;
	}
	if (_printer == nullptr) {
		assert(0);		// No valid printer provided
		return false;
	}
	if (_notifier == nullptr) {
		assert(0);		// No valid notifier provided
		return false;
	}

	InterpreterCore::instance()->attachNotifier(_notifier);
	InterpreterCore::instance()->attachPrinter(_printer);
	return true;
}

void aci::API::cleanUp(void) {
	if (!initialized) {

	}
}

aci::InterpreterCore * aci::API::core(void) {
	if (!initialized) {
		assert(0);		// API not initialized
		return nullptr;
	}
	return InterpreterCore::instance();
}