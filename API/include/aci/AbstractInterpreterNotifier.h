#pragma once

#include "aciAPIsharedTypes.h"

class ACI_API_EXPORT AbstractInterpreterNotifier {
public:
	AbstractInterpreterNotifier() {}
	virtual ~AbstractInterpreterNotifier() {}

	virtual void shutdown(void) = 0;
};