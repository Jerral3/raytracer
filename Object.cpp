#include "Object.h"

#include "Vector.h"

#include <random>
static std::default_random_engine engine;
static std::uniform_real_distribution<double> distrib(0,1);

Base Object::getBaseAt(const Vector& point) const
{
	Vector x = normal(point).normalize();
	Vector y = x.orthogonal().normalize();
	Vector z = (x ^ y).normalize();

	return Base {x, y, z};
}

Vector Object::monteCarloVector(const Vector& point) const
{
	Base base = getBaseAt(point);

	double r1 = distrib(engine);
	double r2 = distrib(engine);

	double x = std::cos(2*M_PI*r1)*std::sqrt(1-r2);
	double y = std::sin(2*M_PI*r1)*std::sqrt(1-r2);
	double z = std::sqrt(r2);

	return x*std::get<2>(base) + y*std::get<1>(base) + z*std::get<0>(base);
}
