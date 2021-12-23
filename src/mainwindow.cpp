#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

UI_data::UI_data(bool isLight_, const Vec3f& p){
    img.load("/home/prianechka/Рабочий стол/course/proj/code/mig/course/textures/bricks.jpg");
    isLight = isLight_;
    shift_x = p.x, shift_y = p.y, shift_z = p.z;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int width = ui->canvas->width();
    int height = ui->canvas->height();

    ui->canvas->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->canvas->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->canvas->setScene(new QGraphicsScene(0, 0, width, height));

    auto f = [&](trans_type t, float dist)
    {
        manager.moveCamera(t, dist);
    };

    auto filter = new Filter(f);
    ui->canvas->installEventFilter(filter);

    manager = SceneManager(width, height, Qt::black, ui->canvas->scene());

    const QStringList textures =
    {
        "Куб",
        "Сфера",
        "Конус",
        "Цилиндр"
    };

    const QStringList lights = {"Точечный источник"};

    ui->objects_list->addItems(textures);

    auto stringList = new QStringList();
    model = new QStringListModel(*stringList);

    ui->scene_list->setModel(model);
    ui->scene_list->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->scene_list, SIGNAL(clicked(QModelIndex)), this, SLOT(fetch(QModelIndex)));

    ui->color_preview->setScene(new QGraphicsScene);
    ui->color_view->setScene(new QGraphicsScene);
    ui->color_view_2->setScene(new QGraphicsScene);
    ui->texture_img->setScene(new QGraphicsScene(0, 0, ui->texture_img->width(), ui->texture_img->height()));
    ui->texture_img->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->texture_img->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->ambient_spin->setValue(ambInt);

    hideButtons(false);

    manager.init();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::lockSignals(bool flag){

    ui->offset_x_spin->blockSignals(flag);
    ui->offset_y_spin->blockSignals(flag);
    ui->offset_z_spin->blockSignals(flag);

    ui->rotate_x_spin->blockSignals(flag);
    ui->rotate_y_spin->blockSignals(flag);
    ui->rotate_z_spin->blockSignals(flag);

    ui->scale_x_spin->blockSignals(flag);
    ui->scale_y_spin->blockSignals(flag);
    ui->scale_z_spin->blockSignals(flag);

    ui->intensity_spin->blockSignals(flag);

    ui->glitter_spin->blockSignals(flag);
}

void MainWindow::fill_data(const UI_data& data){

    lockSignals(true);

    ui->offset_x_spin->setValue(data.shift_x);
    ui->offset_y_spin->setValue(data.shift_y);
    ui->offset_z_spin->setValue(data.shift_z);

    ui->rotate_x_spin->setValue(data.rot_x);
    ui->rotate_y_spin->setValue(data.rot_y);
    ui->rotate_z_spin->setValue(data.rot_z);

    ui->scale_x_spin->setValue(data.scale_x);
    ui->scale_y_spin->setValue(data.scale_y);
    ui->scale_z_spin->setValue(data.scale_z);

    ui->color_flag->setChecked(data.color_flag);
    ui->texture_flag->setChecked(data.texture_flag);

    ui->texture_img->scene()->addPixmap(QPixmap::fromImage(
                                            data.img.scaled(ui->texture_img->width(), ui->texture_img->height())
                                            ));

    ui->color_preview->scene()->setBackgroundBrush(
                QColor(data.color.x *255.f, data.color.y * 255.f, data.color.z * 255.f));

    // block and lock buttons for adding color and texture
    ui->color_add_button->setDisabled(data.texture_flag);
    ui->add_texture_button->setDisabled(data.color_flag);

    ui->glitter_spin->setValue(data.specular);
    ui->transparency_spin->setValue(data.refractive);
    ui->reflection_spin->setValue(data.reflective);

    ui->intensity_spin->setValue(data.intensity);

    lockSignals(false);

}

void MainWindow::save_data(UI_data& data){

    data.shift_x = ui->offset_x_spin->value();
    data.shift_y = ui->offset_y_spin->value();
    data.shift_z = ui->offset_z_spin->value();

    data.rot_x = ui->rotate_x_spin->value();
    data.rot_y = ui->rotate_y_spin->value();
    data.rot_z = ui->rotate_z_spin->value();

    data.scale_x = ui->scale_x_spin->value();
    data.scale_y = ui->scale_y_spin->value();
    data.scale_z = ui->scale_z_spin->value();

    data.texture_flag = ui->texture_flag->isChecked();
    data.color_flag = ui->color_flag->isChecked();

    auto color = ui->color_preview->scene()->backgroundBrush().color();

    data.color = Vec3f(color.redF(), color.greenF(), color.blueF());

    data.specular = ui->glitter_spin->value();
    data.reflective = ui->reflection_spin->value();
    data.refractive = ui->transparency_spin->value();

    data.intensity = ui->intensity_spin->value();

}



