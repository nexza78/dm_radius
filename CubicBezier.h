#ifndef CUBICBEZIER_H
#define CUBICBEZIER_H

#include <QMainWindow>

#include <cmath>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <set>
#include <cmath>


class CubicBezier: public QMainWindow{
public:
    CubicBezier(std::pair<float, float> P0, std::pair<float, float> P1, std::pair<float, float> P2, std::pair<float, float> P3, float scale, float ratio_x, float ratio_y):P0(P0), P1(P1), P2(P2), P3(P3), scale(scale), ratio_x(ratio_x), ratio_y(ratio_y){};

    std::pair<float, float> P0;
    std::pair<float, float> P1;
    std::pair<float, float> P2;
    std::pair<float, float> P3;

    /// @brief первая производная
    float xt(float t);
    /// @brief первая производная
    float xtt(float t);
    /// @brief первая производная
    float yt(float t);
    /// @brief первая производная
    float ytt(float t);
    /// @brief Получить координату икс точки при данном параметре t
    float coordinateX(float t);
    /// @brief Получить координату игрек точки при данном параметре t
    float coordinateY(float t);
    bool curve_to_line = false;
    float scale;
    float ratio_x;
    float ratio_y;
    float distance(std::pair<float, float> P0, std::pair<float, float> P1);
    void start_curve(QGraphicsScene *&scene, float t0);
    void end_curve(QGraphicsScene *&scene, float t0);
    void t_intersec_inputX(QGraphicsScene *&scene, std::vector<float> &intersections, float x, float epsilon);
    void radiuses(QGraphicsScene *&scene, std::set<std::vector<std::pair<float, float>>> &rad_info, float &time, std::vector<std::vector<std::pair<float, float>>> & CtoL);
    float rad_to_sec(float rad);
    void degrees(std::pair<float, float> P0, std::pair<float, float> P1, std::pair<float, float> P2, std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> path);
};

#endif // CUBICBEZIER_H
