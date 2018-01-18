#include "Scene.h"

#include "Object.h"
#include "Ray.h"
#include "Vector.h"

#include <cmath>
#include <iostream>

bool Scene::intersect(const Ray& ray, Object** intersected, Vector* intersection) const
{
	double t = -1., s;

	for (Object* object : m_objects) {
		if ((s = object->intersect(ray.getOrigine(), ray.getDirection())) != -1) {
			if (t == -1. || s < t) {
				*intersected = object;
				t = s;
			}
		}
	}

	if (t == -1.)
		return false;

	*intersection = ray.getOrigine() + t*ray.getDirection();

	return true;
}

double Scene::intensity(const Object* intersected, const Vector& intersection, const Light* light) const
{
	Vector normal         = intersected->normal(intersection);
	Vector lightDirection = light->getOrigine() - intersection;

	double distance = sqrt(squaredNorm(lightDirection));

	Vector l = (1./distance) * lightDirection;

	return std::fmax(0, dotProduct(l, normal))*light->getIntensity()/(distance*distance);
}
