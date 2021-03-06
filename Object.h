#ifndef OBJECT_HEADER
#define OBJECT_HEADER

#include "Vector.h"
#include "Color.h"
#include "Intersection.h"
#include "Animatable.h"

#include <functional>

Color one(const Vector&);

class Intersection;

class Object: public Animatable {
protected:
	Color  m_color;
	bool   m_mirror;
	bool   m_transparency;
	double m_indice;
	bool   m_diffuse;
	bool   m_emissive;
	double m_intensity;

	double m_ks;
	double m_phong;

    std::function<Color(const Vector&)> m_texture;

public:
	virtual Vector normal(const Point&) const = 0;
    virtual double intersect(const Point&, const Vector&, Intersection&) const = 0;
    virtual double area() const = 0;
    virtual Point center() const = 0;
    virtual Point randomPointAround(const Vector&) const = 0;

    Object(Color color = Color::black()): Animatable(), m_color{color}, m_mirror{false}, m_transparency{false}, m_indice{1.}, m_diffuse{false}, m_emissive{false}, m_intensity{0.}, m_ks{0.2}, m_phong{20}, m_texture{one} {}
    virtual ~Object() = default;

    void makeSpecular() { m_mirror = false, m_transparency = false, m_diffuse = false, m_emissive = false; }
    void makeMirror() { m_mirror = true, m_transparency = false, m_diffuse = false, m_emissive = false; }
    void makeTransparent(double indice) { m_mirror = false, m_transparency = true, m_diffuse = false, m_indice = indice, m_emissive = false; }
    void makeDiffuse() { m_mirror = false, m_transparency = false, m_diffuse = true, m_emissive = false; }
    void makeEmissive(double intensity) { m_mirror = false, m_transparency = false, m_diffuse = false, m_emissive = true, m_intensity = intensity; }
    void addTexture(std::function<Color(const Vector&)> texture) { m_texture = texture; }

    Color color(Vector = Vector()) const;
    bool isMirror() const { return m_mirror; }
    bool isTransparent() const { return m_transparency; }
    double indice() const { return m_indice; }
    bool isDiffuse() const { return m_diffuse; }
    bool isEmissive() const { return m_emissive; }
    double intensity() const { return m_intensity / area(); }
    double ks() const { return m_ks; }
    double phongExp() const { return m_phong; }

    Base baseAt(Vector) const;
    Vector monteCarloVector(const Vector&) const;
    Vector phongVector(const Vector&, double) const;
};

#endif
