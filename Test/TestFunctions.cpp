#include <cmath>
#include "TestFunctions.h"
#include "../Core/Constants.h"

using namespace cagd;
using namespace std;

GLdouble spiral_on_cone::u_min = -TWO_PI;
GLdouble spiral_on_cone::u_max = +TWO_PI;

DCoordinate3 spiral_on_cone::d0(GLdouble u)
{
    return DCoordinate3(u * cos(u), u * sin(u), u);
}

DCoordinate3 spiral_on_cone::d1(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(c - u * s, s + u * c, 1.0);
}

DCoordinate3 spiral_on_cone::d2(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(-2.0 * s - u * c, 2.0 * c - u * s, 0);
}

//TORUS_KNOT

GLdouble torus_knot::u_min =0.0;
GLdouble torus_knot::u_max =6 * PI;

DCoordinate3 torus_knot::d0(GLdouble u)
{
    GLdouble c = cos(2.0*u/3.0), s = sin(2.0*u/3.0);
    return DCoordinate3((2.0+c)*cos(u), (2.0+c)*sin(u), s);
}

DCoordinate3 torus_knot::d1(GLdouble u)
{
    GLdouble c2 = cos(2.0*u/3.0), s2 = sin(2.0*u/3.0), c = cos(u), s = sin(u);
    return DCoordinate3((2.0+c2)*(-s) - ((2.0/3.0)*s2)*c, (2.0+c2)*c - (2.0/3.0*s2)*s, (2.0/3.0)*c2);
}

DCoordinate3 torus_knot::d2(GLdouble u)
{
    GLdouble c2 = cos(2.0*u/3.0), s2 = sin(2.0*u/3.0), c = cos(u), s = sin(u);
    return DCoordinate3((2.0/3.0*s2)*s - (2.0+c2)*c - (((4.0/9.0)*c2)*c - ((2.0/3.0)*s2)*s),
                        ((2.0/3.0*s2)*(-c) - (2.0+c2)*s) - ((4.0/9.0*c2)*s + (2.0/3.0*s2)*c),
                        -(4.0/9.0)*s2);
}

//Circle

GLdouble circle::u_max = TWO_PI;
GLdouble circle::u_min = 0.0;

DCoordinate3 circle::d0(GLdouble u)
{
    return DCoordinate3(cos(u), sin(u), u);
}

DCoordinate3 circle::d1(GLdouble u)
{
    return DCoordinate3(-sin(u), cos(u), 1.0);
}

DCoordinate3 circle::d2(GLdouble u)
{
    return DCoordinate3(-cos(u), -sin(u), 0.0);
}


//Lissajous

GLdouble lissajous::u_min = 0.0;
GLdouble lissajous::u_max = TWO_PI;
GLdouble lissajous::a = 2.0;
GLdouble lissajous::b = 2.0;
GLdouble lissajous::kx = 3.0;
GLdouble lissajous::ky = 2.0;

DCoordinate3 lissajous::d0(GLdouble u)
{
    return DCoordinate3(a*cos(u*kx), b*sin(u*ky), u);
}

DCoordinate3 lissajous::d1(GLdouble u)
{
    return DCoordinate3(-a*kx*sin(u*kx), b*ky*cos(u*ky), 1.0);
}

DCoordinate3 lissajous::d2(GLdouble u)
{
    return DCoordinate3(-a*kx*kx*cos(u*kx), -b*ky*ky*sin(u*ky), 0.0);
}

//Hypotrochoid

GLdouble hypotrochoid::u_min = 0.0;
GLdouble hypotrochoid::u_max = 3.0*TWO_PI;
GLdouble hypotrochoid::r = 3.0;
GLdouble hypotrochoid::R = 5.0;
GLdouble hypotrochoid::d = 5.0;


DCoordinate3 hypotrochoid::d0(GLdouble u)
{
    GLdouble Rmr = R-r;
    GLdouble c = cos((Rmr/r)*u);
    GLdouble s = sin((Rmr/r)*u);
    return DCoordinate3(Rmr*cos(u) + d*c, Rmr*sin(u) - d*s, u);
}

