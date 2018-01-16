#include "Tracer.h"

#include "config.h"
#include "Vector.h"
#include "Ray.h"
#include "Object.h"

#include <iostream>
#include <cmath>
#include <random>

static std::default_random_engine engine;
static std::uniform_real_distribution<double> distrib(0,1);

void Tracer::draw()
{
	#pragma omp parallel for schedule(dynamic,1)
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		for (int j = 0; j < SCREEN_WIDTH; j++) {
			Color color = getPixelColor(i, j).gamma();

			m_screen[i][j][0] = static_cast<unsigned char>(std::fmin(255., color.getIntensity(RED)));
			m_screen[i][j][1] = static_cast<unsigned char>(std::fmin(255., color.getIntensity(GREEN)));
			m_screen[i][j][2] = static_cast<unsigned char>(std::fmin(255., color.getIntensity(BLUE)));
		}
	}
}

Color Tracer::getPixelColor(int i, int j)
{
	Color color = Color::black();

	for (Light light : m_scene.getLights())
		for (int l = 0; l < MONTE_CARLO_N; ++l) {
			Ray ray = antiAliasingRay(i, j);

			color += (1./static_cast<float>(MONTE_CARLO_N)) * getColor(ray, light, RECURSION_LEVEL);
		}

	return color;
}

Color Tracer::getColor(const Ray& ray, const Light& light,  int r)
{
	Color color = Color::black();

	if (!r)
		return color;

	double intensity  = -1.;
	Object* intersected = nullptr;
	Vector intersection = Vector(0., 0., 0.);

	if ((intensity = m_scene.intersect(ray, &intersected, &intersection, light)) == -1.)
		return color;

	Vector normal = intersected->normal(intersection).normalize();

	if (intersected->isMirror())
		return getColor(rebound(ray, intersection, intersected), light, --r).specular(intersected->getColor());

	if (intersected->isTransparent())
		return getColor(refract(ray, intersection, intersected), light, --r);

	color += (intensity/M_PI)*intersected->getColor();

	Vector nextIntersection = Vector(0., 0., 0.);
	Object* nextIntersected = nullptr;
	Vector origine = intersection + EPSILON*normal;
	Ray    nextRay = Ray(origine, light.getOrigine() - origine);

	if ((intensity = m_scene.intersect(nextRay, &nextIntersected, &nextIntersection, light)) == -1.)
		;
	else if (squaredNorm(nextIntersection - light.getOrigine()) > squaredNorm(nextIntersection - intersection))
		color = Color::black();

	if (intersected->isDiffuse()) {
		Vector l = intersected->monteCarloVector(intersection);
		Ray monteCarlo = Ray(intersection + EPSILON*normal, l);

		color += getColor(monteCarlo, light, r - 1).specular(intersected->getColor());
	}

	return color;
}

Ray Tracer::rebound(const Ray& ray, const Vector& point, Object* intersected) const
{
	Vector n = intersected->normal(point);

	return Ray(point + EPSILON*n, ray.getDirection() - 2*dotProduct(ray.getDirection(), n)*n);
}

Ray Tracer::refract(const Ray& ray, const Vector& point, Object* intersected) const
{
	Vector n  = intersected->normal(point);
	n *= (1/sqrt(n.squaredNorm()));
	double nn = N_AIR/intersected->getIndice();

	if (dotProduct(ray.getDirection(), n) > 0) {
		nn = 1/nn;
		n  = (-1) * n;
	}

	double subSqrt = 1 - pow(nn, 2)*(1 - pow(dotProduct(ray.getDirection(), n), 2));

	if (subSqrt < 0) {
		std::cout << "Total reflex" << std::endl;
		return rebound(ray, point, intersected);
	}

	Vector tangent = + nn * ray.getDirection() - nn * dotProduct(ray.getDirection(), n) * n;
	Vector normal  = - sqrt(subSqrt) * n;

	return Ray(point - EPSILON*n, tangent + normal);
}

Ray Tracer::antiAliasingRay(int i, int j) const
{
	double r1 = distrib(engine);
	double r2 = distrib(engine);
	double l = std::sqrt(-2 * std::log(r1));

	double dx = l*std::cos(2*M_PI*r2);
	double dy = l*std::sin(2*M_PI*r2);

	Vector direction = Vector(j + 0.5 - SCREEN_WIDTH/2 + dx, SCREEN_HEIGHT/2 - i - 0.5 - dy, -SCREEN_HEIGHT/(2.*tan(m_fov/2.)));

	return Ray(m_camera, direction);
}

void Tracer::save(const char* filename) const // (0,0) is top-left corner
{
    FILE *f;

    int filesize = 54 + CHANNEL * SCREEN_WIDTH*SCREEN_HEIGHT;

    unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
    unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
    unsigned char bmppad[CHANNEL] = { 0,0,0 };

    bmpfileheader[2] = (unsigned char)(filesize);
    bmpfileheader[3] = (unsigned char)(filesize >> 8);
    bmpfileheader[4] = (unsigned char)(filesize >> 16);
    bmpfileheader[5] = (unsigned char)(filesize >> 24);

    bmpinfoheader[4]  = (unsigned char)(SCREEN_WIDTH);
    bmpinfoheader[5]  = (unsigned char)(SCREEN_WIDTH >> 8);
    bmpinfoheader[6]  = (unsigned char)(SCREEN_WIDTH >> 16);
    bmpinfoheader[7]  = (unsigned char)(SCREEN_WIDTH >> 24);
    bmpinfoheader[8]  = (unsigned char)(SCREEN_HEIGHT);
    bmpinfoheader[9]  = (unsigned char)(SCREEN_HEIGHT >> 8);
    bmpinfoheader[10] = (unsigned char)(SCREEN_HEIGHT >> 16);
    bmpinfoheader[11] = (unsigned char)(SCREEN_HEIGHT >> 24);

    f = fopen(filename, "wb");

    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);

    unsigned char *row = new unsigned char[SCREEN_WIDTH * CHANNEL];

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            row[j * 3]   = m_screen[SCREEN_HEIGHT - 1 - i][j][2];
            row[j * 3+1] = m_screen[SCREEN_HEIGHT - 1 - i][j][1];
            row[j * 3+2] = m_screen[SCREEN_HEIGHT - 1 - i][j][0];
        }

        fwrite(row, 3, SCREEN_WIDTH, f);
        fwrite(bmppad, 1, (4 - (SCREEN_WIDTH * 3) % 4) % 4, f);
    }

    fclose(f);

	delete[] row;
}
