#include "Color.h"

#include "config.h"

#include <iostream>
#include <cmath>

Color& Color::operator+=(const Color& c)
{
	m_red   += c.getIntensity(RED);
	m_green += c.getIntensity(GREEN);
	m_blue  += c.getIntensity(BLUE);

	return *this;
}

Color& Color::operator*=(double l)
{
	m_red   *= l;
	m_green *= l;
	m_blue  *= l;

	return *this;
}

double Color::getIntensity(int k) const
{
	double color = 0.;

	switch(k) {
		case RED:
			color = m_red;
			break;
		case GREEN:
			color = m_green;
			break;
		case BLUE:
			color = m_blue;
			break;
	}

	return color;
}

Color& Color::specular(const Color& color)
{
	m_red   *= color.m_red;
	m_green *= color.m_green;
	m_blue  *= color.m_blue;

	return *this;
}

Color& Color::gamma()
{
	m_red   = std::pow(m_red,   1/GAMMA);
	m_blue  = std::pow(m_blue,  1/GAMMA);
	m_green = std::pow(m_green, 1/GAMMA);

	return *this;
}

void Color::print()
{
	std::cout << "(r,g,b) : (" << m_red << "," << m_green << "," << m_blue << ") "; 
}

Color operator*(double l, Color c)
{
	return c *= l;
}
