#include "Scene.h"

#include "Object.h"
#include "Ray.h"
#include "Vector.h"

#include <cmath>
#include <iostream>

double Scene::intersect(const Ray& ray, Object** intersected, Vector* intersection, const Light& light) const
{
	double t = -1., s;

	for (unsigned int i = 0; i < m_objects.size(); i++) {
		if ((s = m_objects[i]->intersect(ray)) != -1) {
			if (t == -1. || s < t) {
				*intersected = m_objects[i];
				t = s;
			}
		}
	}

	if (t == -1.)
		return -1.;

	*intersection         = ray.getOrigine() + t*ray.getDirection();
	Vector normal         = (*intersected)->normal(*intersection);
	Vector lightDirection = light.getOrigine() - *intersection;

	double distance = sqrt(squaredNorm(lightDirection));

	Vector l = (1./distance) * lightDirection;

	return std::fmax(0, dotProduct(l, normal))*light.getColor().getIntensity()/(distance*distance);
}
