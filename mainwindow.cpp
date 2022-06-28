#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(0,0,400,400);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_review_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, "Выберите файл", "", "svg-files (*.svg)");
    ui->lineEdit_path->setText(file_path);
    path_was_chosen = true;
}

void MainWindow::on_pushButton_open_clicked()
{
    const QString path = ui->lineEdit_path->text();
    main_item = new QGraphicsSvgItem(path);
    ratio_x = (main_item->renderer()->defaultSize().width()*1.0)/main_item->renderer()->viewBox().width();
    ratio_y = (main_item->renderer()->defaultSize().height()*1.0)/main_item->renderer()->viewBox().height();
    scale = 1000/(main_item->renderer()->defaultSize().height()*1.0);
    main_item->setScale(scale);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    scene->addItem(main_item);
    qDebug() << main_item->renderer()->defaultSize().height() << main_item->renderer()->viewBox().height();
}

void MainWindow::on_pushButton_analyze_clicked()
{
    const QString filename = ui->lineEdit_path->text();
    //QList<QGraphicsRectItem *> rectList;    // Объявим в стеке список прямоугольников

    QDomDocument doc;       // объект документа
    QFile file(filename);   // Открываем наш SVG-файл
    // Если он не открылся или не удалось передать содержимое в QDocDocument
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return;    // то возвратим список, но пустой

    // Ищем в документе все объекты с тегом g
    QDomNodeList gList = doc.elementsByTagName("g");
    QDomNodeList pathList = doc.elementsByTagName("path");
    // Начинаем их перебирать
    for (int i = 0; i < pathList.size(); i++) {
        QDomNode gNode = pathList.item(i);     // Выделяем из списка ноду
        QDomElement path_data = gNode.toElement();
        qDebug() << path_data.attribute("d");
        QString cur_data = path_data.attribute("d");
        std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> repair_vector = split(cur_data);

        repair_vector = toUpperCase(repair_vector, cur_data[cur_data.size() - 1] == 'z' || cur_data[cur_data.size() - 1] == 'Z');
        repair_vector = merge(repair_vector);
        for(int k = 0; k < repair_vector.size(); k++){
            qDebug() << repair_vector[k].first;
            for(int j = 0; j < repair_vector[k].second.size(); j ++){
                qDebug() << j<< ' '<< repair_vector[k].second[j].first <<", "<< repair_vector[k].second[j].second;
            }
        }
        this->angles(repair_vector);
        this->radiuses(repair_vector);
    }
    file.close();
    return;
}

float MainWindow::get_point(QString points, int &j0){
    while(!points[j0].isDigit() && points[j0] != '-'){
        j0++;
    }
    int j_end = j0;
    while(points[j_end].isDigit() || points[j_end] == '.' || points[j_end] == '-'){
        j_end++;
    }
    QStringRef substr(&points, j0, j_end - j0);
    j0 = j_end;
    return substr.toFloat();
}

std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> MainWindow:: split(QString points){
    std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> res;
    int j0 = 0;
    if(!points[points.size() - 1].isLetter()){
        points += 'p';
    }
    for(int i = 1; i < points.size(); i++){
        if(points[i].isLetter()){
            std::pair<QChar,std::vector<std::pair<float, float>>> cur_pair;
            cur_pair.first = points[j0];
            if(points[j0].toUpper() == 'H'){
                while(j0 < i){
                    std::pair<float, float> cur_point;
                    cur_point.first = get_point(points, j0);
                    cur_point.second = 0;
                    cur_pair.second.push_back(cur_point);
                    j0++;
                }
            }
            else if(points[j0].toUpper() == 'V'){
                while(j0 < i){
                    std::pair<float, float> cur_point;
                    cur_point.first = 0;
                    cur_point.second = get_point(points, j0);
                    cur_pair.second.push_back(cur_point);
                    j0++;
                }
            }
            else if(points[j0].toUpper() == 'A'){

            }
            else{
                while(j0 < i){
                    std::pair<float, float> cur_point;
                    cur_point.first = get_point(points, j0);
                    cur_point.second = get_point(points, j0);
                    cur_pair.second.push_back(cur_point);
                    j0++;
                }
            }
            j0 = i;
            res.push_back(cur_pair);
        }
    }
    return res;
}

