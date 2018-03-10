#include "Object.h"

#include "Vector.h"

#include <random>
static std::default_random_engine engine;
static std::uniform_real_distribution<double> distrib(0,1);

Color Object::color(Vector normale) const
{
	if (normale == Vector())
		return m_color;

	return m_texture(normale) * m_color;
}

/* assume x is normalized */
Base Object::baseAt(Vector x) const
{
	Vector y = x.orthogonal().normalize();
	Vector z = (x ^ y);

	return Base {x, y, z};
}

/* assumes direction is normalized */
Vector Object::monteCarloVector(const Vector& direction) const
{
	Base base = baseAt(direction);

	double r1 = distrib(engine);
	double r2 = distrib(engine);

	double root = std::sqrt(1-r2);

	double x = std::cos(2*M_PI*r1)*root;
	double y = std::sin(2*M_PI*r1)*root;
	double z = std::sqrt(r2);

	return x*std::get<2>(base) + y*std::get<1>(base) + z*std::get<0>(base);
}

/* assumes direction is normalized */
Vector Object::phongVector(const Vector& direction, double phong) const
{
	Base base = baseAt(direction);

	double r1 = distrib(engine);
	double r2 = distrib(engine);

	double root = std::sqrt(1 -std::pow(r2, 2./(phong+1)));

	double x = std::cos(2*M_PI*r1)*root;
	double y = std::sin(2*M_PI*r1)*root;
	double z = std::pow(r2, 1./(phong + 1));

	return x*std::get<2>(base) + y*std::get<1>(base) + z*std::get<0>(base);
}

Color one(const Vector&) { return Color::white(); }
