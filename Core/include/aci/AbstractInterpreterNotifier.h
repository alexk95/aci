#pragma once

#include <aci/aciSharedDataTypes.h>

namespace aci {
	class ACI_API_EXPORT AbstractInterpreterNotifier {
	public:
		AbstractInterpreterNotifier() {}
		virtual ~AbstractInterpreterNotifier() {}

		virtual void disableInput(void) = 0;
		virtual void disableInputAsync(void) = 0;
		virtual void enableInput(void) = 0;
		virtual void enableInputAsync(void) = 0;

		virtual void shutdown(void) = 0;
	};
}