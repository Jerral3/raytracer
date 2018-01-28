#include "Sphere.h"

#include "Vector.h"

#include <cmath>

Sphere Sphere::Specular(Vector center, double radius, Color color)
{
	Sphere sphere = Sphere(center, radius, color);
	sphere.makeSpecular();

	return sphere;
}

Sphere Sphere::Mirror(Vector center, double radius, Color color)
{
	Sphere sphere = Sphere(center, radius, color);
	sphere.makeMirror();

	return sphere;
}

Sphere Sphere::Transparent(Vector center, double radius, Color color, double indice)
{
	Sphere sphere = Sphere(center, radius, color);
	sphere.makeTransparent(indice);

	return sphere;
}

Sphere Sphere::Diffuse(Vector center, double radius, Color color)
{
	Sphere sphere = Sphere(center, radius, color);
	sphere.makeDiffuse();

	return sphere;
}

Sphere Sphere::Emissive(Vector center, double radius, Color color, double intensity)
{
	Sphere sphere = Sphere(center, radius, color);
	sphere.makeEmissive(intensity);

	return sphere;
}

Vector Sphere::normal(const Vector& point) const
{
	return (1./m_radius) * (point - m_center);
}

double Sphere::intersect(const Vector& origine, const Vector& direction, Vector* n, Vector* point, Color* color) const
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

	*point = origine + t*direction;
	*n     = normal(*point);
	*color = m_color;

	return t;
}

double Sphere::area() const 
{
	return (4*M_PI);
	//return (4*M_PI*m_radius*m_radius);
}

Vector Sphere::randomPointAround(const Vector& direction) const
{
	Vector wi = monteCarloVector(direction).normalize();

	return getCenter() + m_radius * wi;
}