std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> MainWindow::toUpperCase(std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> res, bool z){
    for(int i = 0; i < res.size(); i++){
        if(res[i].first == 'H'){
            std::pair<float, float> prev_point;
            prev_point = res[i-1].second[res[i-1].second.size() - 1];
            for(int j = 0; j < res[i].second.size(); j ++){
                res[i].second[j].second = prev_point.second;
            }
        }
        else if(res[i].first == 'V'){
            std::pair<float, float> prev_point;
            prev_point = res[i-1].second[res[i-1].second.size() - 1];
            for(int j = 0; j < res[i].second.size(); j ++){
                res[i].second[j].first = prev_point.first;
            }
        }
        else if(res[i].first.isLower()){
            std::pair<float, float> prev_point;
            prev_point = res[i-1].second[res[i-1].second.size() - 1];
            if(res[i].first == 'c'){
                for(int j = 0; j < res[i].second.size(); j += 3){
                    for(int k = 0; k<3; k++){
                        res[i].second[j+k].first += prev_point.first;
                        res[i].second[j+k].second += prev_point.second;
                    }
                    prev_point = res[i].second[j+2];
                }
            }
            else if(res[i].first == 'q' || res[i].first == 's'){
                for(int j = 0; j < res[i].second.size(); j += 2){
                    for(int k = 0; k<2; k++){
                        res[i].second[j+k].first += prev_point.first;
                        res[i].second[j+k].second += prev_point.second;
                    }
                    prev_point = res[i].second[j+1];
                }
            }
            else{
                for(int j = 0; j < res[i].second.size(); j ++){
                    res[i].second[j].first += prev_point.first;
                    res[i].second[j].second += prev_point.second;
                    prev_point = res[i].second[j];
                }
            }
            res[i].first = res[i].first.toUpper();
        }
        if(res[i].first == 'H' || res[i].first == 'V'){
            res[i].first = 'L';
        }
    }
    if(z){
        if(res[0].second[0] != res[res.size()-1].second[res[res.size()-1].second.size()-1]){
            res.push_back(std::make_pair('L', res[0].second));
        }
        res.push_back(res[1]);
    }
    return res;
}

std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> MainWindow::merge(std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> points){
    std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> res;
    std::vector<std::pair<float, float>> init_vect;
    init_vect.push_back(points[0].second[0]);
    res.push_back(std::make_pair('M', init_vect));
    bool M_flag = 0;
    if(points[0].second.size() > 1){
        M_flag = 1;
        std::vector<std::pair<float, float>> fromMtoL;
        for(int i = 1; i < points[0].second.size(); i ++){
            fromMtoL.push_back(points[0].second[i]);
        }
        res.push_back(std::make_pair('L', fromMtoL));
    }
    //случай где в М одиночная точка
    int i0 = 1;
    int i = 2;
    while(i <= points.size()){
        while(true){
            if(i == points.size()){
                break;
            }
            if(points[i0].first == points[i].first){
                i++;
            }
            else{
                break;
            }
        }
        if(i - i0 == 1){
            res.push_back(points[i0]);
        }
        else{
            res.push_back(points[i0]);
            for(int j = i0 + 1; j < i; j++){
                for(int k = 0; k < points[j].second.size(); k++){
                    res[res.size()-1].second.push_back(points[j].second[k]);
                }
            }
        }
        i0 = i;
        i++;
    }
    if(M_flag && res.size() > 2){
        if(res[2].first == 'L'){
            for(int i = 0; i < res[2].second.size(); i++){
                res[1].second.push_back(res[2].second[i]);
            }
            res.erase(res.begin() + 2);
        }
    }
    return res;
}