void MainWindow::changeHidence(bool flag, bool isLight)
{
    ui->offset_x_spin->setHidden(flag);
    ui->offset_y_spin->setHidden(flag);
    ui->offset_z_spin->setHidden(flag);

    ui->rotate_x_spin->setHidden(flag);
    ui->rotate_y_spin->setHidden(flag);
    ui->rotate_z_spin->setHidden(flag);

    ui->intensity_label->setHidden(flag);
    ui->intensity_spin->setHidden(flag);


    ui->x_label->setHidden(flag);
    ui->y_label->setHidden(flag);
    ui->z_label->setHidden(flag);

    ui->offset_label->setHidden(flag);
    ui->rotate_label->setHidden(flag);

    if (isLight){
        ui->intensity_label->setHidden(flag);
        ui->intensity_spin->setHidden(flag);
        flag = !flag;
    }else{
        ui->intensity_label->setHidden(true);
        ui->intensity_spin->setHidden(true);
    }

    ui->scale_x_spin->setHidden(flag);
    ui->scale_y_spin->setHidden(flag);
    ui->scale_z_spin->setHidden(flag);

    ui->scale_label->setHidden(flag);

    ui->texture_img->setHidden(flag);
    ui->texture_flag->setHidden(flag);
    ui->add_texture_button->setHidden(flag);

    ui->color_flag->setHidden(flag);
    ui->color_add_button->setHidden(flag);
    ui->color_preview->setHidden(flag);

    ui->reflection_spin->setHidden(flag);
    ui->glitter_spin->setHidden(flag);
    ui->transparency_spin->setHidden(flag);

    ui->glitter_label->setHidden(flag);
    ui->transparency_label->setHidden(flag);
    ui->reflection_label->setHidden(flag);

    ui->add_light_button->setHidden(flag);
}

void MainWindow::showButtons(bool isLight)
{
    changeHidence(false, isLight);
}

void MainWindow::hideButtons(bool isLight)
{
    changeHidence(true, isLight);
}

void MainWindow::fetch(QModelIndex index)
{
    if (!index.isValid()) return;
    auto text = model->index(index.row()).data(Qt::DisplayRole).toString();
    showButtons(name_data.at(text).isLight);
    if (prev_selected != "")
        save_data(name_data.at(prev_selected));
    manager.setCurrentModel(text_uid.at(text));
    if (name_data.count(text))
        fill_data(name_data.at(text));
    prev_selected = text;
}

void MainWindow::disableAll(bool flag){

    ui->ambient_spin->setEnabled(flag);
    ui->glitter_spin->setEnabled(flag);
    ui->reflection_spin->setEnabled(flag);
    ui->transparency_spin->setEnabled(flag);

    ui->scale_x_spin->setEnabled(flag);
    ui->scale_y_spin->setEnabled(flag);
    ui->scale_z_spin->setEnabled(flag);

    ui->rotate_x_spin->setEnabled(flag);
    ui->rotate_y_spin->setEnabled(flag);
    ui->rotate_z_spin->setEnabled(flag);

    ui->offset_x_spin->setEnabled(flag);
    ui->offset_y_spin->setEnabled(flag);
    ui->offset_z_spin->setEnabled(flag);

    ui->color_flag->setEnabled(flag);
    ui->texture_flag->setEnabled(flag);
    ui->add_texture_button->setEnabled(flag);
    ui->color_add_button->setEnabled(flag);

    ui->scene_list->setEnabled(flag);
    ui->objects_list->setEnabled(flag);

    ui->intensity_spin->setEnabled(flag);
    ui->ambient_spin->setEnabled(flag);

    ui->add_light_button->setEnabled(flag);
    ui->add_object_button->setEnabled(flag);

    ui->delete_object_button->setEnabled(flag);

    ui->ambient_spin_2->setEnabled(flag);
    ui->lightColor->setEnabled(flag);
    ui->lightColor_2->setEnabled(flag);

}

auto start = std::chrono::high_resolution_clock::now();;

