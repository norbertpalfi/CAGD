#include "SecondOrderTrigonometricArc.h"
#include "../Core/Constants.h"

using namespace cagd;

GLdouble SecondOrderTrigArc::f0(GLdouble t) const
{
    return f3(_alpha - t);
}

GLdouble SecondOrderTrigArc::f1(GLdouble t) const
{
    return f2(_alpha - t);
}

GLdouble SecondOrderTrigArc::f2(GLdouble t) const
{
    GLdouble sin4Alpha = sin(_alpha / 2) * sin(_alpha / 2) * sin(_alpha / 2) * sin(_alpha / 2);
    GLdouble sin3T = sin(t / 2) * sin(t / 2) * sin(t / 2);

    return ((4 * cos(_alpha / 2)) / sin4Alpha) * sin((_alpha - t) / 2) * sin3T +
            ((1 + 2 * cos(_alpha / 2) * cos(_alpha / 2)) / sin4Alpha)
            * (sin((_alpha - t) / 2) * sin((_alpha - t) / 2))
            * (sin(t / 2) * sin(t / 2));
}

GLdouble SecondOrderTrigArc::f3(GLdouble t) const
{
    GLdouble sin4Alpha = sin(_alpha / 2) * sin(_alpha / 2) * sin(_alpha / 2) * sin(_alpha / 2);
    GLdouble sin4T = sin(t / 2) * sin(t / 2) * sin(t / 2) * sin(t / 2);

    return (1 / (sin4Alpha)) * (sin4T);
}

GLdouble SecondOrderTrigArc::f0_1(GLdouble t) const
{
    return -f3_1(_alpha - t);
}

GLdouble SecondOrderTrigArc::f1_1(GLdouble t) const
{
    return -f2_1(_alpha - t);
}

GLdouble SecondOrderTrigArc::f2_1(GLdouble t) const
{
    GLdouble sin4Alpha = sin(_alpha / 2.0) * sin(_alpha / 2.0) * sin(_alpha / 2.0) * sin(_alpha / 2.0);
    return -(sin(t/2.0)*(2.0*cos(_alpha/2.0)*cos((_alpha-t)/2.0)*sin(t/2.0)*sin(t/2.0)+((2.0*cos(_alpha/2.0)*cos(_alpha/2.0)+1.0)*cos((_alpha-t)/2.0)*sin((_alpha-t)/2.0)-6*cos(_alpha/2.0)*sin((_alpha-t)/2.0)*cos(t/2.0))*sin(t/2.0)+(-2.0*cos(_alpha/2.0)*cos(_alpha/2.0)-1.0)*sin((_alpha-t)/2.0)*sin((_alpha-t)/2.0)*cos(t/2.0)))/sin4Alpha;
}

GLdouble SecondOrderTrigArc::f3_1(GLdouble t) const
{
    GLdouble sin4Alpha = sin(_alpha / 2.0) * sin(_alpha / 2.0) * sin(_alpha / 2.0) * sin(_alpha / 2.0);
    GLdouble sin3T = sin(t / 2.0) * sin(t / 2.0) * sin(t / 2.0);
    return (1.0 / (sin4Alpha)) * 2.0 * (sin3T) * cos(t / 2.0);
}

GLdouble SecondOrderTrigArc::f0_2(GLdouble t) const
{
    return f3_2(_alpha - t);
}

GLdouble SecondOrderTrigArc::f1_2(GLdouble t) const
{
    return f2_2(_alpha - t);
}

GLdouble SecondOrderTrigArc::f2_2(GLdouble t) const
{
    GLdouble sin4Alpha = sin(_alpha / 2.0) * sin(_alpha / 2.0) * sin(_alpha / 2.0) * sin(_alpha / 2.0);
    GLdouble sin3T = sin(t/2.0) * sin(t/2.0) * sin(t/2.0);

    return -(8.0*cos(_alpha/2.0)*sin((_alpha-t)/2.0)*sin3T+(12.0*cos(_alpha/2.0)*cos((_alpha-t)/2.0)*cos(t/2.0)+(4.0*cos(_alpha/2.0)*cos(_alpha/2.0)+2.0)*sin((_alpha-t)/2.0)*sin((_alpha-t)/2.0)+(-2*cos(_alpha/2.0)*cos(_alpha/2.0)-1.0)*cos((_alpha-t)/2.0)*cos((_alpha-t)/2.0))*sin(t/2.0)*sin(t/2.0)+((8.0*cos(_alpha/2.0)*cos(_alpha/2.0)+4.0)*cos((_alpha-t)/2.0)*sin((_alpha-t)/2.0)*cos(t/2.0)-12.0*cos(_alpha/2.0)*sin((_alpha-t)/2.0)*cos(t/2.0)*cos(t/2.0))*sin(t/2.0)+(-2.0*cos(_alpha/2.0)*cos(_alpha/2.0)-1.0)*sin((_alpha-t)/2.0)*sin((_alpha-t)/2.0)*cos(t/2.0)*cos(t/2.0))/(2.0*sin4Alpha);
}

GLdouble SecondOrderTrigArc::f3_2(GLdouble t) const
{
    GLdouble sin4Alpha = sin(_alpha / 2.0) * sin(_alpha / 2.0) * sin(_alpha / 2.0) * sin(_alpha / 2.0);
    GLdouble sin3T = sin(t / 2.0) * sin(t / 2.0) * sin(t / 2.0);
    return (1.0 / (sin4Alpha)) * 3.0 * (sin(t / 2.0) * sin(t / 2.0)) * cos(t / 2.0) * cos(t / 2.0) + (1.0 / (sin4Alpha)) * (sin3T) * -sin(t / 2.0);
}


GLboolean SecondOrderTrigArc::BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble> &values) const
{
    values.ResizeColumns(4);

    values[0] = f0(u);
    values[1] = f1(u);
    values[2] = f2(u);
    values[3] = f3(u);

    return GL_TRUE;
}

GLboolean SecondOrderTrigArc::CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives &d) const
{
    d.ResizeRows(max_order_of_derivatives + 1);
    d.LoadNullVectors();

    d[0] = f0(u) * _data[0] + f1(u) * _data[1] + f2(u) * _data[2] + f3(u) * _data[3];

    if(max_order_of_derivatives >= 1) {
        d[1] = f0_1(u) * _data[0] + f1_1(u) * _data[1] + f2_1(u) * _data[2] + f3_1(u) * _data[3];
    }

    if(max_order_of_derivatives == 2) {
        d[2] = f0_2(u) * _data[0] + f1_2(u) * _data[1] + f2_2(u) * _data[2] + f3_2(u) * _data[3];
    }
    return GL_TRUE;
}

GLboolean SecondOrderTrigArc::SetAlpha(GLdouble alpha) {
    _u_max = alpha;
    return _alpha = alpha;
}
GLdouble SecondOrderTrigArc::GetAlpha() const {
    return _alpha;
}

SecondOrderTrigArc::SecondOrderTrigArc(GLdouble alpha, GLenum data_usage_flag):LinearCombination3(0.0, alpha, 4, data_usage_flag)
{
    _alpha = alpha;
}
