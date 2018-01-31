#ifndef ATMOSPHERE_HEADER
#define ATMOSPHERE_HEADER

#include "Color.h"

class Atmosphere {
	double m_density;

public:
	Atmosphere(double density = 0.): m_density{density} {}

	Color contribution() const { return Color::black(); }
	double density() const { return m_density; }
};

#endif
