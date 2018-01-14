#ifndef TRACER_HEADER
#define TRACER_HEADER

#include "config.h"

#include "Vector.h"
#include "Scene.h"
#include "Color.h"
#include "Ray.h"
#include "Light.h"
#include "Object.h"

class Tracer {
	Vector& m_camera;
	double m_fov;

	Scene& m_scene;

	unsigned char m_screen[SCREEN_HEIGHT][SCREEN_WIDTH][CHANNEL];

	bool m_firstMonteCarlo;

public:
	Tracer(Vector& camera, double fov, Scene& scene): m_camera{camera}, m_fov{fov}, m_scene{scene}, m_screen{}, m_firstMonteCarlo{true} {}

	void draw();
	Color getColor(const Ray&, const Light&,  int);
	Ray rebound(const Ray&, const Vector&, Object*) const;
	Ray refract(const Ray&, const Vector&, Object*) const;
	void save(const char* filename) const;
};

#endif
