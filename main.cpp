#include <iostream>
#include <vector>
#include <cmath>

#include "Tracer.h"
#include "Vector.h"
#include "Sphere.h"
#include "Light.h"
#include "Scene.h"
#include "Color.h"

int main(void)
{
	double radius = 10.;
	Sphere sphere = Sphere::Diffuse(Vector(0, 0, 15), radius, Color::white());

	Sphere mur1 = Sphere::Diffuse(Vector(0, 1000, 0), 940, Color::blue());
	Sphere mur2 = Sphere::Diffuse(Vector(0, 0, -1000), 940, Color::green());
	Sphere mur3 = Sphere::Diffuse(Vector(0, -1000, 0), 990, Color::red());
	Sphere mur4 = Sphere::Diffuse(Vector(0, 0, 1000), 940, Color::white());

	std::vector<Object*> spheres {&sphere, &mur1, &mur2, &mur3, &mur4};

	double intensity = 500000000;
	std::vector<Light> lights { Light(Vector(-10, 20, 40), intensity) };

	Scene scene = Scene(lights, spheres);

	double fov = 60.*M_PI/180.;
	Vector camera = Vector(0, 0, 55);

	Tracer tracer = Tracer(camera, fov, scene);

	tracer.draw();
	tracer.save("output.bmp");
}