void MainWindow::on_render_button_clicked()
{
    start = std::chrono::high_resolution_clock::now();
    if (isLocked)
    {
        disableAll(true);
        isLocked = false;
        manager.render();
        return;
    }
    threads = manager.trace();
    if (threads)
    {
        for (auto& th: *threads)
        {
            QObject::connect(th, SIGNAL(finished()), this, SLOT(checkThread()));
            th->start();
        }
        th_amount = threads->size();
        disableAll(false);
        isLocked = true;
        ui->render_button->setEnabled(false);
    }
}

void MainWindow::checkThread()
{
    QMutexLocker ml(&mutex);
    if (--th_amount == 0)
    {
        manager.showTracedResult();
        ui->render_button->setEnabled(true);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    qDebug() << duration.count();
}

void MainWindow::on_rotate_x_spin_valueChanged(double arg1)
{
    manager.rotate(rot_x, arg1);
}

void MainWindow::on_rotate_y_spin_valueChanged(double arg1)
{
    manager.rotate(rot_y, -arg1);
}

void MainWindow::on_rotate_z_spin_valueChanged(double arg1)
{
    manager.rotate(rot_z, arg1);
}

void MainWindow::on_offset_x_spin_valueChanged(double arg1)
{
    manager.shift(shift_x, arg1);
}

void MainWindow::on_offset_y_spin_valueChanged(double arg1)
{
    manager.shift(shift_y, arg1);
}

void MainWindow::on_offset_z_spin_valueChanged(double arg1)
{
    manager.shift(shift_z, arg1);
}

void MainWindow::on_scale_z_spin_valueChanged(double arg1)
{
    manager.scale(scale_z, arg1);
}

void MainWindow::on_scale_x_spin_valueChanged(double arg1)
{
    manager.scale(scale_x, arg1);
}

void MainWindow::on_scale_y_spin_valueChanged(double arg1)
{
    manager.scale(scale_y, arg1);
}

float move_dist = 0.5;
int rot_angle = 10;

bool Filter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->nativeScanCode())
        {
            case 25:
                f(shift_z, move_dist);
                break;
            case 40:
                f(shift_x, -move_dist);
                break;
            case 38:
                f(shift_x, move_dist);
                break;
            case 39:
                f(shift_z, -move_dist);
                break;
            case 112:
                f(rot_x, rot_angle);
                break;
            case 117:
                f(rot_x, -rot_angle);
                break;
            case 110:
                f(rot_y, -rot_angle);
                break;
            case 115:
                f(rot_y, rot_angle);
                break;
            case 24:
                f(up_y, move_dist);
                break;
            case 26:
                f(down_y, -move_dist);
                break;
        }
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::on_add_object_button_clicked()
{
    auto text = ui->objects_list->currentText();
    auto updated_text = text;
    if (!name_data.count(text))
        name_data.insert({text, UI_data{}});
    else{
        auto val = ++name_data.at(text).amount;
        updated_text += QString("%1").arg(val);
        name_data.insert({updated_text, UI_data{}});
    }
    uint32_t uid = 0;
    manager.uploadModel(text.toStdString(), uid);

    if (uid){
        model->insertRow(model->rowCount());
        QModelIndex index = model->index(model->rowCount()-1);
        model->setData(index, updated_text);
        text_uid.insert({updated_text, uid});
    }
    return;

}

void MainWindow::on_delete_object_button_clicked()
{
    auto index = ui->scene_list->currentIndex();
    if (!index.isValid()) return;
    model->removeRow(index.row());
    prev_selected = "";
    hideButtons(false);
    manager.removeModel();
}

void MainWindow::on_color_add_button_clicked()
{
    QColor color = QColorDialog::getColor(Qt::gray, this, QStringLiteral("Выберите цвет модели"));
    if (!color.isValid()) return;
    auto cred = QString::number(color.red()).toFloat();
    auto cgreen = QString::number(color.green()).toFloat();
    auto cblue = QString::number(color.blue()).toFloat();

    ui->color_preview->scene()->setBackgroundBrush(QColor(cred, cgreen, cblue));

    auto color_f = Vec3f(cred / 255.f, cgreen / 255.f, cblue / 255.f);
    manager.setColor(color_f);
    name_data.at(prev_selected).color = color_f;

}

