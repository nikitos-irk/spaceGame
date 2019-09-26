#include "space_ship.hpp"

#include <cstdlib>
#include <iostream>

#include "colorschema.hpp"
#include "primitive/line.hpp"
#include "skeleton.hpp"

constexpr auto kBlockSize = 5;
constexpr auto kRotationDelay = 60ms;
constexpr auto kDisplayDelay = 10ms;
constexpr auto kShootingDelay = 100ms;
constexpr auto kProjLifetime = 5s;

SpaceObject::SpaceObject(SDL_Renderer *renderer,
                         primitive::Size screen_size,
                         primitive::Point coordinate)
    : renderer_{renderer},
      coordinate_{coordinate},
      screen_size_{screen_size}
{
  display_delay_ = primitive::delay(kDisplayDelay);
  rotation_delay_ = primitive::delay(kRotationDelay);
}

SpaceObject::~SpaceObject(){
    std::cout << "SpaceObject destructor" << std::endl;
}

bool SpaceObject::isAlive(){ return alive_; }
void SpaceObject::markAsDead() { alive_ = false; }

Projectile::Projectile(SDL_Renderer *renderer, primitive::Size screen_size,
                       primitive::Direction offset, primitive::Point coordinate)
    : SpaceObject(renderer, screen_size, coordinate),
      direction_{offset} {
    life_time_ = primitive::delay(kProjLifetime);
}

primitive::Point* Projectile::getXY(){ return &coordinate_; }
std::pair<primitive::Point, primitive::Point> Projectile::getLine()
{ return std::make_pair(coordinate_, previous_); }

Projectile::~Projectile(){
    std::cout << "Projectile destructor" << std::endl;
}

Asteroid::Asteroid(SDL_Renderer *renderer,
                   primitive::Size screen_size,
                   primitive::Point coordinate)
    : SpaceObject(renderer, screen_size, coordinate),
      cg_{{160, 177, 188}, {119, 98, 84}, {128, 0, 0}, {81, 81, 81},
          {76, 62, 54}, {139, 69, 19}}
{
    //TODO: think about direction_x;direction_y

    int error_x = rand() % 10;
    int error_y = rand() % 10;
    int size = rand() % 20;

    pp_.push_back(new primitive::Point{coordinate_.x + error_x, coordinate_.y + error_y});
    pp_.push_back(new primitive::Point{coordinate_.x - size + rand() % 5, coordinate_.y + size + rand() % 5});
    pp_.push_back(new primitive::Point{coordinate_.x + rand() % 5, coordinate_.y + size * 2 + rand() % 5});
    pp_.push_back(new primitive::Point{coordinate_.x + size + rand() % 5, coordinate_.y + size * 2 + rand() % 5});
    pp_.push_back(new primitive::Point{coordinate_.x + size * 2 + rand() % 5, coordinate_.y + size + rand() % 5});
    pp_.push_back(new primitive::Point{coordinate_.x + size + rand() % 5, coordinate_.y + rand() % 5});
}

std::vector<primitive::Point*>& Asteroid::get_points(){ return pp_; }

Asteroid::~Asteroid(){
    std::cout << "Asteroid destructor" << std::endl;
}

void out(double x, double y){
    std::cout << x << ":" << y << std::endl;
}

primitive::Point SpaceShip::getMedianIntersaction(){
    double x1 = pp[0].x;
    double y1 = pp[0].y;
    double x2 = (pp[2].x + pp[1].x)/2;
    double y2 = (pp[2].y + pp[1].y)/2;

    double x3 = pp[1].x;
    double y3 = pp[1].y;
    double x4 = (pp[0].x + pp[2].x)/2;
    double y4 = (pp[0].y + pp[2].y)/2;

    return primitive::Line{{x1, y1}, {x2, y2}}.intersect({{x3, y3}, {x4, y4}});
}

SpaceShip::SpaceShip(SDL_Renderer *renderer, primitive::Size screen_size, int max_speed)
    : SpaceObject(renderer, screen_size,
        {double(screen_size.width)/2, double(screen_size.height)/2})
{
    constexpr primitive::Size space_size{30, 60};
    constexpr primitive::Size nozzle_size{8, 0};

    shoot_delay_ = primitive::delay(kShootingDelay);

    speed = new Speed(max_speed);

    auto nozzle_min_height = 25.0;

    // spaceship coordination
    pp.push_back({double(screen_size.width)/2, double(screen_size.height)/2});
    pp.push_back({double(screen_size.width)/2 - double(space_size.width)/2,
                  double(screen_size.height)/2 + double(space_size.height)});
    pp.push_back({double(screen_size.width)/2 + double(space_size.width)/2,
                  double(screen_size.height)/2 + double(space_size.height)});

    left_nozzle_ = new Nozzle(
                renderer,
                {double(screen_size.width/2 + space_size.width/2 + nozzle_size.width),
                 double(screen_size.height/2 + space_size.height + nozzle_min_height)},
                {double(screen_size.width/2 + space_size.width/2),
                 double(screen_size.height/2 + space_size.height)},
                {double(screen_size.width/2 + space_size.width/2 - nozzle_size.width),
                 double(screen_size.height/2 + space_size.height + nozzle_min_height)},
                speed
    );
    right_nozzle_ = new Nozzle(
                renderer,
                {double(screen_size.width/2 - space_size.width/2 - nozzle_size.width),
                 double(screen_size.height/2 + space_size.height + nozzle_min_height)},
                {double(screen_size.width/2 - space_size.width/2),
                 double(screen_size.height/2 + space_size.height)},
                {double(screen_size.width/2 - space_size.width/2 + nozzle_size.width),
                 double(screen_size.height/2 + space_size.height + nozzle_min_height)},
                speed
    );
    initial_median_intersection_ = getMedianIntersaction();
}

