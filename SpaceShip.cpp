#include "SpaceShip.hpp"

SpaceObject::SpaceObject(SDL_Renderer *renderer, int screen_width, int screen_height, int x, int y){
	this->renderer = renderer;
	this->x = x;
	this->y = y;
	this->screen_width = screen_width;
	this->screen_height = screen_height;
    display_delay = NOW + static_cast<std::chrono::milliseconds> (DISPLAY_DELAY);
    rotation_delay = NOW + static_cast<std::chrono::milliseconds> (ROTATION_DELAY);
    alive = true;
}

SpaceObject::~SpaceObject(){
    cout << "SpaceObject destructor" << endl;
}

bool SpaceObject::isAlive(){ return alive; }
void SpaceObject::markAsDead() { alive = false; }

Projectile::Projectile(SDL_Renderer *renderer, int screen_width, int screen_height, int offset_x, int offset_y, int x, int y) : SpaceObject::SpaceObject(renderer, screen_width, screen_height, x, y){
    direction_x = offset_x;
    direction_y = offset_y;
    this->x = x;
    this->y = y;
    life_time = NOW + static_cast<std::chrono::milliseconds> (PROJ_LIFETIME);
}

Point* Projectile::getXY(){ return new Point(x, y); }
pair<Point, Point> Projectile::getLine(){return make_pair(Point(x, y), Point(x_previous, y_previous));}

Projectile::~Projectile(){
    cout << "Projectile destructor" << endl;
}

Asteroid::Asteroid(SDL_Renderer *renderer, int screen_width, int screen_height, int x, int y): SpaceObject::SpaceObject(renderer, screen_width, screen_height, x, y){
    //TODO: think about direction_x;direction_y

    int error_x = rand() % 10;
    int error_y = rand() % 10;
    int size = rand() % 20;
    cg = new colorGeneratorAsteroid();

    pp.push_back(new Point(x + error_x,                 y + error_y));
    pp.push_back(new Point(x - size + rand() % 5,       y + size + rand() % 5));
    pp.push_back(new Point(x + rand() % 5,              y + size * 2 + rand() % 5));
    pp.push_back(new Point(x + size + rand() % 5,       y + size * 2 + rand() % 5));
    pp.push_back(new Point(x + size * 2 + rand() % 5,   y + size + rand() % 5));
    pp.push_back(new Point(x + size + rand() % 5,       y + rand() % 5));
}

vector<Point*>& Asteroid::getPoints(){ return pp; }

Asteroid::~Asteroid(){
    cout << "Asteroid destructor" << endl;
}

