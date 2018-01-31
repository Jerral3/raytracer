#ifndef ROTATION_HEADER
#define ROTATION_HEADER

#include "Animation.h"
#include "Vector.h"

class Rotation : public Animation {
	Point m_origine;

	double m_a;
	double m_b;
	double m_c;

public:
	Rotation(Point origine, double a, double b, double c, double begin, double end);

	Matrix rotmat(double time) const;
	Matrix invmat(double time) const;

	Point rotation(const Point&, double time) const;
	Matrix rotation(double time) const;
	Point invrot(const Point&, double time) const;
	Matrix invrot(double time) const;
};

#endif

