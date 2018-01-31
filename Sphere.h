#ifndef SPHERE_HEADER
#define SPHERE_HEADER

#include "Vector.h"
#include "Object.h"
#include "Color.h"
#include "Intersection.h"

class Sphere : public Object {
	Point m_center;
	double m_radius;

public:
	Sphere(Point center, double radius, Color color = Color::black()) : Object(color), m_center{center}, m_radius{radius} {}

	static Sphere Specular(Point center, double radius, Color color = Color::black()) ;
	static Sphere Mirror(Point center, double radius, Color color = Color::black());
	static Sphere Transparent(Point center, double radius, Color color = Color::black(), double indice = 1.5);
	static Sphere Diffuse(Point center, double radius, Color color = Color::black());
	static Sphere Emissive(Point center, double radius, Color color = Color::black(), double intensity = 1000.);

	Point  center() const { return m_center; }
	double radius() const { return m_radius; }
	double area() const;

	Vector normal(const Point&) const;
	double intersect(const Point&, const Vector&, Intersection&) const;
    Point randomPointAround(const Vector&) const;
};

#endif
