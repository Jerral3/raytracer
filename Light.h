#ifndef LIGHT_HEADER
#define LIGHT_HEADER

#include "Vector.h"
#include "Color.h"

class Light {
	Vector m_origine;

	Color m_color;

public:
	Light(Vector origine, Color color): m_origine{origine}, m_color(color) {}

	Vector getOrigine() const { return m_origine; }
	Color getColor() const { return m_color; }
};

#endif
