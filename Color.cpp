#include "Color.h"

#include "config.h"

#include <iostream>

Color& Color::operator+=(const Color& c)
{
	m_red   = (getIntensity(RED) + c.getIntensity(RED))/(m_intensity + c.m_intensity);
	m_green = (getIntensity(GREEN) + c.getIntensity(GREEN))/(m_intensity + c.m_intensity);
	m_blue  = (getIntensity(BLUE) + c.getIntensity(BLUE))/(m_intensity + c.m_intensity);

	//m_intensity += c.m_intensity;

	return *this;
}

Color& Color::operator*=(double l)
{
	m_intensity *= l;

	return *this;
}

double Color::getIntensity(int k) const
{
	double color = 0.;

	switch(k) {
		case RED:
			color = m_intensity*m_red;
			break;
		case GREEN:
			color = m_intensity*m_green;
			break;
		case BLUE:
			color = m_intensity*m_blue;
			break;
	}

	return color;
}

Color Color::specular(const Color& color) 
{
	m_red   *= color.m_red;
	m_green *= color.m_green;
	m_blue  *= color.m_blue;

	return *this;
}

void Color::print()
{
	std::cout << "(r,g,b) : (" << m_red << "," << m_green << "," << m_blue << ") "; 
}

Color operator*(double l, Color& c)
{
	return c *= l;
}
