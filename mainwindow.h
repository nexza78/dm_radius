#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <cmath>
#include <QDomDocument>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>

#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QMessageBox>
#include <QStringRef>
#include <QSvgGenerator>
#include <QSvgRenderer>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QStringList>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_review_clicked();

    void on_pushButton_open_clicked();

    void on_pushButton_analyze_clicked();

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    bool path_was_chosen = false;
    QGraphicsScene *scene;
    int sharp_ang_number = 0;
    int obtuse_ang_number = 0;
    int radius_number = 0;
    static QList<QGraphicsRectItem *> getElements(const QString filename);
    static QRectF getSizes(const QString filename);
    static float get_point(QString points, int &j0);
    static std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> split(QString points);
    static std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> toUpperCase(std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> points, bool z);
    static std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> merge(std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> points);
    void angles(std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> points);
    void radiuses(std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> points);
    void rad_cubic(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3);
    void degrees(std::pair<float, float> P0, std::pair<float, float> P1, std::pair<float, float> P2);
    float ratio_x = 1;
    float ratio_y = 1;
    float scale = 1;
    QGraphicsSvgItem *main_item = nullptr;
};
#endif // MAINWINDOW_H
