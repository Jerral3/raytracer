int debug = 0; 
#include <iostream>
#include <vector>
#include <cmath>

#include "Tracer.h"
#include "Camera.h"
#include "Vector.h"
#include "Sphere.h"
#include "Light.h"
#include "Scene.h"
#include "Color.h"
#include "Mesh.h"
#include "Animation.h"
#include "Translation.h"
#include "Rotation.h"
#include "Atmosphere.h"

int main(void)
{
	double intensity = 4000000000;
	double radius = 10.;

	Sphere lum    = Sphere::Emissive(Point(-10, 20, 40), 10, Color::white(), intensity);
	Sphere sphere = Sphere::Diffuse(Point(0, 0, 15), radius, Color::white());
	//sphere.addAnimation(new Translation(Vector(80., 0., 0.), 0., 1.));

	Sphere mur1 = Sphere::Diffuse(Point(0, 1000, 0), 940, Color::blue());
	Sphere mur2 = Sphere::Diffuse(Point(0, 0, -1000), 940, Color::green());
	Sphere mur3 = Sphere::Diffuse(Point(0, -1000, 0), 990, Color::red());
	Sphere mur4 = Sphere::Diffuse(Point(0, 0, 1000), 940, Color(0.3, 0.3, 0.3));

	Mesh mesh = Mesh("rex.obj", "Texture.bmp", Point(0., -15.5, 0.), 0.07, Color::white());
	//mesh.addAnimation(new Rotation(mesh.getCenter(), 0, -360, 0, 0, 1));

	//std::vector<Object*> spheres {&lum, &sphere, &mur1, &mur2, &mur3, &mur4};
	std::vector<Object*> spheres {&mesh, &mur1, &mur2, &mur3, &mur4};
	std::vector<Light*> lights { new Light(Point(-10, 20, 40), intensity) };

	//std::vector<Light*> lights { new Light(Point(-10, 20, 40), intensity) , new Light(Point(10, 20, 40), intensity) };

	//std::vector<Object*> spheres {&lum, &sphere, &mur1, &mur2, &mur3, &mur4};
	//std::vector<Light*> lights {};

	double density = 0.04;

	Scene scene = Scene(lights, spheres);
	//scene.addAtmosphere(Atmosphere(density));

	double fov = 60.*M_PI/180.;
	Camera camera = Camera(Point(0, 0, 55), Vector(0., 0., -1.), Vector(0., 1., 0.), fov, 40);
	//camera.addAnimation(new Translation(Vector(0., 10., 0.), 0, 1));
	//camera.addAnimation(new Rotation(Point(0, 0, 55), 0, -140, 0, 0, 1));

	Tracer tracer = Tracer(camera, scene);

	std::cout << "Constructed" << std::endl;

	tracer.draw("animation");
}
