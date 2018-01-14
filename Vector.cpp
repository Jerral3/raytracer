#include "Vector.h"

#include <cmath>

Vector operator+(Vector v, Vector w)
{
	return v += w;
}

Vector operator-(Vector v, Vector w)
{
	return v -= w;
}

Vector operator*(double const l, Vector v)
{
	return v *= l;
}

Vector& Vector::operator^=(const Vector& v)
{
	double x = m_y*v.m_z - m_z*v.m_y;
	double y = m_z*v.m_x - m_x*v.m_z;
	double z = m_x*v.m_y - m_y*v.m_x;

	m_x = x, m_y = y, m_z = z;

	return *this;
}

Vector operator^(Vector v, Vector w)
{
	return v ^= w;
}

double squaredNorm(const Vector& v)
{
	return v.squaredNorm();
}

double dotProduct(const Vector& v, const Vector& w)
{
	return v.dotProduct(w);
}

Vector& Vector::normalize()
{
	*this *=  1/sqrt(squaredNorm());

	return *this;
}

Vector Vector::orthogonal() const
{
	double x, y, z;

	if (m_z != 0.) {
		x = 1.;
		y = 1.;
		z = -(m_x + m_y)/m_z;
	} else if (m_y != 0.) {
		x = 1.;
		z = 1.;
		y = -(m_x + m_z)/m_y;
	} else {
		y = 1.;
		z = 1.;
		x = -(m_y + m_z)/m_x;
	}

	return Vector(x, y, z);
}
