#pragma once

#include <GL/glew.h>

namespace cagd
{
    class Color4
    {
    protected:
        GLfloat _data[4]; // (r, g, b, a)

    public:
        Color4()
        {
            _data[0] = _data[1] = _data[2] = 0.0;
            _data[3] = 1.0;
        }

        Color4(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f)
        {
            _data[0] = r;
            _data[1] = g;
            _data[2] = b;
            _data[3] = a;
        }

        // get components by value
        GLfloat operator [](GLuint rhs) const;
        GLfloat r() const;
        GLfloat g() const;
        GLfloat b() const;
        GLfloat a() const;

        // get components by reference
        GLfloat& operator [](GLuint rhs);
        GLfloat& r();
        GLfloat& g();
        GLfloat& b();
        GLfloat& a();
    };
}