DCoordinate3 hypotrochoid::d1(GLdouble u)
{
    GLdouble Rmr = R-r;
    GLdouble c = cos((Rmr/r)*u);
    GLdouble s = sin((Rmr/r)*u);
    return DCoordinate3(Rmr*(-sin(u)) - d*(Rmr/r)*s, Rmr*cos(u) - d*(Rmr/r)*c, 1.0);
}

DCoordinate3 hypotrochoid::d2(GLdouble u)
{
    GLdouble Rmr = R-r;
    GLdouble c = cos((Rmr/r)*u);
    GLdouble s = sin((Rmr/r)*u);
    return DCoordinate3(Rmr*(-cos(u)) - d*((Rmr*Rmr)/(r*r))*c, Rmr*(-sin(u)) + d*((Rmr*Rmr)/(r*r))*s, 0.0);
}

//Rose
GLdouble rose::u_min = 0.0;
GLdouble rose::u_max = TWO_PI;
GLdouble rose::k = 2.0;

DCoordinate3 rose::d0(GLdouble u)
{
    GLdouble ck = cos(k * u);
    return DCoordinate3(ck * cos(u), ck*sin(u), u);
}

DCoordinate3 rose::d1(GLdouble u)
{
    GLdouble ck = cos(k * u);
    GLdouble sk = sin(k * u);
    return DCoordinate3(-k *sk * cos(u) - ck * sin(u), -k*sk*sin(u) + ck * cos(u), 1.0);
}

DCoordinate3 rose::d2(GLdouble u)
{
    GLdouble ck = cos(k * u);
    GLdouble sk = sin(k * u);
    return DCoordinate3(-k * k * ck * cos(u) - k * sk * sin(u)- (-k*sk * sin(u) + ck * cos(u)),
                        -k * k * ck * sin(u) - k * sk * cos(u) - k * sk * cos(u) - ck * sin(u),
                        0.0);
}


//Sphere Curve
// v(u) = 2 * u
GLdouble sphere_curve::u_max = TWO_PI;
GLdouble sphere_curve::u_min = 0.0;

DCoordinate3 sphere_curve::d0(GLdouble u)
{
    return DCoordinate3(sin(u) * cos(2 * u), sin(u) * sin(2 * u), cos(u));
}

DCoordinate3 sphere_curve::d1(GLdouble u)
{
    return DCoordinate3(sin(u) * -2 * sin(2 * u) + cos(u) * cos(2 * u),
                        cos(u) * sin(2 * u) + sin(u) * 2 * cos(2 * u),
                        -sin(u));
}

DCoordinate3 sphere_curve::d2(GLdouble u)
{
    return DCoordinate3(cos(u) * -2 * sin(2 * u) + sin(u) * -4 * cos(2 * u) - sin(u) * cos(2 * u) - cos(u) * 2 * sin(2 * u),
                        -sin(u) * sin(2 * u) + cos(u) * 2 * cos(2 * u) + cos(u) * 2 * cos(2 * u) - sin(u) * 4 * cos(2 * u),
                        -sin(u));
}


//Torus Curve
// v(u) = 4 * u
GLdouble torus_curve::u_max = TWO_PI;
GLdouble torus_curve::u_min = 0.0;
GLdouble torus_curve::r = 1;
GLdouble torus_curve::R = 2;

DCoordinate3 torus_curve::d0(GLdouble u)
{
    return DCoordinate3(r*sin(4*u), (R + r * cos(4*u)) * sin(u), (R + r * cos(4*u)) * cos(u));
}

DCoordinate3 torus_curve::d1(GLdouble u)
{
    return DCoordinate3(r*cos(4*u) * 4,
                        (r * -sin(4*u) * 4) * sin(u) + (R + r * cos(4*u)) * cos(u),
                        (r * -sin(4*u) * 4) * cos(u) - (R + r * cos(4*u)) * sin(u));
}

