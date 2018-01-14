#ifndef SPHERE_HEADER
#define SPHERE_HEADER

#include "Vector.h"
#include "Ray.h"
#include "Object.h"
#include "Color.h"

class Sphere : public Object {
	Vector m_center;
	double m_radius;

public:
	Sphere(Vector center, double radius, Color color = Color(), bool mirror = false, bool transparency = false, double indice = 1, bool diffuse = false): Object(color, mirror, transparency, indice, diffuse), m_center{center}, m_radius{radius} {}

	Vector getCenter() const { return m_center; }
	double getRadius() const { return m_radius; }

	Vector normal(const Vector&) const;

	double intersect(const Ray&) const;
};

#endif
