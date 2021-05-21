#ifndef SECONDORDERTRIGONOMETRICPATCH_H
#define SECONDORDERTRIGONOMETRICPATCH_H
#include <Core/TensorProductSurfaces3.h>
#include "../Core/Constants.h"

namespace cagd
{
    enum Variable{U, V};

    class SecTrigPatch3: public TensorProductSurface3
    {
    private:
        GLdouble _alpha[2]; // _alpha[U], _alpha[V], i.e., possible shape parameters in directions u and v, respective

        GLdouble f0(GLdouble alpha, GLdouble t) const;
        GLdouble f1(GLdouble alpha, GLdouble t) const;
        GLdouble f2(GLdouble alpha, GLdouble t) const;
        GLdouble f3(GLdouble alpha, GLdouble t) const;

        GLdouble f0_1(GLdouble alpha, GLdouble t) const;
        GLdouble f1_1(GLdouble alpha, GLdouble t) const;
        GLdouble f2_1(GLdouble alpha, GLdouble t) const;
        GLdouble f3_1(GLdouble alpha, GLdouble t) const;

        GLdouble f0_2(GLdouble alpha, GLdouble t) const;
        GLdouble f1_2(GLdouble alpha, GLdouble t) const;
        GLdouble f2_2(GLdouble alpha, GLdouble t) const;
        GLdouble f3_2(GLdouble alpha, GLdouble t) const;

    public:
        SecTrigPatch3(GLdouble u_alpha = PI / 2.0, GLdouble v_alpha = 1.0);

        // inherited pure virtual methods have to be redeclared and defined
        GLboolean UBlendingFunctionValues(GLdouble u, RowMatrix<GLdouble> &blending_values) const;
        GLboolean VBlendingFunctionValues(GLdouble v, RowMatrix<GLdouble> &blending_values) const;
        GLboolean CalculatePartialDerivatives(
                        GLuint maximum_order_of_partial_derivatives,
                        GLdouble u, GLdouble v, PartialDerivatives &pd) const;

        GLboolean SetUAlpha(GLdouble alpha);
        GLdouble GetUAlpha();

        GLboolean SetVAlpha(GLdouble alpha);
        GLdouble GetVAlpha();

    };
}
#endif // SECONDORDERTRIGONOMETRICPATCH_H
