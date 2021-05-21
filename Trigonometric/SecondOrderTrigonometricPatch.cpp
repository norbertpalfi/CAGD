#include "SecondOrderTrigonometricPatch.h"

using namespace cagd;

GLdouble SecTrigPatch3::f0(GLdouble alpha, GLdouble t) const
{
    return f3(alpha, alpha - t);
}

GLdouble SecTrigPatch3::f1(GLdouble alpha, GLdouble t) const
{
    return f2(alpha, alpha - t);
}

GLdouble SecTrigPatch3::f2(GLdouble alpha, GLdouble t) const
{
    GLdouble sin4Alpha = sin(alpha / 2) * sin(alpha / 2) * sin(alpha / 2) * sin(alpha / 2);
    GLdouble sin3T = sin(t / 2) * sin(t / 2) * sin(t / 2);

    return ((4 * cos(alpha / 2)) / sin4Alpha) * sin((alpha - t) / 2) * sin3T +
            ((1 + 2 * cos(alpha / 2) * cos(alpha / 2)) / sin4Alpha)
            * (sin((alpha - t) / 2) * sin((alpha - t) / 2))
            * (sin(t / 2) * sin(t / 2));
}

GLdouble SecTrigPatch3::f3(GLdouble alpha, GLdouble t) const
{
    GLdouble sin4Alpha = sin(alpha / 2) * sin(alpha / 2) * sin(alpha / 2) * sin(alpha / 2);
    GLdouble sin4T = sin(t / 2) * sin(t / 2) * sin(t / 2) * sin(t / 2);

    return (1 / (sin4Alpha)) * (sin4T);
}

GLdouble SecTrigPatch3::f0_1(GLdouble alpha, GLdouble t) const
{
    return -f3_1(alpha, alpha - t);
}

GLdouble SecTrigPatch3::f1_1(GLdouble alpha, GLdouble t) const
{
    return -f2_1(alpha, alpha - t);
}

GLdouble SecTrigPatch3::f2_1(GLdouble alpha, GLdouble t) const
{
    GLdouble sin4Alpha = sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0);
    return -(sin(t/2.0)*(2.0*cos(alpha/2.0)*cos((alpha-t)/2.0)*sin(t/2.0)*sin(t/2.0)+((2.0*cos(alpha/2.0)*cos(alpha/2.0)+1.0)*cos((alpha-t)/2.0)*sin((alpha-t)/2.0)-6*cos(alpha/2.0)*sin((alpha-t)/2.0)*cos(t/2.0))*sin(t/2.0)+(-2.0*cos(alpha/2.0)*cos(alpha/2.0)-1.0)*sin((alpha-t)/2.0)*sin((alpha-t)/2.0)*cos(t/2.0)))/sin4Alpha;
}

GLdouble SecTrigPatch3::f3_1(GLdouble alpha, GLdouble t) const
{
    GLdouble sin4Alpha = sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0);
    GLdouble sin3T = sin(t / 2.0) * sin(t / 2.0) * sin(t / 2.0);
    return (1.0 / (sin4Alpha)) * 2.0 * (sin3T) * cos(t / 2.0);
}

GLdouble SecTrigPatch3::f0_2(GLdouble alpha, GLdouble t) const
{
    return f3_2(alpha, alpha - t);
}

GLdouble SecTrigPatch3::f1_2(GLdouble alpha, GLdouble t) const
{
    return f2_2(alpha, alpha - t);
}

GLdouble SecTrigPatch3::f2_2(GLdouble alpha, GLdouble t) const
{
    GLdouble sin4Alpha = sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0);
    GLdouble sin3T = sin(t/2.0) * sin(t/2.0) * sin(t/2.0);

    return -(8.0*cos(alpha/2.0)*sin((alpha-t)/2.0)*sin3T+(12.0*cos(alpha/2.0)*cos((alpha-t)/2.0)*cos(t/2.0)+(4.0*cos(alpha/2.0)*cos(alpha/2.0)+2.0)*sin((alpha-t)/2.0)*sin((alpha-t)/2.0)+(-2*cos(alpha/2.0)*cos(alpha/2.0)-1.0)*cos((alpha-t)/2.0)*cos((alpha-t)/2.0))*sin(t/2.0)*sin(t/2.0)+((8.0*cos(alpha/2.0)*cos(alpha/2.0)+4.0)*cos((alpha-t)/2.0)*sin((alpha-t)/2.0)*cos(t/2.0)-12.0*cos(alpha/2.0)*sin((alpha-t)/2.0)*cos(t/2.0)*cos(t/2.0))*sin(t/2.0)+(-2.0*cos(alpha/2.0)*cos(alpha/2.0)-1.0)*sin((alpha-t)/2.0)*sin((alpha-t)/2.0)*cos(t/2.0)*cos(t/2.0))/(2.0*sin4Alpha);
}

