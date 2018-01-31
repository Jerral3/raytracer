#include "Vector.h"

#include <cmath>
Vector& Vector::operator^=(const Vector& v)
{
	double x = m_y*v.m_z - m_z*v.m_y;
	double y = m_z*v.m_x - m_x*v.m_z;
	double z = m_x*v.m_y - m_y*v.m_x;

	m_x = x, m_y = y, m_z = z;

	return *this;
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

Vector operator*(Matrix m, Vector v)
{
	double x = m.a().x() * v.x() + m.b().x() * v.y() + m.c().x() * v.z();
	double y = m.a().y() * v.x() + m.b().y() * v.y() + m.c().y() * v.z();
	double z = m.a().z() * v.x() + m.b().z() * v.y() + m.c().z() * v.z();

	return Vector(x, y, z);
}

Vector operator^(Vector v, Vector w)
{
	return v ^= w;
}

Vector max(const Vector& a, const Vector& b)
{
	return Vector(std::max(a.x(), b.x()), std::max(a.y(), b.y()), std::max(a.z(), b.z()));
}

Vector min(const Vector& a, const Vector& b)
{
	return Vector(std::min(a.x(), b.x()), std::min(a.y(), b.y()), std::min(a.z(), b.z()));
}

Vector operator-(Point m, Point n)
{
	return Vector(m.x() - n.x(), m.y() - n.y(), m.z() - n.z());
}

Point operator+(Point p, Vector v)
{
	return Point(p.x() + v.x(), p.y() + v.y(), p.z() + v.z());
}

Point max(const Point& a, const Point& b)
{
	return Point(std::max(a.x(), b.x()), std::max(a.y(), b.y()), std::max(a.z(), b.z()));
}

Point min(const Point& a, const Point& b)
{
	return Point(std::min(a.x(), b.x()), std::min(a.y(), b.y()), std::min(a.z(), b.z()));
}

Matrix operator+(Matrix m, Matrix n)
{
	return m += n;
}

Matrix operator-(Matrix m, Matrix n)
{
	return m -= n;
}

Matrix operator*(double const l, Matrix m)
{
	return Matrix(l*m.a(), l*m.b(), l*m.c());
}

Matrix operator*(Matrix m, Matrix n)
{
	return Matrix(m*n.a(), m*n.b(), m*n.c());
}
