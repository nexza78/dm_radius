#ifndef GRSCENE_H
#define GRSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>

class GrScene: public QGraphicsScene
{
public:
    GrScene(float ratio_x, float ratio_y, float scale): ratio_x(ratio_x), ratio_y(ratio_y), scale(scale) {};
private:
    QPointF     previousPoint;
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    float ratio_x;
    float ratio_y;
    float scale;
};

#endif // GRSCENE_H
