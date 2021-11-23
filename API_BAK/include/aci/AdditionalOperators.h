#pragma once

#include "aciAPIsharedTypes.h"
#include <qdatetime.h>

namespace aci {
	ACI_API_EXPORT bool operator > (const QDateTime& _left, const QDateTime& _right);

	ACI_API_EXPORT bool operator < (const QDateTime& _left, const QDateTime& _right);

	ACI_API_EXPORT bool operator >= (const QDateTime& _left, const QDateTime& _right);

	ACI_API_EXPORT bool operator <= (const QDateTime& _left, const QDateTime& _right);
}
