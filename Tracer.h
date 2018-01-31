#ifndef TRACER_HEADER
#define TRACER_HEADER

#include "config.h"

#include "Vector.h"
#include "Scene.h"
#include "Color.h"
#include "Ray.h"
#include "Light.h"
#include "Object.h"
#include "Camera.h"

class Tracer {
	Camera& m_camera;

	Scene& m_scene;

	unsigned char m_screen[SCREEN_HEIGHT][SCREEN_WIDTH][CHANNEL];

public:
	Tracer(Camera& camera, Scene& scene): m_camera{camera}, m_scene{scene}, m_screen{} {}

	void draw(const char* filename);
	Color getPixelColor(int i, int j, int t);
	Color getColor(const Ray&, const Light*,  int);
	double directLighting(const Ray&, const Vector&, const Point&, const Object*) const;
	double directLighting(const Vector&, const Point&, const Light*) const;
	
	Ray rebound(const Ray&, const Vector&, const Point&) const;
	Ray refract(const Ray&, const Vector&, const Point&, const Object*) const;

	double phong(const Vector&, const Ray&, const Vector&, const Point&, double) const; 

	double extinction(const Point&, const Point&) const;
	double contribution(const Point&, const Point&, Ray*) const;

	Vector randomVector() const;

	void save(const char* filename) const;
};

#endif