void out(double x, double y){
    cout << x << ":" << y << endl;
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

SpaceShip::SpaceShip(SDL_Renderer *renderer, int screen_width, int screen_height, int max_speed) : SpaceObject::SpaceObject(renderer, screen_width, screen_height, screen_width/2, screen_height/2){

    shoot_delay = NOW + static_cast<std::chrono::milliseconds> (SHOOTING_DELAY);
    ship_color_change = NOW + static_cast<std::chrono::milliseconds> (SHIP_COLOR_CHANGE);

    speed = new Speed(max_speed);

    spaceWidth = 30;
    spaceHeight = screen_height / 8;
    nozzleMinHeight = 25;
//    nozzleMaxHeight = 25;
    nozzleWidth = 8;

    cs = new ColorSchema(Color(255, 255, 0), Color(255,8,0));
    cg = new colorGeneratorShip();

    // spaceship coordination
    pp.push_back(Point(screen_width/2, screen_height/2));
    pp.push_back(Point(screen_width/2 - spaceWidth/2, screen_height/2 + spaceHeight));
    pp.push_back(Point(screen_width/2 + spaceWidth/2, screen_height/2 + spaceHeight));

    leftNozzle = new Nozzle(
                renderer,
                Point(screen_width/2 + spaceWidth/2 + nozzleWidth , screen_height/2 + spaceHeight + nozzleMinHeight),
                Point(screen_width/2 + spaceWidth/2                , screen_height/2 + spaceHeight),
                Point(screen_width/2 + spaceWidth/2 - nozzleWidth  , screen_height/2 + spaceHeight + nozzleMinHeight),
                speed
    );
    rightNozzle = new Nozzle(
                renderer,
                Point(screen_width/2 - spaceWidth/2 - nozzleWidth  , screen_height/2 + spaceHeight + nozzleMinHeight),
                Point(screen_width/2 - spaceWidth/2                , screen_height/2 + spaceHeight),
                Point(screen_width/2 - spaceWidth/2 + nozzleWidth  , screen_height/2 + spaceHeight + nozzleMinHeight),
                speed
    );
    initialMedianIntersection = getMedianIntersaction();
}

void SpaceShip::slowdown(){
    speed->slowdown();
}
void SpaceShip::accelarate(){
    speed->accelarate();
}
void SpaceShip::backward_slowdown(){
    speed->backward_slowdown();
}
void SpaceShip::backward_accelarate(){
    speed->backward_accelarate();
}
double SpaceShip::getCurrentA(){
    return speed->getCurrentA();
}

DirectionXY SpaceShip::get_offset(){
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
    cout << "SpaceShip destructor" << endl;
}

DirectionXY SpaceShip::get_direction(){
    double mediana_x = pp[1].x/2 + pp[2].x/2;
    double mediana_y = pp[1].y/2 + pp[2].y/2;

    double diff_x = (mediana_x - pp[0].x)/5;
    double diff_y = (mediana_y - pp[0].y)/5;
	
	return DirectionXY(diff_x, diff_y);
}

void SpaceObject::change_position(DirectionXY directionXY){
}

void SpaceShip::change_y(bool forward){
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

void rotatePointsInVector(vector<Point> &vPoints, Point initialMedianIntersection, double angle){
    for (auto iter = vPoints.begin(); iter != vPoints.end(); ++iter){
        *iter = get_rotated_point(*iter, initialMedianIntersection, angle);
    }
}

void SpaceShip::change_x(bool clockwise){

    if (rotation_delay > NOW) { return; }
    double angle = M_PI/15;

    if (!clockwise){
        angle = -angle;
    }
    rotatePointsInVector(pp, initialMedianIntersection, angle);
    rotatePointsInVector(leftNozzle->originPoints, initialMedianIntersection, angle);
    rotatePointsInVector(rightNozzle->originPoints, initialMedianIntersection, angle);


    leftNozzle->update();
    rightNozzle->update();
    leftNozzle->display();
    rightNozzle->display();

    rotation_delay = NOW + static_cast<std::chrono::milliseconds> (ROTATION_DELAY);
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
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_SetRenderDrawColor(renderer, cs->getR(), cs->getG(), cs->getB(), 255);
//    cs->update();
    for (int k = 0; k < 1/*3*/; ++k){
        int i = 0;
        int kIndex = k*3;
        for (; i < 2; ++i){
            SDL_RenderDrawLine(renderer, pp[i + kIndex].x, pp[i + kIndex].y, pp[i + kIndex + 1].x, pp[i + kIndex + 1].y);
        }
        SDL_RenderDrawLine(renderer, pp[kIndex + 2].x, pp[kIndex + 2].y, pp[kIndex].x, pp[kIndex].y);
    }

    double lengthOfBase = getLengthOfBase();

    updateSkeleton(cg, renderer, getTiltAngel(), lengthOfBase, pp[0], Point(pp[1].x/2 + pp[2].x/2, pp[1].y/2 + pp[2].y/2), pp[2], 4, true, true);
    leftNozzle->update();
    rightNozzle->update();
    leftNozzle->display();
    rightNozzle->display();
    Point a,b,c;
    a = leftNozzle->points[0];
    b = leftNozzle->points[1];
    c = leftNozzle->points[2];
    updateSkeleton(cg, renderer, getTiltAngel(), lengthOfBase, b, Point(a.x/2 + c.x/2, a.y/2 + c.y/2), a, 1, true, false);
    a = rightNozzle->points[0];
    b = rightNozzle->points[1];
    c = rightNozzle->points[2];
    updateSkeleton(cg, renderer, getTiltAngel(), lengthOfBase, b, Point(a.x/2 + c.x/2, a.y/2 + c.y/2), a, 1, true, false);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
}

std::chrono::time_point<std::chrono::system_clock> Projectile::getLifeTime(){ return life_time; }

Projectile * SpaceShip::shoot(){
    if (this->shoot_delay > NOW) { return nullptr; }
    shoot_delay = NOW + static_cast<std::chrono::milliseconds> (SHOOTING_DELAY);

    double mediana_x = pp[1].x/2 + pp[2].x/2;
    double mediana_y = pp[1].y/2 + pp[2].y/2;
    double diff_x = (mediana_x - pp[0].x)/5;
    double diff_y = (mediana_y - pp[0].y)/5;
	
    Projectile *projectile = new Projectile(this->renderer, 0, 0, diff_x, diff_y, pp[0].x - diff_x, pp[0].y - diff_y);
	return projectile;
}

void Projectile::display(bool displaySkeleton){

    if (display_delay > NOW){ return; }

    double error = (double) - BLOCK_SIZE;
	double tmp_x = (double) BLOCK_SIZE - 0.5;
	double tmp_y = (double) 0.5;
	
	double cx = this->x - 0.5;
	double cy = this->y - 0.5;
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	
	while (tmp_x >= tmp_y){
		SDL_RenderDrawPoint(renderer, (int)(cx + tmp_x), (int)(cy + tmp_y));
		SDL_RenderDrawPoint(renderer, (int)(cx + tmp_y), (int)(cy + tmp_x));

		if (tmp_x != 0){
			SDL_RenderDrawPoint(renderer, (int)(cx - tmp_x), (int)(cy + tmp_y));
			SDL_RenderDrawPoint(renderer, (int)(cx + tmp_y), (int)(cy - tmp_x));
		}

		if (tmp_y != 0){
			SDL_RenderDrawPoint(renderer, (int)(cx + tmp_x), (int)(cy - tmp_y));
			SDL_RenderDrawPoint(renderer, (int)(cx - tmp_y), (int)(cy + tmp_x));
		}

        if (tmp_x != 0 && tmp_y != 0){
			SDL_RenderDrawPoint(renderer, (int)(cx - tmp_x), (int)(cy - tmp_y));
			SDL_RenderDrawPoint(renderer, (int)(cx - tmp_y), (int)(cy - tmp_x));
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
	SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
	for (double dy = 1; dy <= BLOCK_SIZE; dy += 1.0){
		double dx = floor(sqrt((2.0 * BLOCK_SIZE * dy) - (dy * dy)));
		double cx = this->x;
		double cy = this->y;
		SDL_RenderDrawLine(renderer, cx - dx, cy + dy - BLOCK_SIZE, cx + dx, cy + dy - BLOCK_SIZE);
		SDL_RenderDrawLine(renderer, cx - dx, cy - dy + BLOCK_SIZE, cx + dx, cy - dy + BLOCK_SIZE);
	}
	this->x -= direction_x;
	this->y -= direction_y;
    display_delay = NOW + (std::chrono::milliseconds) (DISPLAY_DELAY/5);
}

Point Asteroid::getCenterPoint(){
    double x = 0.0, y = 0.0;
    int size = pp.size();
    for (auto iter = pp.begin(); iter != pp.end(); ++iter){
        x += (*iter)->x / size;
        y += (*iter)->y / size;
    }
    return Point(x, y);
}

void Asteroid::fill(){
    
    Point center_point = getCenterPoint();
    int blocksize = 3;

    auto iter = pp.begin();
    auto iter_next = iter + 1;
    
    Point p1 = **iter;
    Point p2 = **iter_next;
    
    while (iter_next != pp.end()){
        p1 = **iter;
        p2 = **iter_next;
        updateSkeleton(cg, renderer, 0.0, getLengthOfVector(p1, p2), center_point, Point((p1.x + p2.x)/2, (p1.y + p2.y)/2), p1, blocksize, false, true);
        ++iter;
        ++iter_next;
        // break;
    }
    iter_next = pp.begin();
    p1 = **iter;
    p2 = **iter_next;
    updateSkeleton(cg, renderer, 0.0, getLengthOfVector(p1, p2), center_point, Point((p1.x + p2.x)/2, (p1.y + p2.y)/2), p1, blocksize, false, true);
}

void Asteroid::display(bool displaySkeleton){

    if (displaySkeleton){
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        auto iter = pp.begin();
        Point *p1;
        Point *p2;
        for (; iter != pp.end()-1; ++iter){
            p1 = *iter;
            p2 = *(iter+1);
            SDL_RenderDrawLine(renderer, p1->x, p1->y, p2->x, p2->y);
        }
        p1 = *pp.begin();
        SDL_RenderDrawLine(renderer, p1->x, p1->y, p2->x, p2->y);
    }

    fill();
}

void Asteroid::change_position(DirectionXY directionXY){
    Point *p1;
    for (auto iter = pp.begin(); iter != pp.end(); ++iter){
        p1 = *iter;
        p1->x += directionXY.x;
        p1->y += directionXY.y;
    }
}

Point* Asteroid::getFirstPoint(){
    return pp[0];
}

void Projectile::change_position(DirectionXY directionXY){
    this->x_previous = x;
    this->y_previous = y;
    this->x += directionXY.x;
    this->y += directionXY.y;
}