void SpaceShip::slowdown(){
    speed->slowdown();
}
void SpaceShip::accelarate(){
    speed->accelarate();
}
void SpaceShip::backwardSlowdown(){
    speed->backwardSlowdown();
}
void SpaceShip::backwardAccelarate(){
    speed->backwardAccelarate();
}

primitive::Direction SpaceShip::getOffset()
{
  left_nozzle_->update();
  right_nozzle_->update();
  auto mediana = primitive::median(pp[1], pp[2]);
  return speed->getOffsetXY({mediana.x, mediana.y}, {pp[0].x, pp[0].y});
}

SpaceShip::~SpaceShip(){
    std::cout << "SpaceShip destructor" << std::endl;
}

primitive::Direction SpaceShip::getDirection(){
    auto mediana = primitive::median(pp[1], pp[2]);

    double diff_x = (mediana.x - pp[0].x)/5;
    double diff_y = (mediana.y - pp[0].y)/5;

    return {diff_x, diff_y};
}

void SpaceShip::changeY(bool forward){
    auto mediana = primitive::median(pp[1], pp[2]);

    double diff_x = (mediana.x - pp[0].x) / 5;
    double diff_y = (mediana.y - pp[0].y) / 5;

    if (!forward){
      diff_x *= -1;
      diff_y *= -1;
    }

    for (auto iter = pp.begin(); iter != pp.end(); ++iter){
        iter->x += diff_x;
        iter->y += diff_y;
    }
}

void SpaceShip::rotatePointsInVector(std::vector<primitive::Point> &points,
                          primitive::Point initial_median_intersection, double angle){
    for (auto iter = points.begin(); iter != points.end(); ++iter) {
        iter->rotate(initial_median_intersection, angle);
    }
}

void SpaceShip::changeX(bool clockwise){

    if (rotation_delay_ > primitive::now()) { return; }
    double angle = M_PI/15;

    if (!clockwise){
        angle = -angle;
    }
    rotatePointsInVector(pp, initial_median_intersection_, angle);
    rotatePointsInVector(left_nozzle_->origin_points, initial_median_intersection_, angle);
    rotatePointsInVector(right_nozzle_->origin_points, initial_median_intersection_, angle);

    rotation_delay_ = primitive::delay(kRotationDelay);
}

double SpaceShip::getTiltAngel(){
    primitive::Point centerTmp = *pp.begin();
    primitive::Point tmp{centerTmp.x, centerTmp.y - 10};
    primitive::Point middle{(*(pp.begin()+1)).x/2 + (*(pp.begin()+2)).x/2,
                            (*(pp.begin()+1)).y/2 + (*(pp.begin()+2)).y/2}; // x4

    double x1 = centerTmp.x;
    double y1 = centerTmp.y;
    double x2 = centerTmp.x;
    double y2 = centerTmp.y - 10;
    double x3 = centerTmp.x;
    double y3 = centerTmp.y;
    double x4 = middle.x;
    double y4 = middle.y;

    return atan((y2 - y1)/(x2 - x1)) - atan((y4 - y3)/(x4 - x3));
}

void SpaceShip::display()
{
    figure::FactoryShape factory{renderer_};
    factory.color(colors_.get_color()).polygon(pp).draw();

    double lengthOfBase{30.0};  // space_size.width

    Skeleton skeleton{renderer_, ColorGenerator{{220,220,220}, {192,192,192},
                                                {105,105,105}, {211,211,211},
                                                {119,136,153}}};
    skeleton.update(getTiltAngel(), lengthOfBase, pp[0],
                   primitive::Point{pp[1].x/2 + pp[2].x/2, pp[1].y/2 + pp[2].y/2}, pp[2], 4, true, true);
    left_nozzle_->display();
    right_nozzle_->display();
    primitive::Point a,b,c;
    a = left_nozzle_->points[0];
    b = left_nozzle_->points[1];
    c = left_nozzle_->points[2];
    skeleton.update(getTiltAngel(), lengthOfBase, b,
                   primitive::Point{a.x/2 + c.x/2, a.y/2 + c.y/2}, a, 1, true, false);
    a = right_nozzle_->points[0];
    b = right_nozzle_->points[1];
    c = right_nozzle_->points[2];
    skeleton.update(getTiltAngel(), lengthOfBase, b,
                   primitive::Point{a.x/2 + c.x/2, a.y/2 + c.y/2}, a, 1, true, false);
}

