#include "MainWindow.h"

namespace cagd
{
    MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
    {
        setupUi(this);

    /*

      the structure of the main window's central widget

     *---------------------------------------------------*
     |                 central widget                    |
     |                                                   |
     |  *---------------------------*-----------------*  |
     |  |     rendering context     |   scroll area   |  |
     |  |       OpenGL widget       | *-------------* |  |
     |  |                           | | side widget | |  |
     |  |                           | |             | |  |
     |  |                           | |             | |  |
     |  |                           | *-------------* |  |
     |  *---------------------------*-----------------*  |
     |                                                   |
     *---------------------------------------------------*

    */
        _side_widget = new SideWidget(this);

        _scroll_area = new QScrollArea(this);
        _scroll_area->setWidget(_side_widget);
        _scroll_area->setSizePolicy(_side_widget->sizePolicy());
        _scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        _gl_widget = new GLWidget(this);

        centralWidget()->setLayout(new QHBoxLayout());
        centralWidget()->layout()->addWidget(_gl_widget);
        centralWidget()->layout()->addWidget(_scroll_area);


        connect(_side_widget->tabWidget, SIGNAL(currentChanged(int)), _gl_widget, SLOT(set_tab(int)));

        // creating a signal slot mechanism between the rendering context and the side widget
        connect(_side_widget->rotate_x_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_x(int)));
        connect(_side_widget->rotate_y_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_y(int)));
        connect(_side_widget->rotate_z_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_z(int)));

        connect(_side_widget->derivatives_scale_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_derivatives_scale(int)));

        connect(_side_widget->zoom_factor_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_zoom_factor(double)));

        connect(_side_widget->trans_x_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_x(double)));
        connect(_side_widget->trans_y_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_y(double)));
        connect(_side_widget->trans_z_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_z(double)));

        connect(_side_widget->parametric_curve_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_parametric_curve(int)));
        connect(_side_widget->models_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_selected_model(int)));
        connect(_side_widget->material_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_selected_material(int)));
        connect(_side_widget->texture_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_selected_texture(int)));

        connect(_side_widget->chb_first_der, SIGNAL(clicked(bool)), _gl_widget, SLOT(set_parametric_first_derivatives(bool)));
        connect(_side_widget->chb_second_der, SIGNAL(clicked(bool)), _gl_widget, SLOT(set_parametric_second_derivatives(bool)));

        connect(_side_widget->chb_apply_texture, SIGNAL(clicked(bool)), _gl_widget, SLOT(set_apply_texture(bool)));

        connect(_side_widget->parametric_surface_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_parametric_surface(int)));

        connect(_side_widget->parametric_division_points_sb, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_subdivision_points(int)));
        connect(_side_widget->cyclic_div_p_sb, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_cyclic_subdivision_points(int)));

        connect(_side_widget->cyclic_x, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_cyclic_point_x(double)));
        connect(_side_widget->cyclic_y, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_cyclic_point_y(double)));
        connect(_side_widget->cyclic_z, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_cyclic_point_z(double)));

        connect(_gl_widget, SIGNAL(update_cyclic_point_x(double)), _side_widget->cyclic_x, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(update_cyclic_point_y(double)), _side_widget->cyclic_y, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(update_cyclic_point_z(double)), _side_widget->cyclic_z, SLOT(setValue(double)));

        connect(_side_widget->trig_sb_x, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trig_point_x(double)));
        connect(_side_widget->trig_sb_y, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trig_point_y(double)));
        connect(_side_widget->trig_sb_z, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trig_point_z(double)));

        connect(_gl_widget, SIGNAL(update_trig_point_x(double)), _side_widget->trig_sb_x, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(update_trig_point_y(double)), _side_widget->trig_sb_y, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(update_trig_point_z(double)), _side_widget->trig_sb_z, SLOT(setValue(double)));

        connect(_side_widget->trig_point_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_trig_point(int)));
        connect(_side_widget->cyclic_point_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_cyclic_point(int)));
        connect(_side_widget->cyclic_curve_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_cyclic_curve(int)));

        connect(_side_widget->alpha_sb, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_alpha(double)));
        connect(_gl_widget, SIGNAL(update_alpha(double)), _side_widget->alpha_sb, SLOT(setValue(double)));

        connect(_side_widget->patch_row_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_patch_row(int)));
        connect(_side_widget->patch_col_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_patch_col(int)));

        connect(_gl_widget, SIGNAL(update_patch_point_x(double)), _side_widget->patch_x_sb, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(update_patch_point_y(double)), _side_widget->patch_y_sb, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(update_patch_point_z(double)), _side_widget->patch_z_sb, SLOT(setValue(double)));

        connect(_gl_widget, SIGNAL(update_alpha_u(double)), _side_widget->patch_a_u, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(update_alpha_v(double)), _side_widget->patch_a_v, SLOT(setValue(double)));;

        connect(_side_widget->patch_x_sb, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_patch_point_x(double)));
        connect(_side_widget->patch_y_sb, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_patch_point_y(double)));
        connect(_side_widget->patch_z_sb, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_patch_point_z(double)));

        connect(_side_widget->patch_a_u, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_patch_alpha_u(double)));
        connect(_side_widget->patch_a_v, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_patch_alpha_v(double)));

        connect(_side_widget->patch_uip_chb, SIGNAL(clicked(bool)), _gl_widget, SLOT(show_patch_uip(bool)));
        connect(_side_widget->patch_vip_chb, SIGNAL(clicked(bool)), _gl_widget, SLOT(show_patch_vip(bool)));

        connect(_side_widget->trig_control_points_chb, SIGNAL(clicked(bool)),_gl_widget,SLOT(set_trig_control_point(bool)));

        connect(_side_widget->_shader_cb, SIGNAL(currentIndexChanged(int)), _gl_widget, SLOT(set_shader(int)));
        connect(_side_widget->scale_sb, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_scale_parameter(double)));
        connect(_side_widget->smoothing_sb, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_smoothing_parameter(double)));
        connect(_side_widget->shading_sb, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_shading_parameter(double)));
    }

    //--------------------------------
    // implementation of private slots
    //--------------------------------
    void MainWindow::on_action_Quit_triggered()
    {
        qApp->exit(0);
    }
}
