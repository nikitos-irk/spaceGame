#include "game.hpp"

#include <iostream>
#include <thread>

#include <unistd.h>

#include <SDL2/SDL.h>

#include "background.hpp"
#include "explosion.hpp"
#include "space_ship.hpp"

using namespace std::chrono;

constexpr auto kAsteroidsRemovingDelay = 10ms;
constexpr auto kChangePositionDelay = 30ms;
constexpr auto kInertiaDelay = 10ms;

Game::Game(SDL_Renderer* renderer, primitive::Size size, int live_amount)
    : renderer_{renderer},
      screen_size_{size},
      live_amount_{live_amount}
{
  // Hope I will think something more interesting background than it is now
  my_background_ = new Background(renderer, screen_size_);

  // Create ship
  my_ship_ = new SpaceShip(renderer, screen_size_, 50);

  // Create asteroids
  for (int i = 0; i < 10; ++i) {
      int tmp_x = rand() % screen_size_.width;
      int tmp_y = rand() % screen_size_.height;
      asteroids_.push_back(new Asteroid(renderer, screen_size_, tmp_x, tmp_y));
  }

  // Initiating delays
  change_position_delay_ = NOW + kChangePositionDelay;
  inertia_delay_ = NOW + kInertiaDelay;
  update_asteroids_delay_ = NOW + kAsteroidsRemovingDelay;
}

void Game::createAsteroid(){
    double theta = (rand() % 360)/M_PI;
    Point ship_center = my_ship_->getMedianIntersaction();
    double tmp_x = (rand() % screen_size_.width) + screen_size_.width;
    double tmp_y = ship_center.y;

    point P(tmp_x, tmp_y);
    point Q(ship_center.x, ship_center.y);
    point P_rotated = (P-Q) * std::polar(1.0, theta) + Q;
    tmp_x = P_rotated.real();
    tmp_y = P_rotated.imag();

    asteroids_.push_back(new Asteroid(renderer_, screen_size_, tmp_x, tmp_y));
}

void Game::updateAsteroids(){
    if (update_asteroids_delay_ >= NOW){ return; }
    Point tmp_p = my_ship_->getMedianIntersaction();
    Point *ap = nullptr;
    double distance;
    double diagonal = sqrt(pow(screen_size_.width, 2) + pow(screen_size_.height, 2));
    auto iter = asteroids_.begin();
    while (iter != asteroids_.end())
    {
        ap = dynamic_cast<Asteroid*>(*iter)->getFirstPoint();
        distance = sqrt(pow(tmp_p.x - ap->x, 2) + pow(tmp_p.y - ap->y, 2));
        if (distance >= 1.5 * diagonal){
            SpaceObject *tmp = *iter;
            asteroids_.erase(iter++);
            delete tmp;
        } else {
            ++iter;
        }
    }
    update_asteroids_delay_ = NOW + kAsteroidsRemovingDelay;
}

void Game::updateProjectiles(){
    auto iter = projectiles_.begin();
    while (iter != projectiles_.end())
    {
        if (dynamic_cast<Projectile*>(*iter)->get_life_time() < NOW){
            SpaceObject *tmp = *iter;
            projectiles_.erase(iter++);
            delete tmp;
        } else {
            ++iter;
        }
    }
}

bool dot_on_line(Point *p1, Point *p2, Point *px)
{
    double k = (p2->y - p1->y) / (p2->x - p1->x);
    double c = p1->y - k * p1->x;
    return abs(px->y - (px->x * k + c)) < 0.1;
}

inline int det (int a, int b, int c, int d) {
    return a * d - b * c;
}

inline bool between (int a, int b, double c) {
    double epsilon = 1E-9;
    return std::min(a,b) <= c + epsilon && c <= std::max(a,b) + epsilon;
}

inline bool intersect_1 (int a, int b, int c, int d) {
    if (a > b)  std::swap(a, b);
    if (c > d)  std::swap(c, d);
    return std::max(a,c) <= std::min(b,d);
}


