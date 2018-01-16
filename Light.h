#ifndef LIGHT_HEADER
#define LIGHT_HEADER

#include "Vector.h"

class Light {
	Vector m_origine;
	double m_intensity;

public:
	Light(Vector origine, double intensity): m_origine{origine}, m_intensity(intensity) {}

	Vector getOrigine() const { return m_origine; }
	double getIntensity() const { return m_intensity; }
};

#endif
