#include<CubicBezier.h>
/*
float CubicBezier::coordinateX(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t){
    return ((1-t)*(1-t)*(1-t)*P0.first + 3*t*(1-t)*(1-t)*P1.first + 3*t*t*(1-t)*P2.first + t*t*t*P3.first)*ratio_x*scale;
}

float CubicBezier::coordinateY(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t){
    return ((1-t)*(1-t)*(1-t)*P0.second + 3*t*(1-t)*(1-t)*P1.second + 3*t*t*(1-t)*P2.second + t*t*t*P3.second)*ratio_y*scale;
}

float CubicBezier::xt(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t){
    return (-3*(1-t)*(1-t)*P0.first + (9*t*t-12*t+3)*P1.first + (6*t - 9*t*t)*P2.first + 3*t*t*P3.first)*ratio_x*scale;
}
float CubicBezier::xtt(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t){
    return (6*(1-t)*P0.first + (18*t - 12)*P1.first + (6-18*t)*P2.first + 6*t*P3.first)*ratio_x*scale;
}
float CubicBezier::yt(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t){
    return (-3*(1-t)*(1-t)*P0.second + (9*t*t-12*t+3)*P1.second + (6*t - 9*t*t)*P2.second + 3*t*t*P3.second)*ratio_y*scale;
}
float CubicBezier::ytt(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t){
    return (6*(1-t)*P0.second + (18*t - 12)*P1.second + (6-18*t)*P2.second + 6*t*P3.second)*ratio_x*scale;
}
*/
