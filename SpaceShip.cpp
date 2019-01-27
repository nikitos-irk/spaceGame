#include "SpaceShip.hpp"

SpaceObject::SpaceObject(SDL_Renderer *renderer, int screen_width, int screen_height, int x, int y){
	this->renderer = renderer;
	this->x = x;
	this->y = y;
	this->screen_width = screen_width;
	this->screen_height = screen_height;
    display_delay = NOW + static_cast<std::chrono::milliseconds> (DISPLAY_DELAY);
    rotation_delay = NOW + static_cast<std::chrono::milliseconds> (ROTATION_DELAY);
}

SpaceObject::~SpaceObject(){
    cout << "SpaceObject destructor" << endl;
}

Projectile::Projectile(SDL_Renderer *renderer, int screen_width, int screen_height, int offset_x, int offset_y, int x, int y) : SpaceObject::SpaceObject(renderer, screen_width, screen_height, x, y){
    direction_x = offset_x;
    direction_y = offset_y;
    this->x = x;
    this->y = y;
    life_time = NOW + static_cast<std::chrono::milliseconds> (PROJ_LIFETIME);
}

Point* Projectile::getXY(){ return new Point(x, y); }

Projectile::~Projectile(){
    cout << "Projectile destructor" << endl;
}

Asteroid::Asteroid(SDL_Renderer *renderer, int screen_width, int screen_height, int x, int y): SpaceObject::SpaceObject(renderer, screen_width, screen_height, x, y){
    //TODO: think about direction_x;direction_y

    int error_x = rand() % 10;
    int error_y = rand() % 10;
    int size = rand() % 20;

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

int get_index(int i){
    return (i > 2) ? i - 3 : i;
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

    double px = ((x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4))/((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));
    double py = ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4))/((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));
    return Point(px, py);
}

SpaceShip::SpaceShip(SDL_Renderer *renderer, int screen_width, int screen_height, int max_speed) : SpaceObject::SpaceObject(renderer, screen_width, screen_height, screen_width/2, screen_height/2){

    shoot_delay = NOW + static_cast<std::chrono::milliseconds> (SHOOTING_DELAY);
    speed = new Speed(max_speed);

    // spaceship coordination
    pp.push_back(Point(screen_width/2, screen_height/2));
    pp.push_back(Point(screen_width/2 - 10, screen_height/2 + screen_height / 10));
    pp.push_back(Point(screen_width/2 + 10, screen_height/2 + screen_height / 10));
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

DirectionXY::DirectionXY(double x, double y){
	this->x = x;
	this->y = y;
}

DirectionXY::DirectionXY(){
	this->x = 0;
	this->y = 0;
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

point rotate(point P, point Q, double theta)
{
    return (P-Q) * polar(1.0, theta) + Q;
}

void SpaceShip::change_x(bool clockwise){
	
    if (rotation_delay > NOW) { return; }
	
    double angel = M_PI/10;

    if (!clockwise){
        angel = -angel;
    }
    for (auto iter = pp.begin(); iter != pp.end(); ++iter){
        point P(iter->x, iter->y);
        point Q(initialMedianIntersection.x, initialMedianIntersection.y);
        point P_rotated = rotate(P, Q, angel);
        iter->x = P_rotated.real();
        iter->y = P_rotated.imag();
    }
    rotation_delay = NOW + static_cast<std::chrono::milliseconds> (ROTATION_DELAY);
}

void SpaceShip::display(){
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    auto iter = pp.begin();
    for (; iter != pp.end() - 1; ++iter){
        SDL_RenderDrawLine(renderer, iter->x, iter->y, (iter+1)->x, (iter+1)->y);
    }
    SDL_RenderDrawLine(renderer, iter->x, iter->y, pp.begin()->x, pp.begin()->y);
}

std::chrono::time_point<std::chrono::system_clock> Projectile::getLifeTime(){ return life_time; }

Projectile * SpaceShip::shoot(){
    if (this->shoot_delay > NOW) { return nullptr; }

    double mediana_x = pp[1].x/2 + pp[2].x/2;
    double mediana_y = pp[1].y/2 + pp[2].y/2;
    double diff_x = (mediana_x - pp[0].x)/5;
    double diff_y = (mediana_y - pp[0].y)/5;
	
    Projectile *projectile = new Projectile(this->renderer, 0, 0, diff_x, diff_y, pp[0].x - diff_x, pp[0].y - diff_y);
    shoot_delay = NOW + static_cast<std::chrono::milliseconds> (SHOOTING_DELAY);
	return projectile;
}

void Projectile::display(){
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
    if (this->display_delay < NOW){
		this->x -= direction_x;
		this->y -= direction_y;
        display_delay = NOW + (std::chrono::milliseconds) DISPLAY_DELAY;
	}
}

void Asteroid::display(){
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
    this->x += directionXY.x;
    this->y += directionXY.y;
}
