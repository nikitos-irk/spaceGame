#include "nozzle.hpp"

namespace space {
Nozzle::Nozzle(primitive::Point coordinate)
    : border_{coordinate,
              {coordinate.x - double(size.width)/2,
               coordinate.y + double(size.height)},
              {coordinate.x + double(size.width)/2,
               coordinate.y + double(size.height)}} {}

double Nozzle::getTiltAngel() const
{
    const auto& center = border_.front();
    primitive::Point tmp{center.x, center.y - 10};
    primitive::Point middle{(*(border_.begin()+1)).x/2 + (*(border_.begin()+2)).x/2,
                           (*(border_.begin()+1)).y/2 + (*(border_.begin()+2)).y/2};

    double x1 = center.x;
    double y1 = center.y;
    double x2 = center.x;
    double y2 = center.y - 10;
    double x3 = center.x;
    double y3 = center.y;
    double x4 = middle.x;
    double y4 = middle.y;

    return atan((y2 - y1)/(x2 - x1)) - atan((y4 - y3)/(x4 - x3));
}

void Nozzle::update()
{
    double offsetLength = 1.0; //speed->get_current_a();
//    points.clear();
//    copy(origin_points.begin(), origin_points.end(), back_inserter(points));

//    if (offsetLength < 0.001) { return; }

    double Cx = (border_[1].x - border_[0].x) * (offsetLength);
    double Cy = (border_[1].y - border_[0].y) * (offsetLength);

    double Cx2 = (border_[0].x - border_[2].x) * (offsetLength);
    double Cy2 = (border_[0].y - border_[2].y) * (offsetLength);

    border_[1].x += Cx;
    border_[1].y += Cy;

    border_[2].x -= Cx2;
    border_[2].y -= Cy2;
}
}  // namespace space