bool intersect (Point a, Point b, Point c, Point d) {
    int A1 = a.y-b.y,  B1 = b.x-a.x,  C1 = -A1*a.x - B1*a.y;
    int A2 = c.y-d.y,  B2 = d.x-c.x,  C2 = -A2*c.x - B2*c.y;
    int zn = det (A1, B1, A2, B2);
    if (zn != 0) {
        double x = - det (C1, B1, C2, B2) * 1. / zn;
        double y = - det (A1, C1, A2, C2) * 1. / zn;
        return between (a.x, b.x, x) && between (a.y, b.y, y)
            && between (c.x, d.x, x) && between (c.y, d.y, y);
    }
    else
        return det (A1, C1, A2, C2) == 0 && det (B1, C1, B2, C2) == 0
            && intersect_1 (a.x, b.x, c.x, d.x)
            && intersect_1 (a.y, b.y, c.y, d.y);
}

void Game::checkShipHits(){
    while (true){
        asteroids_mutex_.lock();
        shipHitsLoop();
        asteroids_mutex_.unlock();
        usleep(100);
    }
}

void Game::shipHitsLoop(){

    std::vector<Point*> tmpPoints;
    bool hitStatus = false;

    for (auto ast = asteroids_.begin(); ast != asteroids_.end(); ++ast){
        hitStatus = false;
        if (!(*ast)->isAlive()){ continue; }
        tmpPoints = dynamic_cast<Asteroid*>(*ast)->get_points();
        Point *p1, *p2;
        Point sp1, sp2;
        for (auto p = tmpPoints.begin(); p != tmpPoints.end() - 1; ++p){
            if (!hitStatus){
                p1 = *p;
                p2 = *(p+1);
                if (tmpPoints.end()-1 == p){
                    p2 = *tmpPoints.begin();
                } else {
                    p2 = *(p + 1);
                }
                for (auto spacePointsIter = my_ship_->pp.begin(); spacePointsIter != my_ship_->pp.end() - 1; ++spacePointsIter){
                    sp1 = *spacePointsIter;
                    sp2 = *(spacePointsIter+1);
                    if (my_ship_->pp.end()-1 == spacePointsIter){
                        sp2 = *my_ship_->pp.begin();
                    } else {
                        sp2 = *(spacePointsIter + 1);
                    }
                    hitStatus = intersect(*p1, *p2, sp1, sp2);
                    if (hitStatus){
                        live_amount_--;
                        if (!live_amount_){
                            try{
                                throw GameOverException();
                            }catch (...){
                                globalExceptionPtr = std::current_exception();
                            }
                        }
                        (*ast)->markAsDead();
                        break;
                    }
                }
            }
        }
    }
    cleanAsteroids();
}

void Game::checkHits(){
    while (true)
    {
        projectiles_mutex_.lock();
        asteroids_mutex_.lock();
        histLoop();
        asteroids_mutex_.unlock();
        projectiles_mutex_.unlock();
        usleep(100);
    }
}

void Game::histLoop(){

    std::vector<Point*> tmpPoints;
    bool hitStatus;

    for (auto pr = projectiles_.begin(); pr != projectiles_.end(); ++pr){
        if (!(*pr)->isAlive()) { continue; }
        hitStatus = false;
        for (auto ast = asteroids_.begin(); ast != asteroids_.end(); ++ast){
            if (!(*ast)->isAlive()) { continue; }
            tmpPoints = dynamic_cast<Asteroid*>(*ast)->get_points();
            Point *p1, *p2, *px;

            for (auto p = tmpPoints.begin(); p != tmpPoints.end() - 1; ++p){
                p1 = *p;
                p2 = *(p+1);
                if (tmpPoints.end() - 1 == p){
                    p2 = *tmpPoints.begin();
                } else {
                    p2 = *(p + 1);
                }

                px = dynamic_cast<Projectile*>(*pr)->getXY();
                std::pair<Point, Point> pLine = dynamic_cast<Projectile*>(*pr)->getLine();

                bool hitStatus = intersect(*p1, *p2, pLine.first, pLine.second);
                delete px;
                if ( hitStatus ){
                    (*ast)->markAsDead();
                    (*pr)->markAsDead();
                    break;
                }
            }
        }
    }
    cleanLoop();
}

