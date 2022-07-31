#ifndef GRSCENE_H
#define GRSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>
#include <QLabel>

class GrScene: public QGraphicsScene
{
public:
    GrScene(float ratio_x, float ratio_y, float scale, QLabel *& x, QLabel *& y): ratio_x(ratio_x), ratio_y(ratio_y), scale(scale), x(x), y(y) {};
private:
    QPointF     previousPoint;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    QLabel *& x;
    QLabel *& y;
    float ratio_x;
    float ratio_y;
    float scale;
};

#endif // GRSCENE_H
