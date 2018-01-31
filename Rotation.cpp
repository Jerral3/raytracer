#include "Rotation.h"

#include <cmath>

Rotation::Rotation(Point origine, double a, double b, double c, double begin, double end): Animation(begin, end), m_origine{origine}, m_a{}, m_b{}, m_c{}
{
	m_a = a*M_PI/180.;
	m_b = b*M_PI/180.;
	m_c = c*M_PI/180.;
}

Matrix Rotation::rotmat(double r) const
{
	Matrix rx = Matrix(Vector(1, 0, 0), Vector(0, std::cos(r*m_a), std::sin(r*m_a)), Vector(0, -std::sin(r*m_a), std::cos(r*m_a)));
	Matrix ry = Matrix(Vector(std::cos(r*m_b), 0, -std::sin(r*m_b)), Vector(0, 1, 0), Vector(std::sin(r*m_b), 0, std::cos(r*m_b)));
	Matrix rz = Matrix(Vector(std::cos(r*m_c), std::sin(r*m_c), 0), Vector(-std::sin(r*m_c), std::cos(r*m_c), 0), Vector(0, 0, 1));

	return rx * ry * rz;
}

Matrix Rotation::invmat(double r) const
{
	Matrix rx = Matrix(Vector(1, 0, 0), Vector(0, std::cos(r*m_a), -std::sin(r*m_a)), Vector(0, std::sin(r*m_a), std::cos(r*m_a)));
	Matrix ry = Matrix(Vector(std::cos(r*m_b), 0, std::sin(r*m_b)), Vector(0, 1, 0), Vector(-std::sin(r*m_b), 0, std::cos(r*m_b)));
	Matrix rz = Matrix(Vector(std::cos(r*m_c), -std::sin(r*m_c), 0), Vector(std::sin(r*m_c), std::cos(r*m_c), 0), Vector(0, 0, 1));

	return rx * ry * rz;
}

Point Rotation::rotation(const Point& v, double time) const
{
	if (time < m_begin)
		return v;

	if (time > m_end)
		return m_origine + rotmat(1) * (v - m_origine);

	return m_origine + rotmat((time - m_begin) / (m_end - m_begin)) * (v - m_origine);
}

Matrix Rotation::rotation(double time) const
{
	if (time < m_begin)
		return Matrix();

	if (time > m_end)
		return rotmat(1);

	return rotmat((time - m_begin) / (m_end - m_begin));
}

Point Rotation::invrot(const Point& v, double time) const
{
	if (time < m_begin)
		return v;

	if (time > m_end)
		return m_origine + invmat(1) * (v - m_origine);

	return m_origine + invmat((time - m_begin) / (m_end - m_begin)) * (v - m_origine);
}

Matrix Rotation::invrot(double time) const
{
	if (time < m_begin)
		return Matrix();

	if (time > m_end)
		return invmat(1);

	return invmat((time - m_begin) / (m_end - m_begin));
}
