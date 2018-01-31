#ifndef MESH_HEADER
#define MESH_HEADER

#include "Vector.h"
#include "Object.h"
#include "Color.h"
#include "Triangle.h"
#include "Intersection.h"

#include <vector>

class Box {
	Box* m_boxes[8];

public:
	Point m_min;
	Point m_max;

	std::vector<Triangle> m_faces;

	Box(Point min = Point(), Point max = Point()): m_boxes{nullptr},  m_min{min}, m_max{max}, m_faces{} {}
	Box(const Box&) = default;
	Box& operator=(const Box&) = default;
	~Box() { for (auto box : m_boxes) delete box; }

	void constructBoxes();
	inline bool contain(Triangle&) const;

	bool intersect(const Point&, const Vector&) const;
	double intersect(const Point&, const Vector&, Intersection&, Vector*) const;
};

class Mesh : public Object {
	Point m_origine;
	double m_scale;

	Box m_box;
	unsigned char* m_texture;
	int m_width;
	int m_height;

public:
	Mesh(std::string, std::string, Point, double, Color = Color::black());
	Mesh(const Mesh& m) = default;
	Mesh& operator=(const Mesh& m) = default;

	Point origine() const { return m_origine; }
	double scale() const { return m_scale; }
	Color  getColorTexture(Vector&) const;

	Vector normal(const Point&) const;
	double intersect(const Point&, const Vector&, Intersection&) const;
	double area() const;
    Point randomPointAround(const Vector&) const;
    Point center() const;
};

#endif

