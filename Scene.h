#ifndef SCENE_HEADER
#define SCENE_HEADER

#include <vector>

#include "Object.h"
#include "Light.h"
#include "Ray.h"
#include "Vector.h"
#include "Atmosphere.h"

class Scene {
	std::vector<Light*> m_lights;
	std::vector<Object*> m_objects;
	Atmosphere m_atmosphere;

	double m_time;

public:
	Scene(std::vector<Light*> lights, std::vector<Object*> objects): m_lights{lights}, m_objects{objects}, m_atmosphere{}, m_time{0.} {}

	std::vector<Light*> getLights() const { return m_lights; }
	std::vector<Object*> getObjects() const { return m_objects; }
	bool containEmissive() const;

	void setTime(double t) { m_time = t; }
	double time() const { return m_time; }

	bool intersect(const Ray&, Object**, Vector*, Point*, Color*) const;
	double intensity(const Vector&, const Point&, const Light*) const;

	void addAtmosphere(Atmosphere a) { m_atmosphere = a; }
	Atmosphere atmosphere() const { return m_atmosphere; }
	double extinction(const Point&, const Point&) const;
};

#endif
