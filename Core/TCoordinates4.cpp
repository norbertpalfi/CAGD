#include "TCoordinates4.h"

using namespace cagd;

// special constructor
TCoordinate4::TCoordinate4(GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
    _data[0] = s;
    _data[1] = t;
    _data[2] = r;
    _data[3] = q;
}
// get components by value
GLfloat TCoordinate4::operator[](GLuint rhs) const
{
    return _data[rhs];
}
GLfloat TCoordinate4::s() const
{
    return _data[0];
}
GLfloat TCoordinate4::t() const
{
    return _data[1];
}
GLfloat TCoordinate4::r() const
{
    return _data[2];
}
GLfloat TCoordinate4::q() const
{
    return _data[3];
}

// get components by reference
GLfloat& TCoordinate4::operator[](GLuint rhs)
{
    return _data[rhs];
}

GLfloat& TCoordinate4::s()
{
    return _data[0];
}
GLfloat& TCoordinate4::t()
{
    return _data[1];
}
GLfloat& TCoordinate4::r()
{
    return _data[2];
}
GLfloat& TCoordinate4::q()
{
    return _data[3];
}
