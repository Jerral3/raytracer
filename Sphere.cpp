#include "Sphere.h"

#include "Vector.h"

#include <cmath>

Sphere Sphere::Specular(Point center, double radius, Color color)
{
	Sphere sphere = Sphere(center, radius, color);
	sphere.makeSpecular();

	return sphere;
}

Sphere Sphere::Mirror(Point center, double radius, Color color)
{
	Sphere sphere = Sphere(center, radius, color);
	sphere.makeMirror();

	return sphere;
}

Sphere Sphere::Transparent(Point center, double radius, Color color, double indice)
{
	Sphere sphere = Sphere(center, radius, color);
	sphere.makeTransparent(indice);

	return sphere;
}

Sphere Sphere::Diffuse(Point center, double radius, Color color)
{
	Sphere sphere = Sphere(center, radius, color);
	sphere.makeDiffuse();

	return sphere;
}

Sphere Sphere::Emissive(Point center, double radius, Color color, double intensity)
{
	Sphere sphere = Sphere(center, radius, color);
	sphere.makeEmissive(intensity);

	return sphere;
}

Vector Sphere::normal(const Point& point) const
{
	return (1./m_radius) * (point - m_center);
}

double Sphere::area() const 
{
	return 4*M_PI;
	//return (M_PI*m_radius*m_radius);
}

double Sphere::intersect(const Point& origine, const Vector& direction, Intersection& intersection) const
{
	Vector lightDir = origine - m_center;

	// assumes direction is normalized: a = 1
	double b = 2*dotProduct(direction, lightDir);
	double c = squaredNorm(lightDir) - m_radius*m_radius;

	double delta = b*b - 4.*c;

	if (delta < 0)     // No intersection
		return -1.;

	double root = sqrt(delta);

	if (root < b)  // Intersections are all behind the cam
		return -1.;

	double t;

	if (root > -b) // Min intersection is behind the cam
		t = (-b + root)/2.;
	else
		t = (-b - root)/2.;

	Point point = origine + t*direction;

	intersection.intersection = point;
	intersection.normale      = normal(point);
	intersection.color        = color(intersection.normale);
	intersection.intersected  = this;

	return t;
}

Point Sphere::randomPointAround(const Vector& direction) const
{
	Vector wi = monteCarloVector(direction).normalize();

	return center() + m_radius * wi;
}
