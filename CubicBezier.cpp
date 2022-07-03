#include<CubicBezier.h>

float CubicBezier::coordinateX(float t, float scale, float ratio){
    return ((1-t)*(1-t)*(1-t)*P0.first + 3*t*(1-t)*(1-t)*P1.first + 3*t*t*(1-t)*P2.first + t*t*t*P3.first)*ratio*scale;
}

float CubicBezier::coordinateY(float t, float scale, float ratio){
    return ((1-t)*(1-t)*(1-t)*P0.second + 3*t*(1-t)*(1-t)*P1.second + 3*t*t*(1-t)*P2.second + t*t*t*P3.second)*ratio*scale;
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

void CubicBezier::t_intersec_inputX(std::vector<float> &intersections, float x, float scale, float ratio, float ratio_y, float epsilon){
    float t0 = 0;
    float t = 0;
    float old = this->coordinateX(t0, scale, ratio)/(ratio * scale);
    float newVal = this->coordinateX(t, scale, ratio)/(ratio * scale);
    while(t <= 1){
        qDebug() << t << "PIDORAS";
        if(x == old){
            intersections.push_back(coordinateY(t, scale, ratio_y)/(ratio_y * scale));
            qDebug() << t << "HUETA";
        }
        else if(x == newVal){
            intersections.push_back(coordinateY(t, scale, ratio_y)/(ratio_y * scale));
            qDebug() << t << "ILI NET";
        }
        else if(x > std::min(old, newVal) && x < std::max(old, newVal)){
            qDebug() << t << "PIDORASINA";

            float t0_here = t0;
            float t_here = t;
            if(std::min(old, newVal) != old){
                t0_here = t;
                t_here = t0;
            }
            qDebug() << t << "VOT V CHEM VOPROS" << x;
            float answer = coordinateX((t_here + t0_here)/2.0, scale, ratio)/(ratio * scale);
            while(std::abs(x - answer) > epsilon){
                if(x > answer){
                    t0_here = (t_here + t0_here)/2.0;
                }
                else{
                    t_here = (t_here + t0_here)/2.0;
                }
                answer = coordinateX((t_here + t0_here)/2.0, scale, ratio)/(ratio * scale);
            }
            intersections.push_back(coordinateY((t_here + t0_here)/2.0, scale, ratio_y)/(ratio_y * scale));
        }
        qDebug() << t << "PIDOR";
        t0 = t;
        t += 0.1;
        old = this->coordinateX(t0, scale, ratio)/(ratio * scale);
        newVal = this->coordinateX(t, scale, ratio)/(ratio * scale);

        qDebug() << t << "IDI SVOEY DOROGOY STALKER";
    }
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
                 cx = coordinateX(scale, t) - yt(scale, t)*numerator/denom;
                 cy = coordinateY(scale, t) + xt(scale, t)*numerator/denom;
                 QGraphicsEllipseItem* circle2 = new QGraphicsEllipseItem();
                 circle2->setRect(coordinateX(scale, t) - 2, coordinateY(scale, t) - 2, 4, 4);
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
