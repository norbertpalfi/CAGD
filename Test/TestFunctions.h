#pragma once

#include "../Core/DCoordinates3.h"

namespace cagd
{
    namespace spiral_on_cone
    {
        extern GLdouble u_min, u_max;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace torus_knot
    {
        extern GLdouble u_min, u_max;
        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace circle
    {
        extern GLdouble u_min, u_max;
        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace lissajous
    {
        extern GLdouble u_min, u_max, a, b, kx, ky;
        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace hypotrochoid
    {
        extern GLdouble u_min, u_max, r, R, d;
        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace rose
    {
        extern GLdouble u_min, u_max, k;
        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace sphere_curve
    {
        extern GLdouble u_min, u_max;
        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace torus_curve
    {
        extern GLdouble u_min, u_max, r, R;
        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace surface_torus
    {
        extern GLdouble u_min, u_max, v_min, v_max, r, R;

        DCoordinate3 d00(GLdouble u, GLdouble v);
        DCoordinate3 d10(GLdouble u, GLdouble v);
        DCoordinate3 d01(GLdouble u, GLdouble v);

    }

    namespace surface_sphere
    {
        extern GLdouble u_min, u_max, v_min, v_max;

        DCoordinate3 d00(GLdouble u, GLdouble v);
        DCoordinate3 d10(GLdouble u, GLdouble v);
        DCoordinate3 d01(GLdouble u, GLdouble v);

    }

    namespace surface_revolution
    {
        extern GLdouble u_min, u_max, v_min, v_max;

        DCoordinate3 d00(GLdouble u, GLdouble v);
        DCoordinate3 d10(GLdouble u, GLdouble v);
        DCoordinate3 d01(GLdouble u, GLdouble v);

    }

    namespace surface_cylinder
    {
        extern GLdouble u_min, u_max, v_min, v_max;

        DCoordinate3 d00(GLdouble u, GLdouble v);
        DCoordinate3 d10(GLdouble u, GLdouble v);
        DCoordinate3 d01(GLdouble u, GLdouble v);

    }

    namespace surface_saddle
    {
        extern GLdouble u_min, u_max, v_min, v_max, a;

        DCoordinate3 d00(GLdouble u, GLdouble v);
        DCoordinate3 d10(GLdouble u, GLdouble v);
        DCoordinate3 d01(GLdouble u, GLdouble v);
    }

    namespace surface_helicoid
    {
        extern GLdouble u_min, u_max, v_min, v_max, a;

        DCoordinate3 d00(GLdouble u, GLdouble v);
        DCoordinate3 d10(GLdouble u, GLdouble v);
        DCoordinate3 d01(GLdouble u, GLdouble v);
    }
}
