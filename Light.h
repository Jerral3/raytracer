#ifndef LIGHT_HEADER
#define LIGHT_HEADER

#include "Vector.h"

class Light {
	Point m_origine;
	double m_intensity;

public:
	Light(Point origine, double intensity): m_origine{origine}, m_intensity(intensity) {}

	Point  origine() const { return m_origine; }
	double intensity() const { return m_intensity; }
};

#endif
