#include "space_ship.hpp"

#include <cstdlib>
#include <iostream>

constexpr auto kBlockSize = 5;
constexpr auto kRotationDelay = 60;
constexpr auto kDisplayDelay = 10;
constexpr auto kShootingDelay = 100;
constexpr auto kProjLifetime = 5000;
constexpr auto kShipColorChange = 50;

SpaceObject::SpaceObject(SDL_Renderer *renderer, int screen_width, int screen_height, int x, int y){
    this->renderer_ = renderer;
    this->x_ = x;
    this->y_ = y;
    this->screen_width_ = screen_width;
    this->screen_height_ = screen_height;
    display_delay_ = NOW + static_cast<std::chrono::milliseconds> (kDisplayDelay);
    rotation_delay_ = NOW + static_cast<std::chrono::milliseconds> (kRotationDelay);
    alive_ = true;
}

SpaceObject::~SpaceObject(){
    std::cout << "SpaceObject destructor" << std::endl;
}

bool SpaceObject::isAlive(){ return alive_; }
void SpaceObject::markAsDead() { alive_ = false; }

Projectile::Projectile(SDL_Renderer *renderer, int screen_width, int screen_height,
                       int offset_x, int offset_y, int x, int y)
    : SpaceObject::SpaceObject(renderer, screen_width, screen_height, x, y){
    direction_x_ = offset_x;
    direction_y_ = offset_y;
    this->x_ = x;
    this->y_ = y;
    life_time_ = NOW + static_cast<std::chrono::milliseconds> (kProjLifetime);
}

Point* Projectile::getXY(){ return new Point(x_, y_); }
std::pair<Point, Point> Projectile::getLine(){return std::make_pair(Point(x_, y_), Point(x_previous_, y_previous_));}

Projectile::~Projectile(){
    std::cout << "Projectile destructor" << std::endl;
}

Asteroid::Asteroid(SDL_Renderer *renderer, int screen_width, int screen_height, int x, int y)
    : SpaceObject::SpaceObject(renderer, screen_width, screen_height, x, y){
    //TODO: think about direction_x;direction_y

    int error_x = rand() % 10;
    int error_y = rand() % 10;
    int size = rand() % 20;
    cg = new ColorGeneratorAsteroid();

    pp_.push_back(new Point(x + error_x,                 y + error_y));
    pp_.push_back(new Point(x - size + rand() % 5,       y + size + rand() % 5));
    pp_.push_back(new Point(x + rand() % 5,              y + size * 2 + rand() % 5));
    pp_.push_back(new Point(x + size + rand() % 5,       y + size * 2 + rand() % 5));
    pp_.push_back(new Point(x + size * 2 + rand() % 5,   y + size + rand() % 5));
    pp_.push_back(new Point(x + size + rand() % 5,       y + rand() % 5));
}

std::vector<Point*>& Asteroid::get_points(){ return pp_; }

Asteroid::~Asteroid(){
    std::cout << "Asteroid destructor" << std::endl;
}

void out(double x, double y){
    std::cout << x << ":" << y << std::endl;
}

Point SpaceShip::getMedianIntersaction(){
    double x1 = pp[0].x;
    double y1 = pp[0].y;
    double x2 = (pp[2].x + pp[1].x)/2;
    double y2 = (pp[2].y + pp[1].y)/2;

    double x3 = pp[1].x;
    double y3 = pp[1].y;
    double x4 = (pp[0].x + pp[2].x)/2;
    double y4 = (pp[0].y + pp[2].y)/2;

    return getTwoLinesIntersaction(Point(x1, y1), Point(x2, y2), Point(x3, y3), Point(x4, y4));
}

