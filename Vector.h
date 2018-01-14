#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include <iostream>
#include <tuple>

class Vector {
	double m_x;
	double m_y;
	double m_z;

public:
	Vector(double x, double y, double z): m_x{x}, m_y{y}, m_z{z} {}

	double squaredNorm() const {return dotProduct(*this); }
	double dotProduct(const Vector& v) const { return m_x*v.m_x + m_y*v.m_y + m_z*v.m_z; }

	Vector& operator+=(const Vector& v) { m_x += v.m_x, m_y += v.m_y, m_z += v.m_z; return *this; }
	Vector& operator-=(const Vector& v) { m_x -= v.m_x, m_y -= v.m_y, m_z -= v.m_z; return *this; }
	Vector& operator*=(const double l) { m_x *= l,   m_y *= l,   m_z *= l;   return *this;  }
	Vector& operator/=(const double l) { m_x /= l,   m_y /= l,   m_z /= l;   return *this; }
	Vector& operator^=(const Vector& v);

	Vector orthogonal() const;
	Vector& normalize();

	void print() const { std::cout << "(" << m_x << ", " << m_y << ", " << m_z << ")"; }
};

typedef std::tuple<Vector, Vector, Vector> Base;

Vector operator+(Vector v, Vector w);
Vector operator-(Vector v, Vector w);
Vector operator*(double const l, Vector v);
Vector operator^(Vector v, Vector w);

double squaredNorm(const Vector& v);
double dotProduct(const Vector& v, const Vector& w);

#endif
