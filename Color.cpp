#include "Color.h"

#include "config.h"

#include <iostream>
#include <cmath>

Color& Color::operator+=(const Color& c)
{
	m_red   += c.intensity(RED);
	m_green += c.intensity(GREEN);
	m_blue  += c.intensity(BLUE);

	return *this;
}

Color& Color::operator*=(double l)
{
	m_red   *= l;
	m_green *= l;
	m_blue  *= l;

	return *this;
}

Color& Color::operator*=(const Color& color)
{
	m_red   *= color.intensity(RED);
	m_green *= color.intensity(GREEN);
	m_blue  *= color.intensity(BLUE);

	return *this;
}

bool Color::operator==(const Color& c) const
{
	return intensity(RED) == c.intensity(RED) && intensity(GREEN) == c.intensity(GREEN) && intensity(BLUE) == c.intensity(BLUE);
}

double Color::intensity(int k) const
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

Color& Color::gamma()
{
#if CORRECTION_GAMMA == 1
	double gamma = 1./GAMMA;

	m_red   = std::pow(intensity(RED),   gamma);
	m_green = std::pow(intensity(GREEN), gamma);
	m_blue  = std::pow(intensity(BLUE),  gamma);

	return *this;
#else
	return *this;
#endif
}

void Color::print()
{
	std::cout << "(r,g,b) : (" << intensity(RED) << "," << intensity(GREEN) << "," << intensity(BLUE) << ") "; 
}

Color operator+(Color c, Color d)
{
	return c += d;
}

Color operator*(Color c, Color d)
{
	return c *= d;
}

Color operator*(double l, Color c)
{
	return c *= l;
}