void Game::generateExplosion(Asteroid *tmp_ast){
    double middle_x = 0.0, middle_y = 0.0;

    std::vector<Point*> tmpPoints = tmp_ast->get_points();
    for (auto iter = tmpPoints.begin(); iter != tmpPoints.end(); ++iter){
        middle_x += (*iter)->x;
        middle_y += (*iter)->y;
    }
    middle_x /= tmpPoints.size();
    middle_y /= tmpPoints.size();
    explosions_.push_back(new Explosion(Point(middle_x, middle_y), renderer_, tmp_ast));
}

void Game::cleanAsteroids(){
    auto ast = asteroids_.begin();
    while (ast != asteroids_.end()){
        if (!(*ast)->isAlive()){
            Asteroid *tmp_ast = dynamic_cast<Asteroid*>(*ast);
            generateExplosion(tmp_ast);
            asteroids_.erase(ast++);
            // delete tmp_ast; // will be remove in ~Explosion() 
        } else {
            ++ast;
        }
    }
}

void Game::cleanProjectiles(){
    auto pr = projectiles_.begin();
    while (pr != projectiles_.end()){
        if (!(*pr)->isAlive()){
            Projectile *tmp_pr = dynamic_cast<Projectile*>(*pr);
            projectiles_.erase(pr++);
            delete tmp_pr;
        } else {
            ++pr;
        }
    }
}

void Game::cleanExplosions(){
    auto expl = explosions_.begin();
    while (expl != explosions_.end()){
        if (!(*expl)->isAlive()){
            Explosion *tmp_expl = *expl;
            explosions_.erase(expl++);
            delete tmp_expl;
        } else {
            ++expl;
        }
    }
}

void Game::cleanLoop(){
    cleanAsteroids();
    cleanProjectiles();
}

void Game::displayObjects(bool display_skeletons=false){
    my_ship_->display(display_skeletons);

    for (auto spaceObject = asteroids_.begin(); spaceObject != asteroids_.end(); ++spaceObject){
        if ((*spaceObject)->isAlive()){
            (*spaceObject)->display(display_skeletons);
        }
    }
    for (auto spaceObject = projectiles_.begin(); spaceObject != projectiles_.end(); ++spaceObject){
        if ((*spaceObject)->isAlive()){
            (*spaceObject)->display(display_skeletons);
        }
    }

    for (auto explosion = explosions_.begin(); explosion != explosions_.end(); ++explosion){
        if ( (*explosion)->isAlive() ){
            (*explosion)->display(display_skeletons);
        }
    }
    cleanExplosions();
}

void Game::changeObjectsPositions(){

    if (change_position_delay_ > NOW){ return; }

    DirectionXY directionXY = my_ship_->getOffset();

    for (auto spaceObject = asteroids_.begin(); spaceObject != asteroids_.end(); ++spaceObject){
        (*spaceObject)->changePosition(directionXY);
    }
    for (auto spaceObject = projectiles_.begin(); spaceObject != projectiles_.end(); ++spaceObject){
        (*spaceObject)->changePosition(directionXY);
    }
    for (auto explosion = explosions_.begin(); explosion != explosions_.end(); ++explosion){
        if ( (*explosion)->isAlive() ){
            (*explosion)->shift(directionXY);
        }
    }

    change_position_delay_ = NOW + kChangePositionDelay;
}

