#ifndef TRIANGLE_HEADER
#define TRIANGLE_HEADER

#include "Vector.h"
#include "Intersection.h"

class Triangle {
	Point m_A;
	Point m_B;
	Point m_C;

	Vector m_tA;
	Vector m_tB;
	Vector m_tC;

	Vector m_nA;
	Vector m_nB;
	Vector m_nC;

	Vector m_normal;

public:
	Triangle(Point A, Point B, Point C, Vector tA = Vector(), Vector tB = Vector(), Vector tC = Vector(), Vector nA = Vector(), Vector nB = Vector(), Vector nC = Vector()): m_A{A}, m_B{B}, m_C{C}, m_tA{tA}, m_tB{tB}, m_tC{tC}, m_nA{nA}, m_nB{nB}, m_nC{nC}, m_normal{((C-A)^(B-A)).normalize()} {}

	Point a() const { return m_A;}
	Point b() const { return m_B;}
	Point c() const { return m_C;}

	double intersect(const Point&, const Vector&, Intersection&, Vector*) const;
};

#endif
