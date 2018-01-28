int debug = 0; 
#include <iostream>
#include <vector>
#include <cmath>

#include "Tracer.h"
#include "Vector.h"
#include "Sphere.h"
#include "Light.h"
#include "Scene.h"
#include "Color.h"
#include "Triangle.h"
#include "Mesh.h"

int main(void)
{
	double intensity = 400000000;
	double radius = 10.;

	Sphere lum    = Sphere::Emissive(Vector(-10, 20, 40), 5, Color::white(), intensity);
	Sphere sphere = Sphere::Diffuse(Vector(0, 0, 15), radius, Color::white());

	Sphere mur1 = Sphere::Diffuse(Vector(0, 1000, 0), 940, Color::blue());
	Sphere mur2 = Sphere::Diffuse(Vector(0, 0, -1000), 940, Color::green());
	Sphere mur3 = Sphere::Diffuse(Vector(0, -1000, 0), 990, Color::red());
	Sphere mur4 = Sphere::Diffuse(Vector(0, 0, 1000), 940, Color::white());

	Mesh mesh = Mesh("rex.obj", "Texture.bmp", Vector(0., -15.5, 0.), 0.07, Color::white());

	//std::vector<Object*> spheres {&sphere, &mur1, &mur2, &mur3, &mur4};
	std::vector<Object*> spheres {&mesh, &mur1, &mur2, &mur3, &mur4};
	std::vector<Light*> lights { new Light(Vector(-10, 20, 40), intensity) };

	//std::vector<Light*> lights { new Light(Vector(-10, 20, 40), intensity) , new Light(Vector(10, 20, 40), intensity) };

	//std::vector<Object*> spheres {&lum, &sphere, &mur1, &mur2, &mur3, &mur4};
	//std::vector<Light*> lights {};

	Scene scene = Scene(lights, spheres);

	double fov = 60.*M_PI/180.;
	Vector camera = Vector(0, 0, 55);

	Tracer tracer = Tracer(camera, fov, scene);

	std::cout << "Constructed" << std::endl;

	tracer.draw();
	tracer.save("output.bmp");
}
