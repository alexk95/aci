#include <aci/Color.h>

aci::Color::Color() : m_r(0), m_g(0), m_b(0), m_a(255) {}

aci::Color::Color(int _r, int _g, int _b, int _a) : m_r(_r), m_g(_g), m_b(_b), m_a(_a) {}

aci::Color::Color(const Color& _other)  : m_r(_other.m_r), m_g(_other.m_g), m_b(_other.m_b), m_a(_other.m_a) {}

aci::Color::~Color() {}

aci::Color& aci::Color::operator = (const Color& _other) {
	m_r = _other.m_r;
	m_g = _other.m_g;
	m_b = _other.m_b;
	m_a = _other.m_a;
	return *this;
}

// ##################################################################################

// Setter

void aci::Color::set(int _r, int _g, int _b, int _a) {
	m_r = _r;
	m_g = _g;
	m_b = _b;
	m_a = _a;
}