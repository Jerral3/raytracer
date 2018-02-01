#include "Tracer.h"

#include "config.h"
#include "Vector.h"
#include "Ray.h"
#include "Object.h"
#include "Intersection.h"

#include <iostream>
#include <cmath>
#include <random>
#include <sstream>

static std::default_random_engine engine;
static std::uniform_real_distribution<double> distrib(0,1);

void Tracer::draw(const char* filename)
{
	for (int t = 0; t < TIME_STEPS; t++) {
		#pragma omp parallel for schedule(dynamic,1)
		for (int i = 0; i < SCREEN_HEIGHT; i++) {
			for (int j = 0; j < SCREEN_WIDTH; j++) {
				Color color = getPixelColor(i, j, t).gamma();

				m_screen[i][j][0] = static_cast<unsigned char>(std::fmin(255., color.intensity(RED)));
				m_screen[i][j][1] = static_cast<unsigned char>(std::fmin(255., color.intensity(GREEN)));
				m_screen[i][j][2] = static_cast<unsigned char>(std::fmin(255., color.intensity(BLUE)));
			}
		}

		save(filename, t);

		std::cout << "Step " << t << std::endl << std::flush;
	}
}

Color Tracer::getPixelColor(int i, int j, int t)
{
	Color color = Color::black();

	for (int l = 0; l < MONTE_CARLO_N; ++l) {
		m_scene.setTimeWithStep(t);

		Ray ray = m_camera.ray(i, j, m_scene.time());

		for (Light* light : m_scene.getLights()) {
			color += (1./static_cast<float>(MONTE_CARLO_N)) * getColor(ray, light, RECURSION_LEVEL);
		}

		if (m_scene.containEmissive())
			color += (1./static_cast<float>(MONTE_CARLO_N)) * getColor(ray, nullptr, RECURSION_LEVEL);
	}

	return color;
}

Color Tracer::getColor(const Ray& ray, const Light* light,  int r) const
{
#if ROULETTE != 1
	if (!r)
		return Color::black();
#endif
	
	Intersection intersection;

	if (!m_scene.intersect(ray, intersection))
		return Color::black();

	if (intersection.intersected->isMirror()) {
		Ray reflect = Ray(intersection.intersection + EPSILON*intersection.normale, rebound(ray.direction(), intersection.normale));

		return roulette(reflect, light, r) * intersection.color;
	}

	if (intersection.intersected->isTransparent()) {
		Ray refracted = refract(ray.direction(), intersection.normale,  intersection.intersection, intersection.intersected);

		return roulette(refracted, light, r);
	}

	Color color = Color::black();

	if (light != nullptr)
		color += directLighting(intersection.normale, intersection.intersection, light) * intersection.color;
	else
		color += directLighting(ray, intersection.normale, intersection.intersection, intersection.intersected) * intersection.color;

	if (intersection.intersected->isDiffuse()) {
		Vector l = intersection.intersected->monteCarloVector(intersection.normale);

#if PHONG == 1
		double exp   = intersection.intersected->phongExp();
		double ks    = intersection.intersected->ks();
		double p     = 1. - ks;
		double enter = distrib(engine); 

		Vector reflect = rebound(ray.direction(), intersection.normale).normalize();

		if (enter > p) {
			l = intersection.intersected->phongVector(reflect, exp);

			if (dotProduct(l, intersection.normale) < 0)
				return Color::black();

			if (dotProduct(l, reflect) < 0)
				return Color::black();
		}

		double proba_phong = (exp + 1) * std::pow(dotProduct(reflect, l), exp) / (2*M_PI);
		double proba       = p * dotProduct(intersection.normale, l) / (M_PI) + (1 - p)*proba_phong;

		intersection.color *= (dotProduct(intersection.normale, l) / (M_PI * proba));

		if (enter > p)
			intersection.color *= M_PI * ks * phong(l, ray.direction(), intersection.normale, exp);
#endif

		Ray monteCarlo = Ray(intersection.intersection + EPSILON*intersection.normale, l);

#if ROULETTE == 1
		double thres = intersection.color.max();
		if (distrib(engine) < thres)
			color += (1./thres) * getColor(monteCarlo, light, r - 1) * intersection.color;
#else
		color += getColor(monteCarlo, light, r - 1) * intersection.color;
#endif
	}

#if ATMOSPHERE == 1
	color *= extinction(ray.getDirection(), intersection.intersection);
	
	Ray contrib = ray;
	double facteur = contribution(ray.getOrigine(), intersection.intersection, &contrib);

	color += facteur *  getColor(contrib, light, r - 1);
#endif

	return color;
}

