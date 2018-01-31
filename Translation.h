#ifndef TRANSLATION_HEADER
#define TRANSLATION_HEADER

#include "Animation.h"
#include "Vector.h"

class Translation : public Animation {
	Vector m_translation;

public:
	Translation(Vector t, double begin, double end): Animation(begin, end),  m_translation{t} {}

	Vector translation(double time) const;
};

#endif
