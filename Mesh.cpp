#include "Mesh.h"

#include "Vector.h"
#include "Triangle.h"

#include "config.h"
#include "extern.h"
#include "texture.h"

#include <cmath>
#include <fstream>
#include <sstream>

int getToken(std::istringstream& iss)
{
	std::string token;

	std::getline(iss, token, '/');

	if (token.empty())  // TODO: find what to do
		return 0;

	return std::stoi(token) - 1;
}

Mesh::Mesh(std::string filename, std::string texture, Vector origine, double scale, Color color) : Object(color), m_origine{origine}, m_scale{scale}, m_box{}, m_texture{}, m_width{0}, m_height{0}
{
	makeDiffuse();

	m_texture = readBMP(texture.c_str(), &m_height, &m_width);

	std::ifstream file(filename);
	std::string line;

	std::vector<Vector> points   {};
	std::vector<Vector> normals  {};
	std::vector<Vector> textures {};

	while (std::getline(file, line)) {
		std::istringstream iss{line};
		char firstCharacter = line[0];
		char character      = line[1];

		char useless; iss >> useless;

		if (firstCharacter == 'v') {
			if (character == ' ') {
				double x, y, z;
				iss >> x >> y >> z;

				//std::cout << x << "," << y << " " << std::flush;

				points.push_back(m_origine + m_scale*Vector(x, y, z));
			} else if (character == 'n') {
				iss >> useless;
				double x, y, z;
				iss >> x >> y >> z;

				//std::cout << x << "," << y << " " << std::flush;

				normals.push_back(Vector(x, y, z));
			} else if (character == 't') {
				iss >> useless;
				double x, y, z;
				iss >> x >> y >> z;

				//std::cout << x << "," << y << " " << std::flush;

				textures.push_back(Vector(x*m_width, y*m_height, 0.));
			}
		}

		if (firstCharacter == 'f') {
			std::string t1, t2, t3, token;
			iss >> t1 >> t2 >> t3;
			std::istringstream s1{t1}, s2{t2}, s3{t3};

			Vector A = points[getToken(s1)];
			Vector B = points[getToken(s2)];
			Vector C = points[getToken(s3)];

			Vector tA = textures[getToken(s1)];
			Vector tB = textures[getToken(s2)];
			Vector tC = textures[getToken(s3)];

			Vector nA = normals[getToken(s1)];
			Vector nB = normals[getToken(s2)];
			Vector nC = normals[getToken(s3)];

			m_box.m_faces.push_back(Triangle(A, B, C, tA, tB, tC, nA, nB, nC));
		}
	}

	Vector pointMin = points[0];
	Vector pointMax = points[0];

	for (auto point: points) {
		pointMin = min(pointMin, point);
		pointMax = max(pointMax, point);
	}

	m_box.m_min = pointMin;
	m_box.m_max = pointMax;

	m_box.constructBoxes();
}

Vector Mesh::normal(const Vector& point) const
{
	return point;
}

double Mesh::intersect(const Vector& origine, const Vector& direction, Vector* normal, Vector* inter, Color* color) const
{
	Vector texture = Vector(0., 0., 0.);
	double t = m_box.intersect(origine, direction, normal, inter, &texture);
	
	*color = getColorTexture(texture);

	return t;
}

Color Mesh::getColorTexture(Vector& texture) const
{
	int x = (int)texture.x();
	int y = (int)texture.y();

	//texture.print(); std::cout << std::endl << std::flush;
	//std::cout << (x*m_width + y)*3 << std::endl << std::flush;

	double red   = m_texture[(y*m_width + x)*3 + 0] / 255.;
	double green = m_texture[(y*m_width + x)*3 + 1] / 255.;
	double blue  = m_texture[(y*m_width + x)*3 + 2] / 255.;

	return Color(red, green, blue);
}

double Mesh::area() const
{
	return 0.;
}

Vector Mesh::getCenter() const
{
	return Vector(0., 0., 0.);
}

Vector Mesh::randomPointAround(const Vector& direction) const
{
	Vector wi = monteCarloVector(direction).normalize();

	return wi;
//	return getCenter() + m_radius * wi;
}

