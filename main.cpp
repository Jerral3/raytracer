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

Color texture(const Vector& normale)
{
	return Color(0.5 + 0.47*cos(2*M_PI*normale.x()), 0.5 + 0.47*sin(2*M_PI*normale.y()), normale.z());
}

int main(void)
{
	double intensity = 4000000000;
	double radius = 10.;

	Sphere lum    = Sphere::Emissive(Point(-10, -13, 60), 5, Color::white(), intensity);
	Sphere sphere = Sphere::Diffuse(Point(0, 10, 20), radius, Color::white());
	//sphere.addTexture(texture);
	//sphere.addAnimation(new Translation(Vector(80., 0., 0.), 0., 1.));

	Sphere mur1 = Sphere::Diffuse(Point(0, 1000, 0), 940, Color::blue());
	Sphere mur2 = Sphere::Diffuse(Point(0, 0, -1000), 940, Color::green());
	Sphere mur3 = Sphere::Diffuse(Point(0, -1000, 0), 990, Color::red());
	Sphere mur4 = Sphere::Diffuse(Point(0, 0, 1000), 940, Color(0.3, 0.3, 0.3));

	Mesh mesh = Mesh("rex.obj", "Texture.bmp", Point(0., -15.5, 0.), 0.07, Color::white());
	//mesh.addTexture(texture);
	//mesh.addAnimation(new Rotation(mesh.getCenter(), 0, -360, 0, 0, 1));

	//std::vector<Object*> spheres {&sphere, &mur1, &mur2, &mur3, &mur4};
	//std::vector<Object*> spheres {&mesh, &mur1, &mur2, &mur3, &mur4};
	std::vector<Object*> spheres {&lum, &mesh, &mur1, &mur2, &mur3, &mur4};

	//std::vector<Light*> lights { new Light(Point(-10, 20, 40), intensity) };
	//std::vector<Light*> lights { new Light(Point(-10, 20, 40), intensity) , new Light(Point(10, 20, 40), intensity) };
	std::vector<Light*> lights {};

	Scene scene = Scene(lights, spheres);

	double fov = 60.*M_PI/180.;
	Camera camera = Camera(Point(0, 0, 55), Vector(0., 0., -1.), Vector(0., 1., 0.), fov, 40);
	//camera.addAnimation(new Translation(Vector(0., 10., 0.), 0, 1));
	//camera.addAnimation(new Rotation(Point(0, 0, 55), 0, -140, 0, 0, 1));

	Tracer tracer = Tracer(camera, scene);

	std::cout << "Constructed" << std::endl;

	tracer.draw("animation");
}
