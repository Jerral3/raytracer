#ifndef MESH_HEADER
#define MESH_HEADER

#include "Vector.h"
#include "Object.h"
#include "Color.h"
#include "Triangle.h"

#include <vector>

class Box {
	Box* m_boxes[8];

public:
	Vector m_min;
	Vector m_max;

	std::vector<Triangle> m_faces;

	Box(Vector min = Vector(0., 0., 0.), Vector max = Vector(0., 0., 0.)): m_boxes{nullptr},  m_min{min}, m_max{max}, m_faces{} {}
	~Box() { for (auto box : m_boxes) delete box; }

	void constructBoxes();
	inline bool contain(Vector) const;
	inline bool contain(Triangle&) const;
	void print() const { m_min.print(); std::cout << " "; m_max.print(); std::cout << std::endl; }

	bool intersect(const Vector&, const Vector&) const;
	double intersect(const Vector&, const Vector&, Vector*, Vector*, Vector*) const;
};

class Mesh : public Object {
	Vector m_origine;
	double m_scale;

	Box m_box;
	unsigned char* m_texture;
	int m_width;
	int m_height;

public:
	Mesh(std::string, std::string, Vector, double, Color = Color::black());

	Vector getOrigine() const { return m_origine; }
	double getScale() const { return m_scale; }
	Color  getColorTexture(Vector&) const;

	Vector normal(const Vector&) const;
	double intersect(const Vector&, const Vector&, Vector*, Vector*, Color*) const;
	double area() const;
    Vector randomPointAround(const Vector&) const;
    Vector getCenter() const;
};

#endif

