#include "game.hpp"

#include <iostream>
#include <memory>
#include <thread>

#include <unistd.h>

#include "explosion.hpp"

#include "figure/factory_shape.hpp"
#include "space/grid.hpp"

constexpr auto kAsteroidsRemovingDelay = 10ms;
constexpr auto kChangePositionDelay = 30ms;
constexpr auto kInertiaDelay = 10ms;
const auto NUMBER_OF_ASTEROIDS = 10;

Game::Game(SDL_Renderer* renderer, primitive::Size size, int life_amount)
    : renderer_{renderer},
      screen_size_{size},
      life_amount_{life_amount},
      ship_{{double(screen_size_.width)/2, double(screen_size_.height)/2}},
      scene_{renderer_, screen_size_}
{
//  Uncomment to show grid
//  background_.grid = std::make_unique<space::Grid>();

  // Create asteroids
  for (int i = 0; i < NUMBER_OF_ASTEROIDS; ++i) {
      double tmp_x = rand() % screen_size_.width;
      double tmp_y = rand() % screen_size_.height;
      asteroids_.push_back(new Asteroid(renderer, {tmp_x, tmp_y}));
  }

  // Initiating delays
  change_position_delay_ = primitive::delay(kChangePositionDelay);
  inertia_delay_ = primitive::delay(kInertiaDelay);
  update_asteroids_delay_ = primitive::delay(kAsteroidsRemovingDelay);

  // Create Stars
  createStars();
}

void Game::createAsteroid() {
    double theta = (rand() % 360)/M_PI;
    auto ship_center = ship_.CalcMedianIntersaction();
    double tmp_x = (rand() % screen_size_.width) + screen_size_.width;
    double tmp_y = ship_center.y;
    primitive::Point p{tmp_x, tmp_y};
    asteroids_.push_back(new Asteroid(renderer_, p.rotate(ship_center, theta)));
}

void Game::createStars() {
    stars_.push_back(new Star(renderer_, primitive::Point{800, 800}, 250));
    stars_.push_back(new Star(renderer_, primitive::Point{0, 500}, 100));
}

void Game::updateAsteroids() {
    if (update_asteroids_delay_ >= primitive::now()){ return; }
    auto tmp_p = ship_.CalcMedianIntersaction();
    primitive::Point *ap = nullptr;
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
    update_asteroids_delay_ = primitive::delay(kAsteroidsRemovingDelay);
}

void Game::updateProjectiles() {
    for (auto it = std::begin(projectiles_); it != std::end(projectiles_); ++it) {
        (*it)->update();
        if ((*it)->get_life_time() < primitive::now()) {
          it = projectiles_.erase(it);
        }
    }
}

bool dot_on_line(primitive::Point *p1, primitive::Point *p2, primitive::Point *px)
{
    double k = (p2->y - p1->y) / (p2->x - p1->x);
    double c = p1->y - k * p1->x;
    return abs(px->y - (px->x * k + c)) < 0.1;
}

inline int det (int a, int b, int c, int d) {
    return a * d - b * c;
}

inline bool between(int a, int b, double c)
{
    double epsilon = 1E-9;
    return std::min(a,b) <= c + epsilon && c <= std::max(a,b) + epsilon;
}

inline bool intersect_1(int a, int b, int c, int d)
{
    if (a > b)  std::swap(a, b);
    if (c > d)  std::swap(c, d);
    return std::max(a,c) <= std::min(b,d);
}


