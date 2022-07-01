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

float CubicBezier::t_intersec_inputX(float x, float scale, float ratio, float epsilon){
    float t0 = 0;
    float t = 0;
    float old = this->coordinateX(t0, scale, ratio)/(ratio * scale);
    float newVal = this->coordinateX(t, scale, ratio)/(ratio * scale);
    while(t <= 1){
        if(x == old){
            return old;
        }
        else if(x == newVal){
            return newVal;
        }
        else if(x > std::min(old, newVal) && x < std::max(old, newVal)){
            if(std::min(old, newVal) != old){
                float temp = t0;
                t0 = t;
                t = temp;
            }
            break;
        }
        t0 = t;
        t += 0.1;
        old = this->coordinateX(t0, scale, ratio)/(ratio * scale);
        newVal = this->coordinateX(t, scale, ratio)/(ratio * scale);
    }
    if(t > 1){
        return -1;
    }
    else{
        float answer = coordinateX((t + t0)/2.0, scale, ratio)/(ratio * scale);
        while(std::abs(x - answer) > epsilon){
            if(x > answer){
                t0 = (t + t0)/2.0;
            }
            else{
                t = (t + t0)/2.0;
            }
        }
        return (t + t0)/2.0;
    }
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
