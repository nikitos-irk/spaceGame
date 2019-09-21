#include "common.hpp"

#include <tuple>

#include "colorschema.hpp"
#include "figure/factory_shape.hpp"
#include "primitive/point.hpp"

DirectionXY::DirectionXY(double x, double y){
    this->x = x;
    this->y = y;
}

DirectionXY::DirectionXY(){
    this->x = 0;
    this->y = 0;
}

std::pair<double, double> getXYOffsetOnVector(primitive::Point px1,
                                              primitive::Point px2,
                                              double offset_length) {
    double length = getLengthOfVector(px1, px2);
    double Cx = (px1.x - px2.x) / (length/(offset_length*2));
    double Cy = (px1.y - px2.y) / (length/(offset_length*2));
    return std::make_pair(Cx, Cy);
}

primitive::Point getTwoLinesIntersaction(primitive::Point p1, primitive::Point p2,
                                         primitive::Point p3, primitive::Point p4) {
    double x1 = p1.x;
    double y1 = p1.y;

    double x2 = p2.x;
    double y2 = p2.y;

    double x3 = p3.x;
    double y3 = p3.y;

    double x4 = p4.x;
    double y4 = p4.y;

    double px = ((x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4))/((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));
    double py = ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4))/((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));

    return primitive::Point{px, py};
}

void putSquareOnPoint(SDL_Renderer* renderer, primitive::Point center_point,
                      double block_hypotenuse_length, double angle) {
    
    std::vector<primitive::Point> littleSqare;
    primitive::Point tmp = center_point;
    tmp.y -= block_hypotenuse_length;

    for (int i = 0; i < 4; ++i){
        double theta = M_PI/4 + i*M_PI/2 - angle; // getTiltAngel();
        auto p = tmp;
        littleSqare.push_back(std::move(p.rotate(center_point, theta)));
    }
    auto iter = littleSqare.begin();
    figure::FactoryShape factory{renderer};
    for (; iter != littleSqare.end() - 1; ++iter){
        factory.line({iter->x, iter->y},
                     {(iter+1)->x, (iter+1)->y}).draw();
    }
    factory.line({iter->x, iter->y},
                 {littleSqare.begin()->x, littleSqare.begin()->y}).draw();
    fillRect(renderer, littleSqare[1], littleSqare[2], littleSqare[0]);
}

void fillRect(SDL_Renderer* renderer, primitive::Point a, primitive::Point b,
              primitive::Point c){
    double Cx_ab, Cy_ab;
    double step = 1;
    double sideLength = getLengthOfVector(a, b);
    std::tie(Cx_ab, Cy_ab) = getXYOffsetOnVector(a, b, step/2);
    int index = 1;
    figure::FactoryShape factory{renderer};
    while (sideLength >= 0){
        factory.line({a.x - Cx_ab*index, a.y - Cy_ab*index},
                     {c.x - Cx_ab*index, c.y - Cy_ab*index}).draw();
        sideLength -= step;
        ++index;
    }
}

std::pair<primitive::Point, primitive::Point> getPerpendicularLineByPoint(
    primitive::Point px, primitive::Point tp1, primitive::Point tp2, double length_of_base) {
    double Cx, Cy;
    double angle = M_PI_2;

    std::tie(Cx, Cy) = getXYOffsetOnVector(px, tp1, length_of_base); // to make sure pz will be found

    primitive::Point px2{tp1.x - Cx, tp1.y - Cy};
    primitive::Point pz = getTwoLinesIntersaction(px, px2.rotate(px, angle), tp1, tp2);

    return std::make_pair(px, pz);
}

void updateSkeleton(ColorGenerator * cg, SDL_Renderer* renderer, double angle,
                    double length_of_base, primitive::Point top_point, primitive::Point down_point,
                    primitive::Point pz, double block_hypotenuse, bool symmetrical, bool random_color) {

    double blockSize = sqrt(pow(block_hypotenuse, 2)/2);
    double length = getLengthOfVector(top_point, down_point);
    double Cx, Cy;
    std::tie(Cx, Cy) = getXYOffsetOnVector(top_point, down_point, blockSize);

    double littleHypotenuse = sqrt(pow(Cx, 2) + pow(Cy, 2));
    int index = 0;
    primitive::Point px1, px2;
    double Vx, Vy;
    double ribLength;
    primitive::Color tmpColor;
    cg->setToEnd();
    figure::FactoryShape factory{renderer};
    while (length >= 0){
        if (random_color){
            tmpColor = cg->getNextColor();
            factory.color(tmpColor);
        }
        primitive::Point vertebra{top_point.x - Cx*index, top_point.y - Cy*index};
        putSquareOnPoint(renderer, vertebra, block_hypotenuse, angle);

        length -= littleHypotenuse;
        ++index;

        std::tie(px1, px2) = getPerpendicularLineByPoint(vertebra, top_point, pz, length_of_base);
        std::tie(Vx, Vy) = getXYOffsetOnVector(px1, px2, blockSize);

        ribLength = getLengthOfVector(px1, px2);
        int vIndex = 1;
        while (ribLength >= 0){
            if (random_color){
                tmpColor = cg->getNextColor();
                factory.color(tmpColor);
            }
            primitive::Point tmpVertebraRight{px1.x - Vx*vIndex, px1.y - Vy*vIndex};
            putSquareOnPoint(renderer, tmpVertebraRight, block_hypotenuse, angle);
            if (symmetrical){
                primitive::Point tmpVertebraLeft{px1.x + Vx*vIndex, px1.y + Vy*vIndex};
                putSquareOnPoint(renderer, tmpVertebraLeft, block_hypotenuse, angle);
            }
            ribLength -= littleHypotenuse;
            ++vIndex;
        }
    }
}
