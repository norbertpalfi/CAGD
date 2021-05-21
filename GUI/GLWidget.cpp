#include "GLWidget.h"

#if !defined(__APPLE__)
#include <GL/glu.h>
#endif

#include <iostream>
#include "../Core/Matrices.h"
#include "../Test/TestFunctions.h"
#include "../Core/Materials.h"
#include "../Core/Constants.h"

using namespace std;

#include <Core/Exceptions.h>

namespace cagd
{
    //--------------------------------
    // special and default constructor
    //--------------------------------
    GLWidget::GLWidget(QWidget *parent): QOpenGLWidget(parent)
    {
        _current_tab = 0;
        _derivatives_scale = 1.0;
        _selected_parametric_curve = 0;
        _selected_parametric_surface = 0;
        _selected_cyclic_curve = 0;
        _selected_model = 0;
        _selected_material = 0;
        _parametric_div_point_count = 200;
        _apply_texture = false;
        _selected_texture = 0;
        _cyclic_div_point_count = 20;

        _parametric_first_derivatives = true;
        _parametric_second_derivatives = true;

        _cyclic_first_derivatives = true;
        _cyclic_second_derivatives = true;

        _cyclic_point = 0;
        _trig_control_point = true;
        _trig_point = 0;
        _show_uip = false;
        _show_vip = false;

        _materials[0] = MatFBBrass;
        _materials[1] = MatFBGold;
        _materials[2] = MatFBSilver;
        _materials[3] = MatFBEmerald;
        _materials[4] = MatFBPearl;
        _materials[5] = MatFBRuby;
        _materials[6] = MatFBTurquoise;

        _models = RowMatrix<TriangulatedMesh3>(20);

        _timer = new QTimer(this);
        _timer->setInterval(0);

        _patch_row = 0;
        _patch_col = 0;
        _show_control_net = true;

        _scale_parameter = 10.0f;
        _smoothing_parameter = 1.0f;
        _shading_parameter = 100.0f;
        _selected_shader = 1;

        connect(_timer, SIGNAL(timeout()), this, SLOT(_animate()));
    }

    GLWidget:: ~GLWidget() {
        /*  Freeing up the Parametric Curves  */
        for (int i = 0; i < _parametric_curves_count; i++) {
            if (_parametric_curves[i]) {
                delete _parametric_curves[i];
                _parametric_curves[i] = nullptr;
            }

            if (_images_of_parametric_curves[i]) {
                delete _images_of_parametric_curves[i];
                _images_of_parametric_curves[i] = nullptr;
            }
        }

        if (_dl) {
            delete _dl; _dl = nullptr;
        }
    }

    void GLWidget::deleteCyclicCurves() {
        if (_cyclic_curve[0]) {
            delete _cyclic_curve[0];
            delete _image_of_cyclic_curve[0];
        }
        if (_cyclic_curve[1]) {
            delete _cyclic_curve[1];
            delete _image_of_cyclic_curve[1];
        }
    }

    void GLWidget::initializeNthParametricCurve(int n) {
        if (n >= _parametric_curves_count) {
            return;
        }
        GLenum use_flag = GL_STATIC_DRAW;
        if (!_parametric_curves[n]) {
             throw Exception("Could_not_generate_the_parametric_curve");
        }

        _images_of_parametric_curves[n] = _parametric_curves[n]->GenerateImage(_parametric_div_point_count, use_flag);

        if (!_images_of_parametric_curves[n]) {
            throw Exception("Could_not_generate_the_image_of_the_parametric_curve");
        }

        if (!_images_of_parametric_curves[n]->UpdateVertexBufferObjects(_derivatives_scale, use_flag)) {
            throw Exception("Could_not_create_the_vbos_of_the_parametric_curve");
        }
    }

    void GLWidget::initializeParametricCurves() {
        /*  Spiral on Cone  */
        RowMatrix<ParametricCurve3::Derivative> spiral_on_cone_derivatives(3);
        spiral_on_cone_derivatives[0] = spiral_on_cone::d0;
        spiral_on_cone_derivatives[1] = spiral_on_cone::d1;
        spiral_on_cone_derivatives[2] = spiral_on_cone::d2;

        _parametric_curves[0] = nullptr;
        _images_of_parametric_curves[0] = nullptr;

        _parametric_curves[0] = new (nothrow) ParametricCurve3(spiral_on_cone_derivatives, spiral_on_cone::u_min, spiral_on_cone::u_max);

         initializeNthParametricCurve(0);

        /*  Torus Knot  */
        RowMatrix<ParametricCurve3::Derivative> torus_knot_derivatives(3);
        torus_knot_derivatives[0] = torus_knot::d0;
        torus_knot_derivatives[1] = torus_knot::d1;
        torus_knot_derivatives[2] = torus_knot::d2;

        _parametric_curves[1] = nullptr;
        _images_of_parametric_curves[1] = nullptr;

        _parametric_curves[1] = new (nothrow) ParametricCurve3(torus_knot_derivatives, torus_knot::u_min, torus_knot::u_max);

        initializeNthParametricCurve(1);

        /* Circle */
        RowMatrix<ParametricCurve3::Derivative> circle_derivatives(3);
        circle_derivatives[0] = circle::d0;
        circle_derivatives[1] = circle::d1;
        circle_derivatives[2] = circle::d2;

        _parametric_curves[2] = nullptr;
        _images_of_parametric_curves[2] = nullptr;

        _parametric_curves[2] = new (nothrow) ParametricCurve3(circle_derivatives, circle::u_min, circle::u_max);

        initializeNthParametricCurve(2);

        /*  Lissajous  */
        RowMatrix<ParametricCurve3::Derivative> lissajous_derivatives(3);
        lissajous_derivatives[0] = lissajous::d0;
        lissajous_derivatives[1] = lissajous::d1;
        lissajous_derivatives[2] = lissajous::d2;

        _parametric_curves[3] = nullptr;
        _images_of_parametric_curves[3] = nullptr;

        _parametric_curves[3] = new (nothrow) ParametricCurve3(lissajous_derivatives, lissajous::u_min, lissajous::u_max);

        initializeNthParametricCurve(3);

        /*  Hypotrochoid  */
        RowMatrix<ParametricCurve3::Derivative> hypotrochoid_derivatives(3);
        hypotrochoid_derivatives[0] = hypotrochoid::d0;
        hypotrochoid_derivatives[1] = hypotrochoid::d1;
        hypotrochoid_derivatives[2] = hypotrochoid::d2;

        _parametric_curves[4] = nullptr;
        _images_of_parametric_curves[4] = nullptr;

        _parametric_curves[4] = new (nothrow) ParametricCurve3(hypotrochoid_derivatives, hypotrochoid::u_min, hypotrochoid::u_max);

        initializeNthParametricCurve(4);

        /*  Rose  */
        RowMatrix<ParametricCurve3::Derivative> rose_derivatives(3);
        rose_derivatives[0] = rose::d0;
        rose_derivatives[1] = rose::d1;
        rose_derivatives[2] = rose::d2;

        _parametric_curves[5] = nullptr;
        _images_of_parametric_curves[5] = nullptr;

        _parametric_curves[5] = new (nothrow) ParametricCurve3(rose_derivatives, rose::u_min, rose::u_max);

        initializeNthParametricCurve(5);

        /*  Sphere Curve  */
        RowMatrix<ParametricCurve3::Derivative> sphere_curve_derivatives(3);
        sphere_curve_derivatives[0] = sphere_curve::d0;
        sphere_curve_derivatives[1] = sphere_curve::d1;
        sphere_curve_derivatives[2] = sphere_curve::d2;

        _parametric_curves[6] = nullptr;
        _images_of_parametric_curves[6] = nullptr;

        _parametric_curves[6] = new (nothrow) ParametricCurve3(sphere_curve_derivatives, sphere_curve::u_min, sphere_curve::u_max);

        initializeNthParametricCurve(6);

        /*  Torus Curve  */
        RowMatrix<ParametricCurve3::Derivative> torus_curve_derivatives(3);
        torus_curve_derivatives[0] = torus_curve::d0;
        torus_curve_derivatives[1] = torus_curve::d1;
        torus_curve_derivatives[2] = torus_curve::d2;

        _parametric_curves[7] = nullptr;
        _images_of_parametric_curves[7] = nullptr;

        _parametric_curves[7] = new (nothrow) ParametricCurve3(torus_curve_derivatives, torus_curve::u_min, torus_curve::u_max);

        initializeNthParametricCurve(7);

    }

