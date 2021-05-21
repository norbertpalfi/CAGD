#pragma once

#include "DCoordinates3.h"
#include <GL/glew.h>
#include <iostream>
#include "Matrices.h"
#include "GenericCurves3.h"
#include "TriangulatedMeshes3.h"
#include <vector>

namespace cagd
{
    class TensorProductSurface3
    {
    public:
        // a nested class the stores the zeroth and higher order partial derivatives associated with a
        // surface point
        class PartialDerivatives: public TriangularMatrix<DCoordinate3>
        {
        public:
            PartialDerivatives(GLuint maximum_order_of_partial_derivatives = 1);


            GLvoid LoadNullVectors();
        };


    protected:
        GLboolean            _u_closed, _v_closed; // is the surface closed in direction u or v
        GLuint               _vbo_data;            // vertex buffer object of the control net
        GLdouble             _u_min, _u_max;       // definition domain in direction u
        GLdouble             _v_min, _v_max;       // definition domain in direction v
        Matrix<DCoordinate3> _data;                // the control net (usually stores position vectors)

    public:
        TensorProductSurface3(
                GLdouble u_min, GLdouble u_max,
                GLdouble v_min, GLdouble v_max,
                GLuint row_count = 4, GLuint column_count = 4,
                GLboolean u_closed = GL_FALSE, GLboolean v_closed = GL_FALSE);

        TensorProductSurface3(const TensorProductSurface3& surface);

        TensorProductSurface3& operator =(const TensorProductSurface3& surface);

        GLvoid SetUInterval(GLdouble u_min, GLdouble u_max);
        GLvoid SetVInterval(GLdouble v_min, GLdouble v_max);

        GLvoid GetUInterval(GLdouble& u_min, GLdouble& u_max) const;
        GLvoid GetVInterval(GLdouble& v_min, GLdouble& v_max) const;

        GLboolean SetData(GLuint row, GLuint column, GLdouble x, GLdouble y, GLdouble z);
        GLboolean SetData(GLuint row, GLuint column, const DCoordinate3& point);

        GLboolean GetData(GLuint row, GLuint column, GLdouble& x, GLdouble& y, GLdouble& z) const;
        GLboolean GetData(GLuint row, GLuint column, DCoordinate3& point) const;


        DCoordinate3 operator ()(GLuint row, GLuint column) const;

        DCoordinate3& operator ()(GLuint row, GLuint column);

        // blending function values in u- and v-direction
        virtual GLboolean UBlendingFunctionValues(
                GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const = 0;

        virtual GLboolean VBlendingFunctionValues(
                GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const = 0;

        // calculates the point and higher order (mixed) partial derivatives of the
        // tensor product surface
        //
        // $\mathbf{s}(u, v) = \sum_{i=0}^{n} \sum_{j = 0}^{m} \mathbf{p}_{i,j} F_{n,i}(u) G_{m,j}(v)$,
        //
        // where $n+1$ and $m+1$ denote the row and column counts of the matrix _data, respectively, while
        // $\left(u, v\right) \in \left[u_{\min}, u_{\max}\right] \times \left[v_{\min}, v_{\max}\right]$
        virtual GLboolean CalculatePartialDerivatives(
                GLuint maximum_order_of_partial_derivatives,
                GLdouble u, GLdouble v, PartialDerivatives& pd) const = 0;

        // generates a triangulated mesh that approximates the shape of the surface above
        virtual TriangulatedMesh3* GenerateImage(
                GLuint u_div_point_count, GLuint v_div_point_count,
                GLenum usage_flag = GL_STATIC_DRAW) const;

        // ensures interpolation, i.e., updates the control net $\left[\mathbf{p}_{i,j}\right]_{i=0,j=0}^{n,m}$ stored by
        // the matrix _data such that interpolation conditions $\mathbf{s}(u_k, v_l) = \mathbf{d}_{k,l}$ hold for
        // all $k = 0,1,...,n$ and $l = 0,1,...,m$
        GLboolean UpdateDataForInterpolation(
                const RowMatrix<GLdouble>& u_knot_vector, const ColumnMatrix<GLdouble>& v_knot_vector,
                Matrix<DCoordinate3>& data_points_to_interpolate);


        virtual GLvoid    DeleteVertexBufferObjectsOfData();
        virtual GLboolean RenderData(GLenum render_mode = GL_LINE_STRIP) const;
        virtual GLboolean UpdateVertexBufferObjectsOfData(GLenum usage_flag = GL_STATIC_DRAW);

        RowMatrix<GenericCurve3*>* GenerateUIsoparametricLines(GLuint iso_line_count,
                                                              GLuint maximum_order_of_derivatives,
                                                              GLuint div_point_count,
                                                              GLenum usage_flag = GL_STATIC_DRAW) const;

        RowMatrix<GenericCurve3*>* GenerateVIsoparametricLines(GLuint iso_line_count,
                                                              GLuint maximum_order_of_derivatives,
                                                              GLuint div_point_count,
                                                              GLenum usage_flag = GL_STATIC_DRAW) const;


        virtual ~TensorProductSurface3();
    };
}
