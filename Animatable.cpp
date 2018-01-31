#include "Animatable.h"

#include "Animation.h"

Vector Animatable::translation(double t) const
{
	Vector movement = Vector();

	for (auto animation: m_animations)
		movement += animation->translation(t);

	return movement;
}

Point Animatable::rotation(const Point& center, double t) const
{
	Point newCenter = center;

	for (auto animation: m_animations)
		newCenter = animation->rotation(newCenter, t);

	return newCenter;
}

Matrix Animatable::rotation(double t) const
{
	Matrix r = Matrix::identity();

	for (auto animation: m_animations)
		r = r * animation->rotation(t);

	return r;
}

Point Animatable::invrot(const Point& center, double t) const
{
	Point newCenter = center;

	for (auto animation: m_animations)
		newCenter = animation->invrot(newCenter, t);

	return newCenter;
}

Matrix Animatable::invrot(double t) const
{
	Matrix r = Matrix::identity();

	for (auto animation: m_animations)
		r = r * animation->invrot(t);

	return r;
}
