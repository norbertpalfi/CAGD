#pragma once
#include "../Core/LinearCombination3.h"
#include "../Core/Matrices.h"

namespace cagd
{
    class CyclicCurve3: public LinearCombination3
    {
        protected:
            GLuint           _n;
            GLdouble         _c_n;
            GLdouble         _lambda_n;

            TriangularMatrix<GLdouble> _bc; //binominal coefficients

            GLdouble _CalculateNormalizingCoefficient(GLdouble n);

            GLvoid _CalculateBinomialCoefficients(GLuint m, TriangularMatrix<GLdouble> &bc);

        public:
            CyclicCurve3(GLuint n, GLenum data_usage_flag = GL_STATIC_DRAW);

            GLboolean BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble> &values) const;

            GLboolean CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives &d) const;
    };
}
