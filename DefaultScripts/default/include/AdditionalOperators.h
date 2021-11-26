#pragma once

#include <qdatetime.h>

namespace aci {
	bool operator > (const QDateTime& _left, const QDateTime& _right);

	bool operator < (const QDateTime& _left, const QDateTime& _right);

	bool operator >= (const QDateTime& _left, const QDateTime& _right);

	bool operator <= (const QDateTime& _left, const QDateTime& _right);
}
