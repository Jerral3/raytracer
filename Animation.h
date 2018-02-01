#ifndef ANIMATION_HEADER
#define ANIMATION_HEADER

#include "Vector.h"

class Animation {
protected:
	double m_begin;
	double m_end;

public:
	Animation(double begin, double end): m_begin{begin}, m_end{end} {}
	virtual ~Animation() = default;

	virtual Vector translation(double)              const { return Vector(); };
	virtual Point  rotation(const Point& v, double) const { return v; };
	virtual Matrix rotation(double)                 const { return Matrix::identity(); };
	virtual Point  invrot(const Point& v, double)   const { return v;};
	virtual Matrix invrot(double)                   const { return Matrix::identity();};
};

#endif

