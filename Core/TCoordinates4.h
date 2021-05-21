#pragma once

#include <GL/glew.h>
#include <iostream>

namespace cagd
{
    //-------------------------------------
    // four dimensional texture coordinates
    //-------------------------------------
    class TCoordinate4
    {
    protected:
        GLfloat _data[4]; // (s, t, r, q)

    public:
        // default constructor
        TCoordinate4();

        // special constructor
        TCoordinate4(GLfloat s, GLfloat t, GLfloat r = 0.0, GLfloat q = 1.0);

        //  get components by value
        GLfloat operator[](GLuint rhs) const;
        GLfloat s() const;
        GLfloat t() const;
        GLfloat r() const;
        GLfloat q() const;

        // get components by reference
        GLfloat& operator[](GLuint rhs);
        GLfloat& s();
        GLfloat& t();
        GLfloat& r();
        GLfloat& q();
    };

    // default constructor
    inline TCoordinate4::TCoordinate4()
    {
        _data[0] = _data[1] = _data[2] = 0.0;
        _data[3] = 1.0;
    }

    // output to stream
    inline std::ostream& operator <<(std::ostream& lhs, const TCoordinate4& rhs)
    {
        return lhs << rhs.s() << " " << rhs.t() << rhs.r() << " " << rhs.q();
    }

    // input from stream
    inline std::istream& operator >>(std::istream& lhs, TCoordinate4& rhs) {
        return lhs >> rhs.s() >> rhs.t() >> rhs.r() >> rhs.q();
    }
}