void Game::displayLifeAmount(){
    SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 255);

    SDL_Rect rect;
    rect.x = 5;
    rect.y = screen_size_.height - 32;
    rect.w = 5 * 15 + 4;
    rect.h = 24;
    SDL_RenderFillRect(renderer_, &rect);

    for (int i = 0; i < live_amount_; ++i){
        rect.x = 10 + i * 15;
        rect.y = screen_size_.height - 30;
        rect.w = 10;
        rect.h = 20;
        SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer_, &rect);
    }
}

void Game::run(){

    my_background_->fillBackground();
    displayObjects();
    SDL_RenderPresent(renderer_);
    SDL_RenderClear(renderer_);

//    std::thread thUpdating(&Game::update, this);
  std::thread thHitsMonitoring(&Game::checkHits, this);
  std::thread thShipHitsMonitoring(&Game::checkShipHits, this);

//    thUpdating.join();
    thHitsMonitoring.detach();
    thShipHitsMonitoring.detach();
    int quit = 1;
    SpaceObject *tmp_space_obj;
    while(quit) {

        if (globalExceptionPtr)
          {
            try
            {
              std::rethrow_exception(globalExceptionPtr);
            }
            catch (const std::exception &ex)
            {
              std::cout << "Gema Over!" << std::endl;
              quit = false;
            }
          }

        projectiles_mutex_.lock();
        asteroids_mutex_.lock();
        while( SDL_PollEvent( &e_ ) != 0 ) {
            if( e_.type == SDL_QUIT ){
                quit = 0;
            } else if (e_.type == SDL_KEYDOWN) {
                switch(e_.key.keysym.sym){
                    case SDL_QUIT:
                        quit = 0;
                        break;
                    case SDLK_UP:
                        up_pushed_ = true;
                        up_unpushed_ = false;
                        break;
                    case SDLK_DOWN:
                        down_pushed_ = true;
                        down_unpushed_ = false;
                        break;
                    case SDLK_LEFT:
                        left_pushed_ = true;
                        break;
                    case SDLK_RIGHT:
                        right_pushed_ = true;
                        break;
                    case SDLK_SPACE:{
                            space_pushed_ = true;
                            tmp_space_obj = my_ship_->shoot();
                            if (nullptr != tmp_space_obj) {
                                projectiles_.push_back(tmp_space_obj);
                            }
                    }
                        break;
                    default:
                        break;
                }
            } else if (e_.type == SDL_KEYUP) {
                switch(e_.key.keysym.sym){
                    case SDLK_UP:
                        up_pushed_ = false;
                        up_unpushed_ = true;
                        break;
                    case SDLK_DOWN:
                        down_pushed_ = false;
                        down_unpushed_ = true;
                        break;
                    case SDLK_LEFT:
                        left_pushed_ = false;
                        break;
                    case SDLK_RIGHT:
                        right_pushed_ = false;
                        break;
                    case SDLK_SPACE:
                        space_pushed_ = false;
                        break;
                    default:
                        break;
                }
            }
        }

        if (up_pushed_)     { my_ship_->backwardAccelarate(); my_ship_->slowdown(); } else if (up_unpushed_) {my_ship_->backwardSlowdown();}
        if (down_pushed_) { my_ship_->accelarate(); my_ship_->backwardSlowdown(); } else if (down_unpushed_) {my_ship_->slowdown();}

        if (left_pushed_)     { my_ship_->changeX(false); }
        if (right_pushed_)     { my_ship_->changeX(true); }
        {
        if (space_pushed_)     {
            tmp_space_obj = my_ship_->shoot();
            if (nullptr != tmp_space_obj) {
                projectiles_.push_back(tmp_space_obj);
            }
        }
        changeObjectsPositions();
        updateAsteroids();
        updateProjectiles();
        if (asteroids_.size() <= 20){
            createAsteroid();
        }
        }
        projectiles_mutex_.unlock();
        asteroids_mutex_.unlock();

        my_background_->fillBackground();
        displayObjects();
        displayLifeAmount();
        SDL_RenderPresent(renderer_);
        SDL_RenderClear(renderer_);
    }

}
