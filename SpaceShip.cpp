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

Point SpaceShip::getTwoLinesIntersaction(Point p1, Point p2, Point p3, Point p4){
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

    availableColors.push_back(Color(220,220,220));
    availableColors.push_back(Color(192,192,192));
    availableColors.push_back(Color(105,105,105));
    availableColors.push_back(Color(211,211,211));
    availableColors.push_back(Color(119,136,153));
    colorIter = availableColors.end();
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

void rotatePointsInVector(vector<Point> &vPoints, Point initialMedianIntersection, double angle){
    for (auto iter = vPoints.begin(); iter != vPoints.end(); ++iter){
        point P(iter->x, iter->y);
        point Q(initialMedianIntersection.x, initialMedianIntersection.y);
        point P_rotated = rotate(P, Q, angle);
        iter->x = P_rotated.real();
        iter->y = P_rotated.imag();
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

inline double getLengthOfVector(Point px1, Point px2) { return sqrt(pow(px1.x - px2.x, 2) + pow(px1.y - px2.y, 2)); }

void SpaceShip::fillRect(Point a, Point b, Point c){
    double Cx_ab, Cy_ab;
    double step = 1;
    double sideLength = getLengthOfVector(a, b);
    tie(Cx_ab, Cy_ab) = getXYOffsetOnVector(a, b, step/2);
    int index = 1;
//    for (int i = a.x; i <= a.x + sideLength; ++i){
//        for (int j = a.y; j <= a.y + sideLength; ++j){
//            SDL_RenderDrawPoint(renderer, i, j);
//        }
//    }
    while (sideLength >= 0){
        SDL_RenderDrawLine(renderer, a.x - Cx_ab*index, a.y - Cy_ab*index, c.x - Cx_ab*index, c.y - Cy_ab*index);
        sideLength -= step;
        ++index;
    }
}

void SpaceShip::putSquareOnPoint(Point centerPoint, double blockHypotenuse){
    vector<Point> littleSqare;
    Point tmp = centerPoint;
    tmp.y -= blockHypotenuse;
    for (int i = 0; i < 4; ++i){
        double theta = M_PI/4 + i*M_PI/2 - getTiltAngel();
        point P(tmp.x, tmp.y);
        point Q(centerPoint.x, centerPoint.y);
        point P_rotated = (P-Q) * polar(1.0, theta) + Q;
        littleSqare.push_back(Point(P_rotated.real(), P_rotated.imag()));
    }
    auto iter2 = littleSqare.begin();
    for (; iter2 != littleSqare.end() - 1; ++iter2){
        SDL_RenderDrawLine(renderer, iter2->x, iter2->y, (iter2+1)->x, (iter2+1)->y);
    }
    SDL_RenderDrawLine(renderer, iter2->x, iter2->y, littleSqare.begin()->x, littleSqare.begin()->y);
    fillRect(littleSqare[1], littleSqare[2], littleSqare[0]);
}

// Length of base of the SpaceShip
double SpaceShip::getLengthOfBase(){ return getLengthOfVector(pp[1], pp[2]); }

pair<Point, Point> SpaceShip::getPerpendicularLineByPoint(Point px, Point tp1, Point tp2){
    double Cx, Cy;
    double angle = M_PI_2;
    tie(Cx, Cy) = getXYOffsetOnVector(px, tp1, getLengthOfBase()); // to make sure pz will be found

    point topComplexPoint(tp1.x - Cx, tp1.y - Cy);
    point baseComplexPoint(px.x, px.y);
    point rotetedComplexPoint = rotate(topComplexPoint, baseComplexPoint, angle);

    Point px2 = Point(rotetedComplexPoint.real(), rotetedComplexPoint.imag());
    Point pz = getTwoLinesIntersaction(px, px2, tp1, tp2);

    return make_pair(px, pz);
}

pair<double, double> SpaceShip::getXYOffsetOnVector(Point px1, Point px2, double offsetLength){
    double length = getLengthOfVector(px1, px2);
    double Cx = (px1.x - px2.x) / (length/(offsetLength*2));
    double Cy = (px1.y - px2.y) / (length/(offsetLength*2));
    return make_pair(Cx, Cy);
}

Color SpaceShip::getRandomColor(){
    Color randomColor;
    switch (rand() % 5){
        case 1: randomColor = Color(220,220,220); break;
        case 2: randomColor = Color(192,192,192); break;
        case 3: randomColor = Color(105,105,105); break;
        case 4: randomColor = Color(211,211,211); break;
        case 5: randomColor = Color(119,136,153); break;
        default: break;
    }
    return randomColor;
}

Color SpaceShip::getNextColor(){
    Color result;
    if (colorIter == availableColors.end()) { colorIter = availableColors.begin();}
    result = *colorIter;
    colorIter++;
    return result;
}

void SpaceShip::updateSkeleton(Point topPoint, Point downPoint, Point pz, double blockHypotenuse, bool symmetrical, bool randomColor){

    double blockSize = sqrt(pow(blockHypotenuse, 2)/2);
    double length = getLengthOfVector(topPoint, downPoint);
    double Cx, Cy;
    tie(Cx, Cy) = getXYOffsetOnVector(topPoint, downPoint, blockSize);

    double littleHypotenuse = sqrt(pow(Cx, 2) + pow(Cy, 2));
    int index = 0;
    Point px1, px2;
    double Vx, Vy;
    double ribLength;
    Color tmpColor;
    colorIter = availableColors.end();
    while (length >= 0){
        if (randomColor){
            tmpColor = getNextColor();
            SDL_SetRenderDrawColor(renderer, tmpColor.r, tmpColor.g, tmpColor.b, 255);
        }
        Point vertebra(topPoint.x - Cx*index, topPoint.y - Cy*index);
        putSquareOnPoint(vertebra, blockHypotenuse);

        length -= littleHypotenuse;
        ++index;

        tie(px1, px2) = getPerpendicularLineByPoint(vertebra, topPoint, pz);
        tie(Vx, Vy) = getXYOffsetOnVector(px1, px2, blockSize);

        ribLength = getLengthOfVector(px1, px2);
        int vIndex = 1;
        while (ribLength >= 0){
            if (randomColor){
                tmpColor = getNextColor();
                SDL_SetRenderDrawColor(renderer, tmpColor.r, tmpColor.g, tmpColor.b, 255);
            }
            Point tmpVertebraRight(px1.x - Vx*vIndex, px1.y - Vy*vIndex);
            putSquareOnPoint(tmpVertebraRight, blockHypotenuse);
            if (symmetrical){
                Point tmpVertebraLeft(px1.x + Vx*vIndex, px1.y + Vy*vIndex);
                putSquareOnPoint(tmpVertebraLeft, blockHypotenuse);
            }
            ribLength -= littleHypotenuse;
            ++vIndex;
        }
    }
}

void SpaceShip::display(){
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
    updateSkeleton(pp[0], Point(pp[1].x/2 + pp[2].x/2, pp[1].y/2 + pp[2].y/2), pp[2], 4, true, true);
    leftNozzle->update();
    rightNozzle->update();
    leftNozzle->display();
    rightNozzle->display();
    Point a,b,c;
    a = leftNozzle->points[0];
    b = leftNozzle->points[1];
    c = leftNozzle->points[2];
    updateSkeleton(b, Point(a.x/2 + c.x/2, a.y/2 + c.y/2), a, 2, true, false);
    a = rightNozzle->points[0];
    b = rightNozzle->points[1];
    c = rightNozzle->points[2];
    updateSkeleton(b, Point(a.x/2 + c.x/2, a.y/2 + c.y/2), a, 2, true, false);

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
    this->x_previous = x;
    this->y_previous = y;
    this->x += directionXY.x;
    this->y += directionXY.y;
}