primitive::Time Projectile::get_life_time(){ return life_time_; }

std::unique_ptr<Projectile> SpaceShip::shoot(){
    if (shoot_delay_ > primitive::now()) { return nullptr; }
    shoot_delay_ = primitive::delay(kShootingDelay);

    auto mediana = primitive::median(pp[1], pp[2]);
    double diff_x = (mediana.x - pp[0].x)/5;
    double diff_y = (mediana.y - pp[0].y)/5;

    auto ball = std::unique_ptr<Projectile>{new Projectile{renderer_, primitive::Size{0, 0},
        {-diff_x, -diff_y}, {pp[0].x - diff_x, pp[0].y - diff_y}}};
    return ball;
}

void Projectile::display()
{
    if (display_delay_ > primitive::now()){ return; }

    double error = (double) - kBlockSize;
    double tmp_x = (double) kBlockSize - 0.5;
    double tmp_y = (double) 0.5;

    double cx = coordinate_.x - 0.5;
    double cy = coordinate_.y - 0.5;

    figure::FactoryShape factory{renderer_};
    factory.color({255, 255, 0, 255});
    while (tmp_x >= tmp_y){
        factory.point({cx + tmp_x, cy + tmp_y}).draw();
        factory.point({cx + tmp_y, cy + tmp_x}).draw();

        if (tmp_x != 0){
            factory.point({cx - tmp_x, cy + tmp_y}).draw();
            factory.point({cx + tmp_y, cy - tmp_x}).draw();
        }

        if (tmp_y != 0){
            factory.point({cx + tmp_x, cy - tmp_y}).draw();
            factory.point({cx - tmp_y, cy + tmp_x}).draw();
        }

        if (tmp_x != 0 && tmp_y != 0){
            factory.point({cx - tmp_x, cy - tmp_y}).draw();
            factory.point({cx - tmp_y, cy - tmp_x}).draw();
        }

        error += tmp_y;
        ++tmp_y;
        error += tmp_y;

        if (error >= 0){
            --tmp_x;
            error -= tmp_x;
            error -= tmp_x;
        }
    }

    factory.color({128, 0, 128, 255});
    for (double dy = 1; dy <= kBlockSize; dy += 1.0){
        double dx = floor(sqrt((2.0 * kBlockSize * dy) - (dy * dy)));
        double cx = coordinate_.x;
        double cy = coordinate_.y;
        factory.line({cx - dx, cy + dy - kBlockSize},
                     {cx + dx, cy + dy - kBlockSize}).draw();
        factory.line({cx - dx, cy - dy + kBlockSize},
                     {cx + dx, cy - dy + kBlockSize}).draw();
    }
    coordinate_.move(direction_);
    display_delay_ = primitive::delay(kDisplayDelay/5);
}

primitive::Point Asteroid::getCenterPoint(){
    double x = 0.0, y = 0.0;
    int size = pp_.size();
    for (auto iter = pp_.begin(); iter != pp_.end(); ++iter){
        x += (*iter)->x / size;
        y += (*iter)->y / size;
    }
    return primitive::Point{x, y};
}

void Asteroid::fill(){
    
  primitive::Point center_point = getCenterPoint();
    int blocksize = 3;

    auto iter = pp_.begin();
    auto iter_next = iter + 1;
    
    primitive::Point p1 = **iter;
    primitive::Point p2 = **iter_next;
    
    Skeleton skeleton{renderer_, cg_};
    while (iter_next != pp_.end()){
        p1 = **iter;
        p2 = **iter_next;
        skeleton.update(0.0, primitive::Line{p1, p2}.length(), center_point,
                       primitive::Point{(p1.x + p2.x)/2, (p1.y + p2.y)/2}, p1, blocksize, false, true);
        ++iter;
        ++iter_next;
        // break;
    }
    iter_next = pp_.begin();
    p1 = **iter;
    p2 = **iter_next;
    skeleton.update(0.0, primitive::Line{p1, p2}.length(), center_point,
                   primitive::Point{(p1.x + p2.x)/2, (p1.y + p2.y)/2}, p1, blocksize, false, true);
}

void Asteroid::display()
{
    auto iter = pp_.begin();
    primitive::Point *p1;
    primitive::Point *p2;
    figure::FactoryShape factory{renderer_};
    factory.color({0, 0, 255, 255});
    for (; iter != pp_.end()-1; ++iter){
        p1 = *iter;
        p2 = *(iter+1);
        factory.line(*p1, *p2).draw();
    }
    p1 = *pp_.begin();
    factory.line(*p1, *p2).draw();
    fill();
}

void Asteroid::changePosition(primitive::Direction direction_xy){
    for (auto iter = pp_.begin(); iter != pp_.end(); ++iter){
        (*iter)->move(direction_xy);
    }
}

primitive::Point* Asteroid::getFirstPoint(){
    return pp_[0];
}

void Projectile::changePosition(primitive::Direction direction_xy){
    previous_ = coordinate_;
    coordinate_.move(direction_xy);
}
