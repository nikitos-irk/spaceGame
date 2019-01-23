#include "SpaceShip.hpp"

SpaceObject::SpaceObject(SDL_Renderer *renderer, int screen_width, int screen_height, int x, int y){
	this->renderer = renderer;
	this->x = x;
	this->y = y;
	this->screen_width = screen_width;
	this->screen_height = screen_height;
    display_delay = std::chrono::system_clock::now() + static_cast<std::chrono::milliseconds> (DISPLAY_DELAY);
    rotation_delay = std::chrono::system_clock::now() + static_cast<std::chrono::milliseconds> (ROTATION_DELAY);
}

SpaceObject::~SpaceObject(){
    cout << "SpaceObject destructor" << endl;
}

Projectile::Projectile(SDL_Renderer *renderer, int screen_width, int screen_height, int offset_x, int offset_y, int x, int y) : SpaceObject::SpaceObject(renderer, screen_width, screen_height, x, y){
    direction_x = offset_x;
    direction_y = offset_y;
    this->x = x;
    this->y = y;
}

Projectile::~Projectile(){
    cout << "Projectile destructor" << endl;
}

Asteroid::Asteroid(SDL_Renderer *renderer, int screen_width, int screen_height, int x, int y): SpaceObject::SpaceObject(renderer, screen_width, screen_height, x, y){
    //TODO: think about direction_x;direction_y

    int error_x = rand() % 10;
    int error_y = rand() % 10;
    int size = rand() % 20;

    points[0] = {x + error_x, 				y + error_y};
    points[1] = {x - size + rand() % 5, 	y + size + rand() % 5};
    points[2] = {x + rand() % 5, 			y + size * 2 + rand() % 5};
    points[3] = {x + size + rand() % 5, 	y + size * 2 + rand() % 5};
    points[4] = {x + size * 2 + rand() % 5, y + size + rand() % 5};
    points[5] = {x + size + rand() % 5, 	y + rand() % 5};
    points[6] = {x + error_x, 				y + error_y};
}

Asteroid::~Asteroid(){
    cout << "Asteroid destructor" << endl;
}

SpaceShip::SpaceShip(SDL_Renderer *renderer, int screen_width, int screen_height, int max_speed) : SpaceObject::SpaceObject(renderer, screen_width, screen_height, screen_width/2, screen_height/2){

    shoot_delay = std::chrono::system_clock::now() + static_cast<std::chrono::milliseconds> (SHOOTING_DELAY);
    speed = new Speed(max_speed);

    // spaceship coordination
	this->points[0] = {screen_width/2, screen_height/2};
	this->points[1] = {screen_width/2 - 10, screen_height/2 + screen_height / 10};
	this->points[2] = {screen_width/2 + 10, screen_height/2 + screen_height / 10};
	this->points[3] = {screen_width/2, screen_height/2};

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
    int mediana_x = points[1].x/2 + points[2].x/2;
    int mediana_y = points[1].y/2 + points[2].y/2;

    return DirectionalVector(DirectionXY(mediana_x, mediana_y),
                             DirectionXY(points[0].x, points[0].y));
}

SpaceShip::~SpaceShip(){
    cout << "SpaceShip destructor" << endl;
}

DirectionXY::DirectionXY(float x, float y){
	this->x = x;
	this->y = y;
}

DirectionXY::DirectionXY(){
	this->x = 0;
	this->y = 0;
}

DirectionXY SpaceShip::get_direction(){
	int mediana_x = points[1].x/2 + points[2].x/2;
	int mediana_y = points[1].y/2 + points[2].y/2;

	int diff_x = (mediana_x - points[0].x)/5;
	int diff_y = (mediana_y - points[0].y)/5;
	
	return DirectionXY(diff_x, diff_y);
}

void SpaceObject::change_position(DirectionXY directionXY){
	this->x += directionXY.x;
	this->y += directionXY.y;
}

void SpaceShip::change_y(bool forward){
	
	int mediana_x = points[1].x/2 + points[2].x/2;
	int mediana_y = points[1].y/2 + points[2].y/2;

	int diff_x = (mediana_x - points[0].x) / 5;
	int diff_y = (mediana_y - points[0].y) / 5;

//	cout << "diff_x = " << diff_x << endl;
//	cout << "diff_y = " << diff_y << endl;

	if (!forward){
		diff_x *= -1;
		diff_y *= -1;
	}

	for (int i = 0; i < POINTS_COUNT; ++i){
		points[i].x += diff_x;
		points[i].y += diff_y;
	}
}

void SpaceShip::change_x(bool clockwise){
	
	if (rotation_delay > std::chrono::system_clock::now()) { return; }
	
    float angel = M_PI / 12;
	if (!clockwise) { angel *= -1; }

    float x1 = points[2].x;
    float y1 = points[2].y;
    float x2 = (points[0].x + points[1].x)/2;
    float y2 = (points[0].y + points[1].y)/2;

    float x3 = points[3].x;
    float y3 = points[3].y;
    float x4 = (points[1].x + points[2].x)/2;
    float y4 = (points[1].y + points[2].y)/2;

    float px = ((x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4))/((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));
    float py = ((x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4))/((x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4));

    float s = sin(angel);
    float c = cos(angel);

    for (int i = 0; i < POINTS_COUNT - 1; ++i){
        int tmp_x = c * (points[i].x - px) - s * (points[i].y - py) + px;
        int tmp_y = s * (points[i].x - px) + c * (points[i].y - py) + py;
        points[i].x = tmp_x;
        points[i].y = tmp_y;
	}
    points[POINTS_COUNT - 1].x = points[0].x;
    points[POINTS_COUNT - 1].y = points[0].y;
    rotation_delay = std::chrono::system_clock::now() + static_cast<std::chrono::milliseconds> (ROTATION_DELAY);
}

void SpaceShip::display(){
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLines(renderer, points, POINTS_COUNT);
}

Projectile * SpaceShip::shoot(){
	if (this->shoot_delay > std::chrono::system_clock::now()) { return nullptr; }

	int mediana_x = points[1].x/2 + points[2].x/2;
	int mediana_y = points[1].y/2 + points[2].y/2;
	int diff_x = (mediana_x - points[0].x)/5;
	int diff_y = (mediana_y - points[0].y)/5;
	
    Projectile *projectile = new Projectile(this->renderer, 0, 0, diff_x, diff_y, points[0].x - diff_x, points[0].y - diff_y);
    shoot_delay = std::chrono::system_clock::now() + static_cast<std::chrono::milliseconds> (SHOOTING_DELAY);
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
	if (this->display_delay < std::chrono::system_clock::now()){
		this->x -= direction_x;
		this->y -= direction_y;
		display_delay = std::chrono::system_clock::now() + (std::chrono::milliseconds) DISPLAY_DELAY;
	}
}

void Asteroid::display(){
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderDrawLines(renderer, points, ASTEROID_POINTS_COUNT);
}

void Asteroid::change_position(DirectionXY directionXY){
	for (int i = 0; i < ASTEROID_POINTS_COUNT; ++i){
		this->points[i].x += directionXY.x;
		this->points[i].y += directionXY.y;
	}
}

void Projectile::change_position(DirectionXY directionXY){
    this->x += directionXY.x;
    this->y += directionXY.y;
}
