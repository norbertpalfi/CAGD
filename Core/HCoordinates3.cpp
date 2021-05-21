#include "HCoordinates3.h"

using namespace cagd;

//  add to this
HCoordinate3& HCoordinate3::operator +=(const HCoordinate3& rhs) {
    x() = rhs.w() * x() + w() * rhs.x(),
    y() = rhs.w() * y() + w() * rhs.y(),
    z() = rhs.w() * z() + w() * rhs.z(),
    w() = w() * rhs.w();

    return *this;
}

//  subtract
const HCoordinate3 HCoordinate3::operator -(const HCoordinate3& rhs) const {
    return HCoordinate3(
                    rhs.w() * x() - w() * rhs.x(),
                    rhs.w() * y() - w() * rhs.y(),
                    rhs.w() * z() - w() * rhs.z(),
                    w() * rhs.w());
}

//  subtract from this
HCoordinate3& HCoordinate3::operator -=(const HCoordinate3& rhs) {
    x() = rhs.w() * x() - w() * rhs.x(),
    y() = rhs.w() * y() - w() * rhs.y(),
    z() = rhs.w() * z() - w() * rhs.z(),
    w() = w() * rhs.w();

    return *this;
}

//  dot product
GLfloat HCoordinate3::operator *(const HCoordinate3& rhs) const {
    GLfloat wd = w() * rhs.w();
    GLfloat res = (x() * rhs.x()) / wd +
                  (y() * rhs.y()) / wd +
                  (z() * rhs.z()) / wd;

    return res;
}

//  cross product
const HCoordinate3 HCoordinate3::operator ^(const HCoordinate3& rhs) const {
    return HCoordinate3(
                y() * rhs.z() - z() * rhs.y(),
                z() * rhs.x() - x() * rhs.z(),
                x() * rhs.y() - y() * rhs.x(),
                w() * rhs.w()
                );
}

//  cross product with this
HCoordinate3& HCoordinate3::operator ^=(const HCoordinate3& rhs) {
    GLfloat xtemp = y() * rhs.z() - z() * rhs.y();
    GLfloat ytemp = z() * rhs.x() - x() * rhs.z();

    z() = x() * rhs.y() - y() * rhs.x();
    w() = w() * rhs.w();

    x() = xtemp;
    y() = ytemp;

    return *this;
}

//  multiplicate with scalar from right
const HCoordinate3 HCoordinate3::operator *(GLfloat rhs) const {
    return HCoordinate3(x()*rhs, y()*rhs, z()*rhs, w());
}

//  multiplicate this with a scalar
HCoordinate3& HCoordinate3::operator *=(GLfloat rhs) {
    x() *= rhs;
    y() *= rhs;
    z() *= rhs;

    return *this;
}

// divide with scalar
const HCoordinate3 HCoordinate3::operator /(GLfloat rhs) const {
    return HCoordinate3(x(), y(), z(), w()*rhs);
};

// divide this with a scalar
HCoordinate3& HCoordinate3::operator /=(GLfloat rhs) {
    w() *= rhs;

    return *this;
}

// length of vector represented by this homogeneous coordinate
GLfloat HCoordinate3::length() const {
    return sqrt((*this) * (*this));
}

// normalize
HCoordinate3& HCoordinate3::normalize() {
    GLfloat l = length();

    if (l && l != 1.0)
        *this /= l;

    return *this;
}

// get components by value
GLfloat HCoordinate3::operator[](GLuint rhs) const
{
    return _data[rhs];
}
GLfloat HCoordinate3::x() const
{
    return _data[0];
}
GLfloat HCoordinate3::y() const
{
    return _data[1];
}
GLfloat HCoordinate3::z() const
{
    return _data[2];
}
GLfloat HCoordinate3::w() const
{
    return _data[3];
}

//get components by reference
GLfloat& HCoordinate3::operator[](GLuint rhs)
{
    return _data[rhs];
}
GLfloat& HCoordinate3::x()
{
    return _data[0];
}
GLfloat& HCoordinate3::y()
{
    return _data[1];
}
GLfloat& HCoordinate3::z()
{
    return _data[2];
}
GLfloat& HCoordinate3::w()
{
    return _data[3];
}
