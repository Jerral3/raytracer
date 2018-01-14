#include "Tracer.h"

#include "config.h"

#include <iostream>
#include <cmath>

#include "Vector.h"
#include "Ray.h"
#include "Object.h"

void Tracer::draw()
{
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		for (int j = 0; j < SCREEN_WIDTH; j++) {
			Vector direction = Vector(j + 0.5 - SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - i - 0.5, -SCREEN_HEIGHT/(2.*tan(m_fov/2.)));

			Ray ray = Ray(m_camera, direction);

			for (Light light : m_scene.getLights())
			{
				m_firstMonteCarlo = true;
				Color color = getColor(ray, light, RECURSION_LEVEL);

				for (int k = 0; k < CHANNEL; ++k)
					m_screen[i][j][k] = static_cast<unsigned char>(std::fmin(255., std::pow(color.getIntensity(k), 1/GAMMA)));
			}
		}
	}
}

Color Tracer::getColor(const Ray& ray, const Light& light,  int r)
{
	if (!r) {
		//std::cout << "Too many recursion level : ";
		return Color();
	}

	double intensity  = -1.;
	Object* intersected = nullptr;
	Vector intersection = Vector(0., 0., 0.);

	if ((intensity = m_scene.intersect(ray, &intersected, &intersection, light)) == -1.) {
		//std::cout << "No object intersected" << std::endl;
		return Color();
	}

	if (intersected->isMirror()) {
		Color color = getColor(rebound(ray, intersection, intersected), light, --r);

		return color.specular(intersected->getColor());
	}

	if (intersected->isTransparent()) {
		Ray refracted = refract(ray, intersection, intersected);

		Color color = getColor(refracted, light, --r);

		return color;
	}

	if (intersected->isDiffuse()) {  // TODO:: éclairage direct
		//Color color = intersected->getColor();
		Color color = Color();

		int n = (m_firstMonteCarlo) ? MONTE_CARLO_N : 1;
		m_firstMonteCarlo = false;
		Vector normal = intersected->normal(intersection).normalize();

		for (int i = 0; i < n; ++i) {
			Vector l = intersected->monteCarloVector(intersection);
			Ray monteCarlo = Ray(intersection + EPSILON*normal, l);

			Color color1 = getColor(monteCarlo, light, r - 1);

			//std::cout << "Inside loop : "; color1.print(); std::cout << std::endl;
			color += color1;
		}

		//((static_cast<float>(COEFF_DIFFUS)/n)*color).print();

		return (static_cast<float>(COEFF_DIFFUS)/n) * color;
	}

	Color color = Color(intersected->getColor(), intensity);

	Vector nextIntersection = Vector(0., 0., 0.);
	Vector origine = intersection + EPSILON*intersected->normal(intersection);
	Ray    nextRay = Ray(origine, light.getOrigine() - origine);

	if ((intensity = m_scene.intersect(nextRay, &intersected, &nextIntersection, light)) == -1.)
		return color;
	else if (squaredNorm(nextIntersection - light.getOrigine()) > squaredNorm(nextIntersection - intersection))
		return Color();

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

//Vector n = intersected->normal(point);
//double p = abs(dotProduct(n, ray.getOrigine()))/M_PI; 


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