DCoordinate3 torus_curve::d2(GLdouble u)
{
    return DCoordinate3(r*sin(4*u) * -16,
                        (r * -cos(4*u) * 16) * sin(u) + (r * -sin(4*u) * 4) * cos(u) + (r * -sin(4*u) * 4) * cos(u) - (R + r * cos(4*u)) * sin(u),
                        (r * -cos(4*u) * 16) * cos(u) - (r * -sin(4*u) * 4) * sin(u) - (r * -sin(4*u) * 4) * sin(u) - (R + r * cos(4*u)) * cos(u));
}

//Surface Torus
GLdouble surface_torus::u_min = 0;
GLdouble surface_torus::u_max = TWO_PI;
GLdouble surface_torus::v_min = 0;
GLdouble surface_torus::v_max = TWO_PI;
GLdouble surface_torus::r = 1;
GLdouble surface_torus::R = 2;

DCoordinate3 surface_torus::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3(r*sin(v), (R + r * cos(v)) * sin(u), (R + r * cos(v)) * cos(u));
}

DCoordinate3 surface_torus::d01(GLdouble u, GLdouble v)
{
     return DCoordinate3(0.0, (R + r * cos(v)) * -cos(u), (R + r * cos(v)) * sin(u));
}

DCoordinate3 surface_torus::d10(GLdouble u, GLdouble v)
{
     return DCoordinate3(r*cos(v), (R - r * sin(v)) * sin(u), (R - r * sin(v)) * cos(u));
}

//Surface Sphere

GLdouble surface_sphere::v_min = 0;
GLdouble surface_sphere::v_max = TWO_PI;
GLdouble surface_sphere::u_min = EPS;
GLdouble surface_sphere::u_max = PI-EPS;

DCoordinate3 surface_sphere::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3(sin(u)*cos(v), sin(u) * sin(v), cos(u));
}
DCoordinate3 surface_sphere::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3(cos(u)*cos(v), cos(u) * sin(v), -sin(u));
}
DCoordinate3 surface_sphere::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(-sin(u)*sin(v), sin(u) * cos(v), 0.0);
}


//Surface Revolution

GLdouble surface_revolution::u_min = 0;
GLdouble surface_revolution::u_max = PI;
GLdouble surface_revolution::v_min = 0;
GLdouble surface_revolution::v_max = TWO_PI;

DCoordinate3 surface_revolution::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3(u * cos(v), u * sin(v), 2*sin(u)*cos(u));
}
DCoordinate3 surface_revolution::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3(cos(v), sin(v), 2*cos(u)*cos(u) - 2*sin(u)*sin(u));
}
DCoordinate3 surface_revolution::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(u * sin(v), -u * cos(v), 0);
}


//Surface Cylinder

GLdouble surface_cylinder::u_min = 0;
GLdouble surface_cylinder::u_max = TWO_PI;
GLdouble surface_cylinder::v_min = 0;
GLdouble surface_cylinder::v_max = TWO_PI;

DCoordinate3 surface_cylinder::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3(4 * cos(u), 4*sin(u), v);
}
DCoordinate3 surface_cylinder::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3(-4 * sin(u), 4*cos(u), v);
}
DCoordinate3 surface_cylinder::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(4 * cos(u), 4*sin(u), 1);
}

//Surface Saddle
GLdouble surface_saddle::u_min = -1;
GLdouble surface_saddle::u_max = 1;
GLdouble surface_saddle::v_min = -1;
GLdouble surface_saddle::v_max = 1;
GLdouble surface_saddle::a = 0.1;

DCoordinate3 surface_saddle::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3(u, v, u*u - v*v);
}
DCoordinate3 surface_saddle::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3(1, 0, 2*u);
}
DCoordinate3 surface_saddle::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(0, 1, -2*v);
}


//Surface Helicoid
GLdouble surface_helicoid::u_min = 0;
GLdouble surface_helicoid::u_max = TWO_PI;
GLdouble surface_helicoid::v_min = 0;
GLdouble surface_helicoid::v_max = TWO_PI;

DCoordinate3 surface_helicoid::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3(u*cos(v),u*sin(v),u*u);
}
DCoordinate3 surface_helicoid::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3(cos(v),sin(v),2*u);
}
DCoordinate3 surface_helicoid::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(-u*sin(v),u*cos(v),0);
}
