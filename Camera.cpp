#include "Camera.h"

#include "config.h"
#include <cmath>
#include <random>

static std::default_random_engine engine;
static std::uniform_real_distribution<double> distrib(0,1);

Ray Camera::ray(int i, int j, double t) const
{
#if ANTIALIASING == 1
	double r1 = distrib(engine);
	double r2 = distrib(engine);
	double l  = std::sqrt(-2 * std::log(r1));

	double dx = l*std::cos(2*M_PI*r2);
	double dy = l*std::sin(2*M_PI*r2);
#else
	double dx = 0, dy = 0;
#endif

	Point  origine      = m_origine;
	Vector direction    = m_direction;
	Vector directionRay = Vector(j + 0.5 - SCREEN_WIDTH/2 + dx, SCREEN_HEIGHT/2 - i - 0.5 - dy, SCREEN_HEIGHT/(2.*tan(fov()/2.))).normalize();

	origine   += translation(t);
	origine    = rotation(origine, t);
	direction  = rotation(t) * direction; 

	Vector right = direction ^ m_up;

	direction = directionRay.x() * right + directionRay.y() * m_up + directionRay.z() * direction;

#if CAMERA_MODEL == 1
	double aperture_x = CAMERA_FIELD * (distrib(engine) - 0.5);
	double aperture_y = CAMERA_FIELD * (distrib(engine) - 0.5);

	origine += Vector(aperture_x, aperture_y, 0.); 
	
	Vector destination = origine + m_focus * directionRay;
	direction = destination - origine;
#endif

	return Ray(origine, direction);
}
