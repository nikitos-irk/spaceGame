#include "common.hpp"

#include <tuple>

#include <SDL2/SDL.h>

#include "colorschema.hpp"

Point get_rotated_point(Point center_point, Point rotated_point, double angle){
    if (0 == angle){
        angle = (rand() % 360)/M_PI;
    }
    point P(center_point.x, center_point.y);
    point Q(rotated_point.x, rotated_point.y);
    point P_rotated = (P-Q) * std::polar(1.0, angle) + Q;
    return Point(P_rotated.real(), P_rotated.imag());
}

DirectionXY::DirectionXY(double x, double y){
    this->x = x;
    this->y = y;
}

DirectionXY::DirectionXY(){
    this->x = 0;
    this->y = 0;
}

Common::Common() {

}

std::pair<double, double> getXYOffsetOnVector(Point px1, Point px2, double offset_length){
    double length = getLengthOfVector(px1, px2);
    double Cx = (px1.x - px2.x) / (length/(offset_length*2));
    double Cy = (px1.y - px2.y) / (length/(offset_length*2));
    return std::make_pair(Cx, Cy);
}

Point getTwoLinesIntersaction(Point p1, Point p2, Point p3, Point p4){
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

    return Point(px, py);
}

void putSquareOnPoint(SDL_Renderer* renderer, Point center_point, double block_hypotenuse_length, double angle){
    
    std::vector<Point> littleSqare;
    Point tmp = center_point;
    tmp.y -= block_hypotenuse_length;

    for (int i = 0; i < 4; ++i){
        double theta = M_PI/4 + i*M_PI/2 - angle; // getTiltAngel();
        littleSqare.push_back(get_rotated_point(tmp, center_point, theta));
    }
    auto iter = littleSqare.begin();
    for (; iter != littleSqare.end() - 1; ++iter){
        SDL_RenderDrawLine(renderer, iter->x, iter->y, (iter+1)->x, (iter+1)->y);
    }
    SDL_RenderDrawLine(renderer, iter->x, iter->y, littleSqare.begin()->x, littleSqare.begin()->y);
    fillRect(renderer, littleSqare[1], littleSqare[2], littleSqare[0]);
}

void fillRect(SDL_Renderer* renderer, Point a, Point b, Point c){
    double Cx_ab, Cy_ab;
    double step = 1;
    double sideLength = getLengthOfVector(a, b);
    std::tie(Cx_ab, Cy_ab) = getXYOffsetOnVector(a, b, step/2);
    int index = 1;
    while (sideLength >= 0){
        SDL_RenderDrawLine(renderer, a.x - Cx_ab*index, a.y - Cy_ab*index, c.x - Cx_ab*index, c.y - Cy_ab*index);
        sideLength -= step;
        ++index;
    }
}

std::pair<Point, Point> getPerpendicularLineByPoint(Point px, Point tp1, Point tp2, double length_of_base){
    double Cx, Cy;
    double angle = M_PI_2;

    std::tie(Cx, Cy) = getXYOffsetOnVector(px, tp1, length_of_base); // to make sure pz will be found

    Point px2 = get_rotated_point(Point(tp1.x - Cx, tp1.y - Cy), px, angle);
    Point pz = getTwoLinesIntersaction(px, px2, tp1, tp2);

    return std::make_pair(px, pz);
}

void updateSkeleton(ColorGenerator * cg, SDL_Renderer* renderer, double angle,
                    double length_of_base, Point top_point, Point down_point,
                    Point pz, double block_hypotenuse, bool symmetrical, bool random_color){

    double blockSize = sqrt(pow(block_hypotenuse, 2)/2);
    double length = getLengthOfVector(top_point, down_point);
    double Cx, Cy;
    std::tie(Cx, Cy) = getXYOffsetOnVector(top_point, down_point, blockSize);

    double littleHypotenuse = sqrt(pow(Cx, 2) + pow(Cy, 2));
    int index = 0;
    Point px1, px2;
    double Vx, Vy;
    double ribLength;
    Color tmpColor;
    cg->setToEnd();
    while (length >= 0){
        if (random_color){
            tmpColor = cg->getNextColor();
            SDL_SetRenderDrawColor(renderer, tmpColor.r, tmpColor.g, tmpColor.b, 255);
        }
        Point vertebra(top_point.x - Cx*index, top_point.y - Cy*index);
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
                SDL_SetRenderDrawColor(renderer, tmpColor.r, tmpColor.g, tmpColor.b, 255);
            }
            Point tmpVertebraRight(px1.x - Vx*vIndex, px1.y - Vy*vIndex);
            putSquareOnPoint(renderer, tmpVertebraRight, block_hypotenuse, angle);
            if (symmetrical){
                Point tmpVertebraLeft(px1.x + Vx*vIndex, px1.y + Vy*vIndex);
                putSquareOnPoint(renderer, tmpVertebraLeft, block_hypotenuse, angle);
            }
            ribLength -= littleHypotenuse;
            ++vIndex;
        }
    }
}