    void GLWidget::initializeModels() {

        if(_angel.LoadFromOFF("Models/angel.off", GL_TRUE))
        {
            if(!_angel.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_bird.LoadFromOFF("Models/bird.off", GL_TRUE))
        {
            if(!_bird.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_bug.LoadFromOFF("Models/bug.off", GL_TRUE))
        {
            if(!_bug.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_cone.LoadFromOFF("Models/cone.off", GL_TRUE))
        {
            if(!_cone.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_cube.LoadFromOFF("Models/cube.off", GL_TRUE))
        {
            if(!_cube.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_dragon.LoadFromOFF("Models/dragon.off", GL_TRUE))
        {
            if(!_dragon.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_elephant.LoadFromOFF("Models/elephant.off", GL_TRUE))
        {
            if(!_elephant.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_gangster.LoadFromOFF("Models/gangster.off", GL_TRUE))
        {
            if(!_gangster.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_goblet.LoadFromOFF("Models/goblet.off", GL_TRUE))
        {
            if(!_goblet.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_horse.LoadFromOFF("Models/horse.off", GL_TRUE))
        {
            if(!_horse.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_icosahedron.LoadFromOFF("Models/icosahedron.off", GL_TRUE))
        {
            if(!_icosahedron.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_Lucy.LoadFromOFF("Models/Lucy.off", GL_TRUE))
        {
            if(!_Lucy.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_mouse.LoadFromOFF("Models/mouse.off", GL_TRUE))
        {
            if(!_mouse.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_seashell.LoadFromOFF("Models/seashell.off", GL_TRUE))
        {
            if(!_seashell.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_space_station.LoadFromOFF("Models/space_station.off", GL_TRUE))
        {
            if(!_space_station.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_sphere.LoadFromOFF("Models/sphere.off", GL_TRUE))
        {
            if(!_sphere.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
            }
        }
        if(_Spot.LoadFromOFF("Models/Spot.off", GL_TRUE))
        {
            if(!_Spot.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
                // @TODO Catch these errors
            }
        }
        if(_star.LoadFromOFF("Models/star.off", GL_TRUE))
        {
            if(!_star.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW))
            {
                // @TODO Catch these errors
            }
        }

        _angle = 0.0;
        _timer->start();
    }

    void GLWidget::initializeTorusSurface() {
        GLuint u_div_point_count = 200;
        GLuint v_div_point_count = 200;
        GLenum usage_flag = GL_STATIC_DRAW;

        TriangularMatrix<ParametricSurface3::PartialDerivative> p_derivative(3);
        p_derivative(0, 0) = surface_torus::d00;
        p_derivative(1, 0) = surface_torus::d10;
        p_derivative(1, 1) = surface_torus::d01;
        _parametric_surfaces[0] = nullptr;
        _image_of_ps[0] = nullptr;

        _parametric_surfaces[0] = new (nothrow) ParametricSurface3(p_derivative, surface_torus::u_min, surface_torus::u_max, surface_torus::v_min,surface_torus::v_max);

        if(!_parametric_surfaces[0])
        {

        }

        _image_of_ps[0] = _parametric_surfaces[0]->GenerateImage(u_div_point_count, v_div_point_count, usage_flag);

//        _image_of_ps[0]->FLipNormals();

        if(!_image_of_ps[0]->UpdateVertexBufferObjects(usage_flag))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_parametric_surface!" << endl;
        }
    }

    void GLWidget::initializeSphereSurface() {
        GLuint u_div_point_count = 200;
        GLuint v_div_point_count = 200;
        GLenum usage_flag = GL_STATIC_DRAW;

        TriangularMatrix<ParametricSurface3::PartialDerivative> p_derivative(3);
        p_derivative(0, 0) = surface_sphere::d00;
        p_derivative(1, 0) = surface_sphere::d10;
        p_derivative(1, 1) = surface_sphere::d01;
        _parametric_surfaces[1] = nullptr;
        _image_of_ps[1] = nullptr;

        _parametric_surfaces[1] = new (nothrow) ParametricSurface3(p_derivative, surface_sphere::u_min, surface_sphere::u_max, surface_sphere::v_min,surface_sphere::v_max);

        if(!_parametric_surfaces[1])
        {

        }

        _image_of_ps[1] = _parametric_surfaces[1]->GenerateImage(u_div_point_count, v_div_point_count, usage_flag);
  //      _image_of_ps[1]->FLipNormals();


        if(!_image_of_ps[1]->UpdateVertexBufferObjects(usage_flag))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_parametric_surface!" << endl;
        }
    }

    void GLWidget::initializeRevolutionSurface() {
        GLuint u_div_point_count = 200;
        GLuint v_div_point_count = 200;
        GLenum usage_flag = GL_STATIC_DRAW;

        TriangularMatrix<ParametricSurface3::PartialDerivative> p_derivative(3);
        p_derivative(0, 0) = surface_revolution::d00;
        p_derivative(1, 0) = surface_revolution::d10;
        p_derivative(1, 1) = surface_revolution::d01;
        _parametric_surfaces[2] = nullptr;
        _image_of_ps[2] = nullptr;

        _parametric_surfaces[2] = new (nothrow) ParametricSurface3(p_derivative, surface_revolution::u_min, surface_revolution::u_max, surface_revolution::v_min,surface_revolution::v_max);

        if(!_parametric_surfaces[2])
        {

        }

        _image_of_ps[2] = _parametric_surfaces[2]->GenerateImage(u_div_point_count, v_div_point_count, usage_flag);
//        _image_of_ps[2]->FLipNormals();


        if(!_image_of_ps[2]->UpdateVertexBufferObjects(usage_flag))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_parametric_surface!" << endl;
        }
    }

    void GLWidget::initializeCylinderSurface() {
        GLuint u_div_point_count = 200;
        GLuint v_div_point_count = 200;
        GLenum usage_flag = GL_STATIC_DRAW;

        TriangularMatrix<ParametricSurface3::PartialDerivative> p_derivative(3);
        p_derivative(0, 0) = surface_cylinder::d00;
        p_derivative(1, 0) = surface_cylinder::d10;
        p_derivative(1, 1) = surface_cylinder::d01;
        _parametric_surfaces[3] = nullptr;
        _image_of_ps[3] = nullptr;

        _parametric_surfaces[3] = new (nothrow) ParametricSurface3(p_derivative, surface_cylinder::u_min, surface_cylinder::u_max, surface_cylinder::v_min,surface_cylinder::v_max);

        if(!_parametric_surfaces[3])
        {

        }

        _image_of_ps[3] = _parametric_surfaces[3]->GenerateImage(u_div_point_count, v_div_point_count, usage_flag);
//        _image_of_ps[3]->FLipNormals();


        if(!_image_of_ps[3]->UpdateVertexBufferObjects(usage_flag))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_parametric_surface!" << endl;
        }
    }

    void GLWidget::initializeGabrielSurface() {
        GLuint u_div_point_count = 200;
        GLuint v_div_point_count = 200;
        GLenum usage_flag = GL_STATIC_DRAW;

        TriangularMatrix<ParametricSurface3::PartialDerivative> p_derivative(3);
        p_derivative(0, 0) = surface_saddle::d00;
        p_derivative(1, 0) = surface_saddle::d10;
        p_derivative(1, 1) = surface_saddle::d01;
        _parametric_surfaces[4] = nullptr;
        _image_of_ps[4] = nullptr;

        _parametric_surfaces[4] = new (nothrow) ParametricSurface3(p_derivative, surface_saddle::u_min, surface_saddle::u_max, surface_saddle::v_min,surface_saddle::v_max);

        if(!_parametric_surfaces[4])
        {

        }

        _image_of_ps[4] = _parametric_surfaces[4]->GenerateImage(u_div_point_count, v_div_point_count, usage_flag);
//        _image_of_ps[4]->FLipNormals();


        if(!_image_of_ps[4]->UpdateVertexBufferObjects(usage_flag))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_parametric_surface!" << endl;
        }
    }

    void GLWidget::initializeHelicoidSurface() {
        GLuint u_div_point_count = 200;
        GLuint v_div_point_count = 200;
        GLenum usage_flag = GL_STATIC_DRAW;

        TriangularMatrix<ParametricSurface3::PartialDerivative> p_derivative(3);
        p_derivative(0, 0) = surface_helicoid::d00;
        p_derivative(1, 0) = surface_helicoid::d10;
        p_derivative(1, 1) = surface_helicoid::d01;
        _parametric_surfaces[5] = nullptr;
        _image_of_ps[5] = nullptr;

        _parametric_surfaces[5] = new (nothrow) ParametricSurface3(p_derivative, surface_helicoid::u_min, surface_helicoid::u_max, surface_helicoid::v_min,surface_helicoid::v_max);

        if(!_parametric_surfaces[5])
        {

        }

        _image_of_ps[5] = _parametric_surfaces[5]->GenerateImage(u_div_point_count, v_div_point_count, usage_flag);
       // _image_of_ps[5]->FLipNormals();



        if(!_image_of_ps[5]->UpdateVertexBufferObjects(usage_flag))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_parametric_surface!" << endl;
        }
    }

    void GLWidget::initializeSurfaces() {
        initializeTorusSurface();
        initializeSphereSurface();
        initializeRevolutionSurface();
        initializeCylinderSurface();
        initializeGabrielSurface();
        initializeHelicoidSurface();
    }

    void GLWidget::initializeCyclicCurves() {
        GLenum usage_flag = GL_STATIC_DRAW;
        GLdouble step = TWO_PI / (5);

        _cyclic_curve[0] = nullptr;
        _image_of_cyclic_curve[0] = nullptr;

        _cyclic_curve[0] = new (nothrow) CyclicCurve3(2, usage_flag);

        if(!_cyclic_curve[0])
        {
            cout << "Couldn't generate CyclicCurve" << endl;
            return;
        }

        CyclicCurve3& curve= *_cyclic_curve[0];

        for(GLuint i = 0; i < 5; i++)
        {
            DCoordinate3 &cp = curve[i];

            cp[0] = cos(i*step);
            cp[1] = sin(i*step);
            cp[2] = 0;
        }

        if(!_cyclic_curve[0]->UpdateVertexBufferObjectsOfData(GL_STATIC_DRAW))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
        }

        _image_of_cyclic_curve[0] = _cyclic_curve[0]->GenerateImage(2, _cyclic_div_point_count, usage_flag);

        if(!_image_of_cyclic_curve[0])
        {
            cout << "Couldn't generate CyclicCurve" << endl;
            return;
        }

        if(!_image_of_cyclic_curve[0]->UpdateVertexBufferObjects(_derivatives_scale, usage_flag))
        {
             cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
        }

        GLint nknot = 5;
        knot_vector.ResizeRows(nknot);
        points.ResizeRows(nknot);
        points[0] = DCoordinate3(0, 0, 0);
        points[1] = DCoordinate3(0, 1, 0);
        points[2] = DCoordinate3(1, 0, 0);
        points[3] = DCoordinate3(-1, 0, 0);
        points[4] = DCoordinate3(0, -1, 0);

        step = TWO_PI / nknot;
        for(int i = 0; i < nknot; i++)
        {
            knot_vector[i] = i*step;
        }
        _cyclic_curve[1] = nullptr;
        _image_of_cyclic_curve[1] = nullptr;

        _cyclic_curve[1] = new (nothrow) CyclicCurve3(2, usage_flag);

        if(!_cyclic_curve[1]->UpdateDataForInterpolation(knot_vector, points))
        {
            cout << "The update was unsuccessful" << std::endl;
            return;
        }

        if(!_cyclic_curve[1]->UpdateVertexBufferObjectsOfData(usage_flag))
        {
            cout << "The update was unsuccessful" << std::endl;
            return;
        }
        _image_of_cyclic_curve[1] = _cyclic_curve[1]->GenerateImage(2, 200, usage_flag);

        if(!_image_of_cyclic_curve[1])
        {
            cout << "Couldn't generate CyclicCurve" << endl;
            return;
        }

        if(!_image_of_cyclic_curve[1]->UpdateVertexBufferObjects(_derivatives_scale, usage_flag))
        {
             cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
        }
    }

    void GLWidget::initializeSecondTrigPatch() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                _patch.SetData(i, j, i - 1.5, j -1.5, 0.0);
            }
        }

        _patch.UpdateVertexBufferObjectsOfData();
        _image_of_patch = _patch.GenerateImage(30, 30, GL_STATIC_DRAW);

        if (_image_of_patch)    _image_of_patch->UpdateVertexBufferObjects();

        DCoordinate3 p;
        _patch.GetData(_patch_row, _patch_col, p);

        emit update_patch_point_x(p.x());
        emit update_patch_point_y(p.y());
        emit update_patch_point_z(p.z());

        emit update_alpha_u(_patch.GetUAlpha());
        emit update_alpha_v(_patch.GetVAlpha());

         _patch_uip = _patch.GenerateUIsoparametricLines(30, 2, 100);
         _patch_vip = _patch.GenerateVIsoparametricLines(30, 2, 100);

    }

    void GLWidget::initializeShaders() {
            try {
                _shaders.ResizeColumns(4);
                if (!_shaders[0].InstallShaders("Shaders/directional_light.vert","Shaders/directional_light.frag"))
                {
                    throw Exception("Directional light creation error");
                }

                if (!_shaders[1].InstallShaders("Shaders/two_sided_lighting.vert","Shaders/two_sided_lighting.frag"))
                {
                    throw Exception("Two sided lighting creation error");
                }

                if (!_shaders[2].InstallShaders("Shaders/toon.vert","Shaders/toon.frag"))
                {
                    throw Exception("Toon creation error.");
                } else {
                    _shaders[2].Enable();
                    _shaders[2].SetUniformVariable4f("default_outline_color", 60.0f, 160.0f, 60.0f, 1.0f);
                    _shaders[2].Disable();
                }

                if (!_shaders[3].InstallShaders("Shaders/reflection_lines.vert","Shaders/reflection_lines.frag"))
                {
                    throw Exception("Reflection lines creation error");
                } else {
                    _shaders[3].Enable();
                    _shaders[3].SetUniformVariable1f("scale_factor", _scale_parameter);
                    _shaders[3].SetUniformVariable1f("smoothing", _smoothing_parameter);
                    _shaders[3].SetUniformVariable1f("shading", _shading_parameter);
                    _shaders[3].Disable();
                }

            } catch (Exception& e) {
                std::cout << e << endl;
            }
        }
    //--------------------------------------------------------------------------------------
    // this virtual function is called once before the first call to paintGL() or resizeGL()
    //--------------------------------------------------------------------------------------
    void GLWidget::initializeGL()
    {
        // creating a perspective projection matrix
        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        _aspect = static_cast<double>(width()) / static_cast<double>(height());
        _z_near = 1.0;
        _z_far  = 1000.0;
        _fovy   = 45.0;

        gluPerspective(_fovy, _aspect, _z_near, _z_far);

        // setting the model view matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        _eye[0] = _eye[1] = 0.0; _eye[2] = 6.0;
        _center[0] = _center[1] = _center[2] = 0.0;
        _up[0] = _up[2] = 0.0; _up[1] = 1.0;

        gluLookAt(_eye[0], _eye[1], _eye[2], _center[0], _center[1], _center[2], _up[0], _up[1], _up[2]);

        // enabling the depth test
        glEnable(GL_DEPTH_TEST);

        // setting the background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // initial values of transformation parameters
        _angle_x = _angle_y = _angle_z = 0.0;
        _trans_x = _trans_y = _trans_z = 0.0;
        _zoom = 1.0;

        // ...

        try
        {
            // initializing the OpenGL Extension Wrangler library
            GLenum error = glewInit();

            if (error != GLEW_OK)
            {
                throw Exception("Could not initialize the OpenGL Extension Wrangler Library!");
            }

            if (!glewIsSupported("GL_VERSION_2_0"))
            {
                throw Exception("Your graphics card is not compatible with OpenGL 2.0+! "
                                "Try to update your driver or buy a new graphics adapter!");
            }

            // create and store your geometry in display lists or vertex buffer objects
            // ...

            texture[0] = new QOpenGLTexture(QImage("Textures/periodic_texture_00.jpg").mirrored());
            texture[0]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            texture[0]->setMagnificationFilter(QOpenGLTexture::Linear);

            texture[1] = new QOpenGLTexture(QImage("Textures/periodic_texture_01.jpg").mirrored());
            texture[1]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            texture[1]->setMagnificationFilter(QOpenGLTexture::Linear);

            texture[2] = new QOpenGLTexture(QImage("Textures/periodic_texture_02.jpg").mirrored());
            texture[2]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            texture[2]->setMagnificationFilter(QOpenGLTexture::Linear);

            texture[3] = new QOpenGLTexture(QImage("Textures/periodic_texture_03.jpg").mirrored());
            texture[3]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            texture[3]->setMagnificationFilter(QOpenGLTexture::Linear);

            texture[4] = new QOpenGLTexture(QImage("Textures/periodic_texture_04.jpg").mirrored());
            texture[4]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            texture[4]->setMagnificationFilter(QOpenGLTexture::Linear);

            texture[5] = new QOpenGLTexture(QImage("Textures/periodic_texture_05.jpg").mirrored());
            texture[5]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            texture[5]->setMagnificationFilter(QOpenGLTexture::Linear);

            texture[6] = new QOpenGLTexture(QImage("Textures/periodic_texture_06.jpg").mirrored());
            texture[6]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            texture[6]->setMagnificationFilter(QOpenGLTexture::Linear);

            texture[7] = new QOpenGLTexture(QImage("Textures/periodic_texture_07.jpg").mirrored());
            texture[7]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            texture[7]->setMagnificationFilter(QOpenGLTexture::Linear);

            texture[8] = new QOpenGLTexture(QImage("Textures/periodic_texture_08.jpg").mirrored());
            texture[8]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            texture[8]->setMagnificationFilter(QOpenGLTexture::Linear);

            texture[9] = new QOpenGLTexture(QImage("Textures/periodic_texture_09.jpg").mirrored());
            texture[9]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            texture[9]->setMagnificationFilter(QOpenGLTexture::Linear);

            initializeParametricCurves();
            initializeModels();
            initializeSurfaces();
            initializeCyclicCurves();
            initializeSecondTrigPatch();
            initializeShaders();

            addTrigCurve();
            emit update_alpha(PI / 2.0);

            HCoordinate3    direction(0.0, 0.0, 1.0, 0.0);
            Color4          ambient(0.4, 0.4, 0.4, 1.0);
            Color4          diffuse(0.8, 0.8, 0.8, 1.0);
            Color4          specular(1.0, 1.0, 1.0, 1.0);

            _dl = new (nothrow) DirectionalLight(GL_LIGHT0, direction, ambient, diffuse, specular);
            if (!_dl) {
                throw("Exception: Could Not Create The Directional Light!");
            }

            glEnable(GL_POINT_SMOOTH);
            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

            glEnable(GL_LINE_SMOOTH);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

            glEnable(GL_POLYGON_SMOOTH);
            glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

            glEnable(GL_DEPTH_TEST);
        }
        catch (Exception &e)
        {
            cout << e << endl;
        }
    }

    void GLWidget::paintDefaultTriangle() {
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(1.1f, 0.0f, 0.0f);

            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 1.1f, 0.0f);

            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 1.1f);
        glEnd();

        glBegin(GL_TRIANGLES);
            // attributes
            glColor3f(1.0f, 0.0f, 0.0f);
            // associated with position
            glVertex3f(1.0f, 0.0f, 0.0f);

            // attributes
            glColor3f(0.0, 1.0, 0.0);
            // associated with position
            glVertex3f(0.0, 1.0, 0.0);

            // attributes
            glColor3f(0.0f, 0.0f, 1.0f);
            // associated with position
            glVertex3f(0.0f, 0.0f, 1.0f);
        glEnd();
    }

    void GLWidget::paintSelectedParametricCurve() {
        glColor3f(1.0f, 0.0f, 0.0f);
        _images_of_parametric_curves[_selected_parametric_curve]->RenderDerivatives(0, GL_LINE_STRIP);



        glPointSize(5.0f);
            if (_parametric_first_derivatives) {
                glColor3f(0.0f, 0.5f, 0.0f);
                _images_of_parametric_curves[_selected_parametric_curve]->RenderDerivatives(1, GL_LINES);
                _images_of_parametric_curves[_selected_parametric_curve]->RenderDerivatives(1, GL_POINTS);
            }

            if (_parametric_second_derivatives) {
                glColor3f(1.0f, 0.5f, 0.9f);
                _images_of_parametric_curves[_selected_parametric_curve]->RenderDerivatives(2, GL_LINES);
                _images_of_parametric_curves[_selected_parametric_curve]->RenderDerivatives(2, GL_POINTS);
            }
        glPointSize(1.0f);
    }

    void GLWidget::paintCastle() {
        MatFBSilver.Apply();
        glTranslated(-3,0,0);
        for(int i=0;i<7;i++)
        {
            if(i!=3)
                _cube.Render();
            glTranslated(1,0,0);
        }
        glTranslated(-1,1,0);
        for(int i=0;i<7;i++)
        {
            if(i!=3)
                _cube.Render();
            glTranslated(-1,0,0);
        }
        glTranslated(1,1,0);

        for(int i=0;i<7;i++)
        {
            _cube.Render();
            glTranslated(1,0,0);
        }
        glTranslated(-1,0,0);
        for(int i=0;i<7;i++)
        {
            _cube.Render();
            glTranslated(0,0,-1);
        }
        glTranslated(0,-1,1);
        for(int i=0;i<7;i++)
        {
            _cube.Render();
            glTranslated(0,0,1);
        }
        glTranslated(0,-1,-1);
        for(int i=0;i<7;i++)
        {
            _cube.Render();
            glTranslated(0,0,-1);
        }
        glTranslated(0,0,+1);
        for(int i=0;i<7;i++)
        {
            _cube.Render();
            glTranslated(-1,0,0);
        }
        glTranslated(1,1,0);
        for(int i=0;i<7;i++)
        {
            _cube.Render();
            glTranslated(1,0,0);
        }
        glTranslated(-1,1,0);
        for(int i=0;i<7;i++)
        {
            _cube.Render();
            glTranslated(-1,0,0);
        }
        glTranslated(1,0,0);
        for(int i=0;i<7;i++)
        {
            _cube.Render();
            glTranslated(0,0,1);
        }
        glTranslated(0,-1,-1);
        for(int i=0;i<7;i++)
        {
            _cube.Render();
            glTranslated(0,0,-1);
        }
        glTranslated(0,-1,1);
        for(int i=0;i<7;i++)
        {
            _cube.Render();
            glTranslated(0,0,1);
        }
        glTranslated(-1,-1,0);
        MatFBEmerald.Apply();
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++)
            {
                _cube.Render();
                glTranslated(0,0,-1);
            }
            glTranslated(1,0,9);
        }
        glTranslated(-5,-2.5,-2);
        //scale is 1
        MatFBPearl.Apply();
        glScaled(8,5,20);
        _sphere.Render();
        glTranslated(-0.3,+0.1,-0.1);
        _sphere.Render();
        glTranslated(-0.6,0.5,-0.1);
        _sphere.Render();
        glTranslated(1.7,-0.2,0);
        _sphere.Render();
        glTranslated(-1.7+0.6+0.3,
                     +0.2-0.5-0.1+2.5-1.2,
                     2+0.1+0.1-2.5+0.55);
        glScaled(0.125,0.2,0.05);
        glTranslated(3.2,-4.3,1.5);
        glScaled(2,2,2);

        _sphere.Render();
        glScaled(0.5,0.5,0.5);
        glTranslated(-3.2,+4.3,-1.5);
        glTranslated(5,-2.5,-1);
        glScaled(4,4,4);
        glRotated(90,-1,0,0);
        MatFBRuby.Apply();
        _dragon.Render();

        //glDepthMask(GL_FALSE);
        //glBlendFunc(GL_SRC_ALPHA,GL_ONE);

        glRotated(90,1,0,0);
        glScaled(0.25,0.25,0.25);
        glTranslated(-5,+2.5,+1);

        //middle off front wall +1
        MatFBGold.Apply();
        //_bug.Render();
        glTranslated(-1,0,0);
        _bird.Render();
        glTranslated(2,0,0);
        //_spacestation.Render();
        glTranslated(1,0,0);
        //_angel.Render();
        glTranslated(-4,0,0);
        //_icosahedron.Render();
        glTranslated(2,0,0);


        glTranslated(1,-3.5,1);
        MatFBBrass.Apply();
        glRotated(90,1.0,0.0,0.0);
        _cone.Render();
        glRotated(90,-1.0,0.0,0.0);
        glTranslated(0,1,0);
        glRotated(180,0.0,1.0,0.0);
        _Spot.Render();
        glRotated(180,0.0,1.0,0.0);
        glTranslated(-2,0,0);
        glRotated(180,0.0,1.0,0.0);
        _Spot.Render();
        glRotated(180,0.0,1.0,0.0);
        glTranslated(0,-1,0);
        glRotated(90,1.0,0.0,0.0);
        _cone.Render();
        glRotated(90,-1.0,0.0,0.0);
        glTranslated(1,+3.5,-1);


        glTranslated(0,-1,-3);
        glScaled(5,5,5);



        glRotated(90,-1,-0.5,-0.5);
        MatFBTurquoise.Apply();
        _dragon.Render();

        //glDepthMask(GL_FALSE);
        //glBlendFunc(GL_SRC_ALPHA,GL_ONE);

        glRotated(90,1,0.5,0.5);



        glScaled(0.2,0.2,0.2);
        glTranslated(0,1,3);

        glTranslated(+3,-1,0);

        glTranslated(-3,-2,-3);
        MatFBSilver.Apply();
        _elephant.Render();
        glTranslated(3,2,3);

        //FR
        {
            glRotated(270,1.0,0.0,0.0);
            glScaled(1.0,1.0,4.0);
            MatFBSilver.Apply();
            _goblet.Render();
            glScaled(1,1,0.25);
            glRotated(270,-1.0,0.0,0.0);

            glTranslated(0,+2.2,0);
            glRotated(90,0.0,0.0,-1.0);
            glScaled(1.5,1.5,1.5);
            MatFBRuby.Apply();
            _seashell.Render();
            glScaled(0.75,0.75,0.75);
            glRotated(90,0.0,0.0,1.0);
            glTranslated(0,-2.2,0);
            glRotated(270,1.0,0.0,0.0);

        }


        glTranslated(0,5.3,0);
        glScaled(1.0,1.0,4.0);
        MatFBSilver.Apply();
        _goblet.Render();
        glScaled(1,1,0.25);
        glRotated(270,-1.0,0.0,0.0);

        glTranslated(0,+2.2,0);
        glRotated(90,0.0,0.0,-1.0);
        glScaled(1.5,1.5,1.5);
        MatFBRuby.Apply();
        _seashell.Render();
        glScaled(0.75,0.75,0.75);
        glRotated(90,0.0,0.0,1.0);
        glTranslated(0,-2.2,0);
        glRotated(270,1.0,0.0,0.0);


        glTranslated(-4.7,0,0);


        glScaled(1.0,1.0,4.0);
        MatFBSilver.Apply();
        _goblet.Render();
        glScaled(1,1,0.25);
        glRotated(270,-1.0,0.0,0.0);

        glTranslated(0,+2.2,0);
        glRotated(90,0.0,0.0,-1.0);
        glScaled(1.5,1.5,1.5);
        MatFBRuby.Apply();
        _seashell.Render();
        glScaled(0.75,0.75,0.75);
        glRotated(90,0.0,0.0,1.0);
        glTranslated(0,-2.2,0);
        glRotated(270,1.0,0.0,0.0);

        glTranslated(-0.05,-4.2,0);

        glScaled(1.0,1.0,4.0);
        MatFBSilver.Apply();
        _goblet.Render();
        glScaled(1,1,0.25);
        glRotated(270,-1.0,0.0,0.0);

        glTranslated(0,+2.2,0);
        glRotated(90,0.0,0.0,-1.0);
        glScaled(1.5,1.5,1.5);
        MatFBRuby.Apply();
        _seashell.Render();
        glScaled(0.75,0.75,0.75);
        glRotated(90,0.0,0.0,1.0);
        glTranslated(0,-2.2,0);
        glRotated(270,1.0,0.0,0.0);

    }

