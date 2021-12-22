#include <aci/Color.h>
#include <cassert>

aci::Color::Color() : m_r(0), m_g(0), m_b(0), m_a(255) {}

aci::Color::Color(int _r, int _g, int _b, int _a) : m_r(_r), m_g(_g), m_b(_b), m_a(_a) {}

aci::Color::Color(DefaultColor _color) {
	set(_color);
}

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

void aci::Color::set(DefaultColor _color) {
	switch (_color)
	{
	case aci::Color::BLACK:		set(0, 0, 0, 255); break;
	case aci::Color::WHITE:		set(255, 255, 255, 255); break;
	case aci::Color::RED:		set(255, 0, 0, 255); break;
	case aci::Color::GREEN:		set(0, 255, 0, 255); break;
	case aci::Color::BLUE:		set(0, 0, 255, 255); break;
	case aci::Color::YELLOW:	set(255, 255, 0, 255); break;
	case aci::Color::ORANGE:	set(255, 128, 0, 255); break;
	case aci::Color::AQUA:		set(0, 255, 255, 255); break;
	case aci::Color::LIME:		set(128, 255, 0, 255); break;
	case aci::Color::PURPLE:	set(255, 0, 255, 255); break;
	default:
		assert(0);
		break;
	}
}