#include "grscene.h"

void GrScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    previousPoint = event->scenePos();
    //qDebug() << "Point mouse : " << event->scenePos().x()/scale/ratio_x << ' ' << event->scenePos().y()/scale/ratio_y;
}

void GrScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //// Update on the previous coordinate data
    previousPoint = event->scenePos();
    //qDebug() << "Point mouse : " << event->scenePos().x() << ' ' << event->scenePos().y();
}
