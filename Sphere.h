#ifndef SPHERE_HEADER
#define SPHERE_HEADER

#include "Vector.h"
#include "Object.h"
#include "Color.h"

class Sphere : public Object {
	Vector m_center;
	double m_radius;

public:
	Sphere(Vector center, double radius, Color color = Color::black()) : Object(color), m_center{center}, m_radius{radius} {}

	static Sphere Specular(Vector center, double radius, Color color = Color::black()) ;
	static Sphere Mirror(Vector center, double radius, Color color = Color::black());
	static Sphere Transparent(Vector center, double radius, Color color = Color::black(), double indice = 1.);
	static Sphere Diffuse(Vector center, double radius, Color color = Color::black());

	Vector getCenter() const { return m_center; }
	double getRadius() const { return m_radius; }

	Vector normal(const Vector&) const;

	double intersect(const Vector&, const Vector&) const;
};

#endif
