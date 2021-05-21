#pragma once

#include <cmath>
#include <GL/glew.h>
#include <iostream>

namespace cagd
{
    //--------------------------------------
    // 3-dimensional homogeneous coordinates
    //--------------------------------------
    class HCoordinate3
    {
    protected:
        GLfloat _data[4]; // x, y, z, w;

    public:
        // default constructor
        HCoordinate3();

        // special constructor
        HCoordinate3(GLfloat x, GLfloat y, GLfloat z = 0.0, GLfloat w = 1.0);

        // homework: get components by value
        GLfloat operator[](GLuint rhs) const;
        GLfloat x() const;
        GLfloat y() const;
        GLfloat z() const;
        GLfloat w() const;

        // homework: get components by reference
        GLfloat& operator[](GLuint rhs);
        GLfloat& x();
        GLfloat& y();
        GLfloat& z();
        GLfloat& w();

        // add
        const HCoordinate3 operator +(const HCoordinate3& rhs) const;

        // add to this
        HCoordinate3& operator +=(const HCoordinate3& rhs);

        // subtract
        const HCoordinate3 operator -(const HCoordinate3& rhs) const;

        //  subtract from this
        HCoordinate3& operator -=(const HCoordinate3& rhs);

        //  dot product
        GLfloat operator *(const HCoordinate3& rhs) const;

        //  cross product
        const HCoordinate3 operator ^(const HCoordinate3& rhs) const;

        //  cross product with this
        HCoordinate3& operator ^=(const HCoordinate3& rhs);

        //  multiplicate with scalar from right
        const HCoordinate3 operator *(GLfloat rhs) const;

        //  multiplicate this with a scalar
        HCoordinate3& operator *=(GLfloat rhs);

        //  divide with scalar
        const HCoordinate3 operator /(GLfloat rhs) const;

        //  divide this with a scalar
        HCoordinate3& operator /=(GLfloat rhs);

        //  length of vector represented by this homogeneous coordinate
        GLfloat length() const;

        //  normalize
        HCoordinate3& normalize();
    };

    // default constructor
    inline HCoordinate3::HCoordinate3()
    {
        _data[0] = _data[1] = _data[2] = 0.0;
        _data[3] = 1.0;
    }

    // special constructor
    inline HCoordinate3::HCoordinate3(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        _data[0] = x;
        _data[1] = y;
        _data[2] = z;
        _data[3] = w;
    }

    // add
    inline const HCoordinate3 HCoordinate3::operator +(const HCoordinate3& rhs) const
    {
        return HCoordinate3(
                rhs.w() * x() + w() * rhs.x(),
                rhs.w() * y() + w() * rhs.y(),
                rhs.w() * z() + w() * rhs.z(),
                w() * rhs.w());
    }

    //  scale from left with a scalar
    inline const HCoordinate3 operator *(GLfloat lhs, const HCoordinate3& rhs) {
        return HCoordinate3(lhs*rhs.x(), lhs*rhs.y(), lhs*rhs.z(), rhs.w());
    }

    //  output to stream
    inline std::ostream& operator <<(std::ostream& lhs, const HCoordinate3& rhs) {
        lhs << rhs.x() << " " << rhs.y() << " " << rhs.z() << " " << rhs.w() << std::endl;
        return lhs;
    }

    //  input from stream
    inline std::istream& operator >>(std::istream& lhs, HCoordinate3& rhs) {
        lhs >> rhs.x() >> rhs.y() >> rhs.z() >> rhs.w();
        return lhs;
    }

}
