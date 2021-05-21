#pragma once

#include <GL/glew.h>
#include <QOpenGLWidget>
#include "../Parametric/ParametricCurves3.h"
#include "../Parametric/ParametricSurfaces3.h"
#include "../Core/GenericCurves3.h"
#include "../Core/Materials.h"
#include "../Core/TriangulatedMeshes3.h"
#include "../Core/Lights.h"
#include "../Cyclic/CyclicCurves3.h"
#include "../Trigonometric/SecondOrderTrigonometricArc.h"
#include "../Trigonometric/SecondOrderTrigonometricPatch.h"
#include "../Core/ShaderPrograms.h"
#include <QTimer>
#include <QImage>
#include <QOpenGLTexture>

namespace cagd
{
    class GLWidget: public QOpenGLWidget
    {
        Q_OBJECT

    private:
        int _current_tab;
        int location;

        // variables defining the projection matrix
        double       _aspect;            // aspect ratio of the rendering window
        double       _fovy;              // field of view in direction y
        double       _z_near, _z_far;    // distance of near and far clipping planes

        // variables defining the model-view matrix
        double       _eye[3], _center[3], _up[3];

        // variables needed by transformations
        int         _angle_x, _angle_y, _angle_z;
        double      _zoom;
        double      _trans_x, _trans_y, _trans_z;
        double      _derivatives_scale;
        bool        _parametric_first_derivatives;
        bool        _parametric_second_derivatives;
        bool        _cyclic_first_derivatives;
        bool        _cyclic_second_derivatives;
        bool        _apply_texture;
        bool        _trig_control_point;


        int         _selected_parametric_curve;
        int         _selected_parametric_surface;
        int         _selected_cyclic_curve;
        int         _selected_model;
        int         _selected_material;
        int         _selected_texture;
        int         _selected_trig_curve;
        int         _cyclic_point;
        int         _trig_point;





        // your other declarations

        /* Parametric curves */
        // 1. Spiral on Cone
        // 2. Torus Knot
        // 3. Circle
        // 4. Lissajous
        // 5. Hypotrochoid
        // 6. Rose
        // 7. Sphere Curve
        // 8. Torus Curve


        RowMatrix<ShaderProgram> _shaders;
        float                    _scale_parameter;
        float                    _smoothing_parameter;
        float                    _shading_parameter;
        int                      _selected_shader;


        ParametricCurve3 *_parametric_curves[8];
        GenericCurve3 *_images_of_parametric_curves[8];

        CyclicCurve3 *_cyclic_curve[2];
        GenericCurve3 *_image_of_cyclic_curve[2];

        ColumnMatrix<GLdouble> knot_vector;
        ColumnMatrix<DCoordinate3> points;

        RowMatrix<SecondOrderTrigArc*> _trig_curve;
        RowMatrix<GenericCurve3*> _image_of_trig_curve;

        /* Parametric surfaces */
        // 1. Torus
        // 2. Sphere
        // 3. Revolution
        // 4. Cylinder
        // 5. Gabriel's Horn
        // 6. Helicoid
        ParametricSurface3 *_parametric_surfaces[6];
        TriangulatedMesh3 *_image_of_ps[6];

        const int _parametric_curves_count = 8;
        GLuint _parametric_div_point_count;
        GLuint _cyclic_div_point_count;

        QOpenGLTexture *texture[10];
        Material _materials[7];
        DirectionalLight *_dl = nullptr;

        // Models
        // 0. Mouse
        // 1. Bird
        // 2. Cube
        // 3. Cone
        // 4. Dragon
        // 5. Bandit
        // 6. Lucy
        // 7. Horse
        RowMatrix<TriangulatedMesh3> _models;
        TriangulatedMesh3 _angel,_bird,_bug,_cone,_cube,_dragon,_elephant,_gangster,_goblet,_horse,_icosahedron,_Lucy,_mouse,_seashell,_space_station,_sphere,_Spot,_star;


