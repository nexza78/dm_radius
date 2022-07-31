#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <cmath>
#include <QDomDocument>
#include <QFile>
#include <QFileDialog>
#include <QtWidgets>

#include <QColor>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSvgItem>
#include <QGraphicsTextItem>
#include <QMouseEvent>

#include <QMessageBox>
#include <QStringRef>
#include <QSvgGenerator>
#include <QSvgRenderer>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QStringList>

#include <vector>
#include <set>

#include "CubicBezier.h"
#include "grscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::vector<std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>>> paths;

private slots:
    void on_pushButton_review_clicked();

    void on_pushButton_analyze_clicked();

    void on_spinBox_obtuse_angles_number_valueChanged(int arg1);

    void on_spinBox_sharp_angles_number_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    bool path_was_chosen = false;
    QGraphicsScene *scene;
    int sharp_ang_number = 0;
    int obtuse_ang_number = 0;
    int radius_number = 0;
    static QList<QGraphicsRectItem *> getElements(const QString filename);
    static QRectF getSizes(const QString filename);
    /// @brief
    /// Получить точку, следующую после индекса j0 строки points
    static float get_point(QString points, int &j0);
    const float pi = 3.14159265358979323846;
     /// @brief
     /// Разделение строки по частям. На выходе получается вектро из пар, первым элементом которых будет буква,
     /// обозначающая тип линии, а вторым элементом будет вектор из точек. Все точки имеют две координаты. Точки нельзя
     /// считать реальными значениями, т.к. обработка относительности точек еще не произошла
    static std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> split(QString points);
    std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> split_polygon(QString points);
    ///@brief Обработка относительности, замена H и V на L
    static std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> toUpperCase(std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> points);

    ///@brief Слияние одинаковых элементов (вроде не всех), обработка случая более одного элемента в М
    void merge();
    void check_dubles();
    float distance(std::pair<float, float> P0, std::pair<float, float> P1);
    void angles();
    bool inner_angle(std::pair<float, float> check_point);
    float intersec_line(std::pair<float, float> P0, std::pair<float, float> P1, float x);
    void radiuses();
    void rad_cubic(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3);
    void degrees(std::pair<float, float> P0, std::pair<float, float> P1, std::pair<float, float> P2, std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> path);
    void degrees_clon(std::pair<float, float> P0, std::pair<float, float> P1, std::pair<float, float> P2);
    void split2(std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> );
    std::set<std::vector<std::pair<float, float>>> rad_info;
    std::pair<float, float> intersec_2_lines(std::pair<float, float> P0, std::pair<float, float> P1, std::pair<float, float> P2, std::pair<float, float> P3);
    float curve_time = 0;
    float ratio_x = 1;
    float ratio_y = 1;
    float scale = 1;
    float perimetr = 0;
    void comments();
    void curve_to_ang();
    std::vector<std::pair<float, float>> angle_points;
    std::vector<std::vector<std::pair<float, float>>> curve_to_line_info;
    QGraphicsSvgItem *main_item = nullptr;
};
#endif // MAINWINDOW_H
