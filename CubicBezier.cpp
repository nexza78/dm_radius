#include<CubicBezier.h>

float CubicBezier::coordinateX(float t){
    return ((1-t)*(1-t)*(1-t)*P0.first + 3*(1-t)*(1-t)*t*P1.first + 3*(1-t)*t*t*P2.first + t*t*t*P3.first);
}

float CubicBezier::coordinateY(float t){
    return ((1-t)*(1-t)*(1-t)*P0.second + 3*(1-t)*(1-t)*t*P1.second + 3*(1-t)*t*t*P2.second + t*t*t*P3.second);
}

float CubicBezier::xt(float t){
    return (-3*(1-t)*(1-t)*P0.first + (9*t*t-12*t+3)*P1.first + (6*t - 9*t*t)*P2.first + 3*t*t*P3.first);
}

float CubicBezier::yt(float t){
    return (-3*(1-t)*(1-t)*P0.second + (9*t*t-12*t+3)*P1.second + (6*t - 9*t*t)*P2.second + 3*t*t*P3.second);
}

float CubicBezier::xtt(float t){
    return (6*(1-t)*P0.first + (18*t - 12)*P1.first + (6-18*t)*P2.first + 6*t*P3.first);
}

float CubicBezier::ytt(float t){
    return (6*(1-t)*P0.second + (18*t - 12)*P1.second + (6-18*t)*P2.second + 6*t*P3.second);
}

float CubicBezier::distance(std::pair<float, float> P0, std::pair<float, float> P1){
    return std::sqrt((P0.first  - P1.first) *(P0.first  - P1.first) + (P0.second - P1.second)*(P0.second - P1.second));
}

