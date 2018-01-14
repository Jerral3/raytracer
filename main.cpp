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
	Sphere* sphere = new Sphere(Vector(0, 0, 15), radius, Color(1., 1., 1.), false, false, 1.5, true);

	Sphere* mur1 = new Sphere(Vector(0, 1000, 0), 940, Color(0., 0., 1.), false, false, 1., false);
	Sphere* mur2 = new Sphere(Vector(0, 0, -1000), 940, Color(0., 1., 0.), false, false, 1., false);
	Sphere* mur3 = new Sphere(Vector(0, -1000, 0), 990, Color(1., 0., 0.), false, false, 1., true);
	Sphere* mur4 = new Sphere(Vector(0, 0, 1000), 940, Color(1., 1., 1.), false, false, 1., false);

	std::vector<Object*> spheres {sphere, mur1, mur2, mur3, mur4};

	double intensity = 100000000;
	std::vector<Light> lights { Light(Vector(-10, 20, 40), Color(1., 1., 1., intensity)) };

	Scene scene = Scene(lights, spheres);

	double fov = 60.*M_PI/180.;
	Vector camera = Vector(0, 0, 55);

	Tracer tracer = Tracer(camera, fov, scene);

	tracer.draw();
	tracer.save("output.bmp");

	delete mur1;
	delete mur2;
	delete mur3;
	delete mur4;
	delete sphere;
}
