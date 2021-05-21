#include "SideWidget.h"

namespace cagd
{
    SideWidget::SideWidget(QWidget *parent): QWidget(parent)
    {
        setupUi(this);

        QPalette p = rotate_x_slider->palette();

        p.setColor(QPalette::Highlight, QColor(255,50,10));

        rotate_x_slider->setPalette(p);

        p = rotate_y_slider->palette();

        p.setColor(QPalette::Highlight, QColor(50,255,10));

        rotate_y_slider->setPalette(p);

        p = rotate_z_slider->palette();

        p.setColor(QPalette::Highlight, QColor(10,10,255));

        rotate_z_slider->setPalette(p);

        p = derivatives_scale_slider->palette();

        p.setColor(QPalette::Highlight, QColor(255,255,0));

        derivatives_scale_slider->setPalette(p);
        /* Parametric curves */
        // 1. Spiral on Cone
        // 2. Torus Knot
        // 3. Circle
        // 4. Lissajous
        // 5. Hypotrochoid
        // 6. Rose
        // 7. Sphere Curve
        // 8. Torus Curve
        parametric_curve_cb->addItem("Spiral on Cone");
        parametric_curve_cb->addItem("Torus Knot");
        parametric_curve_cb->addItem("Circle");
        parametric_curve_cb->addItem("Lissajous");
        parametric_curve_cb->addItem("Hypotrochoid");
        parametric_curve_cb->addItem("Rose");
        parametric_curve_cb->addItem("Sphere Curve");
        parametric_curve_cb->addItem("Torus Curve");

        models_cb->addItem("Angel");
        models_cb->addItem("Bird");
        models_cb->addItem("Bug");
        models_cb->addItem("Cone");
        models_cb->addItem("Cube");
        models_cb->addItem("Dragon");
        models_cb->addItem("Elephant");
        models_cb->addItem("Gangster");
        models_cb->addItem("Goblet");
        models_cb->addItem("Horse");
        models_cb->addItem("Icosahedron");
        models_cb->addItem("Lucy");
        models_cb->addItem("Mouse");
        models_cb->addItem("Seashell");
        models_cb->addItem("Space Station");
        models_cb->addItem("Sphere");
        models_cb->addItem("Spot");
        models_cb->addItem("Star");
        models_cb->addItem("Playground");
        /* Parametric surfaces */
        // 1. Torus
        // 2. Sphere
        // 3. Revolution
        // 4. Cylinder
        // 5. Gabriel's Horn
        // 6. Helicoid
        parametric_surface_cb->addItem("Torus");
        parametric_surface_cb->addItem("Sphere");
        parametric_surface_cb->addItem("Revolution");
        parametric_surface_cb->addItem("Cylinder");
        parametric_surface_cb->addItem("Saddle");
        parametric_surface_cb->addItem("No idea");
//        MatFBBrass;
//        MatFBGold;
//        MatFBSilver;
//        MatFBEmerald;
//        MatFBPearl;
//        MatFBRuby;
//        MatFBTurquoise;
        material_cb->addItem("Brass");
        material_cb->addItem("Gold");
        material_cb->addItem("Silver");
        material_cb->addItem("Emerald");
        material_cb->addItem("Pearl");
        material_cb->addItem("Ruby");
        material_cb->addItem("Turquouse");

        cyclic_curve_cb->addItem("Not Interpolated");
        cyclic_curve_cb->addItem("Interpolated");

        texture_cb->addItem("1");
        texture_cb->addItem("2");
        texture_cb->addItem("3");
        texture_cb->addItem("4");
        texture_cb->addItem("5");
        texture_cb->addItem("6");
        texture_cb->addItem("7");
        texture_cb->addItem("8");
        texture_cb->addItem("9");
        texture_cb->addItem("10");

        cyclic_point_cb->addItem("1");
        cyclic_point_cb->addItem("2");
        cyclic_point_cb->addItem("3");
        cyclic_point_cb->addItem("4");
        cyclic_point_cb->addItem("5");

        trig_point_cb->addItem("1");
        trig_point_cb->addItem("2");
        trig_point_cb->addItem("3");
        trig_point_cb->addItem("4");

        patch_row_cb->addItem("1");
        patch_row_cb->addItem("2");
        patch_row_cb->addItem("3");
        patch_row_cb->addItem("4");

        patch_col_cb->addItem("1");
        patch_col_cb->addItem("2");
        patch_col_cb->addItem("3");
        patch_col_cb->addItem("4");

        _shader_cb->addItem("Directional Light");
        _shader_cb->addItem("Two Sided Light");
        _shader_cb->addItem("Toon");
        _shader_cb->addItem("Reflection Lines");

    }
}
