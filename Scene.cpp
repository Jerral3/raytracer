#include "Scene.h"

#include "Object.h"
#include "Ray.h"
#include "Vector.h"
#include "Intersection.h"

#include <cmath>
#include <random>

static std::default_random_engine engine;
static std::uniform_real_distribution<double> distrib(0,1);

bool Scene::intersect(const Ray& ray, Intersection& intersection) const
{
	double t = -1., s;
	Intersection localIntersection;

	for (Object* object : m_objects) {
		Point origine   = object->invrot(ray.origine(), m_time) + (-1) * object->translation(m_time);
		Vector direction = object->invrot(m_time) * ray.direction(); 

		if ((s = object->intersect(origine, direction, localIntersection)) != -1) {
			if (t == -1. || s < t) {
				intersection.intersected  = localIntersection.intersected;
				intersection.intersection = object->rotation(localIntersection.intersection, m_time) + object->translation(m_time);
				intersection.normale      = object->rotation(m_time) * localIntersection.normale;
				intersection.color        = localIntersection.color;
				t = s;
			}
		}
	}

	if (t == -1.)
		return false;

	return true;
}

double Scene::intensity(const Vector& normal, const Point& intersection, const Light* light) const
{
	Vector lightDirection = light->origine() - intersection;

	double distance = sqrt(squaredNorm(lightDirection));

	Vector l = (1./distance) * lightDirection;

	return std::fmax(0, dotProduct(l, normal))*light->intensity()/(4*M_PI*distance*distance);
}

bool Scene::containEmissive() const
{
	for (Object* object: m_objects)
		if (object->isEmissive())
			return true;

	return false;
}

double Scene::extinction(const Point& origine, const Point& intersection) const
{
	double integrale = m_atmosphere.density() * std::sqrt(squaredNorm(intersection - origine));

	return std::exp(-integrale);
}

