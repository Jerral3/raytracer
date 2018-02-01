#ifndef INTERSECTION_HEADER
#define INTERSECTION_HEADER

#include "Vector.h"
#include "Object.h"
#include "Color.h"

typedef struct Intersection Intersection;

class Object;

struct Intersection {
	Point   intersection      = Point();
	Vector  normale           = Vector();
	const Object* intersected = nullptr;
	Color   color             = Color::black();
};

#endif
