#include "Tracer.h"

#include "config.h"
#include "Vector.h"
#include "Ray.h"
#include "Object.h"

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
			
			//#pragma omp critical
			//std::cout << "Approx. progress : " << (int)(i * 100 / SCREEN_HEIGHT) << " %\n";
		}

		std::ostringstream os;
		os << filename << "_" << 10+t << ".bmp";
		save(os.str().c_str());

		std::cout << "Step " << t << std::endl << std::flush;
	}
}

Color Tracer::getPixelColor(int i, int j, int t)
{
	Color color = Color::black();

	for (int l = 0; l < MONTE_CARLO_N; ++l) {
#if MOTION_BLUR == 1
		m_scene.setTime((t + distrib(engine))/TIME_STEPS);
#else
		m_scene.setTime((double)t/TIME_STEPS);
#endif
		Ray ray = m_camera.ray(i, j, m_scene.time());

		for (Light* light : m_scene.getLights()) {
			color += (1./static_cast<float>(MONTE_CARLO_N)) * getColor(ray, light, RECURSION_LEVEL);
		}

		if (m_scene.containEmissive())
			color += (1./static_cast<float>(MONTE_CARLO_N)) * getColor(ray, nullptr, RECURSION_LEVEL);
	}

	return color;
}

Color Tracer::getColor(const Ray& ray, const Light* light,  int r)
{
	Color color  = Color::black();
	Color cInter = Color::black();

#if ROULETTE != 1
	if (!r)
		return color;
#endif

	Object* intersected = nullptr;
	Point intersection = Point();
	Vector normal = Vector();

	if (!m_scene.intersect(ray, &intersected, &normal, &intersection, &cInter))
		return color;

	if (intersected->isMirror()) {
#if ROULETTE == 1
		if (distrib(engine) < ROULETTE_THRESHOLD)
			return (1./ROULETTE_THRESHOLD) * getColor(rebound(ray, normal, intersection), light, --r).specular(cInter);
		else
			return color;
#else
		return getColor(rebound(ray, normal, intersection), light, --r) * cInter;
#endif
	}

	if (intersected->isTransparent()) {
#if ROULETTE == 1
		if (distrib(engine) < ROULETTE_THRESHOLD)
			return (1./ROULETTE_THRESHOLD) * getColor(refract(ray, normal,  intersection, intersected), light, --r);
		else
			return color;
#else
		return getColor(refract(ray, normal,  intersection, intersected), light, --r);
#endif
	}

	if (light != nullptr)
		color += directLighting(normal, intersection, light) * cInter;
	else
		color += directLighting(ray, normal, intersection, intersected) * cInter;

	if (intersected->isDiffuse()) {
		Vector l = intersected->monteCarloVector(normal);

#if PHONG == 1
		double exp   = 10.;
		double ks    = 0.3;
		double p     = 1. - ks;
		double enter = distrib(engine); 

		Vector reflect = rebound(ray, normal, intersection).getDirection().normalize();

		if (enter > p) {
			l = intersected->phongVector(reflect, exp);

			if (dotProduct(l, normal) < 0)
				return Color::black();

			if (dotProduct(l, reflect) < 0)
				return Color::black();
		}

		double proba_phong = (exp + 1) * std::pow(dotProduct(reflect, l), exp) / (2*M_PI);
		double proba       = p * dotProduct(normal, l) / (M_PI) + (1 - p)*proba_phong;

		cInter = (dotProduct(normal, l) / (M_PI * proba)) * cInter;

		if (enter > p)
			cInter = M_PI * ks * phong(l, ray, normal, intersection, exp) * cInter;
#endif

		Ray monteCarlo = Ray(intersection + EPSILON*normal, l);

#if ROULETTE == 1
		double thres = cInter.max();
		if (distrib(engine) < thres)
			color += (1./thres) * getColor(monteCarlo, light, r - 1).specular(cInter);
#else
		color += getColor(monteCarlo, light, r - 1) * cInter;
#endif
	}

#if ATMOSPHERE == 1
	color *= extinction(ray.getDirection(), intersection);
	
	Ray contrib = ray;
	double facteur = contribution(ray.getOrigine(), intersection, &contrib);

	color += facteur *  getColor(contrib, light, r - 1);
#endif

	return color;
}

