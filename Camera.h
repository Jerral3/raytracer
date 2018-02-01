#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include "Vector.h"
#include "Ray.h"
#include "Animatable.h"

class Camera: public Animatable {
	Point  m_origine;
	Vector m_direction;
	Vector m_up;

	double m_fov;
	double m_focus;

public:
	Camera(Point o, Vector d, Vector u, double fov, double focus): Animatable(), m_origine{o}, m_direction{d}, m_up{u}, m_fov{fov}, m_focus{focus} {}

	Point  origine() const { return m_origine; }
	double fov()     const { return m_fov; }
	double focus()   const { return m_focus; }

	Ray ray(int, int, double) const;
};

#endif

