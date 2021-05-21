#include "TriangularFaces.h"

using namespace cagd;

// copy constructor
TriangularFace::TriangularFace(const TriangularFace& face)
{
    _node[0] = face._node[0];
    _node[1] = face._node[1];
    _node[2] = face._node[2];
}

// assignment operator
TriangularFace& TriangularFace::operator =(const TriangularFace& rhs)
{
    _node[0] = rhs._node[0];
    _node[1] = rhs._node[1];
    _node[2] = rhs._node[2];
    return *this;
}

// get node identifiers by value
GLuint TriangularFace::operator [](GLuint i) const
{
    return _node[i];
}

// get node identifiers by reference
GLuint& TriangularFace::operator [](GLuint i)
{
    return _node[i];
}