void MainWindow::degrees(std::pair<float, float> P0, std::pair<float, float> P1, std::pair<float, float> P2){
    float c2 = (P0.first - P1.first)*(P0.first - P1.first) + (P0.second - P1.second)*(P0.second - P1.second);
    float a2 = (P0.first - P2.first)*(P0.first - P2.first) + (P0.second - P2.second)*(P0.second - P2.second);
    float b2 = (P2.first - P1.first)*(P2.first - P1.first) + (P2.second - P1.second)*(P2.second - P1.second);
    if(a2 > b2 + c2){return;}
    float cosinus = (b2 + c2 - a2)/(2*std::sqrt(c2)*std::sqrt(b2));
    if(cosinus <= 0 && cosinus != -1){
        QGraphicsEllipseItem* circle = new QGraphicsEllipseItem();
        circle->setRect(P1.first*ratio_x*scale - 5, P1.second*ratio_y*scale - 5, 10, 10);
        circle->setBrush(QBrush(Qt::green));
        circle->setPen(QPen(QBrush(Qt::green),2));
        scene->addItem(circle);
    }
    else if(cosinus > 0 && cosinus != 1){
        QGraphicsEllipseItem* circle = new QGraphicsEllipseItem();
        circle->setRect(P1.first*ratio_x*scale - 5, P1.second*ratio_y*scale - 5, 10, 10);
        circle->setBrush(QBrush(Qt::blue));
        circle->setPen(QPen(QBrush(Qt::blue),2));
        scene->addItem(circle);
    }
    QGraphicsEllipseItem* circle1 = new QGraphicsEllipseItem();
    circle1->setRect(P0.first*ratio_x*scale - 2, P0.second*ratio_y*scale - 2, 4, 4);
    circle1->setBrush(QBrush(Qt::yellow));
    circle1->setPen(QPen(QBrush(Qt::black),2));
    scene->addItem(circle1);
    QGraphicsEllipseItem* circle2 = new QGraphicsEllipseItem();
    circle2->setRect(P1.first*ratio_x*scale - 2, P1.second*ratio_y*scale - 2, 4, 4);
    circle2->setBrush(QBrush(Qt::yellow));
    circle2->setPen(QPen(QBrush(Qt::black),2));
    scene->addItem(circle2);
    QGraphicsEllipseItem* circle3 = new QGraphicsEllipseItem();
    circle3->setRect(P2.first*ratio_x*scale - 2, P2.second*ratio_y*scale - 2, 4, 4);
    circle3->setBrush(QBrush(Qt::yellow));
    circle3->setPen(QPen(QBrush(Qt::black),2));
    scene->addItem(circle3);
}

void MainWindow::angles(std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> points){
    for(int i = 1; i < points.size(); i++){
        if(points[i].first == 'L'){
            if(points[i].second.size() >= 3){
                for(int j = 1; j < points[i].second.size() - 1; j++){
                    degrees(points[i].second[j-1], points[i].second[j], points[i].second[j+1]);
                }
            }
        }
        else if(points[i].first == 'C'){
            if(points[i].second.size() > 3){
                for(int j = 1; j < points[i].second.size() - 2; j += 3){
                    degrees(points[i].second[j], points[i].second[j+1], points[i].second[j+2]);
                }
            }
        }
        else if(points[i].first == 'Q'){
            if(points[i].second.size() > 2){
                for(int j = 0; j < points[i].second.size() - 2; j += 2){
                    degrees(points[i].second[j], points[i].second[j+1], points[i].second[j+2]);
                }
            }
        }
    }
    //на стыке
    for(int i = 1;i < points.size() - 2; i ++){
        if(points[i].first == 'C'){
            std::pair<float,float> P0 = points[i].second[ points[i].second.size()-2];
            std::pair<float,float> P1 = points[i].second[ points[i].second.size()-1];
            std::pair<float,float> P2 = points[i+1].second[0];
            degrees(P0, P1, P2);
        }
        else if(points[i].first == 'L'){
            if(points[i].second.size() >= 2){
                std::pair<float,float> P0 = points[i].second[ points[i].second.size()-2];
                std::pair<float,float> P1 = points[i].second[ points[i].second.size()-1];
                std::pair<float,float> P2 = points[i+1].second[0];
                degrees(P0, P1, P2);
            }
            else{
                std::pair<float,float> P0 = points[i-1].second[ points[i].second.size()-1];
                std::pair<float,float> P1 = points[i].second[0];
                std::pair<float,float> P2 = points[i+1].second[0];
                degrees(P0, P1, P2);
            }
            if(points[i-1].first == 'M'){
                if(points[i].second.size() >= 2){
                    std::pair<float,float> P0 = points[i-1].second[0];
                    std::pair<float,float> P1 = points[i].second[0];
                    std::pair<float,float> P2 = points[i].second[1];
                    degrees(P0, P1, P2);
                }
                else{
                    std::pair<float,float> P0 = points[i-1].second[0];
                    std::pair<float,float> P1 = points[i].second[0];
                    std::pair<float,float> P2 = points[i+1].second[0];
                    degrees(P0, P1, P2);
                }
            }
        }
    }
}