double Tracer::directLighting(const Vector& normal, const Point& intersection, const Light* light) const
{
	double intensity = m_scene.intensity(normal, intersection, light);

	Vector nextNormal = Vector();
	Point nextIntersection = Point();
	Object* nextIntersected = nullptr;
	Color color = Color::black();
	Point origine = intersection + EPSILON*normal;
	Ray    nextRay = Ray(origine, light->origine() - origine);

	if (!m_scene.intersect(nextRay, &nextIntersected, &nextNormal, &nextIntersection, &color))
		;
	else if (squaredNorm(nextIntersection - light->origine()) > squaredNorm(nextIntersection - intersection))
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
		Object* nextIntersected = nullptr;
		Vector nextNormal = Vector();
		Point nextIntersection = Point();
		Color color = Color::black();

		if (!m_scene.intersect(rayon, &nextIntersected, &nextNormal, &nextIntersection, &color))
			;
		else if (squaredNorm(nextIntersection - x) < d*0.98) {
			continue;
		}

		Vector nprime   = light->normal(xprime);
		double cosinus  = std::fmax(0., dotProduct(w, normal));
		double cosprime = -dotProduct(w, nprime);

		double denom = d * dotProduct(nprime, lightDir) * light->area();

		double ajout = (light->intensity() * cosinus * cosprime / denom);

#if PHONG == 1
		double ks = 0.3;
		double coef = 10.;
		ajout *= (((1 - ks)/M_PI) + phong(w, ray, normal, x, coef) * ks)/M_PI;
#endif

		intensity += ajout;
	}

	return intensity;
}

Ray Tracer::rebound(const Ray& ray, const Vector& n, const Point& point) const
{
	return Ray(point + EPSILON*n, ray.direction() - 2*dotProduct(ray.direction(), n)*n);
}

Ray Tracer::refract(const Ray& ray, const Vector& normal, const Point& point,  Object* intersected) const
{
	Vector n = normal;
	double n1 = N_AIR;
	double n2 = intersected->indice();

	if (dotProduct(ray.direction(), n) > 0) {
		n1 = n2;
		n2 = N_AIR;
		n  = (-1) * n;
	}

	double nn = n1/n2;

	double subSqrt = 1 - pow(nn, 2)*(1 - pow(dotProduct(ray.direction(), n), 2));

	if (subSqrt < 0) {
		std::cout << "Total reflex" << std::endl;
		return rebound(ray, normal, point);
	}

	Vector tangent = + nn * ray.direction() - nn * dotProduct(ray.direction(), n) * n;
	Vector normalR  = - sqrt(subSqrt) * n;

#if FRESNEL == 1
	Vector i = -1 * ray.direction();

	if (n2 < n1)
		i = tangent + normalR;

	double k = std::pow(n1 - n2, 2) / std::pow(n1 + n2, 2);
	double T = k + (1 - k) * std::pow(1 - dotProduct(normal, i), 5);

	if (distrib(engine) < T)
		return rebound(ray, normal, point);
#endif

	return Ray(point + (-1)*EPSILON*n, tangent + normalR);
}

double Tracer::phong(const Vector& in, const Ray& out, const Vector& n, const Point& point, double phong) const
{
	Vector r = rebound(out, n, point).direction();

	double test = std::pow(dotProduct(r, in), phong) * (phong + 2) / (2*M_PI);

	return test;
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

	//if (distrib(engine) < PROBABILITE_UNIFORME) {
		direction = randomVector();
	//} else {
		//if (m_scene.containEmissive()) {
			//for (Object* light : m_scene.getObjects()) {   // TODO: find way to handle multi light
				//if (!light->isEmissive())
					//continue;

				//Vector lightDir = (point - light->getCenter()).normalize();
				//Vector xprime = light->randomPointAround(lightDir);

				//direction = (xprime - point).normalize();
				//break; 
			//}
		//} else {   // TODO: handle point light
			//direction = Vector(1, 1, 1);
		//}
	//}

	//double proba = PROBABILITE_UNIFORME/distance + (1 - PROBABILITE_UNIFORME) * ;
	double proba = 1/distance;

	*retRay = Ray(point, direction);

	return (phase * m_scene.atmosphere().density() * std::exp(-integrale) / (proba*4*M_PI));
}

Vector Tracer::randomVector() const
{
	double r1 = distrib(engine);
	double r2 = distrib(engine);

	double root = std::sqrt(r2*(1-r2));

	return Vector(std::cos(2*M_PI)*2*r1*root, std::sin(2*M_PI)*2*r2*root, 1 - 2*r2);
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
