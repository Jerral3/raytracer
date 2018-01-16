#ifndef OBJECT_HEADER
#define OBJECT_HEADER

#include "Vector.h"
#include "Color.h"

class Object {
protected:
	Color  m_color;
	bool   m_mirror;
	bool   m_transparency;
	double m_indice;
	bool   m_diffuse;
	bool   m_emissive;
	double m_intensity;

public:
	virtual Vector normal(const Vector&) const = 0;
    virtual double intersect(const Vector&, const Vector&) const = 0;

    Object(Color color = Color::black()): m_color{color}, m_mirror{false}, m_transparency{false}, m_indice{1.}, m_diffuse{false}, m_emissive{false}, m_intensity{0.} {}
    virtual ~Object() = default;

    void makeSpecular() { m_mirror = false, m_transparency = false, m_diffuse = false, m_emissive = false; }
    void makeMirror() { m_mirror = true, m_transparency = false, m_diffuse = false, m_emissive = false; }
    void makeTransparent(double indice) { m_mirror = false, m_transparency = true, m_diffuse = false, m_indice = indice, m_emissive = false; }
    void makeDiffuse() { m_mirror = false, m_transparency = false, m_diffuse = true, m_emissive = false; }
    void makeEmissive(double intensity) { m_mirror = false, m_transparency = false, m_diffuse = false, m_emissive = true, m_intensity = intensity; }

    Color getColor() const { return m_color; }
    bool isMirror() const { return m_mirror; }
    bool isTransparent() const { return m_transparency; }
    double getIndice() const { return m_indice; }
    bool isDiffuse() const { return m_diffuse; }
    bool isEmissive() const { return m_emissive; }
    double getIntensity() const { return m_intensity; }

    Base getBaseAt(const Vector&) const;
    Vector monteCarloVector(const Vector&) const;
};

#endif
