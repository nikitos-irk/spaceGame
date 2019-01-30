#include "Game.hpp"

Game::Game(SDL_Renderer *renderer, int screen_width, int screen_height){

	this->screen_width = screen_width;
	this->screen_height = screen_height;
	
	// Hope I will think something more interesting background than it is now
	my_background = new Background(renderer, screen_width, screen_height);

	// Renderer
	this->renderer = renderer;

	// Create ship
    my_ship = new SpaceShip(renderer, screen_width, screen_height, 50);
	
	// Create asteroids
    for (int i = 0; i < 10; ++i) {
		int tmp_x = rand() % this->screen_width;
		int tmp_y = rand() % this->screen_height;
        asteroids.push_back(new Asteroid(renderer, screen_width, screen_height, tmp_x, tmp_y));
	}
	
	// Initiating delays
	auto now = std::chrono::system_clock::now();
    change_position_delay = now + static_cast<std::chrono::milliseconds> (CHANGE_POSITION_DELAY);
    inertia_delay = now + static_cast<std::chrono::milliseconds> (INERTIA_DELAY);
    update_asteroids_delay = now + static_cast<std::chrono::milliseconds> (ASTEROIDS_REMOVING_DELAY);
	
	space_pushed 	= false;
	left_pushed  	= false;
	right_pushed 	= false;
	up_pushed 		= false;
	down_pushed 	= false;
    up_unpushed     = false;
    down_unpushed   = false;

	inertia_counter_up = 0;
	inertia_counter_down = 0;
}

void Game::create_asteroid(){
    double theta = (rand() % 360)/M_PI;
    Point ship_center = my_ship->getMedianIntersaction();
    double tmp_x = (rand() % this->screen_width) + this->screen_width;
    double tmp_y = ship_center.y;

    point P(tmp_x, tmp_y);
    point Q(ship_center.x, ship_center.y);
    point P_rotated = (P-Q) * polar(1.0, theta) + Q;
    tmp_x = P_rotated.real();
    tmp_y = P_rotated.imag();

    asteroids.push_back(new Asteroid(renderer, screen_width, screen_height, tmp_x, tmp_y));
}

void Game::update_asteroids(){
    if (update_asteroids_delay >= NOW){ return; }
    Point tmp_p = my_ship->getMedianIntersaction();
    Point *ap = nullptr;
    double distance;
    double diagonal = sqrt(pow(this->screen_width, 2) + pow(this->screen_height, 2));
    auto iter = asteroids.begin();
    while (iter != asteroids.end())
    {
        ap = dynamic_cast<Asteroid*>(*iter)->getFirstPoint();
        distance = sqrt(pow(tmp_p.x - ap->x, 2) + pow(tmp_p.y - ap->y, 2));
        if (distance >= 1.5 * diagonal){
            SpaceObject *tmp = *iter;
            asteroids.erase(iter++);
            delete tmp;
        } else {
            ++iter;
        }
    }
    update_asteroids_delay = NOW + static_cast<std::chrono::milliseconds> (ASTEROIDS_REMOVING_DELAY);
}