void MainWindow::on_texture_flag_clicked()
{
    ui->add_texture_button->setDisabled(false);
    auto img = name_data.at(prev_selected).img;
    ui->texture_img->scene()->addPixmap(QPixmap::fromImage(
                                            img.scaled(ui->texture_img->width(), ui->texture_img->height())
                                            ));
    ui->color_add_button->setDisabled(true);
    manager.setFlagTexture(true, Vec3f{1.f, 1.f, 1.f});
}

void MainWindow::on_color_flag_clicked()
{
    ui->color_add_button->setDisabled(false);
    ui->add_texture_button->setDisabled(true);
    manager.setFlagTexture(false, name_data.at(prev_selected).color);
}

void MainWindow::on_add_texture_button_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите текстуру", "/home/prianechka/Рабочий стол/course/proj/code/mig/course/textures/",
                                                    "JPG (*.jpg);;PNG (*.png)");
    QImage img;
    if (!img.load(fileName))
        return;

    ui->texture_img->scene()->addPixmap(QPixmap::fromImage(
                                            img.scaled(ui->texture_img->width(), ui->texture_img->height())
                                            ));


    name_data.at(prev_selected).img = img;
    manager.setTexture(img);
}


Vec3f posLight(const QString& name)
{
    if (name == "Точечный источник")
        return pointLightPosition;
    return Vec3f{0.f, 0.f, 0.f};
}

void MainWindow::on_add_light_button_clicked()
{
    auto text = QString("Точечный источник");

    auto pos = posLight(text);

    auto updated_text = text;
    if (!name_data.count(text))
        name_data.insert({text, UI_data{true, pos}});
    else{
        auto val = ++name_data.at(text).amount;
        updated_text += QString("%1").arg(val);
        name_data.insert({updated_text, UI_data{true, pos}});
    }
    uint32_t uid = 0;

    QColor color = QColorDialog::getColor(Qt::gray, this, QStringLiteral("Выберите цвет источника цвета"));
    if (!color.isValid()) return;
    auto cred = QString::number(color.red()).toFloat();
    auto cgreen = QString::number(color.green()).toFloat();
    auto cblue = QString::number(color.blue()).toFloat();

    auto color_f = Vec3f(cred / 255.f, cgreen / 255.f, cblue / 255.f);
    manager.uploadLight(text.toStdString(), uid, color_f);

    if (uid){
        ui->color_view->scene()->setBackgroundBrush(QColor(cred, cgreen, cblue));
        model->insertRow(model->rowCount());
        QModelIndex index = model->index(model->rowCount()-1);
        model->setData(index, updated_text);
        text_uid.insert({updated_text, uid});
    }
    return;
}

void MainWindow::on_glitter_spin_valueChanged(double arg1)
{
    manager.setSpecular(arg1);
}

void MainWindow::on_reflection_spin_valueChanged(double arg1)
{
    manager.setReflective(arg1);
}

void MainWindow::on_transparency_spin_valueChanged(double arg1)
{
    manager.setRefraction(arg1);
}

void MainWindow::on_intensity_spin_valueChanged(double arg1)
{
    manager.setIntensity(arg1);
}

void MainWindow::on_ambient_spin_valueChanged(double arg1)
{
    manager.setAmbIntensity(arg1);
}

void MainWindow::on_lightColor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::gray, this, QStringLiteral("Выберите цвет источника цвета"));
    if (!color.isValid()) return;
    auto cred = QString::number(color.red()).toFloat();
    auto cgreen = QString::number(color.green()).toFloat();
    auto cblue = QString::number(color.blue()).toFloat();

    ui->color_view->scene()->setBackgroundBrush(QColor(cred, cgreen, cblue));

    auto color_f = Vec3f(cred / 255.f, cgreen / 255.f, cblue / 255.f);
    manager.setLightColor(color_f);
}


void MainWindow::on_lightColor_2_clicked()
{
    QColor color = QColorDialog::getColor(Qt::gray, this, QStringLiteral("Выберите цвет источника цвета"));
    if (!color.isValid()) return;
    auto cred = QString::number(color.red()).toFloat();
    auto cgreen = QString::number(color.green()).toFloat();
    auto cblue = QString::number(color.blue()).toFloat();

    ui->color_view_2->scene()->setBackgroundBrush(QColor(cred, cgreen, cblue));

    auto color_f = Vec3f(cred / 255.f, cgreen / 255.f, cblue / 255.f);
    manager.setAmbientLightColor(color_f);
}


void MainWindow::on_ambient_spin_2_valueChanged(double arg1)
{
    manager.setCoefRefract(arg1);
}

