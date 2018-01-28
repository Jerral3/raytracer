#ifndef COLOR_HEADER
#define COLOR_HEADER

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
	bool operator==(Color v) const { return m_blue == v.m_blue && m_red == v.m_red && m_green == v.m_green;  }

	Color& specular(const Color&);
	Color& gamma();

	double getIntensity(int) const;

	void print();
};

Color operator*(double, Color);

#endif
