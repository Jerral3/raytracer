#include "Triangle.h"

#include "Vector.h"
#include "Intersection.h"

#include <cmath>

double Triangle::intersect(const Point& origine, const Vector& direction, Intersection& intersection, Vector* texture) const
{
	double dot = dotProduct(direction, m_normal);

	if (dot == 0.)
		return -1.;

	double t = - dotProduct(origine - m_C, m_normal) / dot;

	if (t < 0)
		return -1.;

	Point  P = origine + t*direction;
	Vector u = m_B - m_A;
	Vector v = m_C - m_A;
	Vector w = P - m_A;

	double m11 = u.squaredNorm();
	double m12 = dotProduct(u, v);
	double m22 = v.squaredNorm();
	double detm = m11*m22 - m12*m12;

	double b11  = dotProduct(w, u);
	double b21  = dotProduct(w, v);
	double detb = b11*m22 - m12*b21;

	double detg = m11*b21 - m12*b11;

	double beta  = detb / detm; 
	double gamma = detg / detm;
	double alpha = 1 - beta - gamma;

	if (alpha < 0. || alpha > 1. || beta < 0. || beta > 1. || gamma < 0. || gamma > 1.)
		return -1.;

	intersection.intersection = origine + t*direction;
	intersection.normale      = alpha*m_nA + beta*m_nB + gamma*m_nC;
	*texture = alpha*m_tA + beta*m_tB + gamma*m_tC;

	return t;
}
