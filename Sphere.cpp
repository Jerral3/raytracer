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


Vector Sphere::normal(const Vector& point) const
{
	return (1./m_radius) * (point - m_center);
}

double Sphere::intersect(const Vector& origine, const Vector& direction) const
{
	double a = direction.squaredNorm();
	double b = 2*dotProduct(direction, origine - m_center);
	double c = squaredNorm(origine - m_center) - m_radius*m_radius;

	double delta = b*b - 4*a*c;

	if (delta < 0)     // No intersection
		return -1;

	if (sqrt(delta) < b)  // Intersections are all behind the cam
		return -1;

	if (sqrt(delta) > -b) // Min intersection is behind the cam
		return (-b + sqrt(delta))/(2.*a);

	return (-b - sqrt(delta))/(2.*a);
}
