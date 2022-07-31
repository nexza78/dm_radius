#include "grscene.h"

void GrScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //// Update on the previous coordinate data
    previousPoint = event->scenePos();
    x->setText(QString::fromStdString(std::to_string(event->scenePos().x()/*/scale/ratio_x*/)));
    y->setText(QString::fromStdString(std::to_string(event->scenePos().y()/*/scale/ratio_y*/)));
}