bool intersect(primitive::Line l1, primitive::Line l2)
{
    int A1 = l1.begin.y-l1.end.y;
    int B1 = l1.end.x-l1.begin.x;
    int C1 = -A1*l1.begin.x - B1*l1.begin.y;
    int A2 = l2.begin.y-l2.end.y;
    int B2 = l2.end.x-l2.begin.x;
    int C2 = -A2*l2.begin.x - B2*l2.begin.y;
    int zn = det (A1, B1, A2, B2);
    if (zn != 0) {
        double x = - det(C1, B1, C2, B2) * 1. / zn;
        double y = - det(A1, C1, A2, C2) * 1. / zn;
        return between(l1.begin.x, l1.end.x, x) && between(l1.begin.y, l1.end.y, y)
            && between(l2.begin.x, l2.end.x, x) && between(l2.begin.y, l2.end.y, y);
    }
    else
        return det(A1, C1, A2, C2) == 0 && det(B1, C1, B2, C2) == 0
            && intersect_1(l1.begin.x, l1.end.x, l2.begin.x, l2.end.x)
            && intersect_1(l1.begin.y, l1.end.y, l2.begin.y, l2.end.y);
}

void Game::checkShipHits()
{
    while (running_) {
        asteroids_mutex_.lock();
        shipHitsLoop();
        asteroids_mutex_.unlock();
        usleep(100);
    }
}

