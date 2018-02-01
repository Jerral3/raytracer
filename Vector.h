#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include <iostream>
#include <tuple>

class Vector {
	double m_x;
	double m_y;
	double m_z;

public:
	Vector(double x = 0, double y = 0, double z = 0): m_x{x}, m_y{y}, m_z{z} {}

	static Vector randomVector();

	Vector& operator+=(const Vector& v) { m_x += v.m_x, m_y += v.m_y, m_z += v.m_z; return *this; }
	Vector& operator-=(const Vector& v) { m_x -= v.m_x, m_y -= v.m_y, m_z -= v.m_z; return *this; }
	Vector& operator*=(const double l) { m_x *= l,   m_y *= l,   m_z *= l;   return *this;  }
	Vector& operator/=(const double l) { m_x /= l,   m_y /= l,   m_z /= l;   return *this; }
	Vector& operator^=(const Vector& v);
	bool operator==(Vector v) const { return m_x == v.m_x && m_y == v.m_y && m_z == v.m_z;  }
	bool operator<(Vector v) const { return m_x < v.m_x && m_y < v.m_y && m_z < v.m_z; }
	bool operator>(Vector v) const { return v < *this; }

	double x() const { return m_x; }
	double y() const { return m_y; }
	double z() const { return m_z; }

	double squaredNorm() const {return dotProduct(*this); }
	double dotProduct(const Vector& v) const { return m_x*v.m_x + m_y*v.m_y + m_z*v.m_z; }

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

Vector max(const Vector&, const Vector&);
Vector min(const Vector&, const Vector&);

class Point {
	double m_x;
	double m_y;
	double m_z;

public:
	Point(double x = 0, double y = 0, double z = 0): m_x{x}, m_y{y}, m_z{z} {}

	Point& operator+=(const Vector& v) { m_x += v.x(), m_y += v.y(), m_z += v.z(); return *this; }
	Point& operator-=(const Vector& v) { m_x -= v.x(), m_y -= v.y(), m_z -= v.z(); return *this; }
	Point& operator*=(const double l) { m_x *= l,   m_y *= l,   m_z *= l;   return *this;  }
	Point& operator/=(const double l) { m_x /= l,   m_y /= l,   m_z /= l;   return *this; }
	bool operator==(Point v) const { return m_x == v.x() && m_y == v.y() && m_z == v.z();  }

	double x() const { return m_x; }
	double y() const { return m_y; }
	double z() const { return m_z; }
};

Point operator+(Point p, Vector v);
Vector operator-(Point m, Point n);

Point max(const Point&, const Point&);
Point min(const Point&, const Point&);

class Matrix {
	Vector m_a;
	Vector m_b;
	Vector m_c;

public:
	Matrix(Vector a = Vector(), Vector b = Vector(), Vector c = Vector()): m_a{a}, m_b{b}, m_c{c} {}
	static Matrix identity() { return Matrix(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1)); }

	Matrix& operator+=(const Matrix& m) { m_a += m.m_a, m_b += m.m_b, m_c += m.m_c; return *this; }
	Matrix& operator-=(const Matrix& m) { m_a -= m.m_a, m_b -= m.m_b, m_c -= m.m_c; return *this; }

	Vector a() const { return m_a; }
	Vector b() const { return m_b; }
	Vector c() const { return m_c; }
};

Vector operator*(Matrix m, Vector v);

Matrix operator+(Matrix m, Matrix n);
Matrix operator-(Matrix m, Matrix n);
Matrix operator*(double const l, Matrix m);
Matrix operator*(Matrix m, Matrix n);

#endif