QList<QGraphicsRectItem *> MainWindow::getElements(const QString filename)
{
    QList<QGraphicsRectItem *> rectList;    // Объявим в стеке список прямоугольников

    QDomDocument doc;       // объект документа
    QFile file(filename);   // Открываем наш SVG-файл
    // Если он не открылся или не удалось передать содержимое в QDocDocument
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return rectList;    // то возвратим список, но пустой

    // Ищем в документе все объекты с тегом g
    QDomNodeList gList = doc.elementsByTagName("g");
    QDomNodeList pathList = doc.elementsByTagName("path");
    // Начинаем их перебирать
    for (int i = 0; i < pathList.size(); i++) {
        QDomNode gNode = pathList.item(i);     // Выделяем из списка ноду
        QDomElement path_data = gNode.toElement();
        qDebug() << path_data.attribute("d");
        QString cur_data = path_data.attribute("d");
        std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> repair_vector = split(cur_data);
/*
        qDebug() << repair_vector;
        repair_vector = toUpperCase(repair_vector);
        qDebug() << repair_vector;
        repair_vector = merge(repair_vector);
        qDebug() << repair_vector;*/
    }
    //for (int i = 0; i < gList.size(); i++) {
    //    QDomNode gNode = gList.item(i);     // Выделяем из списка ноду
    //    QDomElement path_data = gNode.firstChildElement("path");
    //     //И ищем в ней элемент c тегом rect
    //     //QDomElement path_datas = gNode.
    //    if (path_data.isNull()){
    //        continue;
    //    // Если полученный элементы не нулевой, то
    //    } else {
    //        // начинаем формировать прямоугольник
    //        QGraphicsRectItem *path = new QGraphicsRectItem();
    //        // Этот флаг делает объект перемещаемым, потребуется для проверки
    //        path->setFlag(QGraphicsItem::ItemIsMovable);
    //        // Забираем размеры из тега rect
    //        QDomElement gElement = gNode.toElement();
    //        qDebug() << path_data.attribute("d");
    //    }
    //}
    file.close();
    return rectList;
}



//QRectF MainWindow::getSizes(const QString filename)
//{
//    QDomDocument doc;       // инициализируем в стеке объект QDomDocument
//    QFile file(filename);   // Открываем наш SVG-файл
//    // Если он не открылся или не удалось передать содержимое в QDocDocument
//    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
//        return QRectF(0,0,200,200); // то возвратим значения для сцены по умолчанию
//
//    /* Далее забираем список элементов с тегом svg.
//     * В случае, если список элементов будет не пустой,
//     * то заберём размеры графической сцены.
//     * */
//    QDomNodeList list = doc.elementsByTagName("svg");
//    if(list.length() > 0) {
//        QDomElement svgElement = list.item(0).toElement();
//        QStringList parameters = svgElement.attribute("viewBox").split(" ");
//        return QRectF(parameters.at(0).toInt(),
//                      parameters.at(1).toInt(),
//                      parameters.at(2).toInt(),
//                      parameters.at(3).toInt());
//    }
//    return QRectF(0,0,200,200);
//}


void MainWindow::on_spinBox_valueChanged(int arg1)
{
    if(main_item != nullptr){
        main_item->setScale(main_item->scale()*1.2);
    }

}