SpaceShip::SpaceShip(SDL_Renderer *renderer, int screen_width, int screen_height, int max_speed)
    : SpaceObject::SpaceObject(renderer, screen_width, screen_height, screen_width/2, screen_height/2){

    shoot_delay_ = NOW + static_cast<std::chrono::milliseconds> (kShootingDelay);
    ship_color_change_ = NOW + static_cast<std::chrono::milliseconds> (kShipColorChange);

    speed = new Speed(max_speed);

    space_width_ = 30;
    space_height_ = screen_height / 8;
    nozzle_min_height_ = 25;
//    nozzleMaxHeight = 25;
    nozzle_width_ = 8;

    cs_ = new ColorSchema(Color(255, 255, 0), Color(255,8,0));
    cg = new ColorGeneratorShip();

    // spaceship coordination
    pp.push_back(Point(screen_width/2, screen_height/2));
    pp.push_back(Point(screen_width/2 - space_width_/2, screen_height/2 + space_height_));
    pp.push_back(Point(screen_width/2 + space_width_/2, screen_height/2 + space_height_));

    left_nozzle_ = new Nozzle(
                renderer,
                Point(screen_width/2 + space_width_/2 + nozzle_width_ , screen_height/2 + space_height_ + nozzle_min_height_),
                Point(screen_width/2 + space_width_/2                , screen_height/2 + space_height_),
                Point(screen_width/2 + space_width_/2 - nozzle_width_  , screen_height/2 + space_height_ + nozzle_min_height_),
                speed
    );
    right_nozzle_ = new Nozzle(
                renderer,
                Point(screen_width/2 - space_width_/2 - nozzle_width_  , screen_height/2 + space_height_ + nozzle_min_height_),
                Point(screen_width/2 - space_width_/2                , screen_height/2 + space_height_),
                Point(screen_width/2 - space_width_/2 + nozzle_width_  , screen_height/2 + space_height_ + nozzle_min_height_),
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
double SpaceShip::getCurrentA(){
    return speed->get_current_a();
}

DirectionXY SpaceShip::getOffset(){
    return speed->getOffsetXY(getDerectionalVector());
}

DirectionalVector::DirectionalVector(){
    this->p1 = DirectionXY();
    this->p2 = DirectionXY();
}

DirectionalVector::DirectionalVector(DirectionXY p1, DirectionXY p2){
    this->p1 = p1;
    this->p2 = p2;
}

DirectionalVector SpaceShip::getDerectionalVector(){

    double mediana_x = pp[1].x/2 + pp[2].x/2;
    double mediana_y = pp[1].y/2 + pp[2].y/2;

    return DirectionalVector(DirectionXY(mediana_x, mediana_y),
                             DirectionXY(pp[0].x, pp[0].y));
}

SpaceShip::~SpaceShip(){
    std::cout << "SpaceShip destructor" << std::endl;
}

DirectionXY SpaceShip::getDirection(){
    double mediana_x = pp[1].x/2 + pp[2].x/2;
    double mediana_y = pp[1].y/2 + pp[2].y/2;

    double diff_x = (mediana_x - pp[0].x)/5;
    double diff_y = (mediana_y - pp[0].y)/5;

    return DirectionXY(diff_x, diff_y);
}

void SpaceObject::changePosition(DirectionXY directionXY){
}

void SpaceShip::changeY(bool forward){
    double mediana_x = pp[1].x/2 + pp[2].x/2;
    double mediana_y = pp[1].y/2 + pp[2].y/2;

    double diff_x = (mediana_x - pp[0].x) / 5;
    double diff_y = (mediana_y - pp[0].y) / 5;

    if (!forward){
      diff_x *= -1;
      diff_y *= -1;
    }

    for (auto iter = pp.begin(); iter != pp.end(); ++iter){
        iter->x += diff_x;
        iter->y += diff_y;
    }
}

void rotatePointsInVector(std::vector<Point> &vPoints, Point initialMedianIntersection, double angle){
    for (auto iter = vPoints.begin(); iter != vPoints.end(); ++iter){
        *iter = get_rotated_point(*iter, initialMedianIntersection, angle);
    }
}

void SpaceShip::changeX(bool clockwise){

    if (rotation_delay_ > NOW) { return; }
    double angle = M_PI/15;

    if (!clockwise){
        angle = -angle;
    }
    rotatePointsInVector(pp, initial_median_intersection_, angle);
    rotatePointsInVector(left_nozzle_->origin_points, initial_median_intersection_, angle);
    rotatePointsInVector(right_nozzle_->origin_points, initial_median_intersection_, angle);


    left_nozzle_->update();
    right_nozzle_->update();
    left_nozzle_->display();
    right_nozzle_->display();

    rotation_delay_ = NOW + static_cast<std::chrono::milliseconds> (kRotationDelay);
}

double SpaceShip::getTiltAngel(){
    Point centerTmp = *pp.begin();
    Point tmp(centerTmp.x, centerTmp.y - 10);
    Point middle((*(pp.begin()+1)).x/2 + (*(pp.begin()+2)).x/2, (*(pp.begin()+1)).y/2 + (*(pp.begin()+2)).y/2); // x4

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

// Length of base of the SpaceShip
double SpaceShip::getLengthOfBase(){ return getLengthOfVector(pp[1], pp[2]); }

void SpaceShip::display(bool displaySkeleton){
    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
    SDL_SetRenderDrawColor(renderer_, cs_->get_r(), cs_->get_g(), cs_->get_b(), 255);
//    cs->update();
    for (int k = 0; k < 1/*3*/; ++k){
        int i = 0;
        int kIndex = k*3;
        for (; i < 2; ++i){
            SDL_RenderDrawLine(renderer_, pp[i + kIndex].x, pp[i + kIndex].y, pp[i + kIndex + 1].x, pp[i + kIndex + 1].y);
        }
        SDL_RenderDrawLine(renderer_, pp[kIndex + 2].x, pp[kIndex + 2].y, pp[kIndex].x, pp[kIndex].y);
    }

    double lengthOfBase = getLengthOfBase();

    updateSkeleton(cg, renderer_, getTiltAngel(), lengthOfBase, pp[0], Point(pp[1].x/2 + pp[2].x/2, pp[1].y/2 + pp[2].y/2), pp[2], 4, true, true);
    left_nozzle_->update();
    right_nozzle_->update();
    left_nozzle_->display();
    right_nozzle_->display();
    Point a,b,c;
    a = left_nozzle_->points[0];
    b = left_nozzle_->points[1];
    c = left_nozzle_->points[2];
    updateSkeleton(cg, renderer_, getTiltAngel(), lengthOfBase, b, Point(a.x/2 + c.x/2, a.y/2 + c.y/2), a, 1, true, false);
    a = right_nozzle_->points[0];
    b = right_nozzle_->points[1];
    c = right_nozzle_->points[2];
    updateSkeleton(cg, renderer_, getTiltAngel(), lengthOfBase, b, Point(a.x/2 + c.x/2, a.y/2 + c.y/2), a, 1, true, false);

    SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
}

std::chrono::time_point<std::chrono::system_clock> Projectile::get_life_time(){ return life_time_; }

Projectile * SpaceShip::shoot(){
    if (this->shoot_delay_ > NOW) { return nullptr; }
    shoot_delay_ = NOW + static_cast<std::chrono::milliseconds> (kShootingDelay);

    double mediana_x = pp[1].x/2 + pp[2].x/2;
    double mediana_y = pp[1].y/2 + pp[2].y/2;
    double diff_x = (mediana_x - pp[0].x)/5;
    double diff_y = (mediana_y - pp[0].y)/5;

    Projectile *projectile = new Projectile(this->renderer_, 0, 0, diff_x, diff_y, pp[0].x - diff_x, pp[0].y - diff_y);
    return projectile;
}

void Projectile::display(bool display_skeleton){

    if (display_delay_ > NOW){ return; }

    double error = (double) - kBlockSize;
    double tmp_x = (double) kBlockSize - 0.5;
    double tmp_y = (double) 0.5;

    double cx = this->x_ - 0.5;
    double cy = this->y_ - 0.5;

    SDL_SetRenderDrawColor(renderer_, 255, 255, 0, 255);

    while (tmp_x >= tmp_y){
        SDL_RenderDrawPoint(renderer_, (int)(cx + tmp_x), (int)(cy + tmp_y));
        SDL_RenderDrawPoint(renderer_, (int)(cx + tmp_y), (int)(cy + tmp_x));

        if (tmp_x != 0){
            SDL_RenderDrawPoint(renderer_, (int)(cx - tmp_x), (int)(cy + tmp_y));
            SDL_RenderDrawPoint(renderer_, (int)(cx + tmp_y), (int)(cy - tmp_x));
        }

        if (tmp_y != 0){
            SDL_RenderDrawPoint(renderer_, (int)(cx + tmp_x), (int)(cy - tmp_y));
            SDL_RenderDrawPoint(renderer_, (int)(cx - tmp_y), (int)(cy + tmp_x));
        }

        if (tmp_x != 0 && tmp_y != 0){
            SDL_RenderDrawPoint(renderer_, (int)(cx - tmp_x), (int)(cy - tmp_y));
            SDL_RenderDrawPoint(renderer_, (int)(cx - tmp_y), (int)(cy - tmp_x));
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

    SDL_SetRenderDrawColor(renderer_, 128, 0, 128, 255);
    for (double dy = 1; dy <= kBlockSize; dy += 1.0){
        double dx = floor(sqrt((2.0 * kBlockSize * dy) - (dy * dy)));
        double cx = this->x_;
        double cy = this->y_;
        SDL_RenderDrawLine(renderer_, cx - dx, cy + dy - kBlockSize, cx + dx, cy + dy - kBlockSize);
        SDL_RenderDrawLine(renderer_, cx - dx, cy - dy + kBlockSize, cx + dx, cy - dy + kBlockSize);
    }
    this->x_ -= direction_x_;
    this->y_ -= direction_y_;
    display_delay_ = NOW + (std::chrono::milliseconds) (kDisplayDelay/5);
}

Point Asteroid::getCenterPoint(){
    double x = 0.0, y = 0.0;
    int size = pp_.size();
    for (auto iter = pp_.begin(); iter != pp_.end(); ++iter){
        x += (*iter)->x / size;
        y += (*iter)->y / size;
    }
    return Point(x, y);
}

void Asteroid::fill(){
    
    Point center_point = getCenterPoint();
    int blocksize = 3;

    auto iter = pp_.begin();
    auto iter_next = iter + 1;
    
    Point p1 = **iter;
    Point p2 = **iter_next;
    
    while (iter_next != pp_.end()){
        p1 = **iter;
        p2 = **iter_next;
        updateSkeleton(cg, renderer_, 0.0, getLengthOfVector(p1, p2), center_point, Point((p1.x + p2.x)/2, (p1.y + p2.y)/2), p1, blocksize, false, true);
        ++iter;
        ++iter_next;
        // break;
    }
    iter_next = pp_.begin();
    p1 = **iter;
    p2 = **iter_next;
    updateSkeleton(cg, renderer_, 0.0, getLengthOfVector(p1, p2), center_point, Point((p1.x + p2.x)/2, (p1.y + p2.y)/2), p1, blocksize, false, true);
}

void Asteroid::display(bool display_skeleton){

    if (display_skeleton){
        SDL_SetRenderDrawColor(renderer_, 0, 0, 255, 255);
        auto iter = pp_.begin();
        Point *p1;
        Point *p2;
        for (; iter != pp_.end()-1; ++iter){
            p1 = *iter;
            p2 = *(iter+1);
            SDL_RenderDrawLine(renderer_, p1->x, p1->y, p2->x, p2->y);
        }
        p1 = *pp_.begin();
        SDL_RenderDrawLine(renderer_, p1->x, p1->y, p2->x, p2->y);
    }

    fill();
}

void Asteroid::changePosition(DirectionXY direction_xy){
    Point *p1;
    for (auto iter = pp_.begin(); iter != pp_.end(); ++iter){
        p1 = *iter;
        p1->x += direction_xy.x;
        p1->y += direction_xy.y;
    }
}

Point* Asteroid::getFirstPoint(){
    return pp_[0];
}

void Projectile::changePosition(DirectionXY direction_xy){
    this->x_previous_ = x_;
    this->y_previous_ = y_;
    this->x_ += direction_xy.x;
    this->y_ += direction_xy.y;
}
