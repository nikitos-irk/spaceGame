#include "common.hpp"

Point get_rotated_point(Point center_point, Point rotated_point, double angle){
    if (0 == angle){
        angle = (rand() % 360)/M_PI;
    }
    point P(center_point.x, center_point.y);
    point Q(rotated_point.x, rotated_point.y);
    point P_rotated = (P-Q) * polar(1.0, angle) + Q;
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

pair<double, double> getXYOffsetOnVector(Point px1, Point px2, double offsetLength){
    double length = getLengthOfVector(px1, px2);
    double Cx = (px1.x - px2.x) / (length/(offsetLength*2));
    double Cy = (px1.y - px2.y) / (length/(offsetLength*2));
    return make_pair(Cx, Cy);
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

void putSquareOnPoint(SDL_Renderer* renderer, Point centerPoint, double blockHypotenuseLength, double angle){
    
    vector<Point> littleSqare;
    Point tmp = centerPoint;
    tmp.y -= blockHypotenuseLength;

    for (int i = 0; i < 4; ++i){
        double theta = M_PI/4 + i*M_PI/2 - angle; // getTiltAngel();
        littleSqare.push_back(get_rotated_point(tmp, centerPoint, theta));
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
    tie(Cx_ab, Cy_ab) = getXYOffsetOnVector(a, b, step/2);
    int index = 1;
    while (sideLength >= 0){
        SDL_RenderDrawLine(renderer, a.x - Cx_ab*index, a.y - Cy_ab*index, c.x - Cx_ab*index, c.y - Cy_ab*index);
        sideLength -= step;
        ++index;
    }
}

// void updateSkeleton(
//     Point topPoint,
//     Point downPoint, 
//     Point pz, 
//     double blockHypotenuse, 
//     bool symmetrical, 
//     bool randomColor){

//     double blockSize = sqrt(pow(blockHypotenuse, 2)/2);
//     double length = getLengthOfVector(topPoint, downPoint);
//     double Cx, Cy;
//     tie(Cx, Cy) = getXYOffsetOnVector(topPoint, downPoint, blockSize);

//     double littleHypotenuse = sqrt(pow(Cx, 2) + pow(Cy, 2));
//     int index = 0;
//     Point px1, px2;
//     double Vx, Vy;
//     double ribLength;
//     Color tmpColor;
//     colorIter = availableColors.end();
//     while (length >= 0){
//         if (randomColor){
//             tmpColor = getNextColor();
//             SDL_SetRenderDrawColor(renderer, tmpColor.r, tmpColor.g, tmpColor.b, 255);
//         }
//         Point vertebra(topPoint.x - Cx*index, topPoint.y - Cy*index);
//         putSquareOnPoint(vertebra, blockHypotenuse, getTiltAngel());

//         length -= littleHypotenuse;
//         ++index;

//         tie(px1, px2) = getPerpendicularLineByPoint(vertebra, topPoint, pz);
//         tie(Vx, Vy) = getXYOffsetOnVector(px1, px2, blockSize);

//         ribLength = getLengthOfVector(px1, px2);
//         int vIndex = 1;
//         while (ribLength >= 0){
//             if (randomColor){
//                 tmpColor = getNextColor();
//                 SDL_SetRenderDrawColor(renderer, tmpColor.r, tmpColor.g, tmpColor.b, 255);
//             }
//             Point tmpVertebraRight(px1.x - Vx*vIndex, px1.y - Vy*vIndex);
//             putSquareOnPoint(tmpVertebraRight, blockHypotenuse, getTiltAngel());
//             if (symmetrical){
//                 Point tmpVertebraLeft(px1.x + Vx*vIndex, px1.y + Vy*vIndex);
//                 putSquareOnPoint(tmpVertebraLeft, blockHypotenuse, getTiltAngel());
//             }
//             ribLength -= littleHypotenuse;
//             ++vIndex;
//         }
//     }
// }