double Tracer::directLighting(const Vector& normal, const Point& intersection, const Light* light) const
{
	double intensity = m_scene.intensity(normal, intersection, light);

	Intersection nextIntersection;
	Point origine = intersection + EPSILON*normal;
	Ray    nextRay = Ray(origine, light->origine() - origine);

	if (!m_scene.intersect(nextRay, nextIntersection))
		;
	else if (squaredNorm(nextIntersection.intersection - light->origine()) > squaredNorm(nextIntersection.intersection - intersection))
		intensity = 0;

	return intensity;
}

double Tracer::directLighting(const Ray& ray, const Vector& normal, const Point& x, const Object* intersected) const
{
	double intensity = 0.;

	for (Object* light : m_scene.getObjects()) {
		if (!light->isEmissive() || light == intersected)
			continue;

		Vector lightDir = (x - light->center()).normalize();
		Point xprime = light->randomPointAround(lightDir);

		Vector w = (xprime - x).normalize();
		double d = squaredNorm(xprime - x);

		Ray rayon = Ray(x + EPSILON*normal, w);
		Intersection nextIntersection;

		if (!m_scene.intersect(rayon, nextIntersection))
			;
		else if (squaredNorm(nextIntersection.intersection - x) < d*0.98) {
			continue;
		}

		Vector nprime   = light->normal(xprime);
		double cosinus  = std::fmax(0., dotProduct(w, normal));
		double cosprime = std::fmax(0., -dotProduct(w, nprime));

		double denom = d * dotProduct(nprime, lightDir) * light->area();

		double ajout = (light->intensity() * cosinus * cosprime / denom);

#if PHONG == 1
		double exp   = intersected->phongExp();
		double ks    = intersected->ks();
		ajout *= (((1 - ks)/M_PI) + phong(w, ray.direction(), normal, exp) * ks)/M_PI;
#endif

		intensity += ajout;
	}

	return intensity;
}

Vector Tracer::rebound(const Vector& in, const Vector& n) const
{
	return in - 2*dotProduct(in, n)*n;
}

Ray Tracer::refract(const Vector& in, const Vector& normal, const Point& point, const Object* intersected) const
{
	Vector n = normal;
	double n1 = N_AIR;
	double n2 = intersected->indice();

	if (dotProduct(in, n) > 0) {
		n1 = n2;
		n2 = N_AIR;
		n  = (-1) * n;
	}

	double nn = n1/n2;

	double subSqrt = 1 - pow(nn, 2)*(1 - pow(dotProduct(in, n), 2));

	if (subSqrt < 0) {
		std::cout << "Total reflex" << std::endl;
		return Ray(point + EPSILON*normal, rebound(in, normal));
	}

	Vector tangent = + nn * in - nn * dotProduct(in, n) * n;
	Vector normalR  = - sqrt(subSqrt) * n;

#if FRESNEL == 1
	Vector i = -1 * in;

	if (n2 < n1)
		i = tangent + normalR;

	double k = std::pow(n1 - n2, 2) / std::pow(n1 + n2, 2);
	double T = k + (1 - k) * std::pow(1 - dotProduct(normal, i), 5);

	if (distrib(engine) < T)
		return Ray(point + EPSILON*normal, rebound(in, normal));
#endif

	return Ray(point + (-1)*EPSILON*n, tangent + normalR);
}

double Tracer::phong(const Vector& in, const Vector& out, const Vector& n, double phong) const
{
	Vector r = rebound(out, n);

	double test = std::pow(dotProduct(r, in), phong) * (phong + 2) / (2*M_PI);

	return test;
}

Color Tracer::roulette(const Ray& ray, const Light* light, int r) const
{
	double facteur = 1.;
#if ROULETTE == 1
	if (distrib(engine) > ROULETTE_THRESHOLD)
		return Color::black();

	facteur = 1./ROULETTE_THRESHOLD;
#endif

	return facteur * getColor(ray, light, --r);
}

double Tracer::extinction(const Point& origine, const Point& intersection) const
{
	return  m_scene.extinction(origine, intersection);
}

double Tracer::contribution(const Point& origine, const Point& intersection, Ray* retRay) const
{
	double longueur = std::sqrt(squaredNorm(intersection - origine));
	
	double distance  = distrib(engine) * longueur;
	double integrale = m_scene.atmosphere().density() * distance;
	double phase     = 0.3 / (4*M_PI);

	Vector direction = Vector();
	Point point      = origine + distance*(intersection - origine);

	direction = Vector::randomVector();
	double proba = 1/distance;

	*retRay = Ray(point, direction);

	return (phase * m_scene.atmosphere().density() * std::exp(-integrale) / (proba*4*M_PI));
}

void Tracer::save(const char* filename, int step) const // (0,0) is top-left corner
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

	std::ostringstream os;

#if TIME_STEPS > 1
	os << filename << "_" << 10 + step << ".bmp";
#else
	os << filename << ".bmp";
#endif

	filename = os.str().c_str();

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
