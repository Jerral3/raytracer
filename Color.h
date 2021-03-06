#ifndef COLOR_HEADER
#define COLOR_HEADER

#include <cmath>

class Color {
	double m_red;
	double m_green;
	double m_blue;

public:
	Color(double red, double green, double blue): m_red{red}, m_green{green}, m_blue{blue} {}

	static Color black() { return Color(0., 0., 0.); }
	static Color red() { return Color(1., 0., 0.); }
	static Color green() { return Color(0., 1., 0.); }
	static Color blue() { return Color(0., 0., 1.); }
	static Color white() { return Color(1., 1., 1.); }

	Color& operator+=(const Color&);
	Color& operator*=(double l);
	Color& operator*=(const Color&);
	bool operator==(const Color& v) const;
	double max() const {return std::fmax(m_red, std::fmax(m_green, m_blue)); };

	Color& gamma();

	double intensity(int) const;

	void print();
};

Color operator+(Color, Color);
Color operator*(Color, Color);
Color operator*(double, Color);

#endif
