#pragma once

#include <aci/aciSharedDataTypes.h>

namespace aci {
	class ACI_API_EXPORT AbstractInterpreterNotifier {
	public:
		AbstractInterpreterNotifier() {}
		virtual ~AbstractInterpreterNotifier() {}

		virtual void shutdown(void) = 0;
	};
}