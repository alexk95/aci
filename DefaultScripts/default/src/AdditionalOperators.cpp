#include <AdditionalOperators.h>

bool aci::operator > (const QDateTime& _left, const QDateTime& _right) {
	if (_left.date() > _right.date()) { return true; }
	else if (_left.date() == _right.date()) {
		if (_left.time() > _right.time()) { return true; }
	}
	return false;
}

bool aci::operator < (const QDateTime& _left, const QDateTime& _right) {
	if (_left.date() < _right.date()) { return true; }
	else if (_left.date() == _right.date()) {
		if (_left.time() < _right.time()) { return true; }
	}
	return false;
}

bool aci::operator >= (const QDateTime& _left, const QDateTime& _right) {
	if (_left.date() > _right.date()) { return true; }
	else if (_left.date() == _right.date()) {
		if (_left.time() >= _right.time()) { return true; }
	}
	return false;
}

bool aci::operator <= (const QDateTime& _left, const QDateTime& _right) {
	if (_left.date() < _right.date()) { return true; }
	else if (_left.date() == _right.date()) {
		if (_left.time() <= _right.time()) { return true; }
	}
	return false;
}