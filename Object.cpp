#include "Object.h"

#include "Vector.h"

#include <random>
static std::default_random_engine engine;
static std::uniform_real_distribution<double> distrib(0,1);

/* assume x is normalized */
Base Object::getBaseAt(Vector x) const
{
	Vector y = x.orthogonal().normalize();
	Vector z = (x ^ y);

	return Base {x, y, z};
}

/* assumes direction is normalized */
Vector Object::monteCarloVector(const Vector& direction) const
{
	Base base = getBaseAt(direction);

	double r1 = distrib(engine);
	double r2 = distrib(engine);

	double root = std::sqrt(1-r2);

	double x = std::cos(2*M_PI*r1)*root;
	double y = std::sin(2*M_PI*r1)*root;
	double z = std::sqrt(r2);

	return x*std::get<2>(base) + y*std::get<1>(base) + z*std::get<0>(base);
}
