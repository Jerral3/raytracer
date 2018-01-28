#include "Scene.h"

#include "Object.h"
#include "Ray.h"
#include "Vector.h"

#include <cmath>
#include <iostream>

bool Scene::intersect(const Ray& ray, Object** intersected, Vector* nInter, Vector* intersection, Color* color) const
{
	double t = -1., s;
	Vector n = Vector(0., 0., 0.), point = Vector(0., 0., 0.);
	Color c = Color::black();

	for (Object* object : m_objects) {
		if ((s = object->intersect(ray.getOrigine(), ray.getDirection(), &n, &point, &c)) != -1) {
			if (t == -1. || s < t) {
				*intersected  = object;
				*intersection = point;
				*nInter       = n;
				*color        = c;
				t = s;
			}
		}
	}

	if (t == -1.)
		return false;

	return true;
}

double Scene::intensity(const Vector& normal, const Vector& intersection, const Light* light) const
{
	Vector lightDirection = light->getOrigine() - intersection;

	double distance = sqrt(squaredNorm(lightDirection));

	Vector l = (1./distance) * lightDirection;

	return std::fmax(0, dotProduct(l, normal))*light->getIntensity()/(distance*distance);
}
