#ifndef SOMEARCS3_H
#define SOMEARCS3_H

#include <Core/Constants.h>
#include <Core/LinearCombination3.h>

namespace cagd
{
    class SecondOrderTrigArc: public LinearCombination3
    {
    private:
        GLdouble _alpha = PI / 2.0;

        GLdouble f0(GLdouble t) const;
        GLdouble f1(GLdouble t) const;
        GLdouble f2(GLdouble t) const;
        GLdouble f3(GLdouble t) const;

        GLdouble f0_1(GLdouble t) const;
        GLdouble f1_1(GLdouble t) const;
        GLdouble f2_1(GLdouble t) const;
        GLdouble f3_1(GLdouble t) const;

        GLdouble f0_2(GLdouble t) const;
        GLdouble f1_2(GLdouble t) const;
        GLdouble f2_2(GLdouble t) const;
        GLdouble f3_2(GLdouble t) const;

    public:
        // Variables for the interaction with curves
        SecondOrderTrigArc *left, *rigth;
        GLuint image_index;

        // special/default constructor
        SecondOrderTrigArc(GLdouble alpha = PI / 2.0, GLenum data_usage_flag = GL_STATIC_DRAW);

        // inherited pure virtual methods have to be redeclared and defined
        GLboolean BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble> &values) const;
        GLboolean CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives& d) const;

        // project-dependent setters/getters, e.g.,


        // if exists, you will need to set/get the shape/tension parameter of the basis/blending functions
        GLboolean SetAlpha(GLdouble alpha);
        GLdouble GetAlpha() const;

        GLboolean SetPoint(GLuint row, GLuint col, GLdouble x, GLdouble y, GLdouble z);
        GLboolean SetPoint(GLuint row, GLuint col, const DCoordinate3 &position);
    };
}

#endif // SOMEARCS3_H
