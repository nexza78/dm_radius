#ifndef CUBICBEZIER_H
#define CUBICBEZIER_H

#include <QMainWindow>

#include <cmath>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsRectItem>


class CubicBezier: public QMainWindow{
public:
    CubicBezier(std::pair<float, float> P0, std::pair<float, float> P1, std::pair<float, float> P2, std::pair<float, float> P3):P0(P0), P1(P1), P2(P2), P3(P3){};

    std::pair<float, float> P0;
    std::pair<float, float> P1;
    std::pair<float, float> P2;
    std::pair<float, float> P3;

    /// @brief подозреваю что связано с первой производной
    float xt(float t, float scale, float ratio = 1);
    /// @brief подозреваю что связано со второй производной
    float xtt(float t, float scale, float ratio = 1);
    /// @brief подозреваю что связано с первой производной
    float yt(float t, float scale, float ratio = 1);
    /// @brief подозреваю что связано со второй производной
    float ytt(float t, float scale, float ratio = 1);
    /// @brief Получить координату икс точки при данном параметре t
    float coordinateX(float t, float scale, float ratio = 1);
    /// @brief Получить координату игрек точки при данном параметре t
    float coordinateY(float t, float scale, float ratio = 1);
    float t_intersec_inputX(float x, float scale, float ratio, float epsilon);
    void radiuses(QGraphicsScene *&scene, float scale);
};

#endif // CUBICBEZIER_H
