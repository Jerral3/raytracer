#ifndef COLOR_HEADER
#define COLOR_HEADER

class Color {
	double m_red;
	double m_green;
	double m_blue;

	double m_intensity;

public:
	Color(double red = 0., double green = 0., double blue = 0., double intensity = 1.): m_red{red}, m_green{green}, m_blue{blue}, m_intensity{intensity} {}
	Color(Color color, double intensity): m_red{color.m_red}, m_green{color.m_green}, m_blue{color.m_blue}, m_intensity{intensity} {}

	Color& operator+=(const Color&);
	Color& operator*=(double l);

	Color specular(const Color&);

	double red() const { return m_red; }
	double green() const { return m_green; }
	double blue() const { return m_blue; }

	double getIntensity(int) const;
	double getIntensity() const { return m_intensity; }

	void print();
};

Color operator*(double, Color&);

#endif
