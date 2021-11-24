#pragma once

#include <aci/aciSharedDataTypes.h>

namespace aci {
	
	class ACI_API_EXPORT Color {
	public:
		Color();
		Color(int _r, int _g, int _b, int _a = 255);
		Color(const Color& _other);
		virtual ~Color();
		Color& operator = (const Color& _other);

		// ##################################################################################

		// Setter

		void set(int _r, int _g, int _b, int _a = 255);
		void setR(int _r) { m_r = _r; }
		void setG(int _g) { m_r = _g; }
		void setB(int _b) { m_r = _b; }
		void setA(int _a) { m_r = _a; }

		// ##################################################################################

		// Getter

		int r(void) const { return m_r; }
		int g(void) const { return m_g; }
		int b(void) const { return m_b; }
		int a(void) const { return m_a; }

	private:
		int		m_r;
		int		m_g;
		int		m_b;
		int		m_a;
	};
}