void Game::shipHitsLoop(){

    std::vector<primitive::Point*> tmpPoints;
    bool hitStatus = false;

    for (auto ast = asteroids_.begin(); ast != asteroids_.end(); ++ast){
        hitStatus = false;
        if (!(*ast)->isAlive()){ continue; }
        tmpPoints = dynamic_cast<Asteroid*>(*ast)->get_points();
        primitive::Point *p1, *p2;
        primitive::Point sp1, sp2;
        for (auto p = tmpPoints.begin(); p != tmpPoints.end() - 1; ++p){
            if (!hitStatus){
                p1 = *p;
                p2 = *(p+1);
                if (tmpPoints.end()-1 == p){
                    p2 = *tmpPoints.begin();
                } else {
                    p2 = *(p + 1);
                }
                auto border = ship_.get_border();
                for (auto spacePointsIter = border.begin(); spacePointsIter != border.end() - 1; ++spacePointsIter){
                    sp1 = *spacePointsIter;
                    sp2 = *(spacePointsIter+1);
                    if (border.end()-1 == spacePointsIter){
                        sp2 = *border.begin();
                    } else {
                        sp2 = *(spacePointsIter + 1);
                    }
                    hitStatus = intersect({*p1, *p2}, {sp1, sp2});
                    if (hitStatus){
                        life_amount_--;
                        if (!life_amount_){
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

void Game::checkHits()
{
    while (running_) {
        projectiles_mutex_.lock();
        asteroids_mutex_.lock();
        histLoop();
        asteroids_mutex_.unlock();
        projectiles_mutex_.unlock();
        usleep(100);
    }
}

void Game::histLoop()
{
    for (auto& ball: projectiles_) {
        if (!ball->isAlive()) { continue; }
        for (auto ast: asteroids_) {
            if (!ast->isAlive()) { continue; }
            auto tmpPoints = dynamic_cast<Asteroid*>(ast)->get_points();
            primitive::Point *p1, *p2;

            for (auto p = tmpPoints.begin(); p != tmpPoints.end() - 1; ++p){
                p1 = *p;
                p2 = *(p+1);
                if (tmpPoints.end() - 1 == p){
                    p2 = *tmpPoints.begin();
                } else {
                    p2 = *(p + 1);
                }

                bool hitStatus = intersect(ball->line(), {*p1, *p2});
                if ( hitStatus ){
                    ast->markAsDead();
                    ball->markAsDead();
                    break;
                }
            }
        }
    }
    cleanLoop();
}

void Game::generateExplosion(Asteroid *tmp_ast){
    double middle_x = 0.0, middle_y = 0.0;

    std::vector<primitive::Point*> tmpPoints = tmp_ast->get_points();
    for (auto iter = tmpPoints.begin(); iter != tmpPoints.end(); ++iter){
        middle_x += (*iter)->x;
        middle_y += (*iter)->y;
    }
    middle_x /= tmpPoints.size();
    middle_y /= tmpPoints.size();
    explosions_.push_back(new Explosion(primitive::Point{middle_x, middle_y}, renderer_, tmp_ast));
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
    for (auto it = std::begin(projectiles_); it != std::end(projectiles_); ++it) {
        if (!(*it)->isAlive()){
            it = projectiles_.erase(it);
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

void Game::displayObjects()
{
    background_.display(scene_);
    ship_.display(scene_);
    for (auto& ball: projectiles_) {
        if (ball->isAlive()) {
            ball->display(scene_);
        }
    }

    for (auto& spaceObject: asteroids_) {
        if (spaceObject->isAlive()) {
            spaceObject->display();
        }
    }

    for (auto& explosion: explosions_) {
        if (explosion->isAlive()) {
            explosion->display();
        }
    }

    for (auto &star: stars_){
        star->display();
    }

    cleanExplosions();

    life_amount_.display(scene_);

    SDL_RenderPresent(renderer_);
    SDL_RenderClear(renderer_);
}

void Game::changeObjectsPositions(){

    if (change_position_delay_ > primitive::now()){ return; }

    auto directionXY = ship_.getOffset(&stars_, gravity);
    ship_.update();

    for (auto spaceObject = asteroids_.begin(); spaceObject != asteroids_.end(); ++spaceObject){
        (*spaceObject)->changePosition(directionXY);
        dynamic_cast<Asteroid*>(*spaceObject)->rotate();
    }

    if (gravity) {
        for (auto spaceObject = asteroids_.begin(); spaceObject != asteroids_.end(); ++spaceObject){
            auto tmp_obj = dynamic_cast<Asteroid*>(*spaceObject);
            auto tmp_offset = tmp_obj->gravity_->get_offset(&stars_, tmp_obj->getCenterPoint());
            tmp_obj->changePosition(tmp_offset);
        }
    }

    for (auto& ball: projectiles_) {
        ball->move(directionXY);
    }

    for (auto explosion = explosions_.begin(); explosion != explosions_.end(); ++explosion){
        if ( (*explosion)->isAlive() ){
            (*explosion)->shift(directionXY);
        }
    }

    for (auto star = stars_.begin(); star != stars_.end(); ++star){
        (*star)->changePosition(directionXY);
    }

    change_position_delay_ = primitive::delay(kChangePositionDelay);
}

void Game::run()
{
    running_ = true;

    displayObjects();

    std::thread hits_monitoring(&Game::checkHits, this);
    std::thread ship_hits_monitoring(&Game::checkShipHits, this);

    int quit = 1;
    while(quit) {

        if (globalExceptionPtr)
          {
            try
            {
              std::rethrow_exception(globalExceptionPtr);
            }
            catch (const std::exception &ex)
            {
              std::cout << "Game Over!" << std::endl;
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
                    case SDLK_SPACE:
                        space_pushed_ = true;
                        break;
                    case SDLK_g:
                        gravity = not gravity;
                        std::cout << "Gravity: " << gravity << std::endl;
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

        if (up_pushed_) {
          ship_.backwardAccelarate();
          ship_.slowdown();
        }
        else if (up_unpushed_) {
          ship_.backwardSlowdown();
        }
        if (down_pushed_) {
          ship_.accelarate();
          ship_.backwardSlowdown();
        }
        else if (down_unpushed_) {
          ship_.slowdown();
        }

        if (left_pushed_) {
          ship_.rotate(false);
        }
        if (right_pushed_) {
          ship_.rotate(true);
        }
        {
        if (space_pushed_) {
            auto ball = ship_.shoot();
            if (ball) {
                projectiles_.push_back(std::move(ball));
            }
        }
        changeObjectsPositions();
        updateAsteroids();
        updateProjectiles();
        if (asteroids_.size() <= NUMBER_OF_ASTEROIDS-1){
            createAsteroid();
        }
        }
        projectiles_mutex_.unlock();
        asteroids_mutex_.unlock();

        displayObjects();
    }
    running_ = false;
    hits_monitoring.join();
    ship_hits_monitoring.join();
}