void CubicBezier::t_intersec_inputX(QGraphicsScene *&scene, std::vector<float> &intersections, float x, float epsilon){
    //qDebug() << "CURVE";
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

        if(x == old){
            intersections.push_back(coordinateY(t));
            //qDebug() << t << "HUUUUUU";
        }
        else if(x == newVal){
            intersections.push_back(coordinateY(t));
            //qDebug() << t << "ILI NET";
        }
        else if(std::min(old, newVal) < x && x < std::max(old, newVal)){
            //qDebug() << t << "BETWEEN";
            float t0_here = t0;
            float t_here = t;
            if(std::min(old, newVal) != old){
                t0_here = t;
                t_here = t0;
            }
            //qDebug() << "t/t0/x" << t << ' ' << t0 << ' '<< x;
            float answer = coordinateX((t_here + t0_here)/2.0);
            while(std::abs(x - answer) > epsilon){
                //qDebug() << "x/answer/epsilon" << x << answer << epsilon;
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

float CubicBezier::rad_to_sec(float rad){
    return (19.30039 + (150.7729 - 19.30039)/(1 + std::pow((rad/23.01228),1.117323)))/45;
}

void CubicBezier::start_curve(QGraphicsScene *&scene, float t0){
    float r = 4/scale/ratio_x;
    float x1 = r/std::sqrt(1 + (xt(t0)/yt(t0))*(xt(t0)/yt(t0))) + coordinateX(t0);
    float x2 = - r/std::sqrt(1 + (xt(t0)/yt(t0))*(xt(t0)/yt(t0))) + coordinateX(t0);
    float y1 = -(xt(t0)/yt(t0))*(x1 - coordinateX(t0)) + coordinateY(t0);
    float y2 = -(xt(t0)/yt(t0))*(x2 - coordinateX(t0)) + coordinateY(t0);
    qDebug() << "y coord " << yt(t0) << ' ' << P0 << ' ' << P1 << ' ' << P2 << ' ' << P3;
    float x_half = coordinateX(t0 + 0.001);
    float y_half = coordinateY(t0 + 0.001);
    float partOf = distance(std::make_pair(coordinateX(t0), coordinateY(t0)), std::make_pair(x_half, y_half));

    float x = coordinateX(t0) - (coordinateX(t0) - x_half)*r/partOf;
    float y = coordinateY(t0) - (coordinateY(t0) - y_half)*r/partOf;
    if(yt(t0) == 0){
        scene->addLine(coordinateX(t0)*scale*ratio_x, coordinateY(t0)*scale*ratio_y + 4, coordinateX(t0)*scale*ratio_x, coordinateY(t0)*scale*ratio_y - 4, QPen(Qt::red,  1, Qt::SolidLine, Qt::RoundCap));
        scene->addLine(coordinateX(t0)*scale*ratio_x, coordinateY(t0)*scale*ratio_y + 4, coordinateX(t0)*scale*ratio_x + 4*((int)((P1.first - P0.first) < 0)*(-2) + 1),(coordinateY(t0))*scale*ratio_y, QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
        scene->addLine(coordinateX(t0)*scale*ratio_x, coordinateY(t0)*scale*ratio_y - 4, coordinateX(t0)*scale*ratio_x + 4*((int)((P1.first - P0.first) < 0)*(-2) + 1),(coordinateY(t0))*scale*ratio_y, QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
        return;
    }
    qDebug() << "x1 y1 x2 y2 x y " << x1 << ' '<< y1<< ' ' << x2 << ' '<< y2<< ' ' << x<< ' ' << y;
    scene->addLine(x1*scale*ratio_x,y1*scale*ratio_y, x2*scale*ratio_x,y2*scale*ratio_y, QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
    scene->addLine( x*scale*ratio_x, y*scale*ratio_y, x2*scale*ratio_x,y2*scale*ratio_y, QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
    scene->addLine(x1*scale*ratio_x,y1*scale*ratio_y,  x*scale*ratio_x, y*scale*ratio_y, QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
}

void CubicBezier::end_curve(QGraphicsScene *&scene, float t0){
    float r = 4/scale/ratio_x;
    float x1 = r/std::sqrt(1 + (xt(t0)/yt(t0))*(xt(t0)/yt(t0))) + coordinateX(t0);
    float x2 = - r/std::sqrt(1 + (xt(t0)/yt(t0))*(xt(t0)/yt(t0))) + coordinateX(t0);
    float y1 = -(xt(t0)/yt(t0))*(x1 - coordinateX(t0)) + coordinateY(t0);
    float y2 = -(xt(t0)/yt(t0))*(x2 - coordinateX(t0)) + coordinateY(t0);
    qDebug() << "y coord " << yt(t0) << ' ' << P0 << ' ' << P1 << ' ' << P2 << ' ' << P3;
    float x_half = coordinateX(t0 - 0.001);
    float y_half = coordinateY(t0 - 0.001);
    float partOf = distance(std::make_pair(coordinateX(t0), coordinateY(t0)), std::make_pair(x_half, y_half));

    float x = coordinateX(t0) - (coordinateX(t0) - x_half)*r/partOf;
    float y = coordinateY(t0) - (coordinateY(t0) - y_half)*r/partOf;
    if(yt(t0) == 0){
        scene->addLine(coordinateX(t0)*scale*ratio_x, coordinateY(t0)*scale*ratio_y + 4, coordinateX(t0)*scale*ratio_x, coordinateY(t0)*scale*ratio_y - 4, QPen(Qt::red,  1, Qt::SolidLine, Qt::RoundCap));
        scene->addLine(coordinateX(t0)*scale*ratio_x, coordinateY(t0)*scale*ratio_y + 4, coordinateX(t0)*scale*ratio_x + 4*((int)((P1.first - P0.first) > 0)*(-2) + 1),(coordinateY(t0))*scale*ratio_y, QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
        scene->addLine(coordinateX(t0)*scale*ratio_x, coordinateY(t0)*scale*ratio_y - 4, coordinateX(t0)*scale*ratio_x + 4*((int)((P1.first - P0.first) > 0)*(-2) + 1),(coordinateY(t0))*scale*ratio_y, QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
        return;
    }
    qDebug() << "x1 y1 x2 y2 x y " << x1 << ' '<< y1<< ' ' << x2 << ' '<< y2<< ' ' << x<< ' ' << y;
    scene->addLine(x1*scale*ratio_x,y1*scale*ratio_y, x2*scale*ratio_x,y2*scale*ratio_y, QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
    scene->addLine( x*scale*ratio_x, y*scale*ratio_y, x2*scale*ratio_x,y2*scale*ratio_y, QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
    scene->addLine(x1*scale*ratio_x,y1*scale*ratio_y,  x*scale*ratio_x, y*scale*ratio_y, QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
    }

void CubicBezier::radiuses(QGraphicsScene *&scene, std::set<std::vector<std::pair<float, float>>> &rad_info, float &time, std::vector<std::vector<std::pair<float, float>>> & CtoL){
    float cx = 0;
    float cy = 0;
    float previous_point = -1;
    for(float t = 0; t < 1.001; t+= 0.001){
        //qDebug() << t;
        float denom = ytt(t)*xt(t) - xtt(t)*yt(t);
        if(denom != 0){
            float numerator = xt(t)*xt(t) + yt(t)*yt(t);
            float R = numerator*std::sqrt(numerator)/denom;
            if(abs(R) < 450){
                //QGraphicsEllipseItem* circle2 = new QGraphicsEllipseItem();
                //circle2->setRect(coordinateX(t)*scale*ratio_x - 2, coordinateY(t)*scale*ratio_y - 2, 4, 4);
                //circle2->setBrush(QBrush(Qt::magenta));
                //circle2->setPen(QPen(QBrush(Qt::black),2));
                //scene->addItem(circle2);

                //cx = coordinateX(t) - yt(t)*numerator/denom;
                //cy = coordinateY(t) + xt(t)*numerator/denom;
                //QGraphicsEllipseItem* circle3 = new QGraphicsEllipseItem();
                //circle3->setRect(cx*scale*ratio_x - abs(R)*scale*ratio_x, cy*scale*ratio_y - abs(R)*scale*ratio_y, abs(R)*2*scale*ratio_x, abs(R)*2*scale*ratio_y);
                //circle3->setBrush(QBrush(Qt::transparent));
                //circle3->setPen(QPen(QBrush(Qt::green, Qt::BrushStyle( Qt::DotLine)),2));
                //scene->addItem(circle3);
                if(previous_point != -1){
                    end_curve(scene, t-0.001);
                }
                previous_point = -1;


                std::vector<std::pair<float, float>> new_rad_to_ang;
                while(abs(R) < 450 && t < 1.001){

                    if(new_rad_to_ang.size() == 4){
                        bool not_circle = (distance(new_rad_to_ang[0], new_rad_to_ang[3]) > distance(new_rad_to_ang[1], new_rad_to_ang[2]));
                        if(!not_circle){
                            new_rad_to_ang.clear();
                            break;
                        }
                        new_rad_to_ang[2] = new_rad_to_ang[3];
                        new_rad_to_ang[3] = std::make_pair(coordinateX(t), coordinateY(t));
                    }
                    else{
                        new_rad_to_ang.push_back(std::make_pair(coordinateX(t), coordinateY(t)));
                    }
                    t += 0.001;
                    denom = ytt(t)*xt(t) - xtt(t)*yt(t);
                    if(denom != 0){
                        numerator = xt(t)*xt(t) + yt(t)*yt(t);
                        R = numerator*std::sqrt(numerator)/denom;
                    }
                }
                if(new_rad_to_ang.size() >= 4){
                    CtoL.push_back(new_rad_to_ang);
                }
            }
            else if(abs(R) <= 40000){

                //qDebug() << "radius" << R;
                if(previous_point == -1){
                    previous_point = t;
                    start_curve(scene, t);
                }
                cx = coordinateX(t) - yt(t)*numerator/denom;
                cy = coordinateY(t) + xt(t)*numerator/denom;
                //qDebug() << "radddd to sec" << rad_to_sec(abs(R)/100);
                switch((int)(t*1000)){
                    case 0:{
                        float l = distance(std::make_pair(coordinateX(t), coordinateY(t)), std::make_pair(coordinateX(t+0.0005), coordinateY(t+0.0005)));
                        float alfa = std::asin((l/2)/R);
                        float x = 2*R*alfa;
                        time += rad_to_sec(abs(R)/100)*x/100;
                        std::vector<std::pair<float, float>> temp = {std::make_pair(coordinateX(t), coordinateY(t)), std::make_pair(R, rad_to_sec(R)*x)};
                        rad_info.insert(temp);
                        break;
                    }
                    case 1000:{
                        float l = distance(std::make_pair(coordinateX(t), coordinateY(t)), std::make_pair(coordinateX(t-0.0005), coordinateY(t-0.0005)));
                        float alfa = std::asin((l/2)/R);
                        float x = 2*R*alfa;
                        time += rad_to_sec(abs(R)/100)*x/100;
                        std::vector<std::pair<float, float>> temp = {std::make_pair(coordinateX(t), coordinateY(t)), std::make_pair(R, rad_to_sec(R)*x)};
                        rad_info.insert(temp);
                        break;
                    }
                    default:{
                        float l1 = distance(std::make_pair(coordinateX(t), coordinateY(t)), std::make_pair(coordinateX(t+0.0005), coordinateY(t+0.0005)));
                        float alfa1 = std::asin((l1/2)/R);
                        float x1 = 2*R*alfa1;

                        float l = distance(std::make_pair(coordinateX(t), coordinateY(t)), std::make_pair(coordinateX(t-0.0005), coordinateY(t-0.0005)));
                        float alfa = std::asin(l/2/R);
                        float x = 2*R*alfa;
                        time += rad_to_sec(abs(R)/100)*(x + x1)/100;

                        std::vector<std::pair<float, float>> temp = {std::make_pair(coordinateX(t), coordinateY(t)), std::make_pair(R, rad_to_sec(R)*(x + x1))};
                        rad_info.insert(temp);
                        break;

                    }
                }
                //scene->addEllipse(coordinateX(t)*scale*ratio_x - 2,coordinateY(t)*scale*ratio_y - 2, 4,4, QPen(Qt::NoPen),QBrush(Qt::red));
                //if((int)coordinateX(t)*scale*ratio_x - (int)previous_point.first > 0|| (int)coordinateY(t)*scale*ratio_y - (int)previous_point.second > 0){
                //    scene->addLine(previous_point.first, previous_point.second, coordinateX(t)*scale*ratio_x, coordinateY(t)*scale*ratio_y, QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap));
                //    previous_point.first = coordinateX(t)*scale*ratio_x;
                //    previous_point.second = coordinateY(t)*scale*ratio_y;
                //}

                //if(t - (int)t == 0){
                //    QGraphicsEllipseItem* circle3 = new QGraphicsEllipseItem();
                //    circle3->setRect(cx*scale*ratio_x - abs(R)*scale*ratio_x, cy*scale*ratio_y - abs(R)*scale*ratio_y, abs(R)*2*scale*ratio_x, abs(R)*2*scale*ratio_y);
                //    circle3->setBrush(QBrush(Qt::transparent));
                //    circle3->setPen(QPen(QBrush(Qt::green, Qt::BrushStyle( Qt::DotLine)),2));
                //    scene->addItem(circle3);
                //}

             }else{
                if(previous_point != -1){
                    QGraphicsEllipseItem* circle3 = new QGraphicsEllipseItem();
                    float x = coordinateX((previous_point + t - 0.001)/2)*scale*ratio_x;
                    float y = coordinateY((previous_point + t - 0.001)/2)*scale*ratio_y;
                    circle3->setRect(x - 1, y - 1, 2, 2);
                    circle3->setBrush(QBrush(Qt::transparent));
                    circle3->setPen(QPen(QBrush(Qt::red, Qt::BrushStyle( Qt::DotLine)),2));
                    scene->addItem(circle3);
                    end_curve(scene, t - 0.001);
                }
                previous_point = -1;
            }
        }
    }
    if(previous_point != -1){
        QGraphicsEllipseItem* circle3 = new QGraphicsEllipseItem();
        float x = coordinateX((previous_point + 1)/2)*scale*ratio_x;
        float y = coordinateY((previous_point + 1)/2)*scale*ratio_y;
        circle3->setRect(x - 1, y - 1, 2, 2);
        circle3->setBrush(QBrush(Qt::transparent));
        circle3->setPen(QPen(QBrush(Qt::red, Qt::BrushStyle( Qt::DotLine)),2));
        scene->addItem(circle3);
        end_curve(scene, 1);
    }
    previous_point = -1;
}
