#ifndef TRIANGLE_HEADER
#define TRIANGLE_HEADER

#include "Vector.h"
#include "Object.h"
#include "Color.h"

class Triangle {
	Vector m_A;
	Vector m_B;
	Vector m_C;

	Vector m_tA;
	Vector m_tB;
	Vector m_tC;

	Vector m_nA;
	Vector m_nB;
	Vector m_nC;

	Vector m_normal;

public:
	Triangle(Vector A, Vector B, Vector C, Vector tA = Vector(0, 0, 0), Vector tB = Vector(0, 0, 0), Vector tC = Vector(0, 0, 0), Vector nA = Vector(0, 0, 0), Vector nB = Vector(0, 0, 0), Vector nC = Vector(0, 0, 0)): m_A{A}, m_B{B}, m_C{C}, m_tA{tA}, m_tB{tB}, m_tC{tC}, m_nA{nA}, m_nB{nB}, m_nC{nC}, m_normal{((C-A)^(B-A)).normalize()} {}

	Vector a() const { return m_A;}
	Vector b() const { return m_B;}
	Vector c() const { return m_C;}

	double intersect(const Vector&, const Vector&, Vector*, Vector*, Vector*) const;
};

#endif