/*
void MainWindow::rad_cubic(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3){
    float minR = INFINITY;
    float cx = 0;
    float cy = 0;

    for(float t = 0.1; t < 1; t+= 0.1){
        float denom = ytt(P0, P1, P2, P3, t)*xt(P0, P1, P2, P3, t) - xtt(P0, P1, P2, P3, t)*yt(P0, P1, P2, P3, t);
        if(denom != 0){
            float numerator = xt(P0, P1, P2, P3, t)*xt(P0, P1, P2, P3, t) + yt(P0, P1, P2, P3, t)*yt(P0, P1, P2, P3, t);
             float R = numerator*std::sqrt(numerator)/denom;
             //real size R
             if(R)
             if(R < minR){
                 minR = R;
                 cx = coordinateX(P0, P1, P2, P3, t) - yt(P0, P1, P2, P3, t)*numerator/denom;
                 cy = coordinateY(P0, P1, P2, P3, t) + xt(P0, P1, P2, P3, t)*numerator/denom;
                 QGraphicsEllipseItem* circle2 = new QGraphicsEllipseItem();
                 circle2->setRect(coordinateX(P0, P1, P2, P3, t) - 2, coordinateY(P0, P1, P2, P3, t) - 2, 4, 4);
                 circle2->setBrush(QBrush(Qt::yellow));
                 circle2->setPen(QPen(QBrush(Qt::black),2));
                 scene->addItem(circle2);
             }
        }
    }
    if(minR != INFINITY){
        QGraphicsEllipseItem* circle3 = new QGraphicsEllipseItem();
        qDebug() << "Hello";
        circle3->setRect(cx - minR, cy - minR, minR*2, minR*2);
        circle3->setBrush(QBrush(Qt::transparent));
        circle3->setPen(QPen(QBrush(Qt::green),2));
        scene->addItem(circle3);
    }
}

float MainWindow::coordinateX(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t){
    return ((1-t)*(1-t)*(1-t)*P0.first + 3*t*(1-t)*(1-t)*P1.first + 3*t*t*(1-t)*P2.first + t*t*t*P3.first)*ratio_x*scale;
}

float MainWindow::coordinateY(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t){
    return ((1-t)*(1-t)*(1-t)*P0.second + 3*t*(1-t)*(1-t)*P1.second + 3*t*t*(1-t)*P2.second + t*t*t*P3.second)*ratio_y*scale;
}

float MainWindow::xt(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t){
    return (-3*(1-t)*(1-t)*P0.first + (9*t*t-12*t+3)*P1.first + (6*t - 9*t*t)*P2.first + 3*t*t*P3.first)*ratio_x*scale;
}
float MainWindow::xtt(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t){
    return (6*(1-t)*P0.first + (18*t - 12)*P1.first + (6-18*t)*P2.first + 6*t*P3.first)*ratio_x*scale;
}
float MainWindow::yt(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t){
    return (-3*(1-t)*(1-t)*P0.second + (9*t*t-12*t+3)*P1.second + (6*t - 9*t*t)*P2.second + 3*t*t*P3.second)*ratio_y*scale;
}
float MainWindow::ytt(std::pair<float, float> P0, std::pair<float, float> P1,std::pair<float, float> P2,std::pair<float, float> P3, float t){
    return (6*(1-t)*P0.second + (18*t - 12)*P1.second + (6-18*t)*P2.second + 6*t*P3.second)*ratio_x*scale;
}

void MainWindow::radiuses(std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> points){
    for(int i = 1; i < points.size() - 1; i++){
        if(points[i].first == 'C'){
            std::pair<float, float> prev_point;
            prev_point = points[i-1].second[points[i-1].second.size() - 1];
            for(int j = 0; j < points[i].second.size(); j += 3){
                qDebug() << i << ' ' << j;
                rad_cubic(prev_point, points[i].second[j], points[i].second[j+1], points[i].second[j+2]);
                prev_point = points[i].second[j+2];
            }
        }
    }
}
*/
