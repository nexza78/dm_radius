#ifndef CUBICBEZIER_H
#define CUBICBEZIER_H
#include <QMainWindow>

class CubicBezier: public QMainWindow{
public:
    std::pair<float, float> P0;
    std::pair<float, float> P1;
    std::pair<float, float> P2;
    std::pair<float, float> P3;
    float xt(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t);
    float xtt(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t);
    float yt(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t);
    float ytt(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t);
    float coordinateX(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t);
    float coordinateY(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t);

};

#endif // CUBICBEZIER_H
