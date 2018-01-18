#ifndef SCENE_HEADER
#define SCENE_HEADER

#include <vector>

#include "Object.h"
#include "Light.h"
#include "Ray.h"
#include "Vector.h"

class Scene {
	std::vector<Light*> m_lights;
	std::vector<Object*> m_objects;

public:
	Scene(std::vector<Light*> lights, std::vector<Object*> objects): m_lights{lights}, m_objects{objects} {}

	std::vector<Light*> getLights() const { return m_lights; }
	std::vector<Object*> getObjects() const { return m_objects; }

	bool intersect(const Ray&, Object**, Vector*) const;
	double intensity(const Object*, const Vector&, const Light*) const;
};

#endif
