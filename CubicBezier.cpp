#include<CubicBezier.h>
//RATIO SCALE ARE DELETED!!!!!!!!!!!!!!!!
float CubicBezier::coordinateX(float t){
    return ((1-t)*(1-t)*(1-t)*P0.first + 3*(1-t)*(1-t)*t*P1.first + 3*(1-t)*t*t*P2.first + t*t*t*P3.first);
}

float CubicBezier::coordinateY(float t){
    return ((1-t)*(1-t)*(1-t)*P0.second + 3*(1-t)*(1-t)*t*P1.second + 3*(1-t)*t*t*P2.second + t*t*t*P3.second);
}

float CubicBezier::xt(float t, float scale, float ratio){
    return (-3*(1-t)*(1-t)*P0.first + (9*t*t-12*t+3)*P1.first + (6*t - 9*t*t)*P2.first + 3*t*t*P3.first)*ratio*scale;
}

float CubicBezier::yt(float t, float scale, float ratio){
    return (-3*(1-t)*(1-t)*P0.second + (9*t*t-12*t+3)*P1.second + (6*t - 9*t*t)*P2.second + 3*t*t*P3.second)*ratio*scale;
}

float CubicBezier::xtt(float t, float scale, float ratio){
    return (6*(1-t)*P0.first + (18*t - 12)*P1.first + (6-18*t)*P2.first + 6*t*P3.first)*ratio*scale;
}

float CubicBezier::ytt(float t, float scale, float ratio){
    return (6*(1-t)*P0.second + (18*t - 12)*P1.second + (6-18*t)*P2.second + 6*t*P3.second)*ratio*scale;
}

void CubicBezier::t_intersec_inputX(QGraphicsScene *&scene, std::vector<float> &intersections, float x, float scale, float ratio, float ratio_y, float epsilon){
    qDebug() << "CURVE";
    float t0 = 0;
    float t = 0;
    float old = this->coordinateX(t0);
    float newVal = this->coordinateX(t);

    //QGraphicsEllipseItem* circle6 = new QGraphicsEllipseItem();
    //circle6->setRect(coordinateX(0)*ratio*scale - 5, coordinateY(0)*ratio_y*scale - 5, 10, 10);
    //circle6->setBrush(QBrush(Qt::black));
    //circle6->setPen(QPen(QBrush(Qt::black),10));
    //scene->addItem(circle6);

    while(t <= 1.1){
        //QGraphicsEllipseItem* circle2 = new QGraphicsEllipseItem();
        //circle2->setRect(coordinateX(t0)*ratio*scale - 5, coordinateY(t0)*ratio_y*scale- 5, 10, 10);
        //circle2->setBrush(QBrush(Qt::white));
        //circle2->setPen(QPen(QBrush(Qt::magenta),3));
        //scene->addItem(circle2);



        if((int)x == 15822){
            qDebug() << "old, newVal, t, t0 " << old << ' ' << newVal << ' '<< t << ' ' << t0;

            QGraphicsEllipseItem* circle3 = new QGraphicsEllipseItem();
            circle3->setRect(coordinateX(t)*scale*ratio - 1, coordinateY(t)*scale*ratio_y - 1, 2, 2);
            circle3->setBrush(QBrush(Qt::yellow));
            circle3->setPen(QPen(QBrush(Qt::magenta),1));
            scene->addItem(circle3);

            QGraphicsTextItem* t2 = new QGraphicsTextItem();
            t2->setPlainText(QString::fromStdString(std::to_string(t)));
            t2->setX(coordinateX(t)*ratio*scale);
            t2->setY(coordinateY(t)*ratio_y*scale);
            scene->addItem(t2);
        }
        if(x == old){
            intersections.push_back(coordinateY(t));
            qDebug() << t << "HUUUUUU";
        }
        else if(x == newVal){
            intersections.push_back(coordinateY(t));
            qDebug() << t << "ILI NET";
        }
        else if(std::min(old, newVal) < x && x < std::max(old, newVal)){
            qDebug() << t << "BETWEEN";
            float t0_here = t0;
            float t_here = t;
            if(std::min(old, newVal) != old){
                t0_here = t;
                t_here = t0;
            }
            qDebug() << "t/t0/x" << t << ' ' << t0 << ' '<< x;
            float answer = coordinateX((t_here + t0_here)/2.0);
            while(std::abs(x - answer) > epsilon){
                qDebug() << "x/answer/epsilon" << x << answer << epsilon;
                if(x > answer){
                    t0_here = (t_here + t0_here)/2.0;
                }
                else{
                    t_here = (t_here + t0_here)/2.0;
                }
                //QGraphicsEllipseItem* circle4 = new QGraphicsEllipseItem();
                //circle4->setRect(coordinateX(t0_here, scale, ratio) - 5, coordinateY(t0, scale, ratio_y) - 5, 10, 10);
                //circle4->setBrush(QBrush(Qt::darkCyan));
                //circle4->setPen(QPen(QBrush(Qt::black),2));
                //scene->addItem(circle4);

                //QGraphicsEllipseItem* circle5 = new QGraphicsEllipseItem();
                //circle5->setRect(coordinateX(t_here)*scale*ratio - 5, coordinateY(t)*scale*ratio_y - 5, 10, 10);
                //circle5->setBrush(QBrush(Qt::magenta));
                //circle5->setPen(QPen(QBrush(Qt::black),2));
                //scene->addItem(circle5);
                answer = coordinateX((t_here + t0_here)/2.0);
            }
            intersections.push_back(coordinateY((t_here + t0_here)/2.0));
        }
        t0 = t;
        t += 0.1;
        old = this->coordinateX(t0);
        newVal = this->coordinateX(t);
    }
    //QGraphicsEllipseItem* circle3 = new QGraphicsEllipseItem();
    //circle3->setRect(coordinateX(1)*scale*ratio - 5, coordinateY(1)*scale*ratio_y - 5, 10, 10);
    //circle3->setBrush(QBrush(Qt::magenta));
    //circle3->setPen(QPen(QBrush(Qt::green),5));
    //scene->addItem(circle3);
    return;
}

void CubicBezier::radiuses(QGraphicsScene *&scene, float scale){
    float minR = INFINITY;
    float cx = 0;
    float cy = 0;

    for(float t = 0.1; t < 1; t+= 0.1){
        float denom = ytt(scale, t)*xt(scale, t) - xtt(scale, t)*yt(scale, t);
        if(denom != 0){
            float numerator = xt(scale, t)*xt(scale, t) + yt(scale, t)*yt(scale, t);
             float R = numerator*std::sqrt(numerator)/denom;
             //real size R
             if(R < minR){
                 minR = R;
                 //cx = coordinateX(t)*scale*ratio_x - yt(scale, t)*numerator/denom;
                 //cy = coordinateY(t)*scale*ratio_y + xt(scale, t)*numerator/denom;
                 QGraphicsEllipseItem* circle2 = new QGraphicsEllipseItem();
                 //circle2->setRect(coordinateX(scale, t) - 2, coordinateY(scale, t) - 2, 4, 4);
                 circle2->setBrush(QBrush(Qt::yellow));
                 circle2->setPen(QPen(QBrush(Qt::black),2));
                 scene->addItem(circle2);
             }
        }
    }
    if(minR != INFINITY){
        QGraphicsEllipseItem* circle3 = new QGraphicsEllipseItem();
        circle3->setRect(cx - minR, cy - minR, minR*2, minR*2);
        circle3->setBrush(QBrush(Qt::transparent));
        circle3->setPen(QPen(QBrush(Qt::green),2));
        scene->addItem(circle3);
    }
}
