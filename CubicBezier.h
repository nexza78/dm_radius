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
    CubicBezier(std::pair<float, float> P0, std::pair<float, float> P1, std::pair<float, float> P2, std::pair<float, float> P3):P0(P0), P1(P1), P2(P2), P3(P3){};

    std::pair<float, float> P0;
    std::pair<float, float> P1;
    std::pair<float, float> P2;
    std::pair<float, float> P3;

    /// @brief подозреваю что связано с первой производной
    float xt(float t);
    /// @brief подозреваю что связано с первой производной
    float xtt(float t);
    /// @brief подозреваю что связано с первой производной
    float yt(float t);
    /// @brief подозреваю что связано с первой производной
    float ytt(float t);
    /// @brief Получить координату икс точки при данном параметре t
    float coordinateX(float t);
    /// @brief Получить координату игрек точки при данном параметре t
    float coordinateY(float t);    
    float distance(std::pair<float, float> P0, std::pair<float, float> P1);
    void t_intersec_inputX(QGraphicsScene *&scene, std::vector<float> &intersections, float x, float scale, float ratio, float ratio_y, float epsilon);
    void radiuses(QGraphicsScene *&scene, std::set<std::vector<std::pair<float, float>>> &rad_info, float scale, float ratio_x, float ratio_y, float &time);
    float rad_to_sec(float rad);
    void degrees(std::pair<float, float> P0, std::pair<float, float> P1, std::pair<float, float> P2, std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> path);
};

#endif // CUBICBEZIER_H
