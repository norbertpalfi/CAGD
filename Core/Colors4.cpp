#include "Colors4.h"

using namespace cagd;

GLfloat Color4::operator [](GLuint rhs) const {
    return _data[rhs];
}

GLfloat Color4::r() const {
    return _data[0];
}

GLfloat Color4::g() const {
    return  _data[1];
}

GLfloat Color4::b() const {
    return  _data[2];
}

GLfloat Color4::a() const {
    return _data[3];
}

GLfloat& Color4::operator [](GLuint rhs)
{
    return _data[rhs];
}
GLfloat& Color4::r()
{
    return _data[0];
}
GLfloat& Color4::g()
{
    return _data[1];
}
GLfloat& Color4::b()
{
    return _data[2];
}
GLfloat& Color4::a()
{
    return _data[3];
}
