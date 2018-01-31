#ifndef RAY_HEADER
#define RAY_HEADER

#include "Vector.h"

class Ray {
	Point  m_origine;
	Vector m_direction;

public:
	Ray(Point origine, Vector direction): m_origine{origine}, m_direction{direction.normalize()} {}

	Point  origine() const { return m_origine; }
	Vector direction() const { return m_direction; }
};

#endif
