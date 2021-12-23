#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QKeyEvent>
#include <QStringListModel>
#include <QColorDialog>
#include <QFileDialog>
#include "scene_manager.h"

const float intensityLight = 1.f;

struct UI_data{
    UI_data(bool isLight_ = false, const Vec3f& position_ = {0.f, 0.f, 0.f});
    uint32_t amount = 0;

    float shift_x = 0, shift_y = 0, shift_z = 0;
    float rot_x = 0, rot_y = 0, rot_z = 0;
    float scale_x = 1.f, scale_y = 1.f, scale_z = 1.f;

    bool texture_flag = false, color_flag = true;

    Vec3f color = {0.5, 0.5, 0.5};

    float specular = 0.f, reflective = 0.f, refractive = 0.f;
    float intensity = intensityLight;
    QImage img;

    bool isLight = false;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    SceneManager manager;
    ~MainWindow();

public slots:

    void checkThread();

private slots:
    void fetch(QModelIndex index);

    void on_render_button_clicked();

    void on_rotate_x_spin_valueChanged(double arg1);

    void on_rotate_y_spin_valueChanged(double arg1);

    void on_rotate_z_spin_valueChanged(double arg1);

    void on_offset_x_spin_valueChanged(double arg1);

    void on_offset_y_spin_valueChanged(double arg1);

    void on_offset_z_spin_valueChanged(double arg1);

    void on_scale_z_spin_valueChanged(double arg1);

    void on_scale_x_spin_valueChanged(double arg1);

    void on_scale_y_spin_valueChanged(double arg1);


    void on_add_object_button_clicked();

    void on_delete_object_button_clicked();


    void fill_data(const UI_data& data);
    void save_data(UI_data& data);
    void showButtons(bool isLight);
    void hideButtons(bool isLight);
    void changeHidence(bool, bool);
    void lockSignals(bool signal);

    void on_color_add_button_clicked();

    void on_texture_flag_clicked();

    void on_color_flag_clicked();

    void on_add_texture_button_clicked();

    void on_add_light_button_clicked();

    void on_glitter_spin_valueChanged(double arg1);

    void on_reflection_spin_valueChanged(double arg1);

    void on_transparency_spin_valueChanged(double arg1);

    void on_intensity_spin_valueChanged(double arg1);

    void on_ambient_spin_valueChanged(double arg1);

    void on_lightColor_clicked();

    void on_lightColor_2_clicked();

    void on_ambient_spin_2_valueChanged(double arg1);

private:

    void disableAll(bool flag);

private:
    Ui::MainWindow *ui;
    QStringListModel *model;
    std::map<QString, uint32_t> text_uid;
    std::map<QString, UI_data> name_data;
    QString prev_selected = "";

    ThreadVector* threads;
    int th_amount = 0;
    QMutex mutex;
    bool isLocked = false;
};

class Filter: public QObject{
    Q_OBJECT
public:
    Filter(std::function<void (trans_type, float)> f_): f{f_}{}
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
private:
    std::function<void (trans_type, float)> f;
};


#endif // MAINWINDOW_H