void Box::constructBoxes()
{
	if (m_faces.size() < RECURSION_BOXES)
		return;

	Vector limit = m_min + 0.5 * (m_max - m_min);

	m_boxes[0] = new Box(m_min, limit);
	m_boxes[1] = new Box(Vector(m_min.x(), m_min.y(), limit.z()), Vector(limit.x(), limit.y(), m_max.z()));
	m_boxes[2] = new Box(Vector(m_min.x(), limit.y(), m_min.z()), Vector(limit.x(), m_max.y(), limit.z()));
	m_boxes[3] = new Box(Vector(m_min.x(), limit.y(), limit.z()), Vector(limit.x(), m_max.y(), m_max.z()));
	m_boxes[4] = new Box(Vector(limit.x(), m_min.y(), m_min.z()), Vector(m_max.x(), limit.y(), limit.z()));
	m_boxes[5] = new Box(Vector(limit.x(), m_min.y(), limit.z()), Vector(m_max.x(), limit.y(), m_max.z()));
	m_boxes[6] = new Box(Vector(limit.x(), limit.y(), m_min.z()), Vector(m_max.x(), m_max.y(), limit.z()));
	m_boxes[7] = new Box(Vector(limit.x(), limit.y(), limit.z()), Vector(m_max.x(), m_max.y(), m_max.z()));

	for (auto box: m_boxes)
		for (auto face: m_faces) 
			if (box->contain(face))
				box->m_faces.push_back(face);

	for (auto box: m_boxes)
		box->constructBoxes();
}

bool Box::contain(Triangle& face) const
{
	double boxcenter[3];
	double boxhalfsize[3];
	double triverts[3][3];

	Vector center = 0.5 * (m_min + m_max) ;
    Vector halfsize = 0.5 * (m_max - m_min);

    boxcenter[0] = center.x();
    boxcenter[1] = center.y();
    boxcenter[2] = center.z();

    boxhalfsize[0] = halfsize.x();
    boxhalfsize[1] = halfsize.y();
    boxhalfsize[2] = halfsize.z();

    triverts[0][0] = face.a().x();
    triverts[0][1] = face.a().y();
    triverts[0][2] = face.a().z();
    triverts[1][0] = face.b().x();
    triverts[1][1] = face.b().y();
    triverts[1][2] = face.b().z();
    triverts[2][0] = face.c().x();
    triverts[2][1] = face.c().y();
    triverts[2][2] = face.c().z();

	return triBoxOverlap(boxcenter, boxhalfsize, triverts);
	//return contain(face.a()) || contain(face.b()) || contain(face.c());
}

bool Box::contain(Vector point) const
{
	return (m_min < point) && (point < m_max);
}

bool Box::intersect(const Vector& origine, const Vector& direction) const
{
	Vector vMin = m_min - origine;
	Vector vMax = m_max - origine;

	double tMinX = std::min(vMin.x()/direction.x(), vMax.x()/direction.x());
	double tMaxX = std::max(vMin.x()/direction.x(), vMax.x()/direction.x());

	double tMinY = std::min(vMin.y()/direction.y(), vMax.y()/direction.y());
	double tMaxY = std::max(vMin.y()/direction.y(), vMax.y()/direction.y());

	double tMinZ = std::min(vMin.z()/direction.z(), vMax.z()/direction.z());
	double tMaxZ = std::max(vMin.z()/direction.z(), vMax.z()/direction.z());

	if (std::min(std::min(tMaxX, tMaxY), tMaxZ) - std::max(std::max(tMinX, tMinY), tMinZ) > 0.)
		return true;

	return false;
}

double Box::intersect(const Vector& origine, const Vector& direction, Vector* normal, Vector* inter, Vector* texture) const
{
	if (!intersect(origine, direction))
		return -1.;

	double t = -1.;

	if (m_boxes[0] == nullptr) {
		double s;
		Vector n = Vector(0., 0., 0.), point = Vector(0., 0., 0.), text = Vector(0., 0., 0.);

		for (Triangle face : m_faces) {
			if ((s = face.intersect(origine, direction, &n, &point, &text)) != -1) {
				if (t == -1. || s < t) {
					*inter   = point;
					*normal  = n;
					*texture = text;
					t = s;
				}
			}
		}

		return t;
	}

	double s;
	Vector n = Vector(0., 0., 0.), point = Vector(0., 0., 0.), text = Vector(0., 0., 0.);

	for (auto box : m_boxes) {
		if ((s = box->intersect(origine, direction, &n, &point, &text)) != -1.) {
			if (t == -1 || s < t) {
				*inter   = point;
				*normal  = n;
				*texture = text;
				t = s;
			}
		}
	}

	return t;
}
