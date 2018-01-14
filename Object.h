#ifndef OBJECT_HEADER
#define OBJECT_HEADER

#include "Ray.h"
#include "Vector.h"
#include "Color.h"

class Object {
protected:
	Color m_color;
	bool  m_mirror;
	bool m_transparency;
	double m_indice;
	bool m_diffuse;

public:
	virtual Vector normal(const Vector&) const = 0;
    virtual double intersect(const Ray&) const = 0;

    Object(Color color, bool mirror = false, bool transparency = false, double indice = 1., bool diffuse = false): m_color{color}, m_mirror{mirror}, m_transparency{transparency}, m_indice{indice}, m_diffuse{diffuse} {}
    virtual ~Object() = default;

    Color getColor() const { return m_color; }
    bool isMirror() const { return m_mirror; }
    bool isTransparent() const { return m_transparency; }
    double getIndice() const { return m_indice; }
    bool isDiffuse() const { return m_diffuse; }

    Base getBaseAt(const Vector&) const;
    Vector monteCarloVector(const Vector&) const;
};

#endif