void Game::update_projectiles(){
    auto iter = projectiles.begin();
    while (iter != projectiles.end())
    {
        if (dynamic_cast<Projectile*>(*iter)->getLifeTime() < NOW){
            SpaceObject *tmp = *iter;
            asteroids.erase(iter++);
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
    return min(a,b) <= c + epsilon && c <= max(a,b) + epsilon;
}

inline bool intersect_1 (int a, int b, int c, int d) {
    if (a > b)  swap (a, b);
    if (c > d)  swap (c, d);
    return max(a,c) <= min(b,d);
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

void Game::check_hits(){
    vector<Point*> tmpPoints;
    bool hitStatus;
    while (true)
    {
        projectiles_mutex.lock();
        asteroids_mutex.lock();
        auto pr = projectiles.begin();
        while (pr != projectiles.end()){
            auto ast = asteroids.begin();
            hitStatus = false;
            while (ast != asteroids.end()){

                tmpPoints = dynamic_cast<Asteroid*>(*ast)->getPoints();
                Point *p1, *p2, *px;

                for (auto p = tmpPoints.begin(); p != tmpPoints.end() - 1; ++p){
                    p1 = *p;
                    p2 = *(p+1);
                    if (tmpPoints.end() == p){
                        p2 = *tmpPoints.begin();
                    } else {
                        p2 = *(p + 1);
                    }

                    px = dynamic_cast<Projectile*>(*pr)->getXY();
                    pair<Point, Point> pLine = dynamic_cast<Projectile*>(*pr)->getLine();

                    bool status = intersect(*p1, *p2, pLine.first, pLine.second);
                    delete px;
                    if ( status ){
                        hitStatus = true;
                        (*ast)->markAsDead();
                        (*pr)->markAsDead();
                        delete *ast;
                        delete *pr;
                        asteroids.erase(ast++);
                        projectiles.erase(pr++);
                        break;
                    }
                }
                if (!hitStatus) {++ast;} else { break; }
            }
            if (!hitStatus) {++pr;}
        }
        asteroids_mutex.unlock();
        projectiles_mutex.unlock();
        usleep(100);
    }
}

void Game::displayObjects(){
	my_ship->display();
	for (auto spaceObject = spaceObjects.begin(); spaceObject != spaceObjects.end(); ++spaceObject){
		(*spaceObject)->display();
	}
    for (auto spaceObject = asteroids.begin(); spaceObject != asteroids.end(); ++spaceObject){
        if ((*spaceObject)->isAlive()){
            (*spaceObject)->display();
        }
    }
    for (auto spaceObject = projectiles.begin(); spaceObject != projectiles.end(); ++spaceObject){
        if ((*spaceObject)->isAlive()){
            (*spaceObject)->display();
        }
    }
}

void Game::changeObjectsPositions(){
	auto now = std::chrono::system_clock::now();
	if (change_position_delay >= now){ return; }
    DirectionXY directionXY = my_ship->get_offset();

    for (auto spaceObject = spaceObjects.begin(); spaceObject != spaceObjects.end(); ++spaceObject){
		(*spaceObject)->change_position(directionXY);
	}
    for (auto spaceObject = asteroids.begin(); spaceObject != asteroids.end(); ++spaceObject){
        (*spaceObject)->change_position(directionXY);
    }
    for (auto spaceObject = projectiles.begin(); spaceObject != projectiles.end(); ++spaceObject){
        (*spaceObject)->change_position(directionXY);
    }
    change_position_delay = now + static_cast<std::chrono::milliseconds> (CHANGE_POSITION_DELAY);
}

void Game::run(){

	my_background->fill_background();
    displayObjects();
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);

//    thread thUpdating(&Game::update, this);
    thread thHitsMonitoring(&Game::check_hits, this);

//    thUpdating.join();
    thHitsMonitoring.detach();
    int quit = 1;
    SpaceObject *tmp_space_obj;
    while(quit) {
        projectiles_mutex.lock();
        asteroids_mutex.lock();
        while( SDL_PollEvent( &e ) != 0 ) {
            if( e.type == SDL_QUIT ){
                quit = 0;
            } else if (e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym){
                    case SDL_QUIT:
                        quit = 0;
                        break;
                    case SDLK_UP:
                        up_pushed = true;
                        up_unpushed = false;
                        break;
                    case SDLK_DOWN:
                        down_pushed = true;
                        down_unpushed = false;
                        break;
                    case SDLK_LEFT:
                        left_pushed = true;
                        break;
                    case SDLK_RIGHT:
                        right_pushed = true;
                        break;
                    case SDLK_SPACE:{
                            space_pushed = true;
                            tmp_space_obj = my_ship->shoot();
                            if (nullptr != tmp_space_obj) {
                                projectiles.push_back(tmp_space_obj);
                            }
                    }
                        break;
                    default:
                        break;
                }
            } else if (e.type == SDL_KEYUP) {
                switch(e.key.keysym.sym){
                    case SDLK_UP:
                        up_pushed = false;
                        up_unpushed = true;
                        break;
                    case SDLK_DOWN:
                        down_pushed = false;
                        down_unpushed = true;
                        break;
                    case SDLK_LEFT:
                        left_pushed = false;
                        break;
                    case SDLK_RIGHT:
                        right_pushed = false;
                        break;
                    case SDLK_SPACE:
                        space_pushed = false;
                        break;
                    default:
                        break;
                }
            }
        }
        if (up_pushed) 	{ my_ship->backward_accelarate(); my_ship->slowdown(); } else if (up_unpushed) {my_ship->backward_slowdown();}
        if (down_pushed) { my_ship->accelarate(); my_ship->backward_slowdown(); } else if (down_unpushed) {my_ship->slowdown();}

        if (left_pushed) 	{ my_ship->change_x(false); }
        if (right_pushed) 	{ my_ship->change_x(true); }
        {
        if (space_pushed) 	{
            tmp_space_obj = my_ship->shoot();
            if (nullptr != tmp_space_obj) {
                projectiles.push_back(tmp_space_obj);
            }
        }
        changeObjectsPositions();
        update_asteroids();
        update_projectiles();
        if (asteroids.size() <= 20){
            create_asteroid();
        }
        }
        projectiles_mutex.unlock();
        asteroids_mutex.unlock();

        my_background->fill_background();
        displayObjects();
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }

}