    void GLWidget::paintSelectedModel() {
        if (_dl)
        {
            _dl->Enable();
            glEnable(GL_LIGHTING);
            glEnable(GL_NORMALIZE);
            glEnable(GL_TEXTURE_2D);
            _materials[_selected_material].Apply();

            switch(_selected_model) {
            case 0:
                _angel.Render();
                break;
            case 1:
                _bird.Render();
                break;
            case 2:
                _bug.Render();
                break;
            case 3:
                _cone.Render();
                break;
            case 4:
                _cube.Render();
                break;
            case 5:
                _dragon.Render();
                break;
            case 6:
                _elephant.Render();
                break;
            case 7:
                _gangster.Render();
                break;
            case 8:
                _goblet.Render();
                break;
            case 9:
                _horse.Render();
                break;
            case 10:
                _icosahedron.Render();
                break;
            case 11:
                _Lucy.Render();
                break;
            case 12:
                _mouse.Render();
                break;
            case 13:
                _seashell.Render();
                break;
            case 14:
                _space_station.Render();
                break;
            case 15:
                _sphere.Render();
                break;
            case 16:
                _Spot.Render();
                break;
            case 17:
                _star.Render();
                break;
            case 18:
                paintCastle();
                break;
            }
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_LIGHTING);
            glDisable(GL_NORMALIZE);
            _dl->Disable();
        }
    }

    void GLWidget::paintSelectedSurface() {
        if (_dl)
        {
            _dl->Enable();
                glEnable(GL_LIGHTING);
                glEnable(GL_NORMALIZE);
                glEnable(GL_TEXTURE_2D);
                    _materials[_selected_material].Apply();
                    if (_apply_texture) texture[_selected_texture]->bind();
                    else texture[_selected_texture]->release();
                    _image_of_ps[_selected_parametric_surface]->Render();
                glDisable(GL_TEXTURE_2D);
                glDisable(GL_LIGHTING);
                glDisable(GL_NORMALIZE);
            _dl->Disable();
        }
    }

    void GLWidget::paintSelectedCyclicCurve() {
        glColor3f(1.0f, 0.0f, 0.0f);
        _image_of_cyclic_curve[_selected_cyclic_curve]->RenderDerivatives(0, GL_LINE_STRIP);



        glPointSize(5.0f);
            if (_parametric_first_derivatives) {
                glColor3f(0.0f, 0.5f, 0.0f);
                _image_of_cyclic_curve[_selected_cyclic_curve]->RenderDerivatives(1, GL_LINES);
                _image_of_cyclic_curve[_selected_cyclic_curve]->RenderDerivatives(1, GL_POINTS);
            }

            if (_parametric_second_derivatives) {
                glColor3f(1.0f, 0.5f, 0.9f);
                _image_of_cyclic_curve[_selected_cyclic_curve]->RenderDerivatives(2, GL_LINES);
                _image_of_cyclic_curve[_selected_cyclic_curve]->RenderDerivatives(2, GL_POINTS);
            }

            if (_selected_cyclic_curve == 1){
                glBegin(GL_POINTS);
                glPointSize(15.0f);
                glColor3f(1.0f, 1.0f, 1.0f);
                    for (GLuint i = 0; i < points.GetRowCount(); i++)
                    {
                        if(i == GLuint(_cyclic_point)) {
                            glPointSize(100.0f);
                            glColor3f(0.0,1.0,0.0);
                            glVertex3dv(&points[i][0]);
                            glPointSize(15.0f);
                            glColor3f(1.0f, 1.0f, 1.0f);
                        } else {
                            glVertex3dv(&points[i][0]);
                        }


                    }
                glEnd();
            } else {
                glBegin(GL_POINTS);
                CyclicCurve3& curve= *_cyclic_curve[_selected_cyclic_curve];
                DCoordinate3 &cp = curve[_cyclic_point];


                    for (GLuint i = 0; i < points.GetRowCount(); i++)
                    {
                        if(i == GLuint(_cyclic_point)) {
                            glPointSize(100.0f);
                            glColor3f(0.0,1.0,0.0);
                            glVertex3f(cp.x(),cp.y(),cp.z());
                            glColor3f(1.0,0.0,0.0);
                            glPointSize(15.0f);
                        }


                    }
                glEnd();
                _cyclic_curve[0]->RenderData(GL_POINTS);
                _cyclic_curve[0]->RenderData();


            }
        glPointSize(1.0f);
    }

    void GLWidget::paintTrigonometricCurves() {


        int n = _image_of_trig_curve.GetColumnCount();
        for (int i = 1; i < n; i++) {
            glColor3f(1.0f, 0.0f, 0.0f);
            _image_of_trig_curve[i]->RenderDerivatives(0, GL_LINE_STRIP);

            glPointSize(5.0f);
                if (_parametric_first_derivatives) {
                    glColor3f(0.0f, 0.5f, 0.0f);
                    _image_of_trig_curve[i]->RenderDerivatives(1, GL_LINES);
                    _image_of_trig_curve[i]->RenderDerivatives(1, GL_POINTS);
                }

                if (_parametric_second_derivatives) {
                    glColor3f(1.0f, 0.5f, 0.9f);
                    _image_of_trig_curve[i]->RenderDerivatives(2, GL_LINES);
                    _image_of_trig_curve[i]->RenderDerivatives(2, GL_POINTS);
                }
             glPointSize(15.0f);
             glColor3f(0.0f, 1.0f, 1.0f);


             if(_trig_control_point) {
                 _trig_curve[i]->RenderData(GL_POINTS);
                 _trig_curve[i]->RenderData();
             }



            glPointSize(1.0f);
        }
    }

    void GLWidget::paintSecondTrigPatch() {
        if (_dl)
        {
            _dl->Enable();
                glEnable(GL_LIGHTING);
                glEnable(GL_NORMALIZE);
                glEnable(GL_TEXTURE_2D);
                    _materials[_selected_material].Apply();
                    if (_apply_texture) texture[_selected_texture]->bind();
                    else texture[_selected_texture]->release();

                    _image_of_patch->Render();

                glDisable(GL_TEXTURE_2D);
                glDisable(GL_LIGHTING);
                glDisable(GL_NORMALIZE);
            _dl->Disable();

            glColor3f(1.0f, 0.0f, 0.0f);
            if(_show_uip) {
                for (GLuint i = 0; i < _patch_uip->GetColumnCount(); i++) {
                    GenericCurve3 *curve = (*_patch_uip)[i];
                    if (curve) {
                        curve->RenderDerivatives(0,  GL_LINE_STRIP);
                    }
                }
            }
            glColor3f(0.0f, 0.5f, 0.0f);
            if(_show_vip) {
                for (GLuint i = 0; i < _patch_vip->GetColumnCount(); i++) {
                    GenericCurve3 *curve = (*_patch_vip)[i];
                    if (curve) {
                        curve->RenderDerivatives(0,  GL_LINE_STRIP);
                    }
                }
            }
        }
    }

    //-----------------------
    // the rendering function
    //-----------------------
    void GLWidget::paintGL()
    {
        // clears the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // stores/duplicates the original model view matrix
        glPushMatrix();

            // applying transformations
            glRotatef(_angle_x, 1.0, 0.0, 0.0);
            glRotatef(_angle_y, 0.0, 1.0, 0.0);
            glRotatef(_angle_z, 0.0, 0.0, 1.0);
            glTranslated(_trans_x, _trans_y, _trans_z);
            glScaled(_zoom, _zoom, _zoom);

            // render your geometry (this is oldest OpenGL rendering technique, later we will use some advanced methods)
            switch(_current_tab) {
                case 0:
                    paintDefaultTriangle();
                break;
                case 1:
                    paintSelectedParametricCurve();
                break;
                case 2:
                    paintSelectedModel();
                break;
                case 3:
                    paintSelectedSurface();
                break;
                case 4:
                    paintSelectedCyclicCurve();
                break;
                case 5:
                    paintTrigonometricCurves();
                break;
                case 6:
                    paintSecondTrigPatch();
                break;
                case 7:
                    enableShaders();
                    paintSelectedModel();
                    disableShaders();

            }

        // pops the current matrix stack, replacing the current matrix with the one below it on the stack,
        // i.e., the original model view matrix is restored
        glPopMatrix();
    }

    //----------------------------------------------------------------------------
    // when the main window is resized one needs to redefine the projection matrix
    //----------------------------------------------------------------------------
    void GLWidget::resizeGL(int w, int h)
    {
        // setting the new size of the rendering context
        glViewport(0, 0, w, h);

        // redefining the projection matrix
        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        _aspect = static_cast<double>(w) / static_cast<double>(h);

        gluPerspective(_fovy, _aspect, _z_near, _z_far);

        // switching back to the model view matrix
        glMatrixMode(GL_MODELVIEW);

        update();
    }

    void GLWidget::addTrigCurve() {
        int n = _trig_curve.GetColumnCount();
        if (!_trig_curve.ResizeColumns(n+1)) {
            //@TODO Error
            return;
        }

        //@TODO add a curve to the UI combo box
        _trig_curve[n] = new (nothrow) SecondOrderTrigArc();

        if(!_trig_curve[n])
        {
            cout << "Couldn't generate CyclicCurve" << endl;
            return;
        }

        SecondOrderTrigArc& curve= *_trig_curve[n];

        DCoordinate3 &cp = curve[0];
        DCoordinate3 &cp1 = curve[1];
        DCoordinate3 &cp2 = curve[2];
        DCoordinate3 &cp3 = curve[3];

        cp[0] = -2.0; //x
        cp[1] = -1.0; //y
        cp[2] = 0.0;  //z

        cp1[0] = -1.0; //x
        cp1[1] = 1.0; //y
        cp1[2] = 0.0;  //z

        cp2[0] = 1.0; //x
        cp2[1] = 1.0; //y
        cp2[2] = 0.0;  //z

        cp3[0] = 2.0; //x
        cp3[1] = -1.0; //y
        cp3[2] = 0.0;  //z

        if(!_trig_curve[n]->UpdateVertexBufferObjectsOfData(GL_STATIC_DRAW))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
        }

        _image_of_trig_curve.ResizeColumns(n+1);
        _image_of_trig_curve[n] = _trig_curve[n]->GenerateImage(2, 100, GL_STATIC_DRAW);

        if(!_image_of_trig_curve[n])
        {
            cout << "Couldn't generate Second Order Trig Curve" << endl;
            return;
        }

        if(!_image_of_trig_curve[n]->UpdateVertexBufferObjects(_derivatives_scale, GL_STATIC_DRAW))
        {
             cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
        }
    }

    void GLWidget::enableShaders() {
        _shaders[_selected_shader].Enable();
    }

    void GLWidget::disableShaders() {
        _shaders[_selected_shader].Enable();
    }

    void GLWidget::add_trig_curve() {
        addTrigCurve();
    }

    //-----------------------------------
    // implementation of the public slots
    //-----------------------------------

    void GLWidget::set_angle_x(int value)
    {
        if (_angle_x != value)
        {
            _angle_x = value;
            update();
        }
    }

    void GLWidget::set_angle_y(int value)
    {
        if (_angle_y != value)
        {
            _angle_y = value;
            update();
        }
    }

    void GLWidget::set_angle_z(int value)
    {
        if (_angle_z != value)
        {
            _angle_z = value;
            update();
        }
    }

    void GLWidget::set_zoom_factor(double value)
    {
        if (_zoom != value)
        {
            _zoom = value;
            update();
        }
    }

    void GLWidget::set_trans_x(double value)
    {
        if (_trans_x != value)
        {
            _trans_x = value;
            update();
        }
    }

    void GLWidget::set_trans_y(double value)
    {
        if (_trans_y != value)
        {
            _trans_y = value;
            update();
        }
    }

    void GLWidget::set_trans_z(double value)
    {
        if (_trans_z != value)
        {
            _trans_z = value;
            update();
        }
    }

    void GLWidget::set_parametric_curve(int value) {
        if(_selected_parametric_curve != value) {
            _selected_parametric_curve = value;
            update();
            set_subdivision_points(_parametric_div_point_count);
            set_derivatives_scale(_derivatives_scale * 100);
        }
    }

    void GLWidget::set_derivatives_scale(int value) {
        double res = (double) value / 100.0;
        if (_derivatives_scale != res) {
            _derivatives_scale = res;
            _images_of_parametric_curves[_selected_parametric_curve]->UpdateVertexBufferObjects(_derivatives_scale);
            update();
        }
    }

    void GLWidget::set_selected_model(int value) {
       if(_selected_model != value) {
           _selected_model = value;
           _angle = 0.0;
           update();
       }
    }
    void GLWidget::set_cyclic_subdivision_points(int value) {
        _cyclic_div_point_count = value;
        try {
            _image_of_cyclic_curve[_selected_cyclic_curve]->DeleteVertexBufferObjects();

            if (_selected_cyclic_curve == 0) {
                _image_of_cyclic_curve[_selected_cyclic_curve] = _cyclic_curve[_selected_cyclic_curve]->GenerateImage(2, _cyclic_div_point_count, GL_STATIC_DRAW);
            } else {
                _image_of_cyclic_curve[_selected_cyclic_curve] =  _cyclic_curve[_selected_cyclic_curve]->GenerateImage(2, 100, GL_STATIC_DRAW);
            }


            if(!_image_of_cyclic_curve[_selected_cyclic_curve])
            {
                cout << "Couldn't generate CyclicCurve" << endl;
                return;
            }

            if(!_image_of_cyclic_curve[_selected_cyclic_curve]->UpdateVertexBufferObjects(_derivatives_scale, GL_STATIC_DRAW))
            {
                 cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
            }
            update();
        } catch (Exception &e) {
            cout << e << endl;
        }
    }

    void GLWidget::set_subdivision_points(int value) {
        _parametric_div_point_count = value;
        try {
            if (_parametric_curves[_selected_parametric_curve] &&
                _parametric_curves[_selected_parametric_curve] &&
                _parametric_div_point_count != _images_of_parametric_curves[_selected_parametric_curve]->GetPointCount()){
                // régi kép törlése
                delete _images_of_parametric_curves[_selected_parametric_curve];

                // új osztópontrendszerhez tartozó kép előállítása
                _images_of_parametric_curves[_selected_parametric_curve] = _parametric_curves[_selected_parametric_curve]->GenerateImage(_parametric_div_point_count, GL_STATIC_DRAW);

                if (!_images_of_parametric_curves[_selected_parametric_curve])
                {
                    // hiba- és kivételkezelés
                }

                // az új kép csúcspont-pufferobjektumainak felépítése
                if (!_images_of_parametric_curves[_selected_parametric_curve]->UpdateVertexBufferObjects(_derivatives_scale))
                {
                    // memóriaszivárgás elkerülése, továbbá hiba- és kivételkezelés
                }
                update();
            }
        } catch (Exception &e) {
            cout << e << endl;
        }
    }


    void GLWidget::set_parametric_first_derivatives(bool value) {
        if (_parametric_first_derivatives != value) {
            _parametric_first_derivatives = value;
            update();
        }
    }

    void GLWidget::set_parametric_second_derivatives(bool value) {
        if (_parametric_second_derivatives != value) {
            _parametric_second_derivatives = value;
            update();
        }
    }

   void GLWidget::show_patch_uip(bool value) {
        if(_show_uip == value) return;
        _show_uip = value;
        if (!_show_uip) {update(); return;}

        if (_patch_uip) {
            for (GLuint i = 0; i < _patch_uip->GetColumnCount(); i++) {
                GenericCurve3 *curve = (*_patch_uip)[i];
                if (curve) {
                    curve->DeleteVertexBufferObjects();
                }
            }
        }

        _patch_uip = _patch.GenerateUIsoparametricLines(30, 1, 100);

        if (!_patch_uip) return;

        for (GLuint i = 0; i < _patch_uip->GetColumnCount(); i++) {
           GenericCurve3 *curve = (*_patch_uip)[i];
           if (curve) {
               curve->UpdateVertexBufferObjects();
           }
        }
        update();
   }

   void GLWidget::show_patch_vip(bool value) {
        if(_show_vip == value) return;
        _show_vip = value;
        if (!_show_vip) {update(); return;}

        if (_patch_vip) {
            for (GLuint i = 0; i < _patch_vip->GetColumnCount(); i++) {
               GenericCurve3 *curve = (*_patch_vip)[i];
               if (curve) {
                   curve->DeleteVertexBufferObjects();
               }
            }
        }

        _patch_vip = _patch.GenerateVIsoparametricLines(30, 1, 100);

        if (!_patch_vip) return;

        for (GLuint i = 0; i < _patch_vip->GetColumnCount(); i++) {
           GenericCurve3 *curve = (*_patch_vip)[i];
           if (curve) {
               curve->UpdateVertexBufferObjects();
           }
        }
   }

    void GLWidget::set_cyclic_curve(int value) {
        if (_selected_cyclic_curve != value) {
            _selected_cyclic_curve = value;
            if(_selected_cyclic_curve == 0) {
                CyclicCurve3& curve= *_cyclic_curve[_selected_cyclic_curve];
                DCoordinate3 &cp = curve[_cyclic_point];
                emit update_cyclic_point_x(cp[0]);
                emit update_cyclic_point_y(cp[1]);
                emit update_cyclic_point_z(cp[2]);
            } else {
                DCoordinate3 &cp = points[_cyclic_point];
                emit update_cyclic_point_x(cp[0]);
                emit update_cyclic_point_y(cp[1]);
                emit update_cyclic_point_z(cp[2]);
            }
            update();
        }
    }

    void GLWidget::set_cyclic_point(int value) {
        if (_cyclic_point != value) {
            _cyclic_point = value;
            if(_selected_cyclic_curve == 0) {
                CyclicCurve3& curve= *_cyclic_curve[_selected_cyclic_curve];
                DCoordinate3 &cp = curve[_cyclic_point];
                emit update_cyclic_point_x(cp[0]);
                emit update_cyclic_point_y(cp[1]);
                emit update_cyclic_point_z(cp[2]);
            } else {
                DCoordinate3 &cp = points[_cyclic_point];
                emit update_cyclic_point_x(cp[0]);
                emit update_cyclic_point_y(cp[1]);
                emit update_cyclic_point_z(cp[2]);
            }
        }
    }

    void GLWidget::set_cyclic_point_x(double value) {
        CyclicCurve3& curve= *_cyclic_curve[_selected_cyclic_curve];
        DCoordinate3 &cp = curve[_cyclic_point];
        if (_selected_cyclic_curve == 1) {
            points[_cyclic_point][0] = value;
        } else {
            if (cp[0] == value) return;

             cp[0] = value;
        }

        curve.DeleteVertexBufferObjectsOfData();

        if (_selected_cyclic_curve == 1) {
            if (!curve.UpdateDataForInterpolation(knot_vector, points)) {
                cout << "Could Not Udate Data for interpolation" << endl;
            }
        }

        if(!curve.UpdateVertexBufferObjectsOfData(GL_STATIC_DRAW))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
        }

        _image_of_cyclic_curve[_selected_cyclic_curve]->DeleteVertexBufferObjects();

        if (_selected_cyclic_curve == 0) {
            _image_of_cyclic_curve[_selected_cyclic_curve] = curve.GenerateImage(2, _cyclic_div_point_count, GL_STATIC_DRAW);
        } else {
            _image_of_cyclic_curve[_selected_cyclic_curve] = curve.GenerateImage(2, 100, GL_STATIC_DRAW);
        }


        if(!_image_of_cyclic_curve[_selected_cyclic_curve])
        {
            cout << "Couldn't generate CyclicCurve" << endl;
            return;
        }

        if(!_image_of_cyclic_curve[_selected_cyclic_curve]->UpdateVertexBufferObjects(_derivatives_scale, GL_STATIC_DRAW))
        {
             cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
        }
        update();
    }

    void GLWidget::set_cyclic_point_y(double value) {
        CyclicCurve3& curve= *_cyclic_curve[_selected_cyclic_curve];
        DCoordinate3 &cp = curve[_cyclic_point];
        if (_selected_cyclic_curve == 1) {
            points[_cyclic_point][1] = value;
        } else {
            if (cp[1] == value) return;

             cp[1] = value;
        }

        curve.DeleteVertexBufferObjectsOfData();

        if (_selected_cyclic_curve == 1) {
            if (!curve.UpdateDataForInterpolation(knot_vector, points)) {
                cout << "Could Not Udate Data for interpolation" << endl;
            }
        }

        if(!curve.UpdateVertexBufferObjectsOfData(GL_STATIC_DRAW))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
        }

        _image_of_cyclic_curve[_selected_cyclic_curve]->DeleteVertexBufferObjects();

        if (_selected_cyclic_curve == 0) {
            _image_of_cyclic_curve[_selected_cyclic_curve] = curve.GenerateImage(2, _cyclic_div_point_count, GL_STATIC_DRAW);
        } else {
            _image_of_cyclic_curve[_selected_cyclic_curve] = curve.GenerateImage(2, 100, GL_STATIC_DRAW);
        }


        if(!_image_of_cyclic_curve[_selected_cyclic_curve])
        {
            cout << "Couldn't generate CyclicCurve" << endl;
            return;
        }

        if(!_image_of_cyclic_curve[_selected_cyclic_curve]->UpdateVertexBufferObjects(_derivatives_scale, GL_STATIC_DRAW))
        {
             cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
        }
        update();
    }

    void GLWidget::set_cyclic_point_z(double value) {
        CyclicCurve3& curve= *_cyclic_curve[_selected_cyclic_curve];
        DCoordinate3 &cp = curve[_cyclic_point];
        if (_selected_cyclic_curve == 1) {
            points[_cyclic_point][2] = value;
        } else {
            if (cp[2] == value) return;

             cp[2] = value;
        }

        curve.DeleteVertexBufferObjectsOfData();

        if (_selected_cyclic_curve == 1) {
            if (!curve.UpdateDataForInterpolation(knot_vector, points)) {
                cout << "Could Not Udate Data for interpolation" << endl;
            }
        }

        if(!curve.UpdateVertexBufferObjectsOfData(GL_STATIC_DRAW))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
        }

        _image_of_cyclic_curve[_selected_cyclic_curve]->DeleteVertexBufferObjects();

        if (_selected_cyclic_curve == 0) {
            _image_of_cyclic_curve[_selected_cyclic_curve] = curve.GenerateImage(2, _cyclic_div_point_count, GL_STATIC_DRAW);
        } else {
            _image_of_cyclic_curve[_selected_cyclic_curve] = curve.GenerateImage(2, 100, GL_STATIC_DRAW);
        }


        if(!_image_of_cyclic_curve[_selected_cyclic_curve])
        {
            cout << "Couldn't generate CyclicCurve" << endl;
            return;
        }

        if(!_image_of_cyclic_curve[_selected_cyclic_curve]->UpdateVertexBufferObjects(_derivatives_scale, GL_STATIC_DRAW))
        {
             cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
        }
        update();
    }



    void GLWidget::set_trig_point(int value) {
        if (_trig_point != value) {
            _trig_point = value;
            SecondOrderTrigArc& curve= *_trig_curve[1];
            DCoordinate3 &cp = curve[_trig_point];
            emit update_trig_point_x(cp[0]);
            emit update_trig_point_y(cp[1]);
            emit update_trig_point_z(cp[2]);
        }
    }
    void GLWidget::set_trig_point_x(double value) {
        SecondOrderTrigArc& curve= *_trig_curve[1];
        DCoordinate3 &cp = curve[_trig_point];

        if (cp[0] == value) return;
         cp[0] = value;


        curve.DeleteVertexBufferObjectsOfData();
        if(!curve.UpdateVertexBufferObjectsOfData(GL_STATIC_DRAW))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_trig_curve!" << endl;
        }

        _image_of_trig_curve[1]->DeleteVertexBufferObjects();
        _image_of_trig_curve[1] = curve.GenerateImage(2, 200, GL_STATIC_DRAW);

        if(!_image_of_trig_curve[1])
        {
            cout << "Couldn't generate SecondOrderTrigCurve" << endl;
            return;
        }

        if(!_image_of_trig_curve[1]->UpdateVertexBufferObjects(_derivatives_scale, GL_STATIC_DRAW))
        {
             cout << "Could_not_create_the_vertex_buffer_object_of_the_trig_curve!" << endl;
        }
        update();
    }
    void GLWidget::set_trig_point_y(double value) {
        SecondOrderTrigArc& curve= *_trig_curve[1];
        DCoordinate3 &cp = curve[_trig_point];

        if (cp[1] == value) return;
         cp[1] = value;


        curve.DeleteVertexBufferObjectsOfData();
        if(!curve.UpdateVertexBufferObjectsOfData(GL_STATIC_DRAW))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_trig_curve!" << endl;
        }

        _image_of_trig_curve[1]->DeleteVertexBufferObjects();
        _image_of_trig_curve[1] = curve.GenerateImage(2, 200, GL_STATIC_DRAW);

        if(!_image_of_trig_curve[1])
        {
            cout << "Couldn't generate SecondOrderTrigCurve" << endl;
            return;
        }

        if(!_image_of_trig_curve[1]->UpdateVertexBufferObjects(_derivatives_scale, GL_STATIC_DRAW))
        {
             cout << "Could_not_create_the_vertex_buffer_object_of_the_trig_curve!" << endl;
        }
        update();
    }
    void GLWidget::set_trig_point_z(double value) {
        SecondOrderTrigArc& curve= *_trig_curve[1];
        DCoordinate3 &cp = curve[_trig_point];

        if (cp[2] == value) return;
         cp[2] = value;


        curve.DeleteVertexBufferObjectsOfData();
        if(!curve.UpdateVertexBufferObjectsOfData(GL_STATIC_DRAW))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_trig_curve!" << endl;
        }

        _image_of_trig_curve[1]->DeleteVertexBufferObjects();
        _image_of_trig_curve[1] = curve.GenerateImage(2, 200, GL_STATIC_DRAW);

        if(!_image_of_trig_curve[1])
        {
            cout << "Couldn't generate SecondOrderTrigCurve" << endl;
            return;
        }

        if(!_image_of_trig_curve[1]->UpdateVertexBufferObjects(_derivatives_scale, GL_STATIC_DRAW))
        {
             cout << "Could_not_create_the_vertex_buffer_object_of_the_trig_curve!" << endl;
        }
        update();
    }

    void GLWidget::set_trig_control_point(bool value) {
        _trig_control_point = value;
        update();
    }

    void GLWidget::set_tab(int value) {
        _current_tab = value;
        update();
    }

    void GLWidget::set_apply_texture(bool value) {
        if (value != _apply_texture) {
            _apply_texture = value;
            update();
        }
    }

    void GLWidget::set_parametric_surface(int value) {
        if (value != _selected_parametric_surface) {
            _selected_parametric_surface = value;
            update();
        }
    }

    void GLWidget::set_selected_material(int value) {
        if (value != _selected_material) {
            _selected_material = value;
            update();
        }
    }

    void GLWidget::set_selected_texture(int value) {
        if(_selected_texture != value) {
            if(_apply_texture) texture[_selected_texture]->release();
            _selected_texture = value;
            update();
        }
    }

    void GLWidget::set_alpha(double value) {
        if (_trig_curve[1]->GetAlpha() == value) {
            return;
        }
        _trig_curve[1]->DeleteVertexBufferObjectsOfData();
        _trig_curve[1]->SetAlpha(value);
        if(!_trig_curve[1]->UpdateVertexBufferObjectsOfData(GL_STATIC_DRAW))
        {
            cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
        }
        _image_of_trig_curve[1]->DeleteVertexBufferObjects();
        _image_of_trig_curve[1] = _trig_curve[1]->GenerateImage(2, 100, GL_STATIC_DRAW);

        if(!_image_of_trig_curve[1])
        {
            cout << "Couldn't generate Second Order Trig Curve" << endl;
            return;
        }

        if(!_image_of_trig_curve[1]->UpdateVertexBufferObjects(_derivatives_scale, GL_STATIC_DRAW))
        {
             cout << "Could_not_create_the_vertex_buffer_object_of_the_cyclic_curve!" << endl;
        }

        update();
    }


    void GLWidget::_animate() {
        TriangulatedMesh3 &selected = _mouse;

        if(_current_tab != 2 || _selected_model > 1)
            return;

        GLfloat *vertex = selected.MapVertexBuffer(GL_READ_WRITE);
        GLfloat *normal = selected.MapNormalBuffer(GL_READ_ONLY);

        _angle += DEG_TO_RADIAN;
        if(_angle >= TWO_PI)
            _angle -=TWO_PI;

        GLfloat scale = sin(_angle) / 3000.0;

        for(GLuint i = 0; i < selected.VertexCount(); ++i)
        {
            for(GLuint coordinate = 0; coordinate < 3; ++coordinate, ++vertex, ++normal)
            {
                *vertex += scale * (*normal);
            }
        }

        selected.UnmapVertexBuffer();
        selected.UnmapNormalBuffer();

        update();
    }

    void GLWidget::set_patch_alpha_u(double value) {
        if (value == _patch.GetUAlpha()) return;
        _patch.SetUAlpha(value);

        _patch.DeleteVertexBufferObjectsOfData();
        _image_of_patch->DeleteVertexBufferObjects();

        _patch.UpdateVertexBufferObjectsOfData();
        _image_of_patch = _patch.GenerateImage(30, 30, GL_STATIC_DRAW);

        if (_image_of_patch) _image_of_patch->UpdateVertexBufferObjects();
        update();
    }

    void GLWidget::set_patch_alpha_v(double value) {
        if (value == _patch.GetVAlpha()) return;
        _patch.SetVAlpha(value);

        _patch.DeleteVertexBufferObjectsOfData();
        _image_of_patch->DeleteVertexBufferObjects();

        _patch.UpdateVertexBufferObjectsOfData();
        _image_of_patch = _patch.GenerateImage(30, 30, GL_STATIC_DRAW);

        if (_image_of_patch) _image_of_patch->UpdateVertexBufferObjects();
        update();
    }
    void GLWidget::set_patch_row(int value){
        if (value == _patch_row) return;
        _patch_row = value;

        DCoordinate3 p;
        _patch.GetData(_patch_row, _patch_col, p);

        emit update_patch_point_x(p.x());
        emit update_patch_point_y(p.y());
        emit update_patch_point_z(p.z());
    }
    void GLWidget::set_patch_col(int value){
        if (value == _patch_col) return;
        _patch_col = value;

        DCoordinate3 p;
        _patch.GetData(_patch_row, _patch_col, p);

        emit update_patch_point_x(p.x());
        emit update_patch_point_y(p.y());
        emit update_patch_point_z(p.z());
    }

    void GLWidget::set_patch_point_x(double value){
        DCoordinate3 p;
        _patch.GetData(_patch_row, _patch_col, p);

        if (p.x() == value) return;

        _patch.SetData(_patch_row, _patch_col, value, p.y(), p.z());

        _patch.DeleteVertexBufferObjectsOfData();
        _image_of_patch->DeleteVertexBufferObjects();

        _patch.UpdateVertexBufferObjectsOfData();
        _image_of_patch = _patch.GenerateImage(30, 30, GL_STATIC_DRAW);

        if (_image_of_patch) _image_of_patch->UpdateVertexBufferObjects();
        update();
    }

    void GLWidget::set_patch_point_y(double value){
        DCoordinate3 p;
        _patch.GetData(_patch_row, _patch_col, p);

        if (p.y() == value) return;

        _patch.SetData(_patch_row, _patch_col, p.x(), value, p.z());
        _patch.DeleteVertexBufferObjectsOfData();
        _image_of_patch->DeleteVertexBufferObjects();

        _patch.UpdateVertexBufferObjectsOfData();
        _image_of_patch = _patch.GenerateImage(30, 30, GL_STATIC_DRAW);

        if (_image_of_patch) _image_of_patch->UpdateVertexBufferObjects();
        update();
    }

    void GLWidget::set_patch_point_z(double value){
        DCoordinate3 p;
        _patch.GetData(_patch_row, _patch_col, p);

        if (p.z() == value) return;

        _patch.SetData(_patch_row, _patch_col, p.x(), p.y(), value);
        _patch.DeleteVertexBufferObjectsOfData();
        _image_of_patch->DeleteVertexBufferObjects();

        _patch.UpdateVertexBufferObjectsOfData();
        _image_of_patch = _patch.GenerateImage(30, 30, GL_STATIC_DRAW);

        if (_image_of_patch) _image_of_patch->UpdateVertexBufferObjects();
        update();
    }

    void GLWidget::set_shader(int value) {
        if(_selected_shader != value) {
            _selected_shader = value;
            update();
        }
    }

    void GLWidget::set_scale_parameter(double value) {
        _shaders[_selected_shader].Enable();
        if(_scale_parameter != value) {
            _scale_parameter = value;

                _shaders[_selected_shader].SetUniformVariable4f("default_outline_color", _scale_parameter,_shading_parameter,_smoothing_parameter, 1.0f);

            {
                _shaders[_selected_shader].SetUniformVariable1f("scale_factor", _scale_parameter);
                _shaders[_selected_shader].SetUniformVariable1f("smoothing", _smoothing_parameter);
                _shaders[_selected_shader].SetUniformVariable1f("shading", _shading_parameter);
            }
            _shaders[_selected_shader].Disable();
            update();
        }
    }

    void GLWidget::set_smoothing_parameter(double value) {
        cout << _selected_shader << endl;
        _shaders[_selected_shader].Enable();
        if(_smoothing_parameter != value) {
            _smoothing_parameter = value;

                _shaders[_selected_shader].SetUniformVariable4f("default_outline_color", _scale_parameter,_shading_parameter,_smoothing_parameter, 1.0f);

            {
                _shaders[_selected_shader].SetUniformVariable1f("scale_factor", _scale_parameter);
                _shaders[_selected_shader].SetUniformVariable1f("smoothing", _smoothing_parameter);
                _shaders[_selected_shader].SetUniformVariable1f("shading", _shading_parameter);
            }
            _shaders[_selected_shader].Disable();
            update();
        }
    }

    void GLWidget::set_shading_parameter(double value) {
        _shaders[_selected_shader].Enable();
        if(_shading_parameter != value) {
            _shading_parameter = value;

                _shaders[_selected_shader].SetUniformVariable4f("default_outline_color", _scale_parameter,_shading_parameter,_smoothing_parameter, 1.0f);

            {
                _shaders[_selected_shader].SetUniformVariable1f("scale_factor", _scale_parameter);
                _shaders[_selected_shader].SetUniformVariable1f("smoothing", _smoothing_parameter);
                _shaders[_selected_shader].SetUniformVariable1f("shading", _shading_parameter);
            }
            _shaders[_selected_shader].Disable();
            update();
        }
    }

}
