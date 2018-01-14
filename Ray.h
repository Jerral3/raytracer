#ifndef RAY_HEADER
#define RAY_HEADER

#include "Vector.h"

#include <cmath>

class Ray {
	Vector m_origine;
	Vector m_direction;

public:
	Ray(Vector origine, Vector direction): m_origine{origine}, m_direction{direction.normalize()} {}

	Vector getOrigine() const { return m_origine; }
	Vector getDirection() const { return m_direction; }
};

#endif
