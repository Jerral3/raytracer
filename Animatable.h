#ifndef ANIMATABLE_HEADER
#define ANIMATABLE_HEADER

#include "Animation.h"

#include <vector>

class Animatable {
	std::vector<Animation*> m_animations;	

public:
	Animatable(): m_animations{} {}
    virtual ~Animatable() = default;

	void   addAnimation(Animation* a) { m_animations.push_back(a); }
    Vector translation(double) const;
    Point  rotation(const Point&, double) const;
    Matrix rotation(double) const;
    Point  invrot(const Point&, double) const;
    Matrix invrot(double) const;

};

#endif