GLdouble SecTrigPatch3::f3_2(GLdouble alpha, GLdouble t) const
{
    GLdouble sin4Alpha = sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0) * sin(alpha / 2.0);
    GLdouble sin3T = sin(t / 2.0) * sin(t / 2.0) * sin(t / 2.0);
    return (1.0 / (sin4Alpha)) * 3.0 * (sin(t / 2.0) * sin(t / 2.0)) * cos(t / 2.0) * cos(t / 2.0) + (1.0 / (sin4Alpha)) * (sin3T) * -sin(t / 2.0);
}

SecTrigPatch3::SecTrigPatch3(GLdouble u, GLdouble v):TensorProductSurface3(0.0, u, 0.0, v){ _alpha[0] = u; _alpha[1] = v; }

GLboolean SecTrigPatch3::UBlendingFunctionValues(
        GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const
{
    blending_values.ResizeColumns(5);

    blending_values[0] = f0(_alpha[0], u_knot);
    blending_values[1] = f1(_alpha[0], u_knot);
    blending_values[2] = f2(_alpha[0], u_knot);
    blending_values[3] = f3(_alpha[0], u_knot);

    return GL_TRUE;
}

GLboolean SecTrigPatch3::VBlendingFunctionValues(
        GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const
{
    blending_values.ResizeColumns(5);

    blending_values[0] = f0(_alpha[1], v_knot);
    blending_values[1] = f1(_alpha[1], v_knot);
    blending_values[2] = f2(_alpha[1], v_knot);
    blending_values[3] = f3(_alpha[1], v_knot);

    return GL_TRUE;
}

GLboolean SecTrigPatch3::CalculatePartialDerivatives(
        GLuint maximum_order_of_partial_derivatives,
        GLdouble u, GLdouble v, PartialDerivatives& pd) const
{
    if (u < 0.0 || u > _alpha[0] || v < 0.0 || v > _alpha[1] || maximum_order_of_partial_derivatives > 1)
        return GL_FALSE;

    RowMatrix<GLdouble> u_blending_values(4), d1_u_blending_values(4);
    RowMatrix<GLdouble> v_blending_values(4), d1_v_blending_values(4);

    UBlendingFunctionValues(u, u_blending_values);
    VBlendingFunctionValues(v, v_blending_values);

    d1_u_blending_values(0) = f0_1(_alpha[0], u);
    d1_u_blending_values(1) = f1_1(_alpha[0], u);
    d1_u_blending_values(2) = f2_1(_alpha[0], u);
    d1_u_blending_values(3) = f3_1(_alpha[0], u);

    d1_v_blending_values(0) = f0_1(_alpha[1], v);
    d1_v_blending_values(1) = f1_1(_alpha[1], v);
    d1_v_blending_values(2) = f2_1(_alpha[1], v);
    d1_v_blending_values(3) = f3_1(_alpha[1], v);

    pd.ResizeRows(2);
    pd.LoadNullVectors();

    for(GLuint row = 0; row < 4; ++row)
    {
        DCoordinate3 aux_d0_v, aux_d1_v;
        for (GLuint column = 0; column < 4; ++column)
        {
            aux_d0_v += _data(row,column) * v_blending_values(column);
            aux_d1_v += _data(row, column) * d1_v_blending_values(column);
        }
        pd(0,0) += aux_d0_v * u_blending_values(row);
        pd(1,0) += aux_d0_v * d1_u_blending_values(row);
        pd(1,1) += aux_d1_v * u_blending_values(row);
    }

    return GL_TRUE;
}

GLboolean SecTrigPatch3::SetUAlpha(GLdouble alpha) {
    _alpha[0] = alpha;
    _u_max = alpha;
    return true;
}
GLdouble SecTrigPatch3::GetUAlpha() {
    return _alpha[0];
}

GLboolean SecTrigPatch3::SetVAlpha(GLdouble alpha) {
    _alpha[1] = alpha;
    _v_max = alpha;
    return true;
}
GLdouble SecTrigPatch3::GetVAlpha() {
    return _alpha[1];
}