        struct ModelProperties
        {
            GLuint          id;
            GLuint          material_id;
            DCoordinate3    position;
            GLdouble        angle[3];
            GLdouble        scale[3];
        };

        QTimer              *_timer;
        double             _angle;


        SecTrigPatch3       _patch;
        TriangulatedMesh3   *_image_of_patch;
        RowMatrix<GenericCurve3*>* _patch_uip;
        RowMatrix<GenericCurve3*>* _patch_vip;
        int                 _patch_row;
        int                 _patch_col;
        bool                _show_control_net;
        bool                _show_uip;
        bool                _show_vip;


    public:
        // special and default constructor
        // the format specifies the properties of the rendering window
        GLWidget(QWidget* parent = 0);

        // redeclared virtual functions
        void initializeGL();
        void paintGL();
        void resizeGL(int w, int h);

        // Initialization of the Parametric curves
        void initializeParametricCurves();
        void initializeCyclicCurves();
        void initializeNthParametricCurve(int n);
        void initializeSurfaces();
        void initializeModels();

        void paintDefaultTriangle();
        void paintSelectedParametricCurve();
        void paintSelectedModel();
        void paintCastle();
        void paintSelectedSurface();
        void paintSelectedCyclicCurve();
        void paintTrigonometricCurves();
        void paintSecondTrigPatch();

        void initializeTorusSurface();
        void initializeSphereSurface();
        void initializeRevolutionSurface();
        void initializeCylinderSurface();
        void initializeGabrielSurface();
        void initializeHelicoidSurface();
        void initializeSecondTrigPatch();
        void initializeShaders();

        void addTrigCurve();

        void deleteCyclicCurves();

        void enableShaders();
        void disableShaders();

        virtual ~GLWidget();

    signals:
        void update_cyclic_point_x(double value);
        void update_cyclic_point_y(double value);
        void update_cyclic_point_z(double value);

        void update_trig_point_x(double value);
        void update_trig_point_y(double value);
        void update_trig_point_z(double value);

        void update_alpha(double value);

        void update_patch_point_x(double value);
        void update_patch_point_y(double value);
        void update_patch_point_z(double value);

        void update_alpha_u(double value);
        void update_alpha_v(double value);

    public slots:
        // public event handling methods/slots

        void set_tab(int value);

        void set_angle_x(int value);
        void set_angle_y(int value);
        void set_angle_z(int value);

        void set_zoom_factor(double value);

        void set_trans_x(double value);
        void set_trans_y(double value);
        void set_trans_z(double value);

        void set_derivatives_scale(int value);
        void set_subdivision_points(int value);
        void set_cyclic_subdivision_points(int value);

        void set_parametric_first_derivatives(bool value);
        void set_parametric_second_derivatives(bool value);

        void set_cyclic_point(int value);
        void set_cyclic_point_x(double value);
        void set_cyclic_point_y(double value);
        void set_cyclic_point_z(double value);

        void set_trig_point(int value);
        void set_trig_point_x(double value);
        void set_trig_point_y(double value);
        void set_trig_point_z(double value);

        void set_parametric_curve(int value);
        void set_cyclic_curve(int value);
        void set_selected_model(int value);

        void set_parametric_surface(int value);
        void set_selected_material(int value);

        void set_apply_texture(bool value);
        void set_selected_texture(int value);

        void add_trig_curve();

        void set_alpha(double value);

        void set_patch_alpha_u(double value);
        void set_patch_alpha_v(double value);
        void set_patch_row(int value);
        void set_patch_col(int value);

        void set_patch_point_x(double value);
        void set_patch_point_y(double value);
        void set_patch_point_z(double value);

        void show_patch_uip(bool value);
        void show_patch_vip(bool value);

        void set_trig_control_point(bool value);

        void set_shader(int value);
        void set_scale_parameter(double value);
        void set_shading_parameter(double value);
        void set_smoothing_parameter(double value);

        void _animate();
    };
}
