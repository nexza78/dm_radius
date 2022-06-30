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
    paths.clear();

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
        std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>>  prepare_vector = split(cur_data);
        prepare_vector = toUpperCase(prepare_vector);

        //qDebug() << i << "############################-----toUpperCase------######################################";
        //for(int k = 0; k < prepare_vector.size(); k++){
        //    qDebug() << prepare_vector[k].first;
        //    for(int j = 0; j < prepare_vector[k].second.size(); j ++){
        //        qDebug() << j<< ' '<< prepare_vector[k].second[j].first <<", "<< prepare_vector[k].second[j].second;
        //    }
        //}

        split2(prepare_vector);

        //проверка работы, вывод в дебаг
    }
    //до - все ништяк
    //split2
    merge();
    qDebug() << "##################################################################";
    for(std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> i: paths){
        for(int k = 0; k < i.size(); k++){
            qDebug() << i[k].first;
            for(int j = 0; j < i[k].second.size(); j ++){
                qDebug() << j<< ' '<< i[k].second[j].first <<", "<< i[k].second[j].second;
            }
        }
        this->angles(i);
        this->radiuses(i);
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

//доделать эллипсы

//i равно 1 потому шо 0 это всегда М
//сначала находим первую попавшуюся букву и только потом обрабатываем все точки,
//которые остались между этой буквой и прошлой, относительно прошлой точки j0

//Если мы отыскали следующую точку, то
//создаем вереницу точек, принадлежащую точке под индексом j0
//первый элемент - буква, обозначающая тип (кривая, прямая и т.д.)
//дальше обрабатываем всю оставшуюся строку, разделив на эти точки
//Внимание - здесь не обрабатываются "относительность" и "абсолютность"


std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> MainWindow:: split(QString points){
    std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> res;
    points += 'p';
    int j0 = 0;

    for(int i = 1; i < points.size(); i += 1){

        if(points[i].isLetter()){
            std::pair<QChar,std::vector<std::pair<float, float>>> cur_pair;
            cur_pair.first = points[j0];
            switch(points[j0].toUpper().unicode()){
                case 'H':{
                   while(j0 < i){
                       std::pair<float, float> cur_point;
                       cur_point.first = get_point(points, j0);
                       cur_point.second = 0;
                       cur_pair.second.push_back(cur_point);
                       j0++;
                   }
                   break;
                }
                case 'V':{
                   while(j0 < i){
                       std::pair<float, float> cur_point;
                       cur_point.first = 0;
                       cur_point.second = get_point(points, j0);
                       cur_pair.second.push_back(cur_point);
                       j0++;
                   }
                   break;
                }
                case 'A': break;
                case 'Z': break;
                default:{
                   while(j0 < i){
                       std::pair<float, float> cur_point;
                       cur_point.first = get_point(points, j0);
                       cur_point.second = get_point(points, j0);
                       cur_pair.second.push_back(cur_point);
                       j0++;
                   }
                }
            }
            j0 = i;
            res.push_back(cur_pair);
        }
    }
    return res;
}

std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> MainWindow::toUpperCase(std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> points){
    std::vector<std::pair<float, float>> cur_pointM = points[0].second;
    for(int i = 0; i < points.size(); i++){

        switch(points[i].first.unicode()){
            case 'H':{
                std::pair<float, float> prev_point;
                prev_point = points[i-1].second.back();
                for(int j = 0; j < points[i].second.size(); j ++){
                    points[i].second[j].second = prev_point.second;
                }
                break;
            }
            case 'V':{
                std::pair<float, float> prev_point;
                prev_point = points[i-1].second.back();
                for(int j = 0; j < points[i].second.size(); j ++){
                    points[i].second[j].first = prev_point.first;
                }
                break;
            }
            default:{
                if(points[i].first.isLower()){
                    std::pair<float, float> prev_point;
                    std::pair<float, float> prev_prev_point;

                    switch(points[i].first.unicode()){
                        case 'c': {
                            prev_point = points[i-1].second.back();
                            for(int j = 0; j < points[i].second.size(); j += 3){
                                for(int k = 0; k<3; k++){
                                    points[i].second[j+k].first += prev_point.first;
                                    points[i].second[j+k].second += prev_point.second;
                                }
                                prev_point = points[i].second[j+2];
                            }
                            break;
                        }
                        case 'q':{
                            prev_point = points[i-1].second.back();
                            std::pair<QChar, std::vector<std::pair<float, float>>> new_vals;
                            new_vals.first = 'C';
                            for(int j = 0; j < points[i].second.size(); j += 2){
                                for(int k = 0; k<2; k++){
                                    points[i].second[j+k].first += prev_point.first;
                                    points[i].second[j+k].second += prev_point.second;
                                }
                                new_vals.second.push_back(points[i].second[j]);
                                new_vals.second.push_back(points[i].second[j]);
                                new_vals.second.push_back(points[i].second[j+1]);
                                prev_point = points[i].second[j+1];
                            }
                            points[i] = new_vals;
                            break;
                        }
                        case 's':{
                            prev_point = points[i-1].second.back();
                            prev_prev_point = points[i-1].second[points[i-1].second.size() - 2];
                            std::pair<QChar, std::vector<std::pair<float, float>>> new_vals;
                            new_vals.first = 'C';
                            for(int j = 0; j < points[i].second.size(); j += 2){
                                for(int k = 0; k<2; k++){
                                    new_vals.second.push_back(std::make_pair(2*prev_point.first - prev_prev_point.first, 2*prev_point.second - prev_prev_point.second));
                                    points[i].second[j+k].first += prev_point.first;
                                    points[i].second[j+k].second += prev_point.second;
                                }
                                prev_point = points[i].second[j+1];
                                prev_prev_point = points[i].second[j];
                            }
                            points[i] = new_vals;
                            break;
                        }
                        case 'm':{
                            points[i].second[0].first += cur_pointM[0].first;
                            points[i].second[0].second += cur_pointM[0].second;
                            cur_pointM = points[i].second;
                            break;
                        }
                        case 'z':{
                            break;
                        }
                        default:{
                            prev_point = points[i-1].second.back();
                            for(int j = 0; j < points[i].second.size(); j ++){
                                points[i].second[j].first += prev_point.first;
                                points[i].second[j].second += prev_point.second;
                                prev_point = points[i].second[j];
                            }
                        }
                    }
                }
            }
        }
        points[i].first = points[i].first.toUpper();
        if(points[i].first == 'H' || points[i].first == 'V'){
            points[i].first = 'L';
        }
    }
    return points;
}

void MainWindow::split2(std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> cur_path){
    std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> cur_add_path;
    //int schet = 0;
    for(std::pair<QChar,std::vector<std::pair<float, float>>> i: cur_path){
        if(i.first == 'Z'){
            if(cur_add_path[0].second[0] != cur_add_path[cur_add_path.size()-1].second[cur_add_path[cur_add_path.size()-1].second.size()-1]){
                cur_add_path.push_back(std::make_pair('L', cur_add_path[0].second));
            }
            //для обработки стыка (чтобы не надо было отдельно его обрабатывать)
            cur_add_path.push_back(cur_add_path[1]);
            paths.push_back(cur_add_path);
            //qDebug() <<  schet << "############################-----SPLIT 2------######################################";
            //for(int k = 0; k < cur_add_path.size(); k++){
            //    qDebug() << cur_add_path[k].first;
            //    for(int j = 0; j < cur_add_path[k].second.size(); j ++){
            //        qDebug() << j<< ' '<< cur_add_path[k].second[j].first <<", "<< cur_add_path[k].second[j].second;
            //    }
            //}
            cur_add_path.clear();
            //schet += 1;
        }
        else{
            cur_add_path.push_back(i);
        }
    }
}

void MainWindow::merge(){
    for(int k = 0; k < paths.size(); k++){
        std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> cur_path = paths[k];
        std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> new_path;
        for(int i = 0; i < cur_path.size(); i++){
            switch(cur_path[i].first.unicode()){
                case 'C':{
                    for(int j = 0; j < cur_path[i].second.size(); j += 3){
                        std::pair<float, float> prev_point = new_path.back().second.back();
                        float sum = 0, final = 0;
                        sum += std::sqrt((prev_point.first  - cur_path[i].second[j].first) *(prev_point.first  - cur_path[i].second[j].first) + (prev_point.second - cur_path[i].second[j].second)*(prev_point.second - cur_path[i].second[j].second));
                        for(int t = 0; t < 2; t++){
                            sum += std::sqrt((cur_path[i].second[j+t].first  - cur_path[i].second[j+t+1].first) *(cur_path[i].second[j+t].first  - cur_path[i].second[j+t+1].first) + (cur_path[i].second[j+t].second - cur_path[i].second[j+t+1].second)*(cur_path[i].second[j+t].second - cur_path[i].second[j+t+1].second));
                        }
                        final = sqrt((prev_point.first  - cur_path[i].second[j+2].first) *(prev_point.first  - cur_path[i].second[j+2].first) + (prev_point.second - cur_path[i].second[j+2].second)*(prev_point.second - cur_path[i].second[j+2].second));

                        if(final == sum){
                            if(new_path.back().first == 'L'){
                                new_path.back().second.push_back(cur_path[i].second[j+2]);
                            }
                            else{
                                new_path.push_back(std::make_pair('L', std::vector<std::pair<float, float>>{cur_path[i].second[j+2]}));
                            }
                        }
                        else{
                            if(new_path.back().first != 'C'){
                                new_path.push_back(std::make_pair('C', std::vector<std::pair<float, float>>{}));
                            }
                            for(int t = 0; t < 3; t++){
                                new_path.back().second.push_back(cur_path[i].second[j+t]);
                            }
                        }
                        prev_point = new_path.back().second.back();
                    }
                    break;
                }
                case 'L':{
                    if(new_path.back().first == 'L'){
                        for(std::pair<float, float> t: cur_path[i].second){
                            new_path.back().second.push_back(t);
                        }
                    }else{
                        new_path.push_back(cur_path[i]);
                    }
                    break;
                }
                default:
                    new_path.push_back(cur_path[i]);
                    break;
            }
        }
        qDebug() << "############################-----MERGE------######################################";
        for(int k = 0; k < std::max(cur_path.size(), new_path.size()); k++){
            if(k < new_path.size() && k < cur_path.size()){
                qDebug() << "            " << cur_path[k].first <<  "            ""            " << new_path[k].first;
                for(int j = 0; j < std::max(new_path[k].second.size(), cur_path[k].second.size()); j ++){
                    if(j < cur_path[k].second.size() && j < new_path[k].second.size()){
                        qDebug() << j<< ' '<< cur_path[k].second[j].first <<", "<< cur_path[k].second[j].second << "            "<< new_path[k].second[j].first <<", "<< new_path[k].second[j].second;
                    }
                    else if(j >= cur_path[k].second.size()){
                        qDebug() << j<< ' '<< "пусто" <<"            ""            "<< new_path[k].second[j].first <<", "<< new_path[k].second[j].second;
                    }
                    else{
                        qDebug() << j<< ' '<< cur_path[k].second[j].first <<", "<< cur_path[k].second[j].second << "            "<< "пусто";
                    }
                }
            }
            else if(k >= cur_path.size()){
                qDebug() << "пусто" << "            ""            "<< new_path[k].first;

                for(int j = 0; j < new_path[k].second.size(); j ++){
                    qDebug() << j<< ' '<< "пусто" <<"            ""            "<< new_path[k].second[j].first <<", "<< new_path[k].second[j].second;

                }
            }
            else{
                qDebug() << "            " << cur_path[k].first <<  "            ""            ""            " << "пусто";
                for(int j = 0; j < cur_path[k].second.size(); j ++){
                        qDebug() << j<< ' '<< cur_path[k].second[j].first <<", "<< cur_path[k].second[j].second << "            "<< "пусто";

                }
            }
        }
        paths[k] = new_path;
    }

    //смысл в принципе понятен - если в М больше одной точки, то создаем Л и туда все эти точки запихиваем, но какая-то хуйня
    //честно говоря думаю что в М не должно быть более одной точки
    //if(points[0].second.size() > 1){
    //    M_flag = 1;
    //    std::vector<std::pair<float, float>> fromMtoL;
    //    for(int i = 1; i < points[0].second.size(); i ++){
    //        fromMtoL.push_back(points[0].second[i]);
    //    }
    //    res.push_back(std::make_pair('L', fromMtoL));
    //}
    //если два подряд элемента points начинаются с одинаковых меток типа линии, то соединить в один элемент

    //std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> res;
    //std::vector<std::pair<float, float>> init_vect;
    //init_vect.push_back(points[0].second[0]);
    //res.push_back(std::make_pair('M', init_vect));
    //bool M_flag = 0;
    //
    //int i0 = 1;
    //int i = 2;
    //while(i <= points.size()){
    //    while(i != points.size()){
    //        if(points[i0].first == points[i].first){
    //            i++;
    //        }
    //        else{
    //            break;
    //        }
    //    }
    //    //можно переделать покомпактнее
    //    if(i - i0 == 1){
    //        res.push_back(points[i0]);
    //    }
    //    else{
    //        res.push_back(points[i0]);
    //        for(int j = i0 + 1; j < i; j++){
    //            for(int k = 0; k < points[j].second.size(); k++){
    //                res[res.size()-1].second.push_back(points[j].second[k]);
    //            }
    //        }
    //    }
    //    i0 = i;
    //    i++;
    //}
    //так понимаю если в М было больше двух штук элементов и третий элемент тоже Л, то второй и третий элементы сливаются
    //if(M_flag && res.size() > 2){
    //    if(res[2].first == 'L'){
    //        for(int i = 0; i < res[2].second.size(); i++){
    //            res[1].second.push_back(res[2].second[i]);
    //        }
    //        res.erase(res.begin() + 2);
    //    }
    //}
    //return res;
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


void MainWindow::radiuses(std::vector<std::pair<QChar,std::vector<std::pair<float, float>>>> points){
    for(int i = 1; i < points.size() - 1; i++){
        if(points[i].first == 'C'){
            std::pair<float, float> prev_point;
            prev_point = points[i-1].second.back();
            for(int j = 0; j < points[i].second.size(); j += 3){
                qDebug() << i << ' ' << j;
                CubicBezier curve(prev_point, points[i].second[j], points[i].second[j+1], points[i].second[j+2]);
                curve.radiuses(scene, scale);
                prev_point = points[i].second[j+2];
            }
        }
    }
}

