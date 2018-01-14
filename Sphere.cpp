#include "Sphere.h"

#include "Vector.h"

#include <cmath>

Vector Sphere::normal(const Vector& point) const
{
	return (1./m_radius) * (point - m_center);
}

double Sphere::intersect(const Ray& ray) const
{
	double a = ray.getDirection().squaredNorm();
	double b = 2*dotProduct(ray.getDirection(), ray.getOrigine() - m_center);
	double c = squaredNorm(ray.getOrigine() - m_center) - m_radius*m_radius;

	double delta = b*b - 4*a*c;

	if (delta < 0)     // No intersection
		return -1;

	if (sqrt(delta) < b)  // Intersections are all behind the cam
		return -1;

	if (sqrt(delta) > -b) // Min intersection is behind the cam
		return (-b + sqrt(delta))/(2.*a);

	return (-b - sqrt(delta))/(2.*a);
}